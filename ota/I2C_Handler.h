#ifndef I2C_HANDLER_H
#define I2C_HANDLER_H

#include <Arduino.h>  // For Serial, String, etc.
#include <Wire.h>     // For I2C communication
#include <WiFi.h>
#define debugging_mode 1

// Constants
#if debugging_mode
  #define NUM_SLAVES 1
#else
  #define NUM_SLAVES 4
#endif

// Extern declarations for arrays to be defined elsewhere
extern const int SLAVE_ADDRESSES[NUM_SLAVES];  
extern const int ATTENTION_PINS[NUM_SLAVES];

// Function declarations
void initializeI2C(int sdaPin, int sclPin, int ledPin);
void routeCommandToSlave(const String& command);
void handleSlaveAttention(WiFiClient& client);

#endif  // I2C_HANDLER_H
