## FrontNode
The front MCU is a TEENSY 4.1. It is responsible for collecting inputs from:
 - APPS1 and APPS2 (pedal sensors)
 - Front brake pressure sensor
 - Front left and right wheel speed sensors
 - Front left and right suspension sensors
 - Steering angle sensor

It sends the information collected by the sensors as CAN frames to the VCU.