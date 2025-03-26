#include "OTA_update.h"
#include <Arduino.h>  // For Serial and digitalWrite

void startOTAUpdate(const char* firmware_url, int ledPin) {
  HTTPClient http;
  http.begin(firmware_url);  // Set the firmware URL

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    int contentLength = http.getSize();
    if (Update.begin(contentLength)) {
      Serial.println("Starting OTA update...");
      digitalWrite(ledPin, HIGH);  // Turn on LED to indicate update started

      size_t written = Update.writeStream(http.getStream());

      if (written == contentLength && Update.end()) {
        Serial.println("Update successful! Rebooting...");
        EEPROM.write(0, 0);  // Reset the boot counter after successful update
        EEPROM.commit();
        ESP.restart();  // Reboot ESP32 with new firmware
      } else {
        Serial.println("OTA update failed.");
      }
    } else {
      Serial.println("Not enough space for OTA update.");
    }
  } else {
    Serial.println("No update available or failed to fetch firmware.");
  }

  http.end();
  digitalWrite(ledPin, LOW);  // Turn off LED after update process
}
