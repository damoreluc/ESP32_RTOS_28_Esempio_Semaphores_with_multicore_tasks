 # FreeRTOS Esercizio 28: Esempio illustrativo dell'uso di oggetti del kernel in sistemi multicore

Si considerano due task: __Task0__ eseguito sul _Core0_ e __Task1__ eseguito sul _Core1_.

I due task condividono il semaforo binario `bin_sem`, creato nella funzione `setup()` e quindi appartenente al _Core1_.

__Task0__ rilascia il semaforo e si mette in pausa per un tempo definito dal parametro `task_0_delay`,
poi ricomincia.

__Task1__ attende il rilascio del semaforo e quando lo acquisisce, commuta lo stato di un led.

In breve, il programma fa lampeggiare il led spezzando il problema in due parti:
 * il __Task0__ definisce il periodo di lampeggio; il __Task1__ commuta il pin del led.
 * La sincronizzazione tra i due task avviene per tramite del semaforo `bin_sem`.

L'esempio mette in evidenza la semplicità con cui si gestiscono gli oggetti del kernel
anche in un sistema dual core come la ESP32.