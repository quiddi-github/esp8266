#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

const char* ssid = "Newton";
const char* password = "12345678";

ESP8266WebServer server(80);

const char* www_username = "admin";
const char* www_password = "esp8266";
const int output5 = 5;
const int output4 = 4;

String content;
String output5State = "off";
String output4State = "off";

void make_html(){
        content = "<!DOCTYPE HTML>\r\n<html>";
        content += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
        content +="<link rel=\"icon\" href=\"data:,\">";
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
        content += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
        content += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
        content += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
        content += ".button2 {background-color: #77878A;}</style></head>";
            
            // Web Page Heading
        content += "<body><h1>ESP8266 Web Server</h1>";
// Display current state, and ON/OFF buttons for GPIO 5  
        content += "<p>GPIO 5 - State " + output5State + "</p>";
            // If the output5State is off, it displays the ON button       
            if (output5State=="off") {
        content += "<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>";
            } else {
        content += "<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>";
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 4  
        content += "<p>GPIO 4 - State " + output4State + "</p>";
            // If the output4State is off, it displays the ON button       
            if (output4State=="off") {
        content += "<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>";
            } else {
        content += "<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>";
            }
        content += "</body></html>";


       

  
  }

void setup() {
  Serial.begin(115200);
    pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  ArduinoOTA.begin();

  server.on("/", []() {
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }
    server.send(200, "text/plain", "Login OK");

  });
  server.on("/index", []() {
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }
    make_html();
    server.send(200, "text/html", content);
  });

    server.on("/5/on", []() {
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }

    output5State = "on";
    digitalWrite(output5, HIGH);
    make_html();
    server.send(200, "text/html", content);
  });

      server.on("/5/off", []() {
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }

    output5State = "off";
    digitalWrite(output5, LOW);
    make_html();
    server.send(200, "text/html", content);
  });

server.on("/4/on", []() {
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }

    output4State = "on";
    digitalWrite(output4, HIGH);
    make_html();
    server.send(200, "text/html", content);
  });

      server.on("/4/off", []() {
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }

    output4State = "off";
    digitalWrite(output4, LOW);
    make_html();
    server.send(200, "text/html", content);
  });

  
  server.begin();

  Serial.print("Open http://");
  
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");
}




void loop() {
  ArduinoOTA.handle();
  server.handleClient();
}
