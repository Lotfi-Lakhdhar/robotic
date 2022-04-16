
#include <Arduino.h>
#include <Servo.h>


#define channel_number 6  //set the number of channels
#define PIN_INTER 13


volatile uint64_t StartValue = 0;
volatile uint64_t TempVal=0;
volatile uint64_t PeriodCount=0;
volatile uint64_t temps = 0;
volatile byte last_CH_state;

volatile int ppm[channel_number];
volatile byte cur_chan_numb = 0;

/**************************************************/
volatile uint64_t interruptCounter = 0;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter += 5 ;
  portEXIT_CRITICAL_ISR(&timerMux);

}

/**************************************************/


void IRAM_ATTR isrPpm() {

  TempVal = interruptCounter; 

    if(digitalRead(PIN_INTER)){                             
            if(last_CH_state == 0){                         
              last_CH_state = 1;                           
              StartValue = TempVal;                      
            
            }
    }else if(last_CH_state == 1){                        
            last_CH_state = 0;                             
            ppm[cur_chan_numb] = TempVal - StartValue + 400;
                                  

          
            cur_chan_numb++;

            
            if(cur_chan_numb ==6){
               cur_chan_numb = 0;
               interruptCounter = 0;
               StartValue = 0;

              }
    }
    

  
}

/**************************************************/





//Define widths
int pwm_width_2 = 0;
int pwm_width_3 = 0;
int pwm_width_4 = 0;
int pwm_width_5 = 0;
int pwm_width_6 = 0;
int pwm_width_7 = 0;

Servo PWM2;
Servo PWM3;
Servo PWM4;
Servo PWM5;
Servo PWM6;
Servo PWM7;

                

/**************************************************/

void setup()
{

  Serial.begin(115200);
  
  //Set the pins for each PWM signal
  PWM2.attach(15);
  PWM3.attach(32);
  PWM4.attach(19);
  PWM5.attach(23);
  PWM6.attach(18);
  PWM7.attach(2);

/**************************************************/

  pinMode(PIN_INTER, INPUT_PULLDOWN);
  attachInterrupt(PIN_INTER, isrPpm, CHANGE);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 5, true);
  timerAlarmEnable(timer);

/**************************************************/ 

}



void loop()
{



    pwm_width_2 = ppm[0];     //PWM value on digital pin 15
    pwm_width_3 = ppm[1];     //PWM value on digital pin 2
    pwm_width_4 = ppm[2];     //PWM value on digital pin 0
    pwm_width_5 = ppm[3];     //PWM value on digital pin 4
    pwm_width_6 = ppm[4];     //PWM value on digital pin 16
    pwm_width_7 = ppm[5];     //PWM value on digital pin 17


//Now we write the PWM signal using the servo function
PWM2.writeMicroseconds(pwm_width_2);
PWM3.writeMicroseconds(pwm_width_3);
PWM4.writeMicroseconds(pwm_width_4);
PWM5.writeMicroseconds(pwm_width_5);
PWM6.writeMicroseconds(pwm_width_6);
PWM7.writeMicroseconds(pwm_width_7);

Serial.println(pwm_width_2); Serial.print("  "); 
Serial.print(pwm_width_3); Serial.print("  ");
Serial.print(pwm_width_4); Serial.print("  ");
Serial.print(pwm_width_5); Serial.print("  ");
Serial.print(pwm_width_6); Serial.print("  ");
Serial.print(pwm_width_7); Serial.print("  ");


}//Loop end
/**************************************************/
