#ifndef OTA_UPDATE_H
#define OTA_UPDATE_H

#include <HTTPClient.h>  // HTTP library to fetch firmware
#include <EEPROM.h>      // EEPROM to store boot counter
#include <Update.h>      // For handling OTA updates

void startOTAUpdate(const char* firmware_url, int ledPin);


#endif