#include "I2C_Handler.h"

#if debugging_mode
const int SLAVE_ADDRESSES[NUM_SLAVES] = {0x08};  // I2C slave addresses
const int ATTENTION_PINS[NUM_SLAVES] = {15};  // Pins for slave attention signals
#else
const int SLAVE_ADDRESSES[NUM_SLAVES] = {0x08, 0x09, 0x0A, 0x0B};  // I2C slave addresses
const int ATTENTION_PINS[NUM_SLAVES] = {15, 2, 4, 16};  // Pins for slave attention signals
#endif

// Initialize I2C and pin configurations
void initializeI2C(int sdaPin, int sclPin, int ledPin) {
    Wire.begin(sdaPin, sclPin);
    pinMode(ledPin, OUTPUT);

    for (int i = 0; i < NUM_SLAVES; i++) {
        pinMode(ATTENTION_PINS[i], INPUT_PULLUP);
    }

    Serial.println("I2C Initialized.");
}

void routeCommandToSlave(const String& command) {
    if (command.startsWith("CHN1-")) {
        Wire.beginTransmission(SLAVE_ADDRESSES[0]);
        Wire.write((const uint8_t*)command.c_str(), command.length());
        Wire.endTransmission();
        Serial.println("Sent to Slave 1: " + command);

    } else if (command.startsWith("CHN2-")) {
        Wire.beginTransmission(SLAVE_ADDRESSES[1]);
        Wire.write((const uint8_t*)command.c_str(), command.length());
        Wire.endTransmission();
        Serial.println("Sent to Slave 2: " + command);

    } else if (command.startsWith("CHN3-")) {
        Wire.beginTransmission(SLAVE_ADDRESSES[2]);
        Wire.write((const uint8_t*)command.c_str(), command.length());
        Wire.endTransmission();
        Serial.println("Sent to Slave 3: " + command);

    } else if (command.startsWith("CHN4-")) {
        Wire.beginTransmission(SLAVE_ADDRESSES[3]);
        Wire.write((const uint8_t*)command.c_str(), command.length());
        Wire.endTransmission();
        Serial.println("Sent to Slave 4: " + command);

    } else {
        Serial.println("Unknown command: " + command);
    }
}

void handleSlaveAttention(WiFiClient& client) {
    for (int i = 0; i < NUM_SLAVES; i++) {
        if (digitalRead(ATTENTION_PINS[i]) == LOW) {
            Serial.println("Slave " + String(SLAVE_ADDRESSES[i], HEX) + " needs attention.");

            // Request data from the slave
            Wire.requestFrom(SLAVE_ADDRESSES[i], 16);  // Request up to 16 bytes from the slave
            String response = "";
            while (Wire.available()) {
                char c = Wire.read();
                response += c;
            }
            response.trim();

            if (response.length() > 0) {
                Serial.println("Response from Slave " + String(SLAVE_ADDRESSES[i], HEX) + ": " + response);

                // Send the slave's response back to the Wi-Fi client
                client.println(response);
            }

            // Send acknowledgment to the slave
            Wire.beginTransmission(SLAVE_ADDRESSES[i]);
            Wire.write((const uint8_t*)"ACK", 3);
            Wire.endTransmission();
        }
    }
}