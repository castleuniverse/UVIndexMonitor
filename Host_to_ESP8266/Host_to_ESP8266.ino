// Course: Emmbeded Systems Design
// Project: UV Index Monitor 
// Sketch: Host_to_esp8266 
// Description: Transmit three arguments over the serial interface
// Board: ESP8266
// By: Juan Yepes
// Florida Atlantic University - Summer 2020


// INITIALIZATION

// Libraries
#include <CmdMessenger.h> 
#include <IoTGuru.h>
#include <ESP8266WiFi.h>

// WiFi Credentials
const char* ssid      = "XXXXXXXXX";   
const char* password  = "XXXXXXXXX";

// IoT Guru Credentials
String userShortId    = "XXXXXXXXX";
String deviceShortId  = "XXXXXXXXX";
String deviceKey      = "XXXXXXXXX";
String nodeKey        = "XXXXXXXXX";

// Create objects
IoTGuru iotGuru = IoTGuru(userShortId, deviceShortId, deviceKey);
CmdMessenger cmdMessenger = CmdMessenger(Serial);

// Commands
enum
{
  kAcknowledge         , // (0) Command to acknowledge that cmd was received
  kError               , // (1) Command to report errors
  kSendIoT             , // (2) Command to request to transmit to the IoT-Guru cloud
};

// Attach callback methods
void attachCommandCallbacks()
{
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(kSendIoT, OnSendIoT);
}

// ------------------  C A L L B A C K S -----------------------

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(kError,"Command without attached callback");
}

// Callback function to transmit to the cloud
void OnSendIoT()
{
  // Retreive first parameter as float
  float uvIndex = cmdMessenger.readFloatArg();
  
  // Retreive second parameter as float
  float latitude = cmdMessenger.readFloatArg();

  // Retreive third parameter as float
  float longitude = cmdMessenger.readFloatArg();

  // Send values to the cloud via HTTP
  iotGuru.sendHttpValue(nodeKey, "uv", uvIndex);
  iotGuru.sendHttpValue(nodeKey, "latitude", latitude);
  iotGuru.sendHttpValue(nodeKey, "longitude", longitude);
  
  // Send back the result of the transmition to the host
  cmdMessenger.sendCmdStart(kSendIoT);
  cmdMessenger.sendCmdArg(uvIndex);
  cmdMessenger.sendCmdArg(latitude);
  cmdMessenger.sendCmdArg(longitude);
  cmdMessenger.sendCmdEnd();
}

void setup() {
    Serial.begin(9600);
    delay(10);

    // Adds newline to every command
    cmdMessenger.printLfCr(); 

    // Attach user-defined callback methods
    attachCommandCallbacks();

    // Send the status to the PC that says the esp8266 has booted
    cmdMessenger.sendCmd(kAcknowledge,"ESP8266 module is ready");

    // Connect to WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(50);
        Serial.print(".");
    }
    Serial.println("");\

    // IoT Guru parameters
    iotGuru.setCheckDuration(60000);
    iotGuru.setDebugPrinter(&Serial);
}

void loop() {
    iotGuru.check();

    // Process incoming serial data, and perform callbacks
    cmdMessenger.feedinSerialData();
  
}
