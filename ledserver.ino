#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const int White = 0; 
const int Blue = 1;
const char* ssid = "...";
const char* password = "...";
MDNSResponder mdns;
ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
 
void setup(void){
  pinMode(White, OUTPUT);
  pinMode(Blue, OUTPUT);
//  Serial.begin(115200);
  WiFi.mode(WIFI_AP); //set ESP sebagai Access Point
  WiFi.softAP("WiFuESP","12345678");//set ssid dan password, jangan lupa password minimal 8 karakter 
  server.begin();
//  Serial.println("");

   
  if (mdns.begin("esp8266", WiFi.localIP())) {
//    Serial.println("MDNS responder started");
  }
  
  server.on("/", handleRoot);
  
  server.on("/WON", [](){
  server.send(200, "text/plain", "Okay -- Light is ON!");
  digitalWrite(White, 1);
  });
  
  server.on("/WOFF", [](){
  server.send(200, "text/plain", "Okay -- Light is OFF!");
  digitalWrite(White, 0);
  });

  server.on("/BON", [](){
  server.send(200, "text/plain", "Okay -- Light is ON!");
  digitalWrite(Blue, 1);
  });
  
  server.on("/BOFF", [](){
  server.send(200, "text/plain", "Okay -- Light is OFF!");
  digitalWrite(Blue, 0);
  });

  server.onNotFound(handleNotFound);
  
  server.begin();
//  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
}
