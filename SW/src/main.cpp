/*********
 * By teddycool, based on code from randomnerdtutorials (Rui Santos at http://randomnerdtutorials.com)
 * Compatible with ESP sensorboard HW-version > 7.x, www.github.com/teddycool/ for details
 * This file is the main-class, handling wifi connection and basic logic for hw  
 * but also general functions such as reading of battery level
 * 
 * Each box has it's own implementation of settings and sensor-readings imported as the last #include
 * Rename the current config implementation to .cpp, the rest to ._cp or something...
 * Set last include to the correct -h file
 * *********/

// Including the ESP8266 WiFi library

//#include <PubSubClient.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGeneric.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include "av8s1.h"  


WiFiClient espClient;
ADC_MODE(ADC_TOUT);

//Extern defnitions from the actual box implementation files

extern const char* ssid;
extern const char* password;
//extern const float calvalue; 

extern const char* server; 
extern const char* resource; 

extern int32 sleepTimeS;


void setup() {
	
  //Not used since deep-sleep makes a total reset each restart and runs loop() only once each cycle
	
}

void loop() {		


	// Initializing serial port for debugging purposes
	Serial.begin(9600);
	Serial.println("Starting serial");

	bool wificonnected = false;
	int maxtries = 6;
	int tries = 0;
	pinMode(13, OUTPUT);

	// Connecting to WiFi network
	Serial.println();
	Serial.println("Connecting to ");
	Serial.println( ssid);
	WiFi.begin( ssid,  password);
	delay(2000);
	
	//Maximize number of tries...	
	while ((!wificonnected) && (tries <= maxtries)) {
		if (WiFi.status() == WL_CONNECTED) {
			wificonnected = true;
			Serial.println("");
			Serial.println("WiFi connected");
			Serial.println(WiFi.localIP());
			break;
		}
		else {
			Serial.print(".");
			tries = tries + 1;
			delay(1000);
		}
	}

	//Only do reading if connected to wifi, 
	if (wificonnected) {
		int retries = 3;   
		Serial.println("Connecting to: " + String(server)); 
		while(!espClient.connect(server, 80) && (retries-- > 0)) 
		{     
			Serial.print(".");
			delay(500);
		} 

		if(!espClient.connected()) {     
			 Serial.println("Failed to connect, going back to sleep");   
		}
		else { 
			Serial.println("Connected and ready to make measurements....");
			Serial.println("Setting up power to sensors...");
			digitalWrite(13, HIGH);				
			Serial.println("Waiting to stabilize...");
			delay(2000);

			//Sensor readings moved to cpp file for each sensor
			ReadSensors( espClient, server);

			Serial.println("Reading battery voltage on A0...");
			int batterya = analogRead(A0);
			Serial.println(String(batterya));
			float batv = (float(batterya)/1024.0)/0.178; 
			Serial.println(String(batv) + "V");

			espClient.connect(server, 80);
			espClient.print(getPostMsg("bat",String(batv), "V" )); 
			delay(500);
			espClient.connect(server, 80);
			espClient.print(getPostMsg("wifi",String(tries), "counts" )); 
			delay(500);
		}  //end of connected to server

	espClient.stop(); 
	Serial.println();
	Serial.println("Turning off power to pins...");	
	digitalWrite(13, LOW);
	delay(500);	
		
	} //end of connected to wifi

  //ALLWAYS go to sleep again...

  Serial.println("Going to sleep....");
  uint32 micros = (uint32)sleepTimeS*1000000;
  String secs = String(sleepTimeS).c_str();
  Serial.println("Will sleep for " + secs + " seconds...");
  ESP.deepSleep(micros);  //uS!		
}