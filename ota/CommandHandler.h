#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <WiFiClient.h>  // Needed for sending ACKs to clients

// Function to process incoming commands (declaration)
void processCommand(String command, WiFiClient& client);

#endif  // COMMAND_HANDLER_H
