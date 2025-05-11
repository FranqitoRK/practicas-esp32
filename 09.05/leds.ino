#include "data.h"

#define PIN_RED 12      
#define PIN_GREEN 26    
#define PIN_BLUE 33  

QueueHandle_t myQueue;

void setRGB (int R, int G, int B) {
  if (R == 1 && G == 1 && B == 1) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(PIN_RED, HIGH);
      digitalWrite(PIN_GREEN, HIGH);
      digitalWrite(PIN_BLUE, HIGH);
      vTaskDelay(pdMS_TO_TICKS(300));
      digitalWrite(PIN_RED, LOW);
      digitalWrite(PIN_GREEN, LOW);
      digitalWrite(PIN_BLUE, LOW);
      vTaskDelay(pdMS_TO_TICKS(300));
    }
  }
  else {
    if (R == 1) {
      digitalWrite(PIN_RED, HIGH);
      vTaskDelay(pdMS_TO_TICKS(1000));
      digitalWrite(PIN_RED, LOW);
    }
    if (G == 1) {
      digitalWrite(PIN_GREEN, HIGH);
      vTaskDelay(pdMS_TO_TICKS(1000));
      digitalWrite(PIN_GREEN, LOW);
    }
    if (B == 1) {
      digitalWrite(PIN_BLUE, HIGH);
      vTaskDelay(pdMS_TO_TICKS(1000));
      digitalWrite(PIN_BLUE, LOW);
    }
  }
}

void productor(void *parameter) {
  while (1) {
    LedCommand accion = randomCommand();
    
    if (accion == LED_BLINK) {
      xQueueSendToFront(myQueue, &accion, portMAX_DELAY);
    }
    else {
      xQueueSend(myQueue, &accion, portMAX_DELAY);
    }

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void consumidor(void *parameter) {
  LedCommand accion;

  while (1) {
    if (xQueueReceive(myQueue, &accion, portMAX_DELAY) == pdPASS) {
      if (accion == LED_BLINK) {
        setRGB(1,1,1);
      } 
      else {
        if (accion == LED_RED) {
          setRGB(1,0,0);
        }
        if (accion == LED_GREEN) {
          setRGB(0,1,0);
        }
        if (accion == LED_BLUE) {
          setRGB(0,0,1);
        }
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  myQueue = xQueueCreate(20, sizeof(LedCommand));

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  
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
    1,
    NULL,
    1
  );
}

void loop() {
}