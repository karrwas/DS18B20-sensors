# DS18B20-sensors
Temperature measurement on the radiator in the race car

The project uses UART in single wire mode in STM32L476RG as a hardware solution for DS18B20 utilization. The code stores constant values of previously checked sensors' addresses, based on which enables temperature measurement. Proper library for DS18B20 use with 1-Wire interface has been made.

Future improvements include:
- storing sensor address in external EEPROM (much better than local FLASH)
- enabling the program to automatically recognize that a new sensor has been connected (DS18B20 allows such operation)
