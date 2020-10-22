#include <MPU9250_asukiaaa.h>

//Variales globales utiles:
// para decidir si encender o no el LED...
// ...Es global ya que es compartida por sendData y blinkLED
bool LED_ON = false; 
// Contador para calcular la media de las medidas cada vez que
// se entre a sendData
int cnt = 0; 


// IMU:
//----------------------
#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
float offsetX, offsetY, offsetZ, modulo2;
float aXtotal = 0.0, aYtotal = 0.0, aZtotal = 0.0;



void setup() {
  //--------------------------------------
  //Inicializamos el puerto serie:
  Serial.begin(115200);
  delay(1000);

  //--------------------------------------
  //Inicializamos y calibramos el sensor:
  #ifdef _ESP32_HAL_I2C_H_ // For ESP32
    Wire.begin(SDA_PIN, SCL_PIN);
    mySensor.setWire(&Wire);
  #endif

  mySensor.beginAccel();
  delay(100);
  
  while (!Serial.available()){
  }
  Serial.flush();
  Serial.println("");
  Serial.println("---------------- CALIBRACION -----------------");
  Serial.println("Para calibrar mandar algo por el Puerto Serie:");
  Serial.println("IMPORTANTE!! Debe estar en reposo");
  Serial.println("");

  while (!Serial.available()){
  }
  
  if (mySensor.accelUpdate() == 0) {
    offsetX = mySensor.accelX();
    offsetY = mySensor.accelY();
    aZ = mySensor.accelZ();
  } else {
    Serial.println("Cannot read accel values");
  }

  //En la siguiente líne asumimos que el oofset en el eje Z NO es suficiente como para cambiar el signo en la medición:
  if (aZ > 0){
    offsetZ = aZ - 1.0;
  }
  else {
    offsetZ = aZ + 1.0;
  }
  
  Serial.println("-------------- Sensor calibrado --------------");
  
  //--------------------------------------------------
  // Creamos las tareas FreeRTOS, les asignamos la misma prioridad (1) y la misma memoria en stack (128 palabras);
  //xTaskCreate(blinkLED,"Tarea1",1000,NULL,1,NULL);
  xTaskCreate(getData,"Tarea2",1000,NULL,1,NULL);
  xTaskCreate(sendData,"Tarea3",10000,NULL,1,NULL);
  
}

//En este loop no se hara nada
void loop() {
  delay(1000);
}


//Tarea de FreeRTOS que nos captura los datos del sensor:
void getData( void * pvParameter){
  for(;;){
    //Leemos aceleraciones y actualizamos los sumatorios:
    if (mySensor.accelUpdate() == 0) {
      
      aX = mySensor.accelX(); 
      aXtotal = aXtotal + aX - offsetX;
      
      aY = mySensor.accelY(); 
      aYtotal = aYtotal + aY - offsetY;
      
      aZ = mySensor.accelZ(); 
      aZtotal = aZtotal + aZ - offsetZ;
      
      //aSqrt = mySensor.accelSqrt();
      
      cnt = cnt +1;        //Actualizamos el contador:
    } 
    else {
      Serial.println("Cannot read accel values");
    }
    
    vTaskDelay( 100 / portTICK_PERIOD_MS ); //esperamos 100 ms para tomar una nueva medida
    }
    //Serial.println("Finalizando tarea 2");
    vTaskDelete( NULL );
}

//Tarea para enivar los datos y encender el LED:
void sendData( void * pvParameter ){
  pinMode(17, OUTPUT);
   for(;;){ 
     // if (cnt == 10) {
      //Sacamos por el puerto serie las aceleraciones en cada eje:
      Serial.println("acel_X: " + String(aXtotal/cnt));
      Serial.println("acel_Y: " + String(aYtotal/cnt));
      Serial.println("acel_Z: " + String(aZtotal/cnt));
      
      //Calculamos tambien el modulo de la aceleracion:      
      modulo2 = aXtotal*aXtotal/(cnt*cnt) + aYtotal*aYtotal/(cnt*cnt) + aZtotal*aZtotal/(cnt*cnt);
      Serial.println("Modulo de la aceleracion medio: " + String(sqrt(modulo2)));
  
      //incializamos de nuevo las aceleraciones y el contador
      Serial.println("");
      aXtotal = 0.0;  aYtotal = 0.0; aZtotal = 0.0;
      cnt=0;

      //Encendemos el LED durante 200 ms
      digitalWrite(17, HIGH);
      vTaskDelay( 200 / portTICK_PERIOD_MS );
      //Apagamos el LED y esperamos 800 ms (los datos se mandan cada segundo)
      digitalWrite(17, LOW);
      vTaskDelay( 800 / portTICK_PERIOD_MS );
  }
}
