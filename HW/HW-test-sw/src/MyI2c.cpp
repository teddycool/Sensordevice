
#include <Wire.h> //include Wire.h library
#include "MyI2c.h"
#include <WiFiClient.h>

void i2cscanner(){
  Wire.begin(14,12);; // Wire communication begin on pins:  SDA, SCL

  Serial.println("I2C Scanner");

  byte error; 
  byte address; //variable for error and I2C address
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;

  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
    delay(2);
  }

  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("Done scanning I2C devices");
}