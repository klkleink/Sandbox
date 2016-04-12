#include <AFMotor.h>
#include <SoftwareSerial.h>   //Software Serial Port

//MTC Header stuff
#define MTC_SERIAL1			 0
#define MTC_BLUETOOTH1		 1

//HARDWARE SETTINGS
#define PIN_BT_RxD					2
#define PIN_BT_TxD					3

#define DRIVE_OFF 0
#define TURN_OFF 0

//PROJECT SETTINGS
#define BT_ENABLED  1
#define DEBUG 1

int DRIVE_SPEED = 127;  
int TURN_SPEED = 127;

AF_DCMotor motor1(1); //Left Drive when oriented with Mongster forward in front of you
AF_DCMotor motor2(2); //Middle "Steering" motor when oriented with Mongster forward in front of you
AF_DCMotor motor3(3); //Right Drive when oriented with Mongster forward in front of you
AF_DCMotor motor4(4); //Unused currently on Mongster1

SoftwareSerial blueToothSerial(PIN_BT_RxD, PIN_BT_TxD);

void setup()
{   
    Serial.begin(9600);



/*
    int i = 0;
    Serial.println("Set Font Off...");
    motor2.run(FORWARD);
    for (i=0; i<255; i++) {
    Serial.println(i);
    motor2.setSpeed(i);  
    delay(30);
    }   
*/     
    motor1.setSpeed(0);
    motor1.run(RELEASE);
    motor2.setSpeed(0);
    motor2.run(RELEASE);
    motor3.setSpeed(0);
    motor3.run(RELEASE);
    motor4.setSpeed(0);
    motor4.run(RELEASE);    
    MTCprintln("Motor Setup Complete", MTC_SERIAL1);  
    if (BT_ENABLED)
    {
       pinMode(PIN_BT_RxD, INPUT); 
       pinMode(PIN_BT_TxD, OUTPUT);
       MTCstartBlueTooth();
    }      
}

void loop()
{
  char recvChar;
  //Process incoming local Serial
  if (Serial.available())
  {
    recvChar = Serial.read();
    setMotor(recvChar);
  }

  //Process incoming bluetooth
  if (BT_ENABLED)
  {
    if (blueToothSerial.available())
    {
      recvChar = blueToothSerial.read();
      Serial.print(recvChar);  //echo BT data to serial monitor
      setMotor(recvChar);
    }
  }
}

void MTCprint(char *data, int PORT)
{
   if (PORT == MTC_BLUETOOTH1)
   {
      blueToothSerial.print(data);
   }
   Serial.print(data);
}

void MTCprintln(char *data, int PORT)
{
   MTCprint(data, PORT);
   MTCprint("\r\n", PORT);
}

void MTCstartBlueTooth()
{
   blueToothSerial.begin(38400);                           // Set BluetoothBee BaudRate to default baud rate 38400
   //MTCprint("\r\n+STBD=38400", MTC_BLUETOOTH1); 
   MTCprint("\r\n+STWMOD=0\r\n", MTC_BLUETOOTH1);             // set the bluetooth work in slave mode   
   MTCprint("\r\n+STNA=Mongster1\r\n", MTC_BLUETOOTH1);    // set the bluetooth name as "SeeedBTSlave"  
   MTCprint("\r\n+STOAUT=1\r\n", MTC_BLUETOOTH1);             // Permit Paired device to connect me
   MTCprint("\r\n+STAUTO=0\r\n", MTC_BLUETOOTH1);             // Auto-connection should be forbidden here
   delay(2000);                                            // This delay is required.  
   MTCprint("\r\n+INQ=1\r\n", MTC_BLUETOOTH1);                // make the slave bluetooth inquirable
   MTCprintln("The slave bluetooth is inquirable!", MTC_SERIAL1);
   delay(2000);                                            // This delay is required.
   while (blueToothSerial.available())
   {
     Serial.print(char (blueToothSerial.read()));
   }   
   Serial.println("EndGarbage"); 
   blueToothSerial.flush();   
}


