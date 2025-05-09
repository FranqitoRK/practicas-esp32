#include "data.h"

#define LED_RED 12      
#define LED_GREEN 26    
#define LED_BLUE 33  

int pinMap[] = {
  LED_RED,   // LED_RED (enum 0) → pin 12
  LED_GREEN, // LED_GREEN (enum 1) → pin 26
  LED_BLUE   // LED_BLUE (enum 2) → pin 33
};

QueueHandle_t myQueue;

// Tarea Productor: genera comandos LED aleatorios
void productor(void *parameter) {
  while (1) {
    LedCommand accion = randomCommand();
    xQueueSend(myQueue, &accion, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(500)); // Pequeño retardo para evitar saturación
  }
}

// Tarea Consumidor: ejecuta los comandos LED
void consumidor(void *parameter) {
  LedCommand accion;
  while (1) {
    if (xQueueReceive(myQueue, &accion, portMAX_DELAY) == pdPASS) {
      if (accion == LED_BLINK) {
        for (int i = 0; i < 3; i++) {
          digitalWrite(LED_RED, HIGH);
          digitalWrite(LED_GREEN, HIGH);
          digitalWrite(LED_BLUE, HIGH);
          vTaskDelay(pdMS_TO_TICKS(300));
          digitalWrite(LED_RED, LOW);
          digitalWrite(LED_GREEN, LOW);
          digitalWrite(LED_BLUE, LOW);
          vTaskDelay(pdMS_TO_TICKS(300));
        }
      } else {
          digitalWrite(pinMap[accion], HIGH);
          vTaskDelay(pdMS_TO_TICKS(1000));
          digitalWrite(pinMap[accion], LOW);

      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  myQueue = xQueueCreate(20, sizeof(LedCommand));

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  
  xTaskCreatePinnedToCore(
    productor,
    "Productor",
    4096,
    NULL,
    1,
    NULL,
    0
  );

  xTaskCreatePinnedToCore(
    consumidor,
    "Consumidor",
    4096,
    NULL,
    2,
    NULL,
    1
  );
}

void loop() {
  // No se usa loop
}
