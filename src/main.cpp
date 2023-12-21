#include <Arduino.h>
#include <WiFi.h>
#include <heltec.h>
#include <PubSubClient.h>

#include "ConfigManager.h"

// Set LoRaWAN to EU868
#define BAND    868E6

ConfigManager config;
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi();
void reconnect();

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);

  // Initialize Heltec Board
  Heltec.begin(true /*Display Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, 0 /*LoRa frequency, 0 if not used*/);
  
  // Setup WiFi
  setup_wifi();
  
  // Setup MQTT
  if (config.get_mqtt_enabled()) {
    client.setServer(config.get_mqtt_host(), config.get_mqtt_port());
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long rssi = WiFi.RSSI();

  // Clear the display
  Heltec.display->clear();

  // Display WiFi details
  Heltec.display->drawString(0, 0, "WiFi Signal:");
  Heltec.display->drawString(0, 10, "SSID: " + WiFi.SSID());
  Heltec.display->drawString(0, 20, "RSSI:");
  Heltec.display->drawString(0, 30, String(rssi) + " dBm");

  // Display the content
  Heltec.display->display();

  // Send RSSI to MQTT broker
  if (config.get_mqtt_enabled()) {
    String rssiMsg = String(rssi);
    client.publish(config.get_mqtt_topic(), rssiMsg.c_str());
  }

  delay(config.get_interval());
}

void setup_wifi() {
  delay(10);
  // Connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(config.get_wifi_ssid());
  WiFi.begin(config.get_wifi_ssid(), config.get_wifi_password());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  if (!config.get_mqtt_enabled()) {
    return;
  }

  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client", config.get_mqtt_user(), config.get_mqtt_password())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}