void setMotor(char command)
{
  switch(command){
  case 'F':
    Serial.println("Set Forward..."); 
    motor1.setSpeed(DRIVE_SPEED);
    motor1.run(FORWARD);    
    motor2.setSpeed(TURN_OFF);
    motor2.run(RELEASE);   
    motor3.setSpeed(DRIVE_SPEED);
    motor3.run(BACKWARD);
    break;
  case 'B': 
    Serial.println("Set Backward..."); 
    motor1.setSpeed(DRIVE_SPEED);
    motor1.run(BACKWARD);
    motor2.setSpeed(TURN_OFF);
    motor2.run(RELEASE);
    motor3.setSpeed(DRIVE_SPEED);
    motor3.run(FORWARD);
    break;
  case 'L': 
    Serial.println("Set Left...");   
    motor1.setSpeed(DRIVE_SPEED);
    motor1.run(BACKWARD);
    motor2.setSpeed(DRIVE_SPEED);
    motor2.run(FORWARD);
    motor3.setSpeed(DRIVE_SPEED);
    motor3.run(BACKWARD);   
    break;
  case 'R':
    Serial.println("Set Right...");   
    motor1.setSpeed(DRIVE_SPEED);
    motor1.run(FORWARD);
    motor2.setSpeed(DRIVE_SPEED);
    motor2.run(BACKWARD);
    motor3.setSpeed(DRIVE_SPEED);
    motor3.run(FORWARD);   
    break;
  case 'S': 
    Serial.println("Set Stop..."); 
    motor1.setSpeed(DRIVE_OFF);
    motor1.run(RELEASE);
    motor2.setSpeed(TURN_OFF);
    motor2.run(RELEASE);
    motor3.setSpeed(DRIVE_OFF);
    motor3.run(RELEASE);    
    break; 
  case 'I':  //FR
    Serial.println("Set ForwardRight...");
    motor1.setSpeed(DRIVE_SPEED);
    motor1.run(FORWARD);
    motor2.setSpeed(DRIVE_SPEED);
    motor2.run(BACKWARD);
    motor3.setSpeed(DRIVE_SPEED);
    motor3.run(BACKWARD);              
    break; 
  case 'J':  //BR
    Serial.println("Set BackwardRight...");
    motor1.setSpeed(DRIVE_SPEED);
    motor1.run(BACKWARD);
    motor2.setSpeed(DRIVE_SPEED);
    motor2.run(FORWARD);
    motor3.setSpeed(DRIVE_SPEED);
    motor3.run(FORWARD);  
    break;        
  case 'G':  //FL 
    Serial.println("Set ForwardLeft...");  
    motor1.setSpeed(DRIVE_SPEED);
    motor1.run(FORWARD);
    motor2.setSpeed(DRIVE_SPEED);
    motor2.run(FORWARD);
    motor3.setSpeed(DRIVE_SPEED);
    motor3.run(BACKWARD);      
    break; 
  case 'H':  //BL
    Serial.println("Set BackwardLeft...");
    motor1.setSpeed(DRIVE_SPEED);
    motor1.run(BACKWARD);
    motor2.setSpeed(DRIVE_SPEED);
    motor2.run(BACKWARD);
    motor3.setSpeed(DRIVE_SPEED);
    motor3.run(FORWARD);        
    break;
  case 'W':  //Font ON 
    Serial.println("Set Font...");
        
    break;
  case 'w':  //Font OFF
    Serial.println("Set Font Off...");
    motor2.run(FORWARD);
    for (int x=0; x<255; x++) {
        motor2.setSpeed(x); 
        Serial.print(x); 
        delay(30);
    }
    motor2.run(BACKWARD);
    for (int x=0; x<255; x++) {
        motor2.setSpeed(x); 
        Serial.print(x); 
        delay(30);
    }        
    break;
  case 'U':  //Back ON 
    break;
  case 'u':  //Back OFF 
    break; 
  case 'D':  //Everything OFF 
    break;         
  default:  //Get velocity
    if(command=='q'){
      TURN_SPEED = 255;  //Full velocity
      DRIVE_SPEED = 255;
      //yellowCar.SetSpeed_4W(velocity);
    }
    else{ 
      //Chars '0' - '9' have an integer equivalence of 48 - 57, accordingly.
      if((command >= 48) && (command <= 57)){ 
        //Subtracting 48 changes the range from 48-57 to 0-9.
        //Multiplying by 25 changes the range from 0-9 to 0-225.
        DRIVE_SPEED = (command - 48)*25;     
        TURN_SPEED = (command - 48)*25;
        Serial.println(TURN_SPEED);   
        //yellowCar.SetSpeed_4W(velocity);
      }
    }
  }
}
