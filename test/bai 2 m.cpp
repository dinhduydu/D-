#include<Arduino.h>
#include <Arduino_FreeRTOS.h>

#include <queue.h>
int count = 0 ;
int lReceivedValue;

QueueHandle_t xQueue1 = NULL;

// RANDOM NUMBER GENERATOR

void Live( void *pvParameters );
void Generator( void *pvParameters );
void Display( void *pvParameters );

void  setup() 
{
  Serial.begin(9600);
  // Create Queue
  /* Create a queue capable of containing 1 signed values. */
    xQueue1 = xQueueCreate( 1, /*The maximum number of items the queue can hold at any one time.*/ 
                            sizeof( signed ) );/*The size, in bytes, required to hold each item in the queue.*/
  if(xQueue1!=NULL)
  {
    //Create Task
    xTaskCreate(
    Live            /* Pointer to the function that implements the task. */
    ,"Live"         /* Text name given to the task. */
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
    Generator      /* Pointer to the function that implements the task. */
    ,"Generator"         /* Text name given to the task. */
    ,1024             /* The size of the stack that should be created for the task. */
    ,NULL           /* Task input parameter (can be NULL). */
                    /*
                    (void*) &xParameter          //A reference to xParameters is used as the task parameter.
                                                  //This is cast to a void * to prevent compiler warnings. */
    ,2              /* The priority to assign to the newly created task. */
    ,NULL          /* It can be used to pass out a handle to the task being created. 
                      This handle can then be used to reference the task in API calls that, 
                      for example, change the task priority or delete the task (can be NULL).*/
                    /* &xHandle                   //The handle to the task being created will be placed in xHandle. */
    );

    xTaskCreate(
    Display      /* Pointer to the function that implements the task. */
    ,"Display"         /* Text name given to the task. */
    ,1024            /* The size of the stack that should be created for the task. */
    ,NULL           /* Task input parameter (can be NULL). */
                    /*
                    (void*) &xParameter          //A reference to xParameters is used as the task parameter.
                                                  //This is cast to a void * to prevent compiler warnings. */
    ,2              /* The priority to assign to the newly created task. */
    ,NULL           /* It can be used to pass out a handle to the task being created. 
                      This handle can then be used to reference the task in API calls that, 
                      for example, change the task priority or delete the task (can be NULL).*/
                    /* &xHandle                   //The handle to the task being created will be placed in xHandle. */
    );
    vTaskStartScheduler();
  }
}
void loop() {}


/* Task Live is called every 200ms
  This task flashes the LED on pin 12
*/
void Live(void *pvParameters)  
{
  pinMode(12, OUTPUT);
  for(;;)
  {
    digitalWrite(12, HIGH);
    delay(250);    
    digitalWrite(12, LOW); 
    delay(250);
  }
}

void Generator(void *pvParameters)  
{
  (void) pvParameters;
  pinMode(11, INPUT);
  count++;
  if (count == 256)
  {
    count =0;
  }

  /* Two instances of this task are created so the value that is sent to the queue is passed in via the task parameter 
    - this way each instance can use a different value. 
      The queue was created to hold values of type int, so cast the parameter to the required type. */
    
  /* As per most tasks, this task is implemented within an infinite loop. */
  while(1)
  {
    
              /* Send an unsigned long.  Wait for 10 ticks for space to become available if necessary. */
        xQueueSend( xQueue1,  /*The handle to the queue on which the item is to be posted.*/
                    &count,    /*A pointer to the item that is to be placed on the queue. 
                                              The size of the items the queue will hold was defined when the queue was created, 
                                              so this many bytes will be copied from pvItemToQueue into the queue storage area. */
                    portMAX_DELAY );
    }
    
  
}
   


void Display(void *pvParameters)  
{
  (void) pvParameters;
  /* Declare the variable that will hold the values received from the queue. */
 
  /* This task is also defined within an infinite loop. */
  for( ;; )
  {
    /* Receive data from the queue.
    The first parameter is the queue from which data is to be received. 
    The queue is created before the scheduler is started, and therefore before this task runs for the first time.
    The second parameter is the buffer into which the received data will be placed. 
    In this case the buffer is simply the address of a variable that has the required size to hold the received data.
    The last parameter is the block time – the maximum amount of time that the task will remain in the Blocked state 
    to wait for data to be available should the queue already be empty. */
    if (xQueueReceive( xQueue1,            //The handle of the queue from which the data is being received (read) 
                              
                  &lReceivedValue, /*A pointer to the memory into which the received data will be copied.
                                                  The size of each data item that the queue holds is set when the queue is created. 
                                                  The memory pointed to by pvBuffer must be at least large enough to hold that many bytes*/
                              
                  portMAX_DELAY // 100ms
                  ) == pdPASS)
    // chờ đủ 100ms từ khi bắt đầu nhận ra có tin nhắn gửi qua, chờ xong 100ms thì đã nhận đủ tin nhắn
    {
      Serial.println("Giá trị của count là:");
      Serial.println (lReceivedValue);
      
    }
  }
}
