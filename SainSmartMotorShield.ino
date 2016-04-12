/***********************************************************************
* COPYRIGHT (c) 2015 MongoToys Company, Moscow, Idaho
*
*        FILE NAME:  Mongster.ino
*
* FILE DESCRIPTION: This file contains the functionality to control and
*                   test the functionality of the shield/motors/servos.
*                   The Adafruit/Sainsmart V1.x motor shields (with hard-
*                   ware latch) may use the following pins: 
*                    - 4,7,8,12(M1-M4 latch) 
*                    - 5,6,3,11(M1-M4 pwm)
*                    - 9,10(servo) Brown - GND; Red - 5V; Orange - Signal
*                   
*    FUNCTION LIST: Setmotor 
*
*     GLOBALS READ: 
*
* GLOBALS MODIFIED:
*
***********************************************************************/
#include <AFMotor.h>
#include <Servo.h> 

// DC motor on M2
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
AF_DCMotor Motors[4] = {motor1, motor2, motor3, motor4};
Servo servo1;
Servo servo2;

#define ACCELLERATION_INCREMENT 100
#define DEADZONE_PLUSMINUS 50
#define DEBUG 1

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("SainSmartMotorShield.ino v1.0");
  
  for (int x = 0; x < 4; x++)
  {
    if (DEBUG)
    {
      Serial.print("Setting RELEASE on Motor:");
      Serial.println(x);      
    }
    Motors[x].run(RELEASE);
  }

  // turn on servo
  servo1.attach(9);
  servo1.attach(10);
  if (DEBUG)
  {
    Serial.println("Attaching servo1 to pin 9:");
    Serial.println("Attaching servo2 to pin 10:");
  }  
}

int i;

// Test the DC motor, stepper and servo ALL AT ONCE!
void loop() 
{

  //Serial.println("Setting Servo1: fwd");
  //servo1.write(10);
  //delay(3000);
  //Serial.println("Setting Servo1: off");
  //servo1.write(94);
  //delay(3000);
  //Serial.println("Setting Servo1: backwards");
  //servo1.write(170);
  //delay(1000);
  //Servo1 forward/backward
  int x = 0;
  for (x = 10; x < 170; x+=5)
  {
    servo1.write(x);
    if (DEBUG)
    {
      Serial.print("Setting Servo1: ");
      Serial.println(x);
    }
    delay(1000);
  }
  
  
  //M1 forward/backward
  for (int x=0; x<5; x++)
  {
    SetMotor(0, 255);
    delay(1000);
  }
  for (int x=0; x<5; x++)
  {
    SetMotor(0, -255);
    delay(1000);
  }
  SetMotor(0, 0);  

  //M2 forward/backward
  for (int x=0; x<5; x++)
  {
    SetMotor(1, 255);
    delay(500);
  }
  for (int x=0; x<5; x++)
  {
    SetMotor(1, -255);
    delay(500);
  }
  SetMotor(1, 0);  

  //M3 forward/backward
  for (int x=0; x<5; x++)
  {
    SetMotor(2, 255);
    delay(500);
  }
  for (int x=0; x<5; x++)
  {
    SetMotor(2, -255);
    delay(500);
  }
  SetMotor(2, 0);

  //M4 forward/backward
  for (int x=0; x<5; x++)
  {
    SetMotor(3, 255);
    delay(500);
  }
  for (int x=0; x<5; x++)
  {
    SetMotor(3, -255);
    delay(500);
  }
  SetMotor(3, 0);
  /*  
  motor2.run(FORWARD);
  for (i=0; i<255; i++) {
    motor2.setSpeed(i);  
    delay(30);
 }
 
  for (i=255; i!=0; i--) {
    motor2.setSpeed(i);  
    delay(3);
 }
 
  motor2.run(BACKWARD);
  for (i=0; i<255; i++) {
    motor2.setSpeed(i);  
    delay(30);
 }
 
  for (i=255; i!=0; i--) {
    motor2.setSpeed(i);  
    delay(3);
 }
 */ 
}

