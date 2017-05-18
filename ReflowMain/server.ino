/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/
#include "FS.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* host = "esp8266-webupdate";
const char* ssid = "IEEE 2.4GHz";
const char* password = "Ilovesolder";
bool isWritten= false;
ESP8266WebServer server(80);
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
File f;
void setup(void){
  Serial.begin(115200);
  Serial.println();
  ticker.attach_ms(TICK_LENGTH, tick);
  bool result=SPIFFS.begin(); 
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  if(WiFi.waitForConnectResult() == WL_CONNECTED){
    MDNS.begin(host);
    server.on("/", HTTP_GET, [](){
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/html", serverIndex);
    });
    server.on("/update", HTTP_POST, [](){
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/plain", (Update.hasError())?"FAIL":"Starting the Heating Profile!");
      //ESP.restart();
    },[](){
      HTTPUpload& upload = server.upload();
      if(upload.status == UPLOAD_FILE_START){
        f = SPIFFS.open("/f.txt", "w");
      } else if(upload.status == UPLOAD_FILE_WRITE){
        
        f.write(upload.buf,upload.currentSize);
      } else if(upload.status == UPLOAD_FILE_END){
        f.close();
        isWritten=true;
      }
      yield();
    });
    server.begin();
    MDNS.addService("http", "tcp", 80);
  
    Serial.printf("Ready! Open http://%s.local in your browser\n", host);
  } else {
    Serial.println("WiFi Failed");
  }
}
 
void loop(void){
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
