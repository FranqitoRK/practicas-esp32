#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

//datos wifi
const char* ssid = <SSID>;
const char* password = <PASSWORD>;

//datos mqtt
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_client_id = <USERNAME>;
const char* mqtt_topic = <TOPIC>;

void callback(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a Wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.print("Conectando al broker MQTT...");
    if (client.connect(mqtt_client_id)) {
      Serial.println("conectado!");
      client.subscribe(mqtt_topic);
    }
  }
}

void loop() {
  client.loop();

  if (Serial.available()) {
    // detectar y mandar mensaje
    String msg = mqtt_client_id + ": " + Serial.readStringUntil('\n');
    msg.trim();
    if (msg.length() > 0) {
      client.publish(mqtt_topic, msg.c_str());
    }
  }
}

  }
  //creacion de tarea "torre de control"
  xTaskCreatePinnedToCore(tdc, "control", 4096, NULL, 1, NULL, 1);
}

void loop() {}