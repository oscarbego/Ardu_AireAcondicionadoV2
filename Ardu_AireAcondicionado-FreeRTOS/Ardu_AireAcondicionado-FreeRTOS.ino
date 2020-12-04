#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include <queue.h>

#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"


static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
static uint8_t ip[] = { 192, 168, 2, 34 };

#define PREFIX ""


struct Boton {
  int id;
  //char nombre[15]; //btn + Alta/Baja, bomba, On/Off
};

QueueHandle_t cola_Mensaje;


//void TaskWs( void *pvParameters );


//Led de encendido
//void TaskBlink( void *pvParameters );

//Lectura desde le Serial
//void TaskRead( void *pvParameters );

//Comandos
void on_off(void *pvParameter);
//void alta_baja(void *pvParameter);
//void bomba(void *pvParameter);

//Salida a relevadores
void Actuador(void *pvParameter);


void setup() {


  Ethernet.begin(mac, ip);

   cola_Mensaje = xQueueCreate(10, // Queue length
                              sizeof(struct Boton) // Queue item size
                              );                              
  Serial.begin(9600);
  
  delay(3000);
/*
  xTaskCreate(
    TaskWs
    ,  "TaskWs"   
    ,  128  
    ,  NULL
    ,  4  
    ,  NULL );
*/
    /*
  xTaskCreate(
    TaskBlink
    ,  "Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
*/

    xTaskCreate(
    Actuador
    ,  "Actuador"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

/*
  xTaskCreate(
    TaskRead
    ,  "AnalogRead"
    ,  128  // Stack size
    ,  NULL
    ,  2  // Priority
    ,  NULL );

*/

   xTaskCreate(
    on_off
    ,  "on_off"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

}

void loop()
{
  // Empty. Things are done in Tasks.
}



/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/


void on_off( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
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

/*
void alta_baja( void *pvParameters __attribute__((unused)) )  // This is a Task.
{

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
*/

/*
void bomba( void *pvParameters __attribute__((unused)) )  // This is a Task.
{

  struct Boton boton = { 2 };
  
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
*/


void Actuador(void * pvParameters) {

  Serial.println("Actuador");

  
  pinMode(13, OUTPUT);
  
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
            Serial.println("Se apaga o prende el Aire");

            //if(digitalRead(13) == LOW)            
              digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)                          
            //else            
            //  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW              
            
            vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second                 
         }

         if(boton.id == 1)
         {
            Serial.println("Se cambia la velocidad del Aire");
         }
         
         if(boton.id == 2)
         {
            Serial.println("Se prende o apaga la Bomba");
         }
    }
  }

  vTaskDelay(1);
}

/*
void helloCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  server.httpSuccess();

  if (type != WebServer::HEAD)
  {
    Serial.println("Se apaga o prende el Aire");
    
    P(helloMsg) = "<h1>Hello, World!</h1>";

    server.printP(helloMsg);
  }
}



void TaskWs(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  Serial.println("webserver 1");

  WebServer webserver(PREFIX, 80);


  Serial.println("webserver 2");

  
  webserver.setDefaultCommand(&helloCmd);
  
  Serial.println("webserver 3");
  
  webserver.addCommand("index.html", &helloCmd);

  Serial.println("webserver 4");
  
  webserver.begin();
 
  
  for (;;) // A Task shall never return or exit.
  {
    //webserver.processConnection();

    vTaskDelay(10);
  }
  
  vTaskDelay(10);
}
*/

/*
void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}
*/

void TaskRead(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
  struct Boton boton;
  String *b;
  
  Serial.println("TaskRead");
  for (;;)
  {
    if (Serial.available()) 
    {   
      //Serial.print(Serial.readString()); 
      *b = Serial.readString();
      //Serial.println(*b);
      boton.id = 0;
      
      if(b == "b0")
      {
          boton.id = 0;
          //Serial.println("...");
      }      
        
      if(b == "b1")      
        {
          boton.id = 1;
          //Serial.println("---");
        }

      if(b == "b2")      
        {
          boton.id = 2;
          //Serial.println("+++");
        }


      Serial.println(boton.id);
      if (xQueueSend(cola_Mensaje, &boton, 2000 / portTICK_PERIOD_MS) != pdTRUE)
      {
        Serial.println("error");  
      }
          
      vTaskDelay( 500 / portTICK_PERIOD_MS );     
   }    
    vTaskDelay(10);  // one tick delay (15ms) in between reads for stability
  }
  
}
