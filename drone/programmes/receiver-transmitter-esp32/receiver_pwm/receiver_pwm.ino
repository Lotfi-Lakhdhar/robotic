#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"


#define LED_PIN_1 15
#define LED_PIN_2 2
#define LED_PIN_3 0
#define LED_PIN_4 4
#define LED_PIN_5 16
#define LED_PIN_6 17



hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile uint32_t timerTicks = 50; // start 50 * 1 us = 0.05 ms
volatile uint16_t count = 0;    // 

//Define widths
volatile int pwm_width_2 = 0;
volatile int pwm_width_3 = 0;
volatile int pwm_width_4 = 0;
volatile int pwm_width_5 = 0;
volatile int pwm_width_6 = 0;
volatile int pwm_width_7 = 0; 

                      //We could use up to 32 channels
struct MyData {
byte throttle;      //We define each byte of data input, in this case just 6 channels
byte yaw;
byte pitch;
byte roll;
byte AUX1;
byte AUX2;
};

MyData data;


const uint64_t pipeIn = 0xE8E8F0F0E1LL;     //Remember that this code is the same as in the transmitter
RF24 radio(12, 14, 26, 25, 27);



void resetData()
{
//We define the inicial value of each data input
//3 potenciometers will be in the middle position so 127 is the middle from 254
data.throttle = 0;
data.yaw = 127;
data.pitch = 127;
data.roll = 127;
data.AUX1 = 0;
data.AUX2 = 0;
}

/**************************************************/

void IRAM_ATTR onTimer(){
  
  // Critical Code here
  portENTER_CRITICAL_ISR(&timerMux);


  count += 50;
  

  portEXIT_CRITICAL_ISR(&timerMux);


  if(count < 16000){

          ////Pin 15-Throttle////

          if(count < pwm_width_2){
            
            digitalWrite(LED_PIN_1, HIGH);
          }
      
           if(count > pwm_width_2){
            
            digitalWrite(LED_PIN_1, LOW);
          }

          ////Pin 2-Yaw////

          if(count < pwm_width_3){
            
            digitalWrite(LED_PIN_2, HIGH);
          }
      
           if(count > pwm_width_3){
            
            digitalWrite(LED_PIN_2, LOW);
          }
          
          ////Pin 0-Pitch////

          if(count < pwm_width_4){
            
            digitalWrite(LED_PIN_3, HIGH);
          }
      
           if(count > pwm_width_4){
            
            digitalWrite(LED_PIN_3, LOW);
          }


          ////Pin 4-Roll////


          if(count < pwm_width_5){
            
            digitalWrite(LED_PIN_4, HIGH);
          }
      
           if(count > pwm_width_5){
            
            digitalWrite(LED_PIN_4, LOW);
          }

          ////Pin 16-Aux1////

          if(count < pwm_width_6){
            
            digitalWrite(LED_PIN_5, HIGH);
          }
      
           if(count > pwm_width_6){
            
            digitalWrite(LED_PIN_5, LOW);
          }

          ////Pin 17-Aux2////


          if(count < pwm_width_7){
            
            digitalWrite(LED_PIN_6, HIGH);
          }
      
           if(count > pwm_width_7){
            
            digitalWrite(LED_PIN_6, LOW);
          }
    

 }

 if(count > 16000){
 
  count =0;
  
 }
 
}

void setup() {
  Serial.begin(115200);

     // Configure des sorties 
   pinMode(LED_PIN_1, OUTPUT);
   digitalWrite(LED_PIN_1, LOW);
   
   
   pinMode(LED_PIN_2, OUTPUT);
   digitalWrite(LED_PIN_2, LOW);

   
   
   pinMode(LED_PIN_3, OUTPUT);
   digitalWrite(LED_PIN_3, LOW);
   
   
   pinMode(LED_PIN_4, OUTPUT);
   digitalWrite(LED_PIN_4, LOW);

      pinMode(LED_PIN_1, OUTPUT);
   digitalWrite(LED_PIN_1, LOW);
   
   
   pinMode(LED_PIN_5, OUTPUT);
   digitalWrite(LED_PIN_5, LOW);

   
   
   pinMode(LED_PIN_6, OUTPUT);
   digitalWrite(LED_PIN_6, LOW);
   

   //Configure the NRF24 module
  resetData();
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);  
  radio.openReadingPipe(1,pipeIn);
  
  //we start the radio comunication
  radio.startListening();


   

  
  timer = timerBegin(0, 80, true);  
  timerAttachInterrupt(timer, &onTimer, true); 
  timerAlarmWrite(timer, timerTicks, true); 
  timerAlarmEnable(timer); // enable
}

/**************************************************/

unsigned long lastRecvTime = 0;

void recvData()
{
while ( radio.available() ) {
radio.read(&data, sizeof(MyData));
lastRecvTime = millis(); //here we receive the data
}
}

/**************************************************/

void loop()
{
recvData();
unsigned long now = millis();
//Here we check if we've lost signal, if we did we reset the values 
if ( now - lastRecvTime > 1000 ) {
// Signal lost?
resetData();
}

pwm_width_2 = map(data.throttle, 0, 255, 1000, 2000);     //PWM value on digital pin 15
pwm_width_3 = map(data.yaw,      0, 255, 1000, 2000);     //PWM value on digital pin 2
pwm_width_4 = map(data.pitch,    0, 255, 1000, 2000);     //PWM value on digital pin 0
pwm_width_5 = map(data.roll,     0, 255, 1000, 2000);     //PWM value on digital pin 4
pwm_width_6 = map(data.AUX1,     0, 255, 1000, 2000);     //PWM value on digital pin 16
pwm_width_7 = map(data.AUX2,     0, 255, 1000, 2000);     //PWM value on digital pin 17




Serial.print(pwm_width_2);
Serial.print("  ");
Serial.print(pwm_width_3);
Serial.print("  ");
Serial.print(pwm_width_4);
Serial.print("  ");
Serial.print(pwm_width_5);
Serial.print("  ");
Serial.print(pwm_width_6);
Serial.print("  ");
Serial.println(pwm_width_7);


//Serial.print(data.throttle); Serial.print("  ");
//Serial.print(data.yaw); Serial.print("  ");
//Serial.print(data.pitch); Serial.print("  ");
//Serial.print(data.roll); Serial.print("  ");
//Serial.print(data.AUX1); Serial.print("  ");
//Serial.println(data.AUX2); Serial.print("  "); 






}//Loop end
/**************************************************/
