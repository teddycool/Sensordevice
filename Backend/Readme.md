This directory contains example of how to use the data from the sensors in a graph with google chart.
The *.html files creates and shows the graphs and the *.php files creates the json data used by the graph.

Since some of the graphs uses combined values from severeal sensors and the sensors don't read the information 
at the same time, I create avarge-values of the readings for each hour before creating the actual json.

The CreateTable.sql creates the needed table structure for using the sql and this is also the structured 
used in the C code of the sensor software.

The view_data.php dumps all data for the las hour to a webpage for debugging purposes. 





