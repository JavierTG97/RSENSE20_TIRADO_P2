

void setup() {

  Serial.begin(115200);
  delay(1000);

    // -------------------------- INFO sobre xTaskCreate -------------------------------------
  //xTaskCreate(1: nombre de la función, 2: nombre para debugging que usará el programa,...
  // ... 3: tamano en palabras para almacenar datos (creo) en la memoria asociada a la task...
  // ...   (a nivel de sistema operativo) es algo complejo por lo que por defecto 128 está bien,... 
  //...  4: Son los pvParameters = las variables que se introducen a la función (NULL indica ninguna),...
  //...  5: Prioridad (0 = reposo), conforme aumenta el numero -> aumenta la prioridad....
  //...  6: parámetro que representa un handle -> se puede utilizar para llamarlo desde 
  //...     otras funcionas y modificar sus parámetros (por ejemplo la prioridad).
  //...     Al igual que el 4, puede ser NULL si no hace falta dicho handle:
  // ---------------------------------------------------------------------------------------

  xTaskCreate(blinkLED,"Tarea1",10000,NULL,1,NULL);
  xTaskCreate(sendHelloworld,"Tarea2",10000,NULL,1,NULL);

  //Tras esto, el scheduler se inicia automáticamente. Si no, habría que realizar la siguiente instruccion:
  //vTaskStartScheduler();

}

void loop() {
  delay(1000);
}

void blinkLED( void * pvParameter ){
    pinMode(17, OUTPUT);
    for(;;){
      digitalWrite(17, HIGH);
      //delay(200); 
      vTaskDelay( 200 / portTICK_PERIOD_MS ); 
      digitalWrite(17, LOW);
      vTaskDelay( 200 / portTICK_PERIOD_MS ); 
    }

    //Serial.println("Finalizando tarea 1");
    //vTaskDelete( NULL );

}

void sendHelloworld( void * pvParameter){

    for(;;){

        Serial.println("Hola mundo");
        //delay(1000);
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
    }
    //Serial.println("Finalizando tarea 2");
    //vTaskDelete( NULL );
}
