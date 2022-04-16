
#include <Arduino.h>
#include <Servo.h>


#define channel_number 6  //set the number of channels

#define PIN_INTER_1 12
#define PIN_INTER_2 14
#define PIN_INTER_3 27
#define PIN_INTER_4 26
#define PIN_INTER_5 25
#define PIN_INTER_6 33



volatile uint64_t StartValue_1 = 0;
volatile uint64_t StartValue_2 = 0;
volatile uint64_t StartValue_3 = 0;
volatile uint64_t StartValue_4 = 0;
volatile uint64_t StartValue_5 = 0;
volatile uint64_t StartValue_6 = 0;

volatile uint64_t TempVal=0;
volatile uint64_t TempVal_1=0;
volatile uint64_t TempVal_2=0;
volatile uint64_t TempVal_3=0;
volatile uint64_t TempVal_4=0;
volatile uint64_t TempVal_5=0;
volatile uint64_t TempVal_6=0;


volatile byte last_CH_state_1;
volatile byte last_CH_state_2;
volatile byte last_CH_state_3;
volatile byte last_CH_state_4;
volatile byte last_CH_state_5;
volatile byte last_CH_state_6;

volatile int pwm[channel_number];
// volatile byte cur_chan_numb = 0;

/**************************************************/
volatile  uint64_t interruptCounter_1 = 0;
volatile  uint64_t interruptCounter_2 = 0;
volatile  uint64_t interruptCounter_3 = 0;
volatile  uint64_t interruptCounter_4 = 0;
volatile  uint64_t interruptCounter_5 = 0;
volatile  uint64_t interruptCounter_6 = 0;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter_1 += 5 ;
  interruptCounter_2 += 5 ;
  interruptCounter_3 += 5 ;
  interruptCounter_4 += 5 ;
  interruptCounter_5 += 5 ;
  interruptCounter_6 += 5 ;
  portEXIT_CRITICAL_ISR(&timerMux);

}

/**************************************************/


void IRAM_ATTR isrPwm() {

  TempVal_1 = interruptCounter_1; 
  TempVal_2 = interruptCounter_2; 
  TempVal_3 = interruptCounter_3; 
  TempVal_4 = interruptCounter_4; 
  TempVal_5 = interruptCounter_5; 
  TempVal_6 = interruptCounter_6; 

   ///////////////////////////////////////Channel 

    if(digitalRead(PIN_INTER_1)){                             
            if(last_CH_state_1 == 0){                        
              last_CH_state_1 = 1; 
              StartValue_1 = TempVal_1;                      
           
            }
    }else if(last_CH_state_1 == 1){                       
            last_CH_state_1 = 0; 
            pwm[0] = TempVal_1 - StartValue_1;
                                  
             interruptCounter_1 = 0;
             TempVal_1 = 0;
             StartValue_1 = 0;

           
            
    }
    
 ///////////////////////////////////////Channel 2

if(digitalRead(PIN_INTER_2)){                             
            if(last_CH_state_2 == 0){                        
              last_CH_state_2 = 1; 
              StartValue_2 = TempVal_2;                    
            
            }
    }else if(last_CH_state_2 == 1){                          
            last_CH_state_2 = 0; 
            pwm[1] = TempVal_2 - StartValue_2;
                                 
             interruptCounter_2 = 0;
             TempVal_2 = 0;
             StartValue_2 = 0;

           
            
    }


 
 ///////////////////////////////////////Channel 3
if(digitalRead(PIN_INTER_3)){                              
            if(last_CH_state_3 == 0){                        
              last_CH_state_3 = 1; 
              StartValue_3 = TempVal_3;                      
            
            }
    }else if(last_CH_state_3 == 1){                         
            last_CH_state_3 = 0; 
            pwm[2] = TempVal_3 - StartValue_3;
                                  
             interruptCounter_3 = 0;
             TempVal_3 = 0;
             StartValue_3 = 0;

           
            
    }

 
 ///////////////////////////////////////Channel 4


if(digitalRead(PIN_INTER_4)){                           
            if(last_CH_state_4 == 0){                        
              last_CH_state_4 = 1; 
              StartValue_4 = TempVal_4;                     
            
            }
    }else if(last_CH_state_4 == 1){                           
            last_CH_state_4 = 0; 
            pwm[3] = TempVal_4 - StartValue_4;
                                  
             interruptCounter_4 = 0;
             TempVal_4 = 0;
             StartValue_4 = 0;

          
            
    }
 
 ///////////////////////////////////////Channel 5


if(digitalRead(PIN_INTER_5)){                              
            if(last_CH_state_5 == 0){                         
              last_CH_state_5 = 1; 
              StartValue_5 = TempVal_5;                      
            
            }
    }else if(last_CH_state_5 == 1){                       
            last_CH_state_5 = 0; 
            pwm[4] = TempVal_5 - StartValue_5;
                                  
             interruptCounter_5 = 0;
             TempVal_5 = 0;
             StartValue_5 = 0;

          
            
    }
 
 ///////////////////////////////////////Channel 6


if(digitalRead(PIN_INTER_6)){                              
            if(last_CH_state_6 == 0){                        
              last_CH_state_6 = 1; 
              StartValue_6 = TempVal_6;                      
              
            }
    }else if(last_CH_state_6 == 1){                      
            last_CH_state_6 = 0; 
            pwm[5] = TempVal_6 - StartValue_6;
                                  
             interruptCounter_6 = 0;
             TempVal_6 = 0;
             StartValue_6 = 0;

           
            
    }


//----------**********************************************-------------/



  
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
  PWM2.attach(0);
  PWM3.attach(2);
  PWM4.attach(17);
  PWM5.attach(16);
  PWM6.attach(4);
  PWM7.attach(5);

  //PWM2.attach(15);
  //PWM3.attach(15);
  //PWM4.attach(15);
  //PWM5.attach(15);
  //PWM6.attach(15);
  //PWM7.attach(15);

/**************************************************/

  pinMode(PIN_INTER_1, INPUT_PULLDOWN);
  attachInterrupt(PIN_INTER_1, isrPwm, CHANGE);

  /**************************************************/

  pinMode(PIN_INTER_2, INPUT_PULLDOWN);
  attachInterrupt(PIN_INTER_2, isrPwm, CHANGE);

  /**************************************************/

  pinMode(PIN_INTER_3, INPUT_PULLDOWN);
  attachInterrupt(PIN_INTER_3, isrPwm, CHANGE);

  /**************************************************/

  pinMode(PIN_INTER_4, INPUT_PULLDOWN);
  attachInterrupt(PIN_INTER_4, isrPwm, CHANGE);

  /**************************************************/

  pinMode(PIN_INTER_5, INPUT_PULLDOWN);
  attachInterrupt(PIN_INTER_5, isrPwm, CHANGE);
  /**************************************************/

  pinMode(PIN_INTER_6, INPUT_PULLDOWN);
  attachInterrupt(PIN_INTER_6, isrPwm, CHANGE);
  
  /**************************************************/
  
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 5, true);
  timerAlarmEnable(timer);

/**************************************************/ 

}



void loop()
{



    pwm_width_2 = pwm[0];     //PWM value on digital pin 15
    pwm_width_3 = pwm[1];     //PWM value on digital pin 2
    pwm_width_4 = pwm[2];     //PWM value on digital pin 0
    pwm_width_5 = pwm[3];     //PWM value on digital pin 4
    pwm_width_6 = pwm[4];     //PWM value on digital pin 16
    pwm_width_7 = pwm[5];     //PWM value on digital pin 17
   


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
