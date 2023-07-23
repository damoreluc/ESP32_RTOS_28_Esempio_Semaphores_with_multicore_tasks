/*
 * FreeRTOS Esercizio 28: Esempio illustrativo dell'uso di oggetti del kernel in sistemi multicore
 *
 * Si considerano due task: Task0 eseguito sul Core0, e Task1 eseguito sul Core1.
 * I due task condividono il semaforo binario bin_sem, creato nella funzione setup() e quindi appartenente al Core1.
 *
 * Task 0 rilascia il semaforo e si mette in pausa per un tempo definito dal parametro task_0_delay,
 * poi ricomincia.
 * 
 * Task 1 attende il rilascio del semaforo e quando lo acquisisce, commuta lo stato di un led.
 * 
 * In breve, il programma fa lampeggiare il led spezzando il problema in due parti:
 * il Task0 definisce il periodo di lampeggio; il Task1 commuta il pin del led.
 * La sincronizzazione tra i due task avviene per tramite del semaforo bin_sem.
 * 
 * L'esempio mette in evidenza la semplicità con cui si gestiscono gli oggetti del kernel
 * anche in un sistema dual core come la ESP32.
 *
 * Nota: nel file soc.h sono definiti i riferimenti ai due core della ESP32:
 *   #define PRO_CPU_NUM (0)
 *   #define APP_CPU_NUM (1)
 *
 * Qui vengono adoperati entrambi i core
 *
 */

#include <Arduino.h>

// Impostazioni  ***************************************************************
// tempo (in ms) di pausa del Task 0
static const uint32_t task_0_delay = 500;  

// Pins ************************************************************************
// LED pin
static const int pin_1 = GPIO_NUM_23;

// Variabili Globali  **********************************************************
// semaforo condiviso tra i due task
static SemaphoreHandle_t bin_sem;

//*****************************************************************************
// Tasks

// Task0 sul Core 0
void doTask0(void *parameters) {

  // Configura il pin del led
  pinMode(pin_1, OUTPUT);

  // Ciclo infinito
  while (1) {

    // Notifica il task1
    xSemaphoreGive(bin_sem);
    
    // rilascia il core0 mettendo il task0 in pausa per un certo intervallo di tempo
    vTaskDelay(pdMS_TO_TICKS( task_0_delay ));
  }
}

// Task sul Core 1
void doTask1(void *parameters) {

    // Ciclo infinito
  while (1) {
    // Attende la notifica dal Task0
    xSemaphoreTake(bin_sem, portMAX_DELAY);

    // Toggle LED
    digitalWrite(pin_1, !digitalRead(pin_1));
  }
}

//************************************************************************************
// Main (sul core 1, con priorità 1)

// configurazione del sistema
void setup()
{
  // Configurazione della seriale
  Serial.begin(115200);

  // Crea il semaforo binario prima di avviare i due task
  bin_sem = xSemaphoreCreateBinary();

  // breve pausa
  vTaskDelay(pdMS_TO_TICKS(1000));
  Serial.println();
  Serial.println("FreeRTOS Esempio di semaforo condiviso tra task su core distinti");

  // Crea e avvia i due task; 
  //  NOTA: provare a sostituire il pinning del task da APP_CPU_NUM a tskNO_AFFINITY

  // Start Task 0 (sul Core0)
  xTaskCreatePinnedToCore(doTask0,
                          "Task 0",
                          1024,
                          NULL,
                          1,
                          NULL,
                          PRO_CPU_NUM);

  // Start Task 1 (sul Core1)
  xTaskCreatePinnedToCore(doTask1,
                          "Task H",
                          1024,
                          NULL,
                          1,
                          NULL,
                          APP_CPU_NUM);

  // Elimina il task con "Setup e Loop"
  vTaskDelete(NULL);
}

void loop()
{
  // lasciare vuoto
}
