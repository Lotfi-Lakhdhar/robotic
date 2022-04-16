 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*Create a unique pipe out. The receiver has to 
  wear the same unique code*/
  
const uint64_t pipeOut = 0xE8E8F0F0E1LL;

RF24 radio(12, 14, 26, 25, 27);

// The sizeof this struct should not exceed 32 bytes
// This gives us up to 32 8 bits channals
struct MyData {
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte AUX1;
  byte AUX2;
};

MyData data;

void resetData() 
{
  //This are the start values of each channal
  // Throttle is 0 in order to stop the motors
  
    
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.AUX1 = 0;
  data.AUX2 = 0;
}

void setup()
{
  pinMode(16, INPUT);
  pinMode(17, INPUT);

  //Start everything up
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  resetData();
}

/**************************************************/

// Returns a corrected value for a joystick position that takes into account
// the values of the outer extents and the middle of the joystick range.
int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if ( val < middle )
    val = map(val, lower, middle, 0, 128);
  else
    val = map(val, middle, upper, 128, 255);
  return ( reverse ? 255 - val : val );
}

void loop()
{
  // The calibration numbers used here should be measured 
  // for your joysticks till they send the correct values.
  data.throttle = mapJoystickValues( analogRead(15), 13, 2047, 4095, true );
  data.yaw      = mapJoystickValues( analogRead(2),  1, 2047, 4095, true );
  data.pitch    = mapJoystickValues( analogRead(0), 12, 2047, 4095, true );
  data.roll     = mapJoystickValues( analogRead(4), 34, 2047, 4095, true );
  
  

 if(digitalRead(16)){
  
  data.AUX1     = 255;
  
  }else{
    
    data.AUX1     = 0;
    
    }

  if(digitalRead(17)){
    data.AUX2     = 255;
  }else{
    data.AUX2     = 0;
    }


  radio.write(&data, sizeof(MyData));
}
