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
#define BUTTON_LIVING     12
#define BUTTON_KITCHEN    13
#define BUTTON_BEDROOM    15
#define BUTTON_BATHROOM   25
#define PIN_LDR           34

bool living = HIGH, kitchen = HIGH, bedroom = HIGH, bathroom = HIGH, all = HIGH;

void setup() {
  // todo pull-up
  pinMode(BUTTON_LIVING, INPUT);    
  pinMode(BUTTON_KITCHEN, INPUT);
  pinMode(BUTTON_BEDROOM, INPUT);
  pinMode(BUTTON_BATHROOM, INPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi conectado");

  client.setServer(mqtt_server, mqtt_port);

  if (client.connect(mqtt_client_id)) {
    Serial.println("MQTT conectado");
  } else {
    Serial.println("Error al conectar MQTT");
  }
}

void loop() {
  if (digitalRead(BUTTON_LIVING) == LOW && living == HIGH) {
    client.publish(mqtt_topic_prefix + "living", "ON");
    Serial.println("Luz living.");
    living = LOW;
  }
  if (digitalRead(BUTTON_KITCHEN) == LOW && kitchen == HIGH) {
    client.publish(mqtt_topic_prefix + "kitchen", "ON");
    Serial.println("Luz cocina.");
    kitchen = LOW;
  }
  if (digitalRead(BUTTON_BEDROOM) == LOW && bedroom == HIGH) {
    client.publish(mqtt_topic_prefix + "bedroom", "ON");
    Serial.println("Luz dormitorio.");
    bedroom = LOW;
  }
  if (digitalRead(BUTTON_BATHROOM) == LOW && bathroom == HIGH) {
    client.publish(mqtt_topic_prefix + "bathroom", "ON");
    Serial.println("Luz baño.");
    bathroom = LOW;
  }
  if (analogRead(PIN_LDR) <= 700 && all == HIGH) {
    client.publish(mqtt_topic_prefix + "todas", "ON");
    Serial.println("Luz todas.");
    all = LOW;
  }
}
