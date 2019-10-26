#pragma once
#ifndef _AV8BOX1_H
#define _AV8BOX1_H

#include <OneWire.h>
#include <DallasTemperature.h>

#include <WiFiClient.h>



//DO NOT change these method signatures
String getPostMsg(String pmeasurement, String pvalue, String punit);
void ReadSensors(WiFiClient espClient, const char* server);

#endif  //_AV8BOX1_H