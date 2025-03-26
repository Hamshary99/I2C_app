#include "CommandHandler.h"

void processCommand(String command, WiFiClient& client) {
  command.trim();

  if (command.startsWith("CHN")) {
    int valueStart = command.indexOf('-');
    if (valueStart != -1 && valueStart < command.length() - 1) {
      String channel = command.substring(3, valueStart);  // Extract "N" from "CHN1-", "CHN2-", etc.
      String action = command.substring(valueStart + 1);  // Extract "xxx"

      if (action == "ON" || action == "OFF") {
        Serial.println("Channel " + channel + " turned " + action);
        client.println("ACK: CHN" + channel + "-" + action);  // Send acknowledgment
      } else {
        int value = action.toInt();
        if (value > 0 || action == "0") {  // Handle numeric values properly (including 0)
          Serial.println("Channel " + channel + " set to " + value);
          client.println("ACK: CHN" + channel + "-" + String(value));  // Send acknowledgment
        } else {
          Serial.println("ERROR: Invalid value");
          client.println("ERROR: Invalid value");
        }
      }
    } else {
      Serial.println("ERROR: Malformed command");
      client.println("ERROR: Malformed command");
    }
  } else {
    Serial.println("ERROR: Command must start with CHNx-xxx");
    client.println("ERROR: Command must start with CHNx-xxx");
  }
}
