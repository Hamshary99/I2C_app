#include <WiFi.h>        // Wi-Fi library for Station and AP modes
#include <HTTPClient.h>  // HTTP library to fetch firmware
#include <Update.h>      // OTA update library
#include <EEPROM.h>      // EEPROM to store boot counter

#include "CommandHandler.h"
#include "OTA_update.h"
#include "I2C_Handler.h"

// Wi-Fi Credentials (Station Mode for OTA update)
const char* sta_ssid = "ceauto00_plus";
const char* sta_password = "@ceauto00";

// Access Point (AP) credentials (used if OTA fails)
const char* ap_ssid = "ESP32_Master";
const char* ap_password = "12345678";

// OTA firmware URL
const char* firmware_url = "https://raw.githubusercontent.com/Hamshary99/esp32_blip/refs/heads/main/ota.ino.bin";

#define LED_PIN 2              // Onboard LED pin on ESP32
#define EEPROM_SIZE 1          // Reserve 1 byte for boot counter storage in EEPROM
#define BOOT_CHECK_INTERVAL 5  // Perform OTA check every 5 boots
#define SDA_PIN 21  
#define SCL_PIN 22  


int bootCounter = 0;
WiFiServer server(80);  // Create a server listening on port 80

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  EEPROM.begin(EEPROM_SIZE);  // Initialize EEPROM to store boot counter

  bootCounter = EEPROM.read(0);  // Read the stored boot counter
  Serial.println("\nESP32 Booting...");

  // Increment boot counter and save it in EEPROM
  bootCounter = (bootCounter + 1) % BOOT_CHECK_INTERVAL;
  Serial.println("Boot Count: " + String(bootCounter));
  EEPROM.write(0, bootCounter);
  EEPROM.commit();
  initializeI2C(SDA_PIN, SCL_PIN, LED_PIN);

  // Connect to Wi-Fi in Station Mode
  WiFi.mode(WIFI_STA);
  WiFi.begin(sta_ssid, sta_password);
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    Serial.println("Connected to Internet for OTA update...");

    // Perform OTA update only if boot counter hits the interval (e.g., every 5th boot)
    if (bootCounter == 0) {
      startOTAUpdate(firmware_url, LED_PIN);
    }
  } else {
    Serial.println("Failed to connect to Wi-Fi. Switching to AP mode...");
  }


  // Switch to Access Point Mode (AP Mode)
  WiFi.disconnect(true);  // Disconnect from any Wi-Fi connection
  WiFi.mode(WIFI_AP);     // Force AP mode explicitly
  WiFi.softAP(ap_ssid, ap_password);
  server.begin();  // Start the server
  Serial.println("AP Started. Connect to SSID: " + String(ap_ssid));
  Serial.println("AP IP Address: " + WiFi.softAPIP().toString());
}

void loop() {
  // Handle client connections
  WiFiClient client = server.accept();  // Use the updated method

  if (client) {
    Serial.println("New client connected.");
    while (client.connected()) {
      if (client.available()) {
        String command = client.readStringUntil('\n');  // Read the command from the client
        command.trim();

        // Pass command to the handler function in your separate file
        processCommand(command, client);

        routeCommandToSlave(command);  // Route command to the appropriate slave
        client.println("Command routed: " + command);
      }
    }
    client.stop();  // Close the connection
    Serial.println("Client disconnected.");
  }

  // Blink LED in normal loop
  handleSlaveAttention(client); 
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}