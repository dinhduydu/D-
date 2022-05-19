/*
Code không sử dụng cảm biến siêu âm để tránh vật cản
**********************************************************************/
#include<Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <limits.h>
#include <SoftwareSerial.h>

const byte RX = 2;
const byte TX = 3;
SoftwareSerial mySerial(RX,TX);

#define PWM_MOTOR_1 6
#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 10
#define PWM_MOTOR_2 5
#define IR_sensor A2

void driveTask(void *pvParameters);
void checkTask(void *pvParameters);
/* Declare a variable that is used to hold the handle of driveTask . */
static TaskHandle_t driveTaskHandle = NULL;
/* Declare a variable that is used to hold the handle of checkTask . */
static TaskHandle_t checkTaskHandle = NULL;
void setup()
{
//Serial Begin at 9600 Baud
mySerial.begin(9600);
pinMode(IR_sensor,INPUT);
pinMode(PWM_MOTOR_1, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
pinMode(PWM_MOTOR_2, OUTPUT);
pinMode(ECHO,INPUT);
pinMode(TRIG,OUTPUT);
xTaskCreate(
driveTask
, "Drive"
, 128
, NULL
, 2
, &driveTaskHandle); /* This task uses the task handle. */
xTaskCreate(
checkTask
, "Check"
, 128
, NULL
, 4
, &checkTaskHandle); /* This task uses the task handle. */
/* Start the scheduler so the tasks start executing. */
vTaskStartScheduler();/* Start the created tasks running. */
}
/*
* driveTask runs the state machine:
* Go FORWARD until Zone 3 is detected
* (blocks in FORWARD state until notified from distanceTask that obstacle is
ahead)
* then change to REVERSE state and reverse
* for about 1 second and enter TURN state. Do a short turn in TURN state and
return to FORWARD state.
*/
void driveTask(void *pvParameters __attribute__((unused)))
{
// Lệnh đọc dữ liệu từ chân Rx (từ ESP8266)
if (mySerial.available())
{// Có dữ liệu gửi đến
String RxBuffer = "";
while(mySerial.available())
{
RxBuffer = mySerial.readString();// Đọc dữ liệu nhận được
}if(RxBuffer == "1")// Start A = 1
{
system_turn_left();
}
else if (RxBuffer == "2")// Start B = 2
{
system_turn_right();
}
}
xTaskNotifyWaitIndexed( 1, // "allway ưaits for notifications on index 1" OR
"Wait for 1th notification"
ULONG_MAX, /* Clear all notification bits on
entry. */
ULONG_MAX, /* Reset the notification value to
0 on exit. */
&ulNotifiedValue,/* Notified value pass out
in ulNotifiedValue. */
portMAX_DELAY /* Block indefinitely. */
);
}
void system_turn_left()
{
uint32_t ulNotifiedValue;
enum States{FORWARD,REVERSE,TURN};
States state=FORWARD;
int Start;
if((Start == 1))
{
_start();
}
/* As per most tasks, this task is implemented in an infinite loop. */
for(;;) // A Task shall never return or exit.
{
switch(state) // state là 1 giá trị hằng, có thể là biểu thức nhưng kết
quả phải là hằng số
{
case FORWARD:
forward();
setSpeed(110);
xTaskNotifyWaitIndexed( 0, // "allway ưaits for notifications on
index 0" OR "Wait for 0th notification"
ULONG_MAX, /* Clear all notification bits on
entry. */ULONG_MAX, /* Reset the notification value to
0 on exit. */
&ulNotifiedValue,/* Notified value pass out
in ulNotifiedValue. */
portMAX_DELAY /* Block indefinitely. */
);
state=REVERSE;
break;
case REVERSE:
reverse();
setSpeed(255);
vTaskDelay(pdMS_TO_TICKS((1000+random(-500, 50))));
state=TURN;
break;
case TURN:
turnLeft();
setSpeed(110);
vTaskDelay(pdMS_TO_TICKS((1000+random(-500, 50))));
state=FORWARD;
break;
}
}
}
void system_turn_right()
{
uint32_t ulNotifiedValue;
enum States{FORWARD,REVERSE,TURN};
States state=FORWARD;
int Start;
if((Start == 1))
{
_start();
}
/* As per most tasks, this task is implemented in an infinite loop. */
for(;;) // A Task shall never return or exit.
{
switch(state) // state là 1 giá trị hằng, có thể là biểu thức nhưng kết
quả phải là hằng số
{
case FORWARD:
forward();setSpeed(110);
xTaskNotifyWaitIndexed( 0, // "allway ưaits for notifications on
index 0" OR "Wait for 0th notification"
ULONG_MAX, /* Clear all notification bits on
entry. */
ULONG_MAX, /* Reset the notification value to
0 on exit. */
&ulNotifiedValue,/* Notified value pass out
in ulNotifiedValue. */
portMAX_DELAY /* Block indefinitely. */
);
state=REVERSE;
break;
case REVERSE:
reverse();
setSpeed(255);
vTaskDelay(pdMS_TO_TICKS((1000+random(-500, 50))));
state=TURN;
break;
case TURN:
turnRight();
setSpeed(110);
vTaskDelay(pdMS_TO_TICKS((1000+random(-500, 50))));
state=FORWARD;
break;
}
}
}
void _start() {
digitalWrite(PWM_MOTOR_1,HIGH);
digitalWrite(PWM_MOTOR_2,HIGH);
}
void _stop() {
digitalWrite(PWM_MOTOR_1,LOW);
digitalWrite(PWM_MOTOR_2,LOW);
}
void forward() {
//_start();
digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW);
digitalWrite(IN3,HIGH);
digitalWrite(IN4,LOW);
}
void reverse() {
//_start();
digitalWrite(IN1,LOW);
digitalWrite(IN2,HIGH);
digitalWrite(IN3,LOW);
digitalWrite(IN4,HIGH);
}
void turnRight() {
//_start();
digitalWrite(IN1,HIGH);
digitalWrite(IN2,LOW);
digitalWrite(IN3,LOW);
digitalWrite(IN4,HIGH);
}
void turnLeft() {
//_start();
digitalWrite(IN1,LOW);
digitalWrite(IN2,HIGH);
digitalWrite(IN3,HIGH);
digitalWrite(IN4,LOW);
}
void setSpeed(int s) {
analogWrite(PWM_MOTOR_1,s);
analogWrite(PWM_MOTOR_2,s);
}
void checkTask(void *pvParameters __attribute__((unused)))
{
int check_point; // biến lưu cảm biến hồng ngoại line giữa
/* As per most tasks, this task is implemented in an infinite loop. */
for(;;)
{
if((analogRead(IR_sensor) >1000))
{
check_point++;
vTaskDelay(pdMS_TO_TICKS((1000+random(-500, 50))));mySerial.print(check_point);
delay(2000);
}
if (check_point = 3)
{
xTaskNotify(driveTaskHandle // Việc xử lý Task sẽ được thông
báo
,0 // Dữ liệu được gửi với thông báo
,eNoAction // Task nhận được thông báo mà giá
trị thông báo của nó không được cập nhật
// ulValue is not used and
xTaskNotify() always returns pdPASS in this case
// --> luôn gửi noti thành công
);
}
if(check_point=5)
{
_stop();
// and move driveTask to Block State
xTaskNotify(driveTaskHandle // Việc xử lý Task sẽ được thông báo
,1 // Dữ liệu được gửi với thông báo
,eNoAction // Task nhận được thông báo mà giá
trị thông báo của nó không được cập nhật
// ulValue is not used and
xTaskNotify() always returns pdPASS in this case
// --> luôn gửi noti thành công
);
}
}
}
void loop()
{
// empty
}