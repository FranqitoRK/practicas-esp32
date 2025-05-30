SemaphoreHandle_t mySemaphore;  // Declaramos el semáforo (mutex)
int rojo0 = 32, ama0 = 13, ver0 = 12;

void setup() {
  Serial.begin(115200);
  
  pinMode(rojo0,OUTPUT);
  pinMode(ama0,OUTPUT);
  pinMode(ver0,OUTPUT);

  mySemaphore = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(
    taskSemaphore,  // Función que vamos a ejecutar
    "Task_Core1",   // Nombre para la función (solo para debugging)
    4096,           // Tamaño del stack
    NULL,           // Parámetros
    1,              // Prioridad de la tarea
    NULL,           // No es importante
    1               // Asignamos el núcleo 1
  );

  xTaskCreatePinnedToCore(
    taskSemaphore,
    "Task_Core0",
    4096,
    NULL,
    1,
    NULL,
    0
  );
}

void taskSemaphore(void *parameter) {
  while (1) {
    int coreID = xPortGetCoreID();  // Detecta en qué core estamos

    // Intentamos tomar el semáforo
    if (xSemaphoreTake(mySemaphore, portMAX_DELAY) == pdTRUE) {
      Serial.print("Core ");
      Serial.print(coreID);
      Serial.println(" tiene el semáforo");

    // Simulación de un semáforo
      if (coreID == 0) {
        digitalWrite(rojo0,LOW);
        digitalWrite(ver0, HIGH);
        vTaskDelay(pdMS_TO_TICKS(2000));
        digitalWrite(ver0,LOW);
        digitalWrite(ama0, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
        digitalWrite(ama0,LOW);
        digitalWrite(rojo0, HIGH);
      }
      else if (coreID == 1) {
        Serial.println("Esperando...");
	vTaskDelay(pdMS_TO_TICKS(1500));
      }
      xSemaphoreGive(mySemaphore);

      Serial.print("Core ");
      Serial.print(coreID);
      Serial.println(" liberó el semáforo");
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
}

void loop() {

}
