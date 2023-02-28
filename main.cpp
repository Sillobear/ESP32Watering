#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <cstdio>

const char* ssid = "";
const char* password = "";

const char* serverName = "http://27.00.232/new/";

const int sensor_id = 1;

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

int sense_pin = 35;
float value = 0;

const char buf[50];

void setup() {
  Serial.begin(9600);
  pinMode(sense_pin, INPUT);  
  setupWifi();
}

void loop() {
  value = analogRead(sense_pin);
}

void setupWifi(){
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds, it will take 5 seconds before publishing the first reading.");
}

void postNewMeasurement(){
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      http.begin(client, serverName);
      // Specify content-type header
      http.addHeader("Content-Type", "application/json");
      // Data to send with HTTP POST
      sscanf(buf, "{\"sensor_id\":\"1\",\"moisture\":\"%f\"}", value);
      // Send HTTP POST request
      int httpResponseCode = http.POST(buf);
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}