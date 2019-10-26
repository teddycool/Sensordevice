//Main setting file for sensorboard
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiClient.h>
#include "av8s1.h"

#define ONE_WIRE_BUS 5
//Hardware version:
#define hw_version 8
//Client WiFi-settings
#define client_name "av8_s2"

// Measure real voltage level for the batteries and calculate this value for the actual box,
// depending on component exact values, should be around 0.18
// calvalue = (analogvalue/1024.0) / (real voltage)
const float calvalue = 0.178; 

//Backend info for publishing the values
const char* server = "server"; 
const char* resource = "/SensorBoardBackEnd/post_value.php?devid=5&devname=Demo&"; 

//WiFi connection
const char* ssid = "sssid";
const char* password = "key";
int32 sleepTimeS = 3600; //sleptime in seconds, 


//Each post contains one value and should add this to the url-string:
// measm=type&value=value&unit=unit,  i e measm=bat&value=4.6&unit=V

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress outSensor = {0x28, 0xFF, 0x88, 0xC0, 0xA7, 0x16, 0x05, 0x71}; //  short wire - outside
DeviceAddress foundSensor = {0x28, 0xFF, 0x61, 0x01, 0x00, 0x17, 0x03, 0x7B}; // long wire - inside



//Complete test-url can look like:
// http://server/SensorBoardBackEnd/post_value.php?devid=1&devname=av8_box1&measm=bat&value=4.6&unit=V
String getPostMsg(String pmeasurement, String pvalue, String punit){

	String ppost = "GET " + String(resource) + "&measm=" + pmeasurement +"&value=" + pvalue + 
				"&unit=" + punit +  " HTTP/1.1\r\n" + "Host: " + String(server) + "\r\n"+ "Connection: close\r\n\r\n";

	Serial.println("Created http request:");
	Serial.println(ppost);
	return ppost;
}

//The sensor readings for the box, depending on the sensor used
void ReadSensors(WiFiClient espClient, const char* server){
    sensors.requestTemperatures(); 
    delay(1000);
    if (sensors.isConversionComplete()){
        Serial.println("Conversion complete. Reading and sending data");
        //Read outdoor sensor
        float outtemp = sensors.getTempC(outSensor);
        //Read inside foundation sensor
        float foundtemp = sensors.getTempC(foundSensor);

        espClient.print(getPostMsg("otemp", String(outtemp), "C")); 
        delay(500);

        espClient.connect(server, 80);
        espClient.print(getPostMsg("ftemp",String(foundtemp), "C" )); 
        delay(500);

        }
        else {
            Serial.println("Conversion NOT complete. Dont send any data");
        }
}
