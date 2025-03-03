#include "headers.h"
#include <IRsend.h>
#include <map>

// extern std::map<String, uint32_t> commandToCode;
// extern std::map<uint32_t, String> codeToCommand;
extern IRsend irsend;

void handleCommand(const String& command) {
    auto it = commandToCode.find(command);
    if (it != commandToCode.end()) {
        Serial.println("Sending command: " + command);
        Serial.println("Command code: " + String(it->second, HEX));
        irsend.sendNEC(it->second, 32);
    } else {
        Serial.println("Unknown command: " + command);
    }
}

void populateCodeToCommand() {
    for (auto const& x : commandToCode) {
        codeToCommand[x.second] = x.first;
    }
}

void transmitSignal(uint32_t command) {
    Serial.println("Transmitting signal...");
    Serial.print("Command: 0x");
    Serial.println(command, HEX);
    irsend.sendNEC(command, 32);
    Serial.println("Signal transmitted.");
}

String getProtocolString(decode_type_t protocol) {
    switch(protocol) {
        case decode_type_t::NEC: return "NEC";
        case decode_type_t::SONY: return "SONY";
        case decode_type_t::RC5: return "RC5";
        case decode_type_t::RC6: return "RC6";
        default: return "UNKNOWN";
    }
}
