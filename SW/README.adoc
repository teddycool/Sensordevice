This directory contains software as an example for how to use the sensorboards.
All version of the hardware can use this software. 
I use Microsoft Visual Studio Code with plattformiIO extentions as developing environment.

There is a general main.cpp used by all boxes.

For each sensor-box I have a .cpp and a .h file with settings for that box' sensors and wifi. Each box sends data to the backend, identifying itself as a device-id and a box name. Each of the sensors data instances are sent as their own backend call and ends up in a database table. The backend can then create tables of data to visualize in a graph or for other purposes. Example graphs and data-mangling can be found in the backend directory of this project. The backend directory also contains details for the database table.

TODO: Create a key/cert to send in the url that is checked before insert into the database.

TODO: Set up a proper build pipe-line for each sensorbox instance. Now I copy and rename files...