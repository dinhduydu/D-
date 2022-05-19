// Must define Arduino.h when we use Platform IO
#include<Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <task.h>

SemaphoreHandle_t sem; // handler for semaphore

TaskHandle_t ADCtask;   // task handlers
void IR_Sensor (void *pvParameters);

TaskHandle_t Ultrasonictask;
void Ultrasonic_HC_SR04 (void *pvParameters); 

void TaskBlink(void *pvParameters);

void setup() 
{
  Serial.begin(9600); // Enable serial communication library.

 // Create tasks
   xTaskCreate(IR_Sensor, // Task function
              "ADC", // Task name
              128, // Stack size 
              NULL, 
              1, // Priority
              &ADCtask );
              
   xTaskCreate(Ultrasonic_HC_SR04, // Task function
              "Ultrasonic", // Task name
              128, // Stack size 
              NULL, 
              1, // Priority
              &Ultrasonictask);   

    xTaskCreate(TaskBlink,// Task function
              "Blink", // Task name
              128,// Stack size 
              NULL,
              1,// Priority
              NULL);
    
    /* The semaphore cannot be used before it is created using a call to xSemaphoreCreateCounting(). 
    The maximum value to which the semaphore can count in this example case is set to 1, 
    and the initial value assigned to the count is set to 1. */
    sem = xSemaphoreCreateCounting(1,1);
    vTaskStartScheduler();
}


void loop() {}

void IR_Sensor(void *pvParameters)
{ //adc task
  (void) pvParameters;
  for (;;) 
    { 
        int adc; //variable to store adc value

        adc = analogRead(A0);  //Read adc value from A0 channel and store it in adc variable 
        xSemaphoreTake(sem, portMAX_DELAY);  // acquire the semaphore
        delay(800);
       
        //print on the virtual terminal
        Serial.print("ADC : ");
        Serial.print(adc);    
        Serial.println();
  
        xSemaphoreGive(sem); // give the sem to Ultrasonic_HC_SR04 which is the ultrasonic task
      vTaskDelay(1);
  }
}

void Ultrasonic_HC_SR04(void *pvParameters)
{   //Ultrasonic
    (void) pvParameters;
    for (;;) 
    {
        long distance,duration; // variables for storing data
        pinMode(A1, OUTPUT);   //trigger pin
        pinMode(A2, INPUT);    //echo pin 

        digitalWrite(A1, LOW);
        delayMicroseconds(2);
        digitalWrite(A1, HIGH);
        delayMicroseconds(10);
        digitalWrite(A1, LOW);
  
        duration = pulseIn(A2, HIGH);   // how long pin is high
        distance = (duration/2) /29.1; //the duration value is to be divided by 2, since the signal bounces back after sending it and multiplies by speed of sound
        xSemaphoreTake(sem, portMAX_DELAY); //acquire the semaphore

        delay(800);

        Serial.print("Distance:"); // display it on the virtual terminal
        Serial.print(distance);
        Serial.print("cm");
        Serial.println();

        xSemaphoreGive(sem); //release the semaphore
        vTaskDelay(1);
  }
}

void TaskBlink(void *pvParameters)
{
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  for (;;)
  {
    digitalWrite(12,HIGH);
    delay(200);
    digitalWrite(12,LOW);
    delay(200);
  }
}
