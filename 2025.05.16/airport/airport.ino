<<<<<<< HEAD
QueueHandle_t queue;
SemaphoreHandle_t pista[3];

void avion0(void *parameter) {
  while (1) {
    int id = 1;
    vTaskDelay(pdMS_TO_TICKS(random(300,500)));
    xQueueSend(queue, &id, portMAX_DELAY);
  }
}
void avion1(void *parameter) {
  while (1) {
    int id = 2;
    vTaskDelay(pdMS_TO_TICKS(random(300,500)));
    xQueueSend(queue, &id, portMAX_DELAY);
  }
}
void avion2(void *parameter) {
  while (1) {
    int id = 3;
    vTaskDelay(pdMS_TO_TICKS(random(300,500)));
    xQueueSend(queue, &id, portMAX_DELAY);
  }
}
void avion3(void *parameter) {
  while (1) {
    int id = 4;
    vTaskDelay(pdMS_TO_TICKS(random(300,500)));
    xQueueSend(queue, &id, portMAX_DELAY);
  }
}
void avion4(void *parameter) {
  while (1) {
    int id = 5;
    vTaskDelay(pdMS_TO_TICKS(random(300,500)));
=======
QueueHandle_t queue;  //lista FIFO
SemaphoreHandle_t pista[3];

int avion_ids[5] = { 1, 2, 3, 4, 5 };
int pista_ids[3] = { 0, 1, 2 };

void avion(void *parameter) {
  int id = *((int *)parameter);

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(random(300, 500)));
>>>>>>> b05ba1b923f2925f4408d98cdb645f7bf780768c
    xQueueSend(queue, &id, portMAX_DELAY);
  }
}

<<<<<<< HEAD
void (*avion[])(void *) = {avion0, avion1, avion2, avion3, avion4};

void tdc(void *parameter) {
  int idRecibida;
  int turnoPista = 0;  // Para rotar cuál pista se prueba primero

  while (1) {
    if (xQueueReceive(queue, &idRecibida, portMAX_DELAY) == pdPASS) {
      bool aterrizado = false;

      while (!aterrizado) {
        for (int j = 0; j < 3; j++) {
          int i = (turnoPista + j) % 3;  // Rota entre 0,1,2 → 1,2,0 → 2,0,1

          if (xSemaphoreTake(pista[i], 0) == pdTRUE) {
            Serial.printf("Avion %d aterrizando en pista %d\n", idRecibida, i+1);
            vTaskDelay(pdMS_TO_TICKS(random(2000, 5000)));
            xSemaphoreGive(pista[i]);
            Serial.printf("Avion %d libero la pista %d\n", idRecibida, i+1);
            aterrizado = true;
            turnoPista = (i + 1) % 3;  // Cambiar el orden de la próxima iteración
            break;
          }
        }

        if (!aterrizado) {
          vTaskDelay(pdMS_TO_TICKS(100));  // Espera antes de volver a intentar
        }
=======
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
>>>>>>> b05ba1b923f2925f4408d98cdb645f7bf780768c
      }
    }
  }
}

<<<<<<< HEAD

=======
>>>>>>> b05ba1b923f2925f4408d98cdb645f7bf780768c
void setup() {
  Serial.begin(115200);
  queue = xQueueCreate(5, sizeof(int));

  //creacion de mutex "pista"
  for (int i = 0; i < 3; i++) {
    pista[i] = xSemaphoreCreateMutex();
  }

  //creacion de tareas "avion"
  for (int i = 0; i < 5; i++) {
<<<<<<< HEAD
    xTaskCreatePinnedToCore(avion[i], "task", 4096, NULL, 1, NULL, 0);
  }
  //creacion de tarea "torre de control"
  xTaskCreatePinnedToCore(tdc, "control", 4096, NULL, 1, NULL, 1);
=======
    xTaskCreatePinnedToCore(avion, "avion", 4096, &avion_ids[i], 1, NULL, 0);
  }
  //creacion de tarea "torre de control"
  for (int i = 0; i < 3; i++) {
    xTaskCreatePinnedToCore(tdc, "control", 4096, &pista_ids[i], 1, NULL, 1);
  }
>>>>>>> b05ba1b923f2925f4408d98cdb645f7bf780768c
}

void loop() {}