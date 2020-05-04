// Course: Emmbeded Systems Design
// Project: UV Index Monitor 
// Board: Arduino Mega2560
// By: Juan Yepes
// Florida Atlantic University - Summer 2020

// Initialization
#define UV_SAMPLING_TIME      250 // Sample time (mseg) (More than 250)
#define GPS_SAMPLING_TIME    2000 // Sample time (mseg) (More than 1000)
#define WIFI_SAMPLING_TIME  20000 // Sample time (mseg) (More than 15000)

// Libraries
#include <Wire.h>
#include "Adafruit_SI1145.h"      // UV Sensor library
#include <TinyGPS++.h>            // GPS Sensor library

// Global Variables
unsigned long sysDeltaTime, previousSysTime; 
unsigned long timerUV, previousTimeUV;
unsigned long timerGPS, previousTimeGPS;
unsigned long timerWIFI, previousTimeWIFI;
float uvIndex;
float latitude;
float longitude;

// Create objects for sensors and network
Adafruit_SI1145 uv = Adafruit_SI1145();   // UV Sensor object
TinyGPSPlus gps;                          // GPS Sensor object

// SETUP
void setup() {
  // Initialize serial ports 
  Serial1.begin(9600);    // GPS sensor serial port
  Serial2.begin(9600);    // WiFi module serial port
  Serial.begin(9600);     // Console serial port

  previousSysTime = millis(); // Initialize system time  
  
  // Check presence of UV Sensor
  if (! uv.begin()) {
     Serial.println("No UV sensor (Si1145) detected: check wiring");
     while (1);
  } else {
     Serial.println("UV Sensor (Si1145): OK");
  }
  
  // Check presence of GPS Sensor
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS sensor (GPS6MV2) detected: check wiring."));
    while(true);
    } else {
      Serial.println("GPS sensor (GPS6MV2): OK");
    }
}

// MAIN LOOP
void loop() {

  sysDeltaTime = (millis() - previousSysTime); // Get the system sampling time interval
  previousSysTime = millis();                  // Store the system time 

  updateUV(UV_SAMPLING_TIME);           // Update UV sensor values
  updateGPS(GPS_SAMPLING_TIME);         // Update GPS sensor values
  updateWIFI(WIFI_SAMPLING_TIME);       // Update WIFI cloud transmition

}

// UV Index update
void updateUV(int sampleTime){
  timerUV = timerUV + sysDeltaTime;               // Keep record of time 
  if (timerUV >= sampleTime) {                    // Execute on every sample time
    timerUV = 0;                                  // Reset timer
    unsigned long dT = millis() - previousTimeUV; // Get the delta time
    previousTimeUV = millis();                    // store last value of system time 

    uvIndex = uv.readUV();                        // Update UV reading
    Serial.print("UV Index - ");
    Serial.println(uvIndex);   
  }
}

// GPS location update
void updateGPS(int sampleTime){

    timerGPS = timerGPS + sysDeltaTime;               // Keep record of time 

    while (Serial1.available() > 0){
      if (gps.encode(Serial1.read())){                // Read serial port
        if (gps.location.isValid()) {
          latitude = gps.location.lat();                 // Update latitude reading
          longitude = gps.location.lng();                // Update longitude reading
        }
      }
    }

    if (timerGPS >= sampleTime) {                    // Execute on every sample time
      timerGPS = 0;                                  // Reset timer
      unsigned long dT = millis() - previousTimeGPS; // Get the delta time
      previousTimeGPS = millis();                    // store last value of system time 

          Serial.print("GPS - ");
          Serial.print(latitude);
          Serial.print(", ");
          Serial.println(longitude);   

    }
}

// Update the IoT Cloud 
void updateWIFI(int sampleTime){
  timerWIFI = timerWIFI + sysDeltaTime;             // Keep record of time 
  if (timerWIFI >= sampleTime) {                    // Execute on every sample time
    timerWIFI = 0;                                  // Reset timer
    unsigned long dT = millis() - previousTimeWIFI; // Get the delta time
    previousTimeWIFI = millis();                    // store last value of system time 

    Serial.print("WIFI - ");
    Serial.print("2,");
    Serial.print(uvIndex);
    Serial.print(F(","));
    Serial.print(latitude, 6);
    Serial.print(F(","));
    Serial.print(longitude, 6);
    Serial.println(F(";"));

    // Sending data to the WIFI module
    Serial2.print("2,");                      // Command (2) for sending
    Serial2.print(uvIndex);                   // First argument
    Serial2.print(F(","));
    Serial2.print(latitude, 6);               // Second argument
    Serial2.print(F(","));
    Serial2.print(longitude, 6);              // Third argument
    Serial2.println(F(";"));
   
  }
}
