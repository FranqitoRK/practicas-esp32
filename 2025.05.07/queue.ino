#include "data.h"

QueueHandle_t myQueue;    //FIFO

void productor(void *parameter) {
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(random(500,2000)));
    int idrandom = randomId();
    if (xQueueSend(myQueue, &idrandom, portMAX_DELAY) == pdPASS) {
      Serial.printf("[Productor] Enviado ID: %d\n", idrandom);
    }
  }
}

void consumidor(void *parameter) {
  int idReceived;
  while (1) {
    if (xQueueReceive(myQueue, &idReceived, portMAX_DELAY) == pdPASS) {
      Person persona = getPersonById(idReceived);
      Serial.printf("[Consumidor] ID: %d\n", persona.id);
      Serial.printf("  NAME: %s\n", persona.name.c_str());
      Serial.printf("  LAST NAME: %s\n", persona.last_name.c_str());
      Serial.printf("  EMAIL: %s\n", persona.email.c_str());
    }
  }
}

void setup() {
  myQueue = xQueueCreate(30, sizeof(int));
  
  Serial.begin(115200);
  
  xTaskCreatePinnedToCore(
    productor,     // Función que ejecuta la tarea
    "Task_Core1",  // Nombre de la tarea
    4096,          // Tamaño del stack en palabras
    NULL,          // Parámetro que se le pasa a la tarea
    1,             // Prioridad de la tarea
    NULL,          // Handle (opcional, no lo usamos aquí)
    0              // Core donde queremos que corra la tarea (0 o 1)
  );

  xTaskCreatePinnedToCore(
    consumidor,    // Función que ejecuta la tarea
    "Task_Core2",  // Nombre de la tarea
    4096,          // Tamaño del stack en palabras
    NULL,          // Parámetro que se le pasa a la tarea
    2,             // Prioridad de la tarea
    NULL,          // Handle (opcional, no lo usamos aquí)
    1              // Core donde queremos que corra la tarea (0 o 1)
  );
}

void loop() {
}
