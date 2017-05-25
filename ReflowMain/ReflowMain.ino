//library for the timed software interrupt that we use for our custom PWM
#include <Ticker.h>

//library for ESP8266 filesystem
#include "FS.h"
//libraries to set up web server
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define MAXCS 2 //thermocouple module SPI chip select pin

/***********************
 * thermocouple module * 
 ***********************/
 
Adafruit_MAX31855 thermocouple(MAXCS);

/*****************************
 * file system and webserver * 
 *****************************/

/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/

const char* host = "IEEE_ReflowOven"; //host name
const char* ssid = "IEEE 2.4GHz"; //lab network name
const char* password = "Ilovesolder"; //network password

bool isWritten= false;

ESP8266WebServer server(80); //initialize esp8266 webserver at port 80

const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
File f;

/**************
 * PWM ticker *
 **************/

#define TICK_LENGTH 20 //tick length in milliseconds
#define FULL_DUTY_VAL 100 //max duty cycle value allowed

#define relay 2 //digital pin for switching relay

uint8_t pwm_dutyCycle = 0; //percentage (min 0, max 100DUTY_VAL)

Ticker ticker;

int pwm_counter = 0;

void tick() //this function is called every TICK_LENGTH milliseconds
{
  if (pwm_counter >= FULL_DUTY_VAL) //reached end of a period, so we reset everything
    pwm_counter = 0;

  digitalWrite(relay, (pwm_counter < pwm_dutyCycle));
  pwm_counter++;
  //Serial.println(pwm_counter);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Helper Functions
/////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t getTemp() 
{
   /*
   * The readCelsius() method actually returns 32-bit signed integers representing
   * 14-bit temperature values in 1/4 Celsius
   * 
   * We're going to assume that we never get negative temperatures in the oven, so we'll just
   * convert these values to uint16_t 
   */

  uint16_t temp = (uint16_t)(thermocouple.readCelsius());
  if (isnan(temp))
    return 0; //define 0 to be an error code (we'll have to check for this later)
  return temp; 
}


/////////////////////////////////////////////////////////////////////////////////////////////////
// Temperature "Control" fnctions
/////////////////////////////////////////////////////////////////////////////////////////////////

void PID(uint16_t setPoint,uint16_t currentTemp)
{
  /*
   * takes in the desired temperature, and the current temperature
   * implements standard PID
   * calls PWM to change reflow oven temperature
   */
  double error = 0, derivative = 0, correction = 0, prev_error = -999; //fix

  //PID
  error = setPoint - currentTemp;
  total_error += error;
  if(prev_error != -999)
    derivative = error - prev_error;
  correction = Kp*error + Ki*total_error + Kd*derivative;
  prev_error = error;

  //Change temperature based on PID
  currentTemp += correction;

  //Write to PWM

}

bool runCurStep(uint16_t target_temp, uint16_t prev_temp, unsigned long target_time, unsigned long start_time) 
{
  /*
  * target_temp is the temperature at the end of the step.
  * prev_temp is the temperature at start of the step.
  * target_time is the time the step should run for.
  * start_time is the millis() time when the function is called 
  */

  unsigned long cur_time = millis() - start_time;
  float slope = (float)(target_temp - prev_temp) / target_time;
  while (cur_time < target_time) {
    cur_time = millis() - start_time;
    PID(prev_temp + slope * cur_time, getTemp()); //PID implements change in temp, analogWrites
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Main Loop
/////////////////////////////////////////////////////////////////////////////////////////////////

void setup(){

  Serial.begin(115200);
  while (!Serial) delay(1); //wait for Serial

  initProfile(); //initialize the server and filesystem, and the read the profile

  pinMode(relay, OUTPUT); //set up relay control pin as output
  ticker.attach_ms(TICK_LENGTH, tick); //set up PWM ticker

}

/*
 * TODO: notify website if oven is in use
 * allow oven to stop halfway through
 * more aesthetics in front end
 */
void loop()
{
  server.handleClient();
  delay(1);

  if (isWritten){
    Serial.println("file available");
    File fo = SPIFFS.open("/f.txt", "r");

    /*
     * get room temperature
     */
    uint32_t ovenTemp = 0;
    for (int i = 0; i < 10; i++) {
      ovenTemp += getTemp();
    }
    ovenTemp /= 10;
    uint16_t prev_temp = (uint16_t)ovenTemp;

    //this while loop runs the entire profile
    while(fo.available()) {
      /*
       * parses the string and runs current step with parsed string
       */
      String line = fo.readStringUntil('\n');
      //Lets read line by line from the file
      int index = line.indexOf(',');
      int temp=line.substring(0,index).toInt();
      int start=index;
      index = line.indexOf(',',start+1);
      int time=line.substring(start+1,index).toInt();

      // Call heating step here
      runCurStep(temp, prev_temp, time, millis());

      prev_temp = temp;

      //these were for testing
      Serial.print(temp); Serial.print('\t'); 
      Serial.println(time); 
    }
    fo.close();
    isWritten=false;
  }
  
  initProfile(); //reinitialize for next profile
}
