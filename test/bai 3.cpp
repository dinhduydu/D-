// Must define Arduino.h when we use Platform IO
#include<Arduino.h>
// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>
// Include queue support
#include <queue.h>

// Define a count and read_count value
int count = 0;
int read_count;
//Function Declaration
void TaskBlink(void *pvParameters);
void Generator(void *pvParameters);
void Display(void *pvParameters);
//Declaring a global variable of type QueueHandle_t 
QueueHandle_t Queue;

void setup() {

Queue=xQueueCreate(10, //Queue length
                        sizeof(int)); //Queue item size
if(Queue!=NULL){

  // Create task that consumes the queue if it was created.
  xTaskCreate(Display,// Task function
              "Display",// Task name
              128,// Stack size 
              NULL,
              1,// Priority
              NULL);

  // Create task that publish data in the queue if it was created.
  xTaskCreate(Generator, // Task function
              "Generator",// Task name
              128,// Stack size 
              NULL,
              1,// Priority
              NULL);
              
   // Create other task that publish data in the queue if it was created.
 
   xTaskCreate(TaskBlink,// Task function
              "Blink", // Task name
              128,// Stack size 
              NULL,
              1,// Priority
              NULL);

}
}

void loop() {}

void Generator(void *pvParameters)
{

  pinMode(11, INPUT_PULLUP);
  for (;;)
  {
    count++;;
    if (count == 256)
    {
      count =0;
      
    }
    while(1)
    {
      if (!digitalRead(11))
      {
        xQueueSend(Queue,&count,portMAX_DELAY);
        // One tick delay (15ms) in between reads for stability
        vTaskDelay(1);
      }
      else break;
    }
  }
}

/**
 * Display task.
 * Prints the received items from the queue to the serial monitor.
 */
void Display(void *pvParameters){


  // Init Arduino serial
  Serial.begin(9600);

  // Wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  while (!Serial) {
    vTaskDelay(1);
  }
  
  for (;;){
    if(xQueueReceive(Queue,&read_count,portMAX_DELAY) == pdPASS ){
      Serial.print("Giá trị:");
      Serial.println(read_count);
      
      bool bits[8];
      for (int i = 0; i <=7; i++)
      {
        int a = read_count %2;    // chia lấy dư, ví dụ 155%2 = 1 (còn 155/2 = 77), lấy 1 này đưa vào bit 0
        if ( a==1 )
        {
          bits[i] = HIGH;
        }
        else 
        {
          bits[i] = LOW;
        }
        read_count = read_count/2;
      }
      digitalWrite(2,bits[0]);
      digitalWrite(3,bits[1]);
      digitalWrite(4,bits[2]);
      digitalWrite(5,bits[3]);
      digitalWrite(6,bits[4]);
      digitalWrite(7,bits[5]);
      digitalWrite(8,bits[6]);
      digitalWrite(9,bits[7]);
    }
  }
}

/* 
 * Blink task. 
 * See Blink_AnalogRead example. 
 */
void TaskBlink(void *pvParameters){

  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  for (;;){
    digitalWrite(12,HIGH);
    delay(250);
    digitalWrite(12,LOW);
    delay(250);
  }
}