/******************************************************************************
*FUNCTION NAME:   Setmotor
*
*  DESCRIPTION:   This function works with the motors connected to the Sain-
*                 smart motor shield.  Additional features: 
*                  - Stops between positive/negative throttle values.
*                  - Increments/decrements to set value on consecutive calls
*                    based on global parameter specified.
*                  - Ignores consecutive calls
*                  - Has serial debug support through global variable
*
*       INPUTS:   int MotorNumber = 0 to 3 referring to physical M1-4 shield designation
*                 unsigned int Speed = -255 to +255 determining throttle and direction
*
*      OUTPUTS:   
*
*  RETURN VALUE:  
*
*         NOTES:  
*
* -DATE-----NAME-----REVISION HISTORY
* 1/11/16   KLK      Rev A  
******************************************************************************/
void SetMotor(int motor, int value)
{
  //update only if needed to, but incrementally
  //this code keeps a condition of full throttl forward
  //to full throttle reverse, for example, from happening
  //without intermediate call(s) to slow down/stop the motor.
  //we were snapping hubs on Mongster3wheeled
  boolean shouldUpdate = false;
  static int lastValue[4] = {0, 0, 0, 0};
  if (value > lastValue[motor]) //ramping up
  {
    shouldUpdate = true;
    if (lastValue[motor] < 0 && value >= 0) //if switching directions, go to 0 value rather than ramp down
    {  
      lastValue[motor] = 0;
    }
    else
    {
      lastValue[motor] += ACCELLERATION_INCREMENT;
    }
    if (lastValue[motor] > value)  //don't increment past intended set value
    {
      lastValue[motor] = value;  
    }
  }
  else if (value < lastValue[motor])  //ramping down
  {
    shouldUpdate = true;
    if (lastValue[motor] > 0 && value <= 0) //if switching directions, go to 0 value rather than ramp down
    {  
      lastValue[motor] = 0;
    }
    else
    {    
      lastValue[motor] -= ACCELLERATION_INCREMENT;
    }
    if (lastValue[motor] < value) //don't increment past intended set value
    {
      lastValue[motor] = value;  
    }    
  }
  else  //same value as previous set - skip it
  {
    shouldUpdate = false;
  }
  
  //don't make identical calls, else update
  if (shouldUpdate == true)
  {
    if (lastValue[motor] < DEADZONE_PLUSMINUS && lastValue[motor] > -DEADZONE_PLUSMINUS)
    {
      Motors[motor].run(BACKWARD);
      Motors[motor].setSpeed(0);
      if (DEBUG)
      {
        Serial.print("M#:Speed ");
        Serial.print(motor);
        Serial.print(":");
        Serial.print(lastValue[motor]);
        Serial.println(" - DeadZone setting 0");         
      }
    }
    else if (lastValue[motor] > 0)
    {     
      Motors[motor].run(FORWARD);
      Motors[motor].setSpeed(lastValue[motor]);
      if (DEBUG)
      {
        Serial.print("M#:Speed ");
        Serial.print(motor);
        Serial.print(":");
        Serial.print(lastValue[motor]);
        Serial.println(" - Forward Throttle");         
      }      
    }
    else if (lastValue[motor] < 0)
    {
      int tmpValue = lastValue[motor] * -1;      
      Motors[motor].run(BACKWARD);
      Motors[motor].setSpeed(tmpValue); 
      if (DEBUG)
      {
        Serial.print("M#:Speed ");
        Serial.print(motor);
        Serial.print(":");
        Serial.print(tmpValue);
        Serial.println(" - Backward Throttle");         
      }
    }
  }
  else
  {
    if (DEBUG)
    {
      Serial.print("M#:Speed ");
      Serial.print(motor);
      Serial.print(":");
      Serial.print(lastValue[motor]);
      Serial.println(" - Skipped redundant setting");         
    }    
  }
}


