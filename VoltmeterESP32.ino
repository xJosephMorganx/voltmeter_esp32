#include <WiFi.h>
#include <WebServer.h>

#include "index_html.h"
#include "style_css.h"
#include "script_js.h"

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

const int ADC_PIN = 34;
const float ADC_REFERENCE_VOLTAGE = 3.3;
const int ADC_MAX_READING = 4095;

WebServer server(80);

void handleVoltageApi() {
  int raw = analogRead(ADC_PIN);
  float voltage = (raw * ADC_REFERENCE_VOLTAGE) / ADC_MAX_READING;

  String json = "{";
  json += "\"raw\":";
  json += raw;
  json += ",\"voltage\":";
  json += String(voltage, 3);
  json += ",\"maxVoltage\":";
  json += String(ADC_REFERENCE_VOLTAGE, 1);
  json += "}";

  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  delay(300);

  analogReadResolution(12);
  analogSetPinAttenuation(ADC_PIN, ADC_11db);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Server ready: http://");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send_P(200, "text/html", INDEX_HTML);
  });

  server.on("/style.css", []() {
    server.send_P(200, "text/css", STYLE_CSS);
  });

  server.on("/script.js", []() {
    server.send_P(200, "application/javascript", SCRIPT_JS);
  });

  server.on("/api/voltage", handleVoltageApi);

  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
