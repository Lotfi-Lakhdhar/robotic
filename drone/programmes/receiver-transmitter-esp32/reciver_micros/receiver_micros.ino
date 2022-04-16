#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Servo.h>

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

void setup()
{
  //Set the pins for each PWM signal
  PWM2.attach(15);
  PWM3.attach(2);
  PWM4.attach(0);
  PWM5.attach(4);
  PWM6.attach(16);
  PWM7.attach(17);

  //Configure the NRF24 module
  resetData();
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);  
  radio.openReadingPipe(1,pipeIn);
  
  //we start the radio comunication
  radio.startListening();




  
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


//Now we write the PWM signal using the servo function
PWM2.writeMicroseconds(pwm_width_2);
PWM3.writeMicroseconds(pwm_width_3);
PWM4.writeMicroseconds(pwm_width_4);
PWM5.writeMicroseconds(pwm_width_5);
PWM6.writeMicroseconds(pwm_width_6);
PWM7.writeMicroseconds(pwm_width_7);


}//Loop end
/**************************************************/
