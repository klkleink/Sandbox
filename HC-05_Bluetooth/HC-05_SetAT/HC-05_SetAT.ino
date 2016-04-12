/*

AUTHOR: Hazim Bitar (techbitar)
DATE: Aug 29, 2013
LICENSE: Public domain (use at your own risk)
CONTACT: techbitar at gmail dot com (techbitar.com)

*/
/*leekl 12/24/15
 * HC-05 defaults to 9600 in both regular and command mode in slave mode
 * HC-06 defaults to 9600 in xxxxxxxxxxxx and always command mode
 * You must set the baud of the unit in BTSerial.begin(xxxx) if garbled data
 * 
 * To Test Functionality and Connect to device:
 * 
 * Notes: HC-05 set from AT commands to master mode (slave default) with 'key' 
 *        pin connected to arduino 9 as this sketch calls for.
 *        HC-06 has defaults set and happens to be a module with 3.3v IO so 
 *        a voltage divider into it's rx has been installed.
 *        Aside from that, RX to TX on each module respectively with PWR/GND 
 *        connected to arduino 5V/GND
 * AT                         Responds OK if wired and working
 * AT+ORGL                    Reset device to defaults
 * AT+RNAME? 2014,11,272827   Checks remote module name at address specified
 * AT+NAME?   AT+NAME=name    Checks/sets module name
 * AT+ROLE?   AT_ROLE=0,1,2   Checks/sets mode 0-Slave(default), 1-Master, 2-Slave-Loop
 * AT+LINK=2014,11,272827     Connects to slave remotely, responds OK or FAIL
 *                            Now setup the led to turn on/off with 1/0 in that 
 *                            device and they now operate the LED remotely.
 */
 
 /*
 12/25/15 leekl
 HC-05 - 9600 default, with KEY high (3.3v is fine) baud is stuck at 38400 in AT mode.
 Using Arduino's UART and connect HC TX to Ard SilkScreen 1 TX, HC TX to Ard Silkscreen 0; load blank sketch;
   VCC to +5V and GND to GND
 Issue AT and should receive OK with this module (must send NL/CR)
 Set name (max 10 char and OK if received):
   Analog controller 1 = AT+NAME=Meekster01
 Verify name AT+NAME?
 Default is slave role, set to master with:
   AT+ROLE?   AT_ROLE=0,1,2   Checks/sets mode 0-Slave(default), 1-Master, 2-Slave-Loop
 Scan for devices and connect to one (should aut-connect to last module, if available, on startup without KEY high):
   AT+RMAAD - Releases module from any current pair, responds OK whether there is one or not
   AT+INIT - To use as common SerialPortProfile, responds 17 if already in this mode so can move on
   AT+INQ - Responds with available devices and my slave will be 2014,11,272827 CHANGE COLONS TO COMMAS
   AT+LINK=2014,11,272827 - Attempts to connect to device and enters transparent com mode at baud specified in AT 
     mode. Responded OK twice...was able to send 1/0 and enable/disable LED 13 with a sample sketch.
     Modules change from single to double flashes when connected - faster single flash when AT mode exited
   Slave not aut-reconnecting after power cycle, gonna try cycling Master/Pulling KEY pin...
   
   
   Match baud for slave:
     AT+UART? or AT+UART=9600,0,0 - will change AT mode speed after power cycle
     
12/25/15 leekl

38400 with KEY high on boot (slow blink for AT)
AT - OK
AT+INIT - OK or error 17 already in that mode
AT+LINK=2014,11,272827 - OK or FAIL
pc baud to 9600 - rrrccchhhh, still talk to module at 38400 and slave is at 9600 passing "LED Oxx..." data
AT+CMODE=1 does autoconnect to last when KEY is disconnected - in NON ATmode I'm "LED Ox"ing with this 
  termianl 38400 (slave appears to detect baud), should set to 0 as it keeps you connected where you wanna
  be and especially once we have several bt modules in proximity of one another. CMODE=0 is default and 
  I have verified this does autonnect to the last AT+LINK'ed module after power cycle so long as KEY not high.
KEY held high after paired disables com and gives AT - OK but led connection status doesn't change



 */
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX | TX

void setup() 
{
  pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  digitalWrite(9, HIGH); 
  Serial.begin(9600);
  Serial.println("Enter AT commands:");
  BTSerial.begin(38400);  // HC-05 default speed for AT and Command mode 9600 (HC-06 may be 38400)
}

void loop()
{

  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (BTSerial.available())
    Serial.write(BTSerial.read());

  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
    BTSerial.write(Serial.read());
}


