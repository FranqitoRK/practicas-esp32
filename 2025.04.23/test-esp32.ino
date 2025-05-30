//Preferences -> URL -> https://dl.espressif.com/dl/package_esp32_index.json

void setup() {
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    taskFunction,  // Referencia a la funcion que vamos a ejecutar
    "Task_Core1",  // Nombre para la funcion, sirve solamente para propositos de debugging
    4096,          // Tamaño del stack la tarea
    NULL,          // Parametro que recibe la funcion que le vamos a pasar
    1,             // Prioridad de la tarea
    NULL,          // no es importante
    1              // El core donde queremos que corra la task (0/1)
  );

  xTaskCreatePinnedToCore(
    taskFunction,
    "Task_Core0",
    4096,
    NULL,
    1,
    NULL,
    0
  );
}

void taskFunction(void *parameter) {
  while (1) {
    int coreID = xPortGetCoreID();  // Detecta en qué core estamos
    Serial.print("Hola mundo dicho por el Core ");
    Serial.println(coreID);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void loop() {}
