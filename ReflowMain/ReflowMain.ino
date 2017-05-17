#include <Ticker.h> //library for the timed software interrupt that we use for our custom PWM

#include "FS.h" //library for ESP8266 filesystem
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
  {
    pwm_counter = 0;
  }

  digitalWrite(relay, (pwm_counter < pwm_dutyCycle));

  pwm_counter++;

  //Serial.println(pwm_counter);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Helper Functions
/////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t getTemp() 
{
  
  //returns temperature in 1/4 Celsius

  uint32_t temp = (uint32_t)(thermocouple.readCelsius());
  
  if (isnan(temp))
  {
    return 0; //define 0 to be an error code (we'll have to check for this later)
  }
  
  return temp; //the readCelcius() function actually returns
               //32-bit signed integers, but we're going to 
               //assume that we never get negative temperatures
                                                 
}

void setup(){
  
  Serial.begin(115200);
  while (!Serial) delay(1); //wait for Serial

  initFS();

  //set up relay control pin and PWM ticker
  
  pinMode(relay, OUTPUT);
  ticker.attach_ms(TICK_LENGTH, tick);
  
}

void loop()
{
  server.handleClient();
  delay(1);
  if (isWritten){
    Serial.println("file available");
    File fo = SPIFFS.open("/f.txt", "r");
    while(fo.available()) {
      String line = fo.readStringUntil('\n');
      //Lets read line by line from the file
      int index = line.indexOf(',');
      int temp=line.substring(0,index).toInt();
      int start=index;
      index = line.indexOf(',',start+1);
      int time=line.substring(start+1,index).toInt();
      // Call heating step here
      Serial.print(temp); Serial.print('\t'); 
      Serial.println(time); 
    }
    fo.close();
    isWritten=false;
  }
} 



