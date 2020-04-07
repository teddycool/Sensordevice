//Main setting file for sensorboard
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiClient.h>
#include "MyOneWire.h"

#define ONE_WIRE_BUS 5

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//Replace with the actual adress for the tested box
// 287389CE0B00000A
DeviceAddress boxSensor = {0x28, 0x73, 0x89, 0xCE, 0x0B, 0x00, 0x00, 0x0A};

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}

//The sensor readings for the box, depending on the sensor used
void ReadOneWireSensors(){

    sensors.begin();

    // Number of temperature devices found
    int numberOfDevices;

    // We'll use this variable to store a found device address
    DeviceAddress tempDeviceAddress; 
    numberOfDevices = sensors.getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
// Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++){
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)){
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }


    sensors.requestTemperatures(); 

    delay(1000);
    if (sensors.isConversionComplete()){
        Serial.println("Conversion complete");

        float boxtemp = sensors.getTempC(boxSensor);
        Serial.print("Boxtemp: " + String(boxtemp) +  " C"); 
        delay(500);
        }
        else {
            Serial.println("FAILED: Conversion NOT complete for box temp-sensor");
        }
}
