# smart-meter

The aim of the project is to digitalise the electricity meter in our home. We are measuring the total power being consumed by every household and compare it to the power consumed from the transformer. 

To measure the power consumed we are measuring the current consumed per second and the voltage consumed.

After calculation we are uploading all this data to Thingspeak using ESP8266 module.

Same will be done for the meter at the transformer side and then the power differece will be calculated, if a significant difference is found then it means that electrictiy is being stolen in between.
