#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <map>
#include <headers.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <uri/UriBraces.h>
#include <cstdlib>

const uint16_t IR_RECEIVE_PIN = 27;  
const uint16_t IR_SEND_PIN = 33;     

IRrecv irrecv(IR_RECEIVE_PIN);
IRsend irsend(IR_SEND_PIN);
decode_results results;

const char* ssid = std::getenv("SSID");
const char* password = std::getenv("PASSWORD");

WebServer server(80);

std::map<String, uint32_t> commandToCode = {
 {"Off/On", 0x807F00FF},
 {"Timer", 0x807F807F},
 {"Sleep", 0x807F609F},
 {"Study", 0x807FD02F},
 {"Brighter", 0x807FA05F},
 {"Darker", 0x807F20DF},
 {"Colder", 0x807F50AF},
 {"Warmer", 0x807F40BF}
};

std::map<uint32_t, String> codeToCommand;


void sendHtml() {
  String response = R"(
<!DOCTYPE HTML>
<html>
<head>
  <title>ESP32 IR Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
    h2 { color: #333; }
    button { 
      background-color: #4CAF50; 
      border: none; 
      color: white; 
      padding: 15px 32px; 
      text-align: center; 
      text-decoration: none; 
      display: inline-block; 
      font-size: 16px; 
      margin: 4px 2px; 
      cursor: pointer; 
    }
  </style>
</head>
<body>
  <h2>ESP32 Control</h2>
  <div id="buttons">
    %BUTTONPLACEHOLDER%
  </div>
  <script>
    function sendCommand(command) {
      fetch('/send?command=' + command)
        .then(response => response.text())
        .then(data => console.log(data))
        .catch(error => console.error('Error:', error));
    }
  </script>
</body>
</html>
  )";
  
  String buttons = "";
  for (const auto& command : commandToCode) {
    buttons += "<button onclick=\"sendCommand('" + command.first + "')\">" + command.first + "</button>";
  }
  
  response.replace("%BUTTONPLACEHOLDER%", buttons);
  server.send(200, "text/html", response);
}

void handleSend() {
  String command = server.arg("command");
  handleCommand(command);
  server.send(200, "text/plain", "Sent command: " + command);
}

String processor(const String& var) {
  if (var == "BUTTONPLACEHOLDER") {
    String buttons = "";
    for (const auto& command : commandToCode) {
      buttons += "<button onclick=\"sendCommand('" + command.first + "')\">" + command.first + "</button><br>";
    }
    return buttons;
  }
  return String();
}

void setup() {
  Serial.begin(115200);   

  irrecv.enableIRIn();  
  irsend.begin();       

  populateCodeToCommand();


  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ");
  Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", sendHtml);
  server.on("/send", handleSend);
  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("Protocol: ");
    Serial.print(getProtocolString(results.decode_type));
    Serial.print(" Value: 0x");
    Serial.print(results.value, HEX);
    irrecv.resume();
  }

  server.handleClient();
  delay(2);

}


