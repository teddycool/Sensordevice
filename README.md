# ESP8266-07
Hardware:

A simple generic circuit for a battery powered ESP8266-07.
The circuit can handle one-wire, DHTs and all kinds of I2C sensors. It has capabillities to measures the status of the battery and tell when charging or replacement is needed.

The device comes in two flavours:

HW7.x: Powered by 1 1500mAh LiPo and have a built-in charger. Perfect for sensors where charging now and then is an option. The circuitboard is optimised for a FB-01 box

HW8.x: Powered by 3 AAA alkaline batteries. Perfect for sensors that should have a long life-time without battery change and measures rather seldom. The circuitboard is optimised for a FB-29 box.

TODO: A HW9.x, power by 2 AA alkaline and using a dc/dc step-up converter for low power loss. Work is in progress...

Since everything will fit i a small enclosure and can be used without any external power supply for long a time, it is well suited for different small sensors or IoT devices. 


Software:
Example of software for a device using 2 one-wire temperature gauges and sends information to backend each hour.

Backend:
Example of html, php, json and sql needed to create some charts with the data.
Data is inserted in a database table using a simple php script, extracted and mangled to json in an other php script and drawn with google chart javascript in the html file.


