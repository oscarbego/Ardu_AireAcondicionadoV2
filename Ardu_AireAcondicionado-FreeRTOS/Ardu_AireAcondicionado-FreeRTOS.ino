#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>


struct Boton {
  int id;
  //char nombre[15]; //btn + Alta/Baja, bomba, On/Off
};

QueueHandle_t cola_Mensaje;


void on_off(void *pvParameter);
void alta_baja(void *pvParameter);
void bomba(void *pvParameter);

void baja_bomba(void *pvParameter);
void alta_bomba(void *pvParameter);

void Actuador(void *pvParameter);


void setup() {


   cola_Mensaje = xQueueCreate(10, // Queue length
                              sizeof(struct Boton) // Queue item size
                              );                              
  Serial.begin(9600);

  Serial.println("Init");
  delay(1500);

  xTaskCreate(
    on_off
    ,  "on_off"
    ,  64  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );


    xTaskCreate(
    bomba
    ,  "bomba"
    ,  64  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );


    xTaskCreate(
    alta_baja
    ,  "alta_baja"
    ,  64  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );


    xTaskCreate(
    baja_bomba
    ,  "baja_bomba"
    ,  64  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

    xTaskCreate(
    alta_bomba
    ,  "alta_bomba"
    ,  65  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );


   xTaskCreate(
    Actuador
    ,  "Actuador"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );


    


}

void loop()
{}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void on_off( void *pvParameters )  // This is a Task.
{
  Serial.println("on_off"); 
  
  struct Boton boton = { 0 };
  
  uint8_t pushButton = 2;

  pinMode(pushButton, INPUT);

  for (;;) // A Task shall never return or exit.
  {
    int buttonState = digitalRead(pushButton);

    if ( buttonState == 1 )
    {     
      if (xQueueSend(cola_Mensaje, &boton, 2000 / portTICK_PERIOD_MS) != pdTRUE)
      {
        Serial.println("error");  
      }
          
      vTaskDelay( 500 / portTICK_PERIOD_MS );
    }
    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}

void bomba( void *pvParameters )  // This is a Task.
{
  Serial.println("bomba"); 
  
  struct Boton boton = { 2 };
  
  uint8_t pushButton = 6;

  pinMode(pushButton, INPUT);

  for (;;) // A Task shall never return or exit.
  {
    int buttonState = digitalRead(pushButton);

    if ( buttonState == 1 )
    {     
      if (xQueueSend(cola_Mensaje, &boton, 2000 / portTICK_PERIOD_MS) != pdTRUE)
      {
        Serial.println("error");  
      }
          
      vTaskDelay( 500 / portTICK_PERIOD_MS );
    }
    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}


void alta_baja( void *pvParameters )  // This is a Task.
{
  Serial.println("alta_baja"); 
  
  struct Boton boton = { 1 };
  
  uint8_t pushButton = 4;

  pinMode(pushButton, INPUT);

  for (;;) // A Task shall never return or exit.
  {
    int buttonState = digitalRead(pushButton);

    if ( buttonState == 1 )
    {     
      if (xQueueSend(cola_Mensaje, &boton, 2000 / portTICK_PERIOD_MS) != pdTRUE)
      {
        Serial.println("error");  
      }
          
      vTaskDelay( 500 / portTICK_PERIOD_MS );
    }
    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}


void baja_bomba( void *pvParameters )  // This is a Task.
{
  Serial.println("baja_bomba"); 
  
  struct Boton boton = { 3 };
  
  uint8_t pushButton = 3;

  pinMode(pushButton, INPUT);

  for (;;) // A Task shall never return or exit.
  {
    int buttonState = digitalRead(pushButton);

    if ( buttonState == 1 )
    {     
      if (xQueueSend(cola_Mensaje, &boton, 2000 / portTICK_PERIOD_MS) != pdTRUE)
      {
        Serial.println("error");  
      }
          
      vTaskDelay( 500 / portTICK_PERIOD_MS );
    }
    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}

void alta_bomba( void *pvParameters )  // This is a Task.
{
  Serial.println("alta_bomba"); 
  
  struct Boton boton = { 4 };
  
  uint8_t pushButton = 7;

  pinMode(pushButton, INPUT);

  for (;;) // A Task shall never return or exit.
  {
    int buttonState = digitalRead(pushButton);

    if ( buttonState == 1 )
    {     
      if (xQueueSend(cola_Mensaje, &boton, 2000 / portTICK_PERIOD_MS) != pdTRUE)
      {
        Serial.println("error");  
      }
          
      vTaskDelay( 500 / portTICK_PERIOD_MS );
    }
    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}



void Actuador(void * pvParameters) {

  Serial.println("Actuador");

  
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  
  bool bOn_Off = false;
  bool bAlta_Baja = false;
  bool bBomba = false;
  
  for (;;) 
  {
    struct Boton boton; 
   
    if (xQueueReceive(cola_Mensaje, &boton, portMAX_DELAY) == pdPASS) {
      //Serial.print("Id: ");
      //Serial.println(boton.id);   

         if(boton.id == 0)
         {
            if(digitalRead(13) == LOW){            
              digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)  
              Serial.println("Aire prendido");
            }                        
            else{            
              digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW 
              Serial.println("Aire Apagado");
            }             
            
            vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second                 
         }

         if(boton.id == 1)
         {
            if(digitalRead(12) == LOW){            
              digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)  
              Serial.println("Vel Baja");
            }                        
            else{            
              digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW 
              Serial.println("Vel Alta");
            }             
            
            vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second                 
         }
         
         if(boton.id == 2)
         {
            if(digitalRead(11) == LOW){            
              digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)  
              Serial.println("Bomba prendida");
            }                        
            else{            
              digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW 
              Serial.println("Bomba apagada");
            }             
            
            vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second                 
         }

         if(boton.id == 3)
         {
            //if(digitalRead(11) == LOW){            
              digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level) 
              digitalWrite(12, LOW); 
              Serial.println("Vel Baja con Bomba prendida");
            //}                        
            
            vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second                 
         }

         if(boton.id == 4)
         {
            //if(digitalRead(11) == LOW){            
              digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level) 
              digitalWrite(12, HIGH); 
              Serial.println("Vel Alta con Bomba prendida");
            //}                        
            
            vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second                 
         }
         
    }
  }

  vTaskDelay(1);
}
