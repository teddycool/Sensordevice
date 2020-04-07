/*********
 * By teddycool, based on code from randomnerdtutorials (Rui Santos at http://randomnerdtutorials.com)
 * Compatible with ESP sensorboard HW-version = 7.x and 8.x, www.github.com/teddycool/ for details
 * See SW dir for implementation example for a real use
 * 
 * This program tests the hw. These tests are implemented:
 *  - WiFi connection
 *  - DHT interface (needs a connected DHT sensor)
 *  - OneWire interface (the onboard sensor and extrenal if connected)
 *  - I2C interface, lists found adresses incl HSM if this this present on the board
 *  - Analog input (battery health status)
 *  - Debug LED
 *  - DeepSleep and reset at wakeup
 * *********/

// Including the ESP8266 WiFi library

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

//Includes for the different hw interfaces

#include "MyOneWire.h"
#include "MyDht.h"
#include "MyI2c.h"

//Client WiFi-settings
#define client_name "hwtestbox"

WiFiClient espClient;
ADC_MODE(ADC_TOUT);

// Measure the real voltage level for the batteries and calculate this value for the actual box,
// depending on component exact values, should be around 0.18
// calvalue = (analogvalue/1024.0) / (real voltage)
const float calvalue = 0.1725; 

//Backend info for publishing the values
const char* server = "www.sundback.com"; 

//WiFi connection
const char* ssid = "ssid";
const char* password = "paass/key";

//Deep-sleep time in seconds
int32 sleepTimeS = 10;



void setup() {

	// Initializing serial port for debugging purposes
	Serial.begin(9600);
	Serial.println("Starting serial");

	bool wificonnected = false;
	int maxtries = 6;
	int tries = 0;
	pinMode(13, OUTPUT); //Sensor power
	pinMode(15, OUTPUT); //Debug led

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
	
	if (wificonnected) {
		int retries = 3;   
		Serial.println("Connecting to: " + String(server)); 
		while(!espClient.connect(server, 80) && (retries-- > 0)) 
		{     
			Serial.print(".");
			delay(500);
		} 
	}
	else {
		Serial.println("FAILED: Couldn't connect to WiFi");
	}

			
	Serial.println("Setting up power to sensors...");
	digitalWrite(13, HIGH);				
	Serial.println("Waiting for power to stabilize...");
	delay(2000);

	//Test the OneWire interface
	Serial.println("Testing OneWire interface...");
	ReadOneWireSensors();

	//Test the DHT interface
	Serial.println("Testing DHT interface is not yet implemented");


	//Test the I2C interface
	Serial.println("Testing I2C interface and scanning for devices...");
	i2cscanner();

	//Test the Debug-LED
	Serial.println("Testing the debug LED ...");
	digitalWrite(15, HIGH);
	delay(2000);
	digitalWrite(15, LOW);

	//Test the analog input and reading the battery health 

	Serial.println("Reading  analog value on A0...");
	int batterya = analogRead(A0);
	Serial.println(String(batterya));
	Serial.println("Calculating battery voltage with the supplied calibration factor...");
	Serial.println("How to calculate the calibration value? =>  calvalue = (analogvalue/1024.0) / (real voltage)");

	float batv = (float(batterya)/1024.0)/calvalue; 

	Serial.println(String(batv) + "V");

	

	Serial.println();
	Serial.println("Turning off power to pins...");	
	digitalWrite(13, LOW);
		delay(500);	

		espClient.stop(); 
		
	//ALLWAYS go to sleep again...

  Serial.println("Going to sleep....");
  uint32 micros = (uint32)sleepTimeS*1000000;
  String secs = String(sleepTimeS).c_str();
  Serial.println("Will sleep for " + secs + " seconds...");
  ESP.deepSleep(micros);  //uS!		
}


void loop() {	

  //Not used since deep-sleep makes a total reset each restart and runs the setup at start 
	
}

