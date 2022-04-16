#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"


////////////////////// PPM CONFIGURATION//////////////////////////
#define channel_number 6  //set the number of channels
#define sigPin 5  //set PPM signal output pin on the esp32
#define PPM_FrLen 27000  
#define PPM_PulseLen 400  //set the pulse length
//////////////////////////////////////////////////////////////////

volatile int ppm[channel_number];
volatile boolean state = true;
volatile byte cur_chan_numb = 0;
volatile unsigned int calc_rest = 0;

/**************************************************/
volatile uint32_t timerTicks = 1000000; // start 1000000 * 1 us = 1 s
volatile uint64_t StartValue = 0;
volatile uint64_t TempVal=0;

/**************************************************/


// The sizeof this struct should not exceed 32 bytes
struct MyData {
  byte throttle;
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
  // 'safe' values to use when no radio input is detected
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.AUX1 = 0;
  data.AUX2= 0;
  
  setPPMValuesFromData();
}

void setPPMValuesFromData()
{
  ppm[0] = map(data.throttle, 0, 255, 1000, 2000);
  ppm[1] = map(data.yaw,      0, 255, 1000, 2000);
  ppm[2] = map(data.pitch,    0, 255, 1000, 2000);
  ppm[3] = map(data.roll,     0, 255, 1000, 2000);
  ppm[4] = map(data.AUX1,     0, 255, 1000, 2000);
  ppm[5] = map(data.AUX2,     0, 255, 1000, 2000);  
  }

/**************************************************/


hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


 
void IRAM_ATTR onTimer() {
  
 portENTER_CRITICAL_ISR(&timerMux);
    
          if ( state) {
          //end pulse
          digitalWrite(sigPin, LOW); // turn pin 5 off. Could also use: digitalWrite(sigPin,LOW)
          
            timerTicks = PPM_PulseLen; 
            
          state = false;
          
        }
        else {
          //start pulse
          
          digitalWrite(sigPin, HIGH); // turn pin 5 on. Could also use: digitalWrite(sigPin,HIGH)
          state = true;
      
                if(cur_chan_numb >= channel_number) {
                  digitalWrite(sigPin, LOW); // turn pin 5 off. Could also use: digitalWrite(sigPin,LOW)
                  cur_chan_numb = 0;
                  calc_rest += PPM_PulseLen;
                  
                  timerTicks = PPM_FrLen - calc_rest; 
      
                  
                  calc_rest = 0;
                } else {
                  
                  timerTicks = ppm[cur_chan_numb] - PPM_PulseLen; 
            
                        
                  calc_rest += ppm[cur_chan_numb];
                  cur_chan_numb++;
                }     
        }
      
        timerAlarmWrite(timer, timerTicks, true); // // change timer ticks, autoreload true
  portEXIT_CRITICAL_ISR(&timerMux);

}

/**************************************************/ 


void setupPPM() {
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, LOW);  //set the PPM signal pin to the default state (off)

}

void setup()
{ 
  Serial.begin(115200);
  resetData();
  setupPPM();

 
 // Timer interrupt
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, timerTicks, true);
  timerAlarmEnable(timer);
  
  // Set up radio module
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS); // Both endpoints must have this set the same
  radio.openReadingPipe(1,pipeIn);
  radio.startListening();
}

/**************************************************/

unsigned long lastRecvTime = 0;

void recvData()
{  
  while ( radio.available() ) {        
    radio.read(&data, sizeof(MyData));
    lastRecvTime = millis();
  }
}

/**************************************************/

void loop()
{
  recvData();

  unsigned long now = millis();
  if ( now - lastRecvTime > 1000 ) {
    // signal lost?
    resetData();
  }
  
  setPPMValuesFromData();


Serial.print(ppm[0]); Serial.print("  ");
Serial.print(ppm[1]); Serial.print("  ");
Serial.print(ppm[2]); Serial.print("  ");
Serial.print(ppm[3]); Serial.print("  ");
Serial.print(ppm[4]); Serial.print("  ");
Serial.println(ppm[5]); Serial.print("  "); 





//Serial.print(data.throttle); Serial.print("  ");
//Serial.print(data.yaw); Serial.print("  ");
//Serial.print(data.pitch); Serial.print("  ");
//Serial.print(data.roll); Serial.print("  ");
//Serial.print(data.AUX1); Serial.print("  ");
//Serial.println(data.AUX2); Serial.print("  "); 



}

/**************************************************/
