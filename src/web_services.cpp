
#include <Arduino.h>
#include <WebServer.h>

#include "ArduinoJson.h"
#include "global.h"

const char *SSID = "Fiush machine";
const char *PWD = "Fiush123456";
WebServer server(80);

StaticJsonDocument<2000> jsonDocument;
char buffer[2000];

namespace _10klab {
namespace web_services {

void SetUpRouting();

void ConnectWiFi() {
  WiFi.softAP(SSID, PWD);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  SetUpRouting();
  server.begin();
}

void LoopServer(){
    server.handleClient();
}

void CreateJson(char *tag, float value) {
  jsonDocument.clear();
  jsonDocument["state"] = value;
  serializeJson(jsonDocument, buffer); // Serializa los datos
}

// Respond to the app the machine state "bussy or available"
void GetMachineState() {
  jsonDocument.clear();
  // bool state = _10klab::global::machine_state;
  CreateJson("state", _10klab::global::machine_state);
  // CreateJson("state", _10klab::global::machine_state);
  server.send(200, "application/json", buffer);
  Serial.println("en get state");
  
}

void ReceptionRecipe() {
  if (server.hasArg("plain") == false) {
    // handle error here
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  JsonObject obj = jsonDocument.as<JsonObject>();
  server.send(200, "application/json", "{Receta recivida}");
    // server.send(200, "application/json", "{\"code\":0}");

 //get the values of recipe variables
  // String name_input = jsonDocument["name"];
  // _10klab::global::coctel_name = name_input;
  int name_input = jsonDocument["name"];
  _10klab::global::coctel_name = String(name_input);
  
  _10klab::global::pump1 = jsonDocument["pump1"];
  _10klab::global::pump1 = _10klab::global::pump1 - 1;
  _10klab::global::ml1 = jsonDocument["ml1"];
  _10klab::global::priority1 = jsonDocument["priority1"];
  _10klab::global::priority1 = _10klab::global::priority1 - 1;
  _10klab::global::rotation1 = jsonDocument["rotation1"];
  _10klab::global::ka1 = jsonDocument["ka1"];
  _10klab::global::kb1 = jsonDocument["kb1"];

  _10klab::global::pump2 = jsonDocument["pump2"];
  _10klab::global::pump2 = _10klab::global::pump2 - 1;
  _10klab::global::ml2 = jsonDocument["ml2"];
  _10klab::global::priority2 = jsonDocument["priority2"];
  _10klab::global::priority2 = _10klab::global::priority2 - 1;
  _10klab::global::rotation2 = jsonDocument["rotation2"];
  _10klab::global::ka2 = jsonDocument["ka2"];
  _10klab::global::kb2 = jsonDocument["kb2"];

  _10klab::global::pump3 = jsonDocument["pump3"];
  _10klab::global::pump3 = _10klab::global::pump3 - 1;
  _10klab::global::ml3 = jsonDocument["ml3"];
  _10klab::global::priority3 = jsonDocument["priority3"];
  _10klab::global::priority3 = _10klab::global::priority3 - 1;
  _10klab::global::rotation3 = jsonDocument["rotation3"];
  _10klab::global::ka3 = jsonDocument["ka3"];
  _10klab::global::kb3 = jsonDocument["kb3"];

  _10klab::global::pump4 = jsonDocument["pump4"];
  _10klab::global::pump4 = _10klab::global::pump4 - 1;
  _10klab::global::ml4 = jsonDocument["ml4"];
  _10klab::global::priority4 = jsonDocument["priority4"];
  _10klab::global::priority4 = _10klab::global::priority4 - 1;
  _10klab::global::rotation4 = jsonDocument["rotation4"];
  _10klab::global::ka4 = jsonDocument["ka4"];
  _10klab::global::kb4 = jsonDocument["kb4"];

  _10klab::global::pump5 = jsonDocument["pump5"];
  _10klab::global::pump5 = _10klab::global::pump5 - 1;
  _10klab::global::ml5 = jsonDocument["ml5"];
  _10klab::global::priority5 = jsonDocument["priority5"];
  _10klab::global::priority5 = _10klab::global::priority5 - 1;
  _10klab::global::rotation5 = jsonDocument["rotation5"];
  _10klab::global::ka5 = jsonDocument["ka5"];
  _10klab::global::kb5 = jsonDocument["kb5"];

  _10klab::global::pump6 = jsonDocument["pump6"];
  _10klab::global::pump6 = _10klab::global::pump6 - 1;
  _10klab::global::ml6 = jsonDocument["ml6"];
  _10klab::global::priority6 = jsonDocument["priority6"];
  _10klab::global::priority6 = _10klab::global::priority6 - 1;
  _10klab::global::rotation6 = jsonDocument["rotation6"];
  _10klab::global::ka6 = jsonDocument["ka6"];
  _10klab::global::kb6 = jsonDocument["kb6"];

  _10klab::global::cherry = jsonDocument["G_red"];
  _10klab::global::lemon = jsonDocument["G_yellow"];
  _10klab::global::orange = jsonDocument["G_orange"];
  _10klab::global::herb = jsonDocument["G_green"];

  _10klab::global::machine_state = true;
  delay(100);

  Serial.println("state machine en 1");
  const int less = 1;
  Serial.println("Name");
  Serial.println(name_input);
  Serial.println("Pump 1");
  Serial.println(_10klab::global::pump1 + less);
  Serial.println(_10klab::global::priority1 + less);
  Serial.println(_10klab::global::rotation1);
  Serial.println(_10klab::global::ml1);
  Serial.println(_10klab::global::ka1);
  Serial.println(_10klab::global::kb1);
  Serial.println("Pump 2");
  Serial.println(_10klab::global::pump2 + less);
  Serial.println(_10klab::global::priority2 + less);
  Serial.println(_10klab::global::rotation2);
  Serial.println(_10klab::global::ml2);
  Serial.println(_10klab::global::ka2);
  Serial.println(_10klab::global::kb2);
  Serial.println("Pump 3");
  Serial.println(_10klab::global::pump3 + less);
  Serial.println(_10klab::global::priority3 + less);
  Serial.println(_10klab::global::rotation3);
  Serial.println(_10klab::global::ml3);
  Serial.println(_10klab::global::ka3);
  Serial.println(_10klab::global::kb3);
  Serial.println("Pump 4");
  Serial.println(_10klab::global::pump4 + less);
  Serial.println(_10klab::global::priority4 + less);
  Serial.println(_10klab::global::rotation4);
  Serial.println(_10klab::global::ml4);
  Serial.println(_10klab::global::ka4);
  Serial.println(_10klab::global::kb4);
  Serial.println("Pump 5");
  Serial.println(_10klab::global::pump5 + less);
  Serial.println(_10klab::global::priority5 + less);
  Serial.println(_10klab::global::rotation5);
  Serial.println(_10klab::global::ml5);
  Serial.println(_10klab::global::ka5);
  Serial.println(_10klab::global::kb5);
  Serial.println("Pump 6");
  Serial.println(_10klab::global::pump6 + less);
  Serial.println(_10klab::global::priority6 + less);
  Serial.println(_10klab::global::rotation6);
  Serial.println(_10klab::global::ml6);
  Serial.println(_10klab::global::ka6);
  Serial.println(_10klab::global::kb6);
  Serial.println("Garnish");
  Serial.println(_10klab::global::cherry);
  Serial.println(_10klab::global::lemon);
  Serial.println(_10klab::global::orange);
  Serial.println(_10klab::global::herb);
}

void SetUpRouting() {
  server.on("/state", HTTP_POST, GetMachineState); 
  server.on("/preparation", HTTP_POST, ReceptionRecipe); 
}

} // namespace web_services
} // namespace _10klab
