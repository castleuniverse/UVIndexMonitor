# UV Index Monitor
Ultraviolet light monitor

This is a project from my Embedded System Design I course at Florida Atlantic University.

# Objective
A solar powered ultraviolet radiation (UV Index) monitoring device with wireless communication and GPS capabilities to be used on Smart Cities. Location data and UV Index information is captured and published to the cloud. Consolidated data will be available to be displayed on different media:  web, billboards, or on mobile apps.

# UV Index
UV Index is a number linearly related to the intensity of sunlight reaching the earth, and is weighted according to the CIE Erythemal Action Spectrum.  This weighting is a standardized measure of human skin's response to different wavelengths of sunlight from UVB to UVA. 
The UV Index has been standardized by the World Health Organization and includes a simplified consumer UV exposure level (1 to 11).

# Hardware modules
- UV Sensor (SI1145)
- GPS module (NEO6MV2)
- WiFi module (ESP8266)
- Microcontroller (Arduino Mega 2560)
- Solar Panel 6V
- Lithium Power Bank Battery

# Sofware
- Host_to_ESP8266.ino : This is the code that runs on the ESP8266 WiFi module.
- UVMonitor_Host : This is the code that runs on the Arduino Mega 2560

# Demo video
https://youtu.be/k-6yTQbIVSc



