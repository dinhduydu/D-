#include<Arduino.h>
#include <Arduino_FreeRTOS.h>
void TaskBlink1( void *pvParameters );

void TaskBlink2( void *pvParameters );

void TaskBlink3( void *pvParameters );

void TaskBlink4( void *pvParameters );

void setup() {
  Serial.begin(9600);
  xTaskCreate(
  TaskBlink1      /* Pointer to the function that implements the task. */
  ,"ct 1"         /* Text name given to the task. */
  ,64             /* The size of the stack that should be created for the task. */
  ,NULL           /* Task input parameter (can be NULL). */
                  /*
                  (void*) &xParameter          //A reference to xParameters is used as the task parameter.
                                                //This is cast to a void * to prevent compiler warnings. */
  ,1              /* The priority to assign to the newly created task. */
  ,NULL           /* It can be used to pass out a handle to the task being created. 
                    This handle can then be used to reference the task in API calls that, 
                    for example, change the task priority or delete the task (can be NULL).*/
                  /* &xHandle                   //The handle to the task being created will be placed in xHandle. */
  );

  xTaskCreate(
  TaskBlink2      /* Pointer to the function that implements the task. */
  ,"ct 2"         /* Text name given to the task. */
  ,64             /* The size of the stack that should be created for the task. */
  ,NULL           /* Task input parameter (can be NULL). */
                  /*
                  (void*) &xParameter          //A reference to xParameters is used as the task parameter.
                                                //This is cast to a void * to prevent compiler warnings. */
  ,1              /* The priority to assign to the newly created task. */
  ,NULL           /* It can be used to pass out a handle to the task being created. 
                    This handle can then be used to reference the task in API calls that, 
                    for example, change the task priority or delete the task (can be NULL).*/
                  /* &xHandle                   //The handle to the task being created will be placed in xHandle. */
  );

  xTaskCreate(
  TaskBlink3      /* Pointer to the function that implements the task. */
  ,"ct 3"         /* Text name given to the task. */
  ,64             /* The size of the stack that should be created for the task. */
  ,NULL           /* Task input parameter (can be NULL). */
                  /*
                  (void*) &xParameter          //A reference to xParameters is used as the task parameter.
                                                //This is cast to a void * to prevent compiler warnings. */
  ,1              /* The priority to assign to the newly created task. */
  ,NULL           /* It can be used to pass out a handle to the task being created. 
                    This handle can then be used to reference the task in API calls that, 
                    for example, change the task priority or delete the task (can be NULL).*/
                  /* &xHandle                   //The handle to the task being created will be placed in xHandle. */
  );

  xTaskCreate(
  TaskBlink4      /* Pointer to the function that implements the task. */
  ,"ct 4"         /* Text name given to the task. */
  ,64             /* The size of the stack that should be created for the task. */
  ,NULL           /* Task input parameter (can be NULL). */
                  /*
                  (void*) &xParameter          //A reference to xParameters is used as the task parameter.
                                                //This is cast to a void * to prevent compiler warnings. */
  ,1              /* The priority to assign to the newly created task. */
  ,NULL           /* It can be used to pass out a handle to the task being created. 
                    This handle can then be used to reference the task in API calls that, 
                    for example, change the task priority or delete the task (can be NULL).*/
                  /* &xHandle                   //The handle to the task being created will be placed in xHandle. */
  );

}
void loop() {}

void TaskBlink1(void *pvParameters)  
{
  pinMode(12, OUTPUT);
  while(1)
  {
    digitalWrite(12, HIGH);   
    Serial.println("Task1 hoat dong");
    Serial.println("led 12 ON");
    vTaskDelay( 250 / portTICK_PERIOD_MS ); 
    digitalWrite(12, LOW);    
    Serial.println("led 12 OFF");
    vTaskDelay( 250 / portTICK_PERIOD_MS ); 
  }
}

void TaskBlink2(void *pvParameters)  
{
  pinMode(11, OUTPUT);
  while(1)
  {
    digitalWrite(11, HIGH);   
    Serial.println("Task2 hoat dong");
    Serial.println("led 11 ON");
    vTaskDelay( 500 / portTICK_PERIOD_MS ); 
    digitalWrite(11, LOW);    
    Serial.println("led 11 OFF");
    vTaskDelay( 500 / portTICK_PERIOD_MS ); 
  }
}

void TaskBlink3(void *pvParameters)  
{
  pinMode(10, OUTPUT);
  while(1)
  {
    digitalWrite(10, HIGH);   
    Serial.println("Task3 hoat dong");
    Serial.println("led 10 ON");
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
    digitalWrite(10, LOW);    
    Serial.println("led 10 OFF");
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

void TaskBlink4(void *pvParameters)  
{
  pinMode(9, OUTPUT);
  while(1)
  {
    digitalWrite(9, HIGH);   
    Serial.println("Task4 hoat dong");
    Serial.println("led 9 ON");
    vTaskDelay( 2000 / portTICK_PERIOD_MS ); 
    digitalWrite(9, LOW);    
    Serial.println("led 9 OFF");
    vTaskDelay( 2000 / portTICK_PERIOD_MS ); 
  }
}