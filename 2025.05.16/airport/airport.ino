QueueHandle_t queue;  //lista FIFO
SemaphoreHandle_t pista[3];

int avion_ids[5] = { 1, 2, 3, 4, 5 };
int pista_ids[3] = { 0, 1, 2 };

void avion(void *parameter) {
  int id = *((int *)parameter);

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(random(300, 500)));
    xQueueSend(queue, &id, portMAX_DELAY);
  }
}

void tdc(void *parameter) { //3 tdc con una pista c/u
  int pista_id = *((int *)parameter);  // 0, 1 o 2
  int idRecibida;

  while (1) {
    if (xQueueReceive(queue, &idRecibida, portMAX_DELAY) == pdPASS) {
      if (xSemaphoreTake(pista[pista_id], portMAX_DELAY) == pdTRUE) {  //toma mutex
        Serial.printf("Avion %d aterrizando en pista %d\n", idRecibida, pista_id + 1);
        vTaskDelay(pdMS_TO_TICKS(random(1000, 2500)));
        xSemaphoreGive(pista[pista_id]);  //libera mutex
        Serial.printf("Avion %d libero la pista %d\n", idRecibida, pista_id + 1);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  queue = xQueueCreate(5, sizeof(int));

  //creacion de mutex "pista"
  for (int i = 0; i < 3; i++) {
    pista[i] = xSemaphoreCreateMutex();
  }

  //creacion de tareas "avion"
  for (int i = 0; i < 5; i++) {
    xTaskCreatePinnedToCore(avion, "avion", 4096, &avion_ids[i], 1, NULL, 0);
  }
  //creacion de tarea "torre de control"
  for (int i = 0; i < 3; i++) {
    xTaskCreatePinnedToCore(tdc, "control", 4096, &pista_ids[i], 1, NULL, 1);
  }
}

void loop() {}