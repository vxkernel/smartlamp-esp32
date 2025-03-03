#ifndef HEADERS_H
#define HEADERS_H

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <map>

extern std::map<String, uint32_t> commandToCode;
extern std::map<uint32_t, String> codeToCommand;

void transmitSignal(uint32_t command);
String getProtocolString(decode_type_t protocol);
void populateCodeToCommand();
void handleCommand(const String& command);
String processor(const String& var);
void sendHtml();
void handleSend();

#endif // HEADERS_H