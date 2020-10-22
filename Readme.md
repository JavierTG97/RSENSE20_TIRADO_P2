# Práctica 2: Diseño de firmware basado en sistema operativo de tiempo real (RTOS)

### *Redes de sensores Electrónicos*

**Partes de la práctica** (cada una tiene un folder asociado):

1. **BlinkLED_HelloWorld**: Código que hace uso de FreeRTOS para hacer que un LED parpadee cada 200 ms y envíe el mensaje "Hola mundo" cada 1 s.
2. **FreeRTOS-IMU**: Usando FreeRTOS &rarr Se conecta un sensor inercial por I2C (o SPI), muestrea la aceleración cada 100 ms y manda los datos cada segundo vía UART (cada vez que se envían se activa un LED durante 200ms).

### Tutoriales/ Documentación principalmente utilizados:

* [ESP32_FreeRtos](https://github.com/uagaviria/ESP32_FreeRtos) (link dado en el guión).
* Documentación de la orden [xTaskCreate](https://www.freertos.org/a00125.html) de la web de FreeRTOS.
* [Arduino FreeRTOS Tutorial 1 - Creating a FreeRTOS task to Blink LED in Arduino Uno](https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial1-creating-freertos-task-to-blink-led-in-arduino-uno) (aunque hay diferencias al no utilizar ahí el IDE de Arduino)
* [Foro del ESP32](https://esp32.com/viewtopic.php?t=8614) en el que explican el fallo debido a falta de stack en una tarea.