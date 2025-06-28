#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

// datos WiFi
const char* ssid = "<SSID>";
const char* password = "<PASSWORD>";

// datos mqtt
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_client_id = "<USERNAME>";
const char* mqtt_topic_prefix = "maio/casa/luz/";

// pines
#define LIVING_PIN     4
#define KITCHEN_PIN    16
#define BEDROOM_PIN    17
#define BATHROOM_PIN   27

bool livingRoom = false, kitchen = false, bedroom = false, bathroom = false, allLights = false;

void callback(char* topic, byte* payload, unsigned int length) {
  String topicStr = String(topic);
  Serial.println("Callback triggered.");

  if (topicStr == String(mqtt_topic_prefix) + "living" && !allLights) {
    livingRoom = !livingRoom;
    digitalWrite(LIVING_PIN, livingRoom ? HIGH : LOW);
    Serial.println(livingRoom ? "Luz living ON." : "Luz living OFF.");
  }

  else if (topicStr == String(mqtt_topic_prefix) + "cocina" && !allLights) {
    kitchen = !kitchen;
    digitalWrite(KITCHEN_PIN, kitchen ? HIGH : LOW);
    Serial.println(kitchen ? "Luz cocina ON." : "Luz cocina OFF.");
  }

  else if (topicStr == String(mqtt_topic_prefix) + "dormitorio" && !allLights) {
    bedroom = !bedroom;
    digitalWrite(BEDROOM_PIN, bedroom ? HIGH : LOW);
    Serial.println(bedroom ? "Luz dormitorio ON." : "Luz dormitorio OFF.");
  }

  else if (topicStr == String(mqtt_topic_prefix) + "bathroom" && !allLights) {
    bathroom = !bathroom;
    digitalWrite(BATHROOM_PIN, bathroom ? HIGH : LOW);
    Serial.println(bathroom ? "Luz baño ON." : "Luz baño OFF.");
  }

  else if (topicStr == String(mqtt_topic_prefix) + "todas") {
    allLights = !allLights;
    digitalWrite(LIVING_PIN, allLights ? HIGH : LOW);
    digitalWrite(KITCHEN_PIN, allLights ? HIGH : LOW);
    digitalWrite(BEDROOM_PIN, allLights ? HIGH : LOW);
    digitalWrite(BATHROOM_PIN, allLights ? HIGH : LOW);
    Serial.println(allLights ? "Luces ON." : "Luces OFF.");

    livingRoom = kitchen = bedroom = bathroom = false;
  }
}

void setup() {
  // salidas
  pinMode(LIVING_PIN, OUTPUT);
  pinMode(KITCHEN_PIN, OUTPUT);
  pinMode(BEDROOM_PIN, OUTPUT);
  pinMode(BATHROOM_PIN, OUTPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.print("Conectando al broker MQTT...");
    if (client.connect(mqtt_client_id)) {
      Serial.println("conectado!");
      String topicFilter = String(mqtt_topic_prefix) + "+";
      client.subscribe(topicFilter.c_str());
    } else {
      Serial.print(".");
      delay(1000);
    }
  }
}

void loop() {
  client.loop();
}
