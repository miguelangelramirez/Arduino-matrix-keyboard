// OneAnalogPin5switches
// Doug LaRue
// November 2008
//
// In some cases when you are running out of digial I/O pins for switches
// and want a simple solution for adding many switches with a small amount
// of added software. This example shows how 5 switches can be identified
// using 5 .5K ohm resistors and one analog pin. It's mostly only important
// that the resistor values are large enough so that there isn't too much
// current through them but enough to have a reliable and stable voltage
// divider circuit. I used .5K resistors because I had them close by but
// 1K, 2.2K, and 4.7K are very common and should work fine.
// If DEBUG_ON is defined, this code will set a pulse width value on the
// ouput digial/PWM pin to dim an attached LED based on the switch position.
// It also puts out strings on the serial port based on what switch is set.
//
// All code released under
// Creative Commons Attribution-Noncommercial-Share Alike 3.0


#define ERROR_WINDOW 50  // +/- this value
#define BUTTONDELAY 20
#define DEBUG_ON

int ledPin = 9;      // LED connected to digital pin 9
int analogPin = 3;   // switch circuit input connected to analog pin 3
long buttonLastChecked = 0; // variable to limit the button getting checked every cycle

void setup()
{
pinMode(ledPin, OUTPUT);   // sets the pin as output on a PWM capable pin
Serial.begin(115200);
}

void loop()
{
if( buttonLastChecked == 0 ) // see if this is the first time checking the buttons
buttonLastChecked = millis()+BUTTONDELAY;  // force a check this cycle
if( millis() - buttonLastChecked > BUTTONDELAY ) { // make sure a reasonable delay passed
if( int buttNum = buttonPushed(analogPin) ) {
 Serial.print("Button "); Serial.print(buttNum); Serial.println(" was pushed.");  
}
buttonLastChecked = millis(); // reset the lastChecked value
}
}

/* 
* Read 5( or more or less ) buttons using one analog pin and an equal number of
* resistors configured in series across +5 and ground.
* The concept is to create a voltage divider and the switches connect to one
* analog intput pin the voltage at some fixed position in the series circuit.
* The software just checks within a wide range of values and returns a button
* number if a button was determined to have been pushed.
*
* The internal 20K resistor is enabled so we can't just divide the 1023 by the
* number of resistors to get the range for each switch. With the 20K int R, the
* circuit looks like intR is in parallel with the switch resistors above the
* switch pushed. For example, SW2 would be 20K in parallel with 1K resulting in
* an equivalent R of .566K ohms. This then looks almost like there is only 4
* resistors in series and 1023 / 4 = ~255 and 1023-255=~767. Someone could probably
* spend the time to calculate this but I just used imperical measurements and a
* +/- 50 step range.
*
*
*   analogPin                 +5 V
*      |                         |
*      |                         \
*      ----------------          /   
*                     |          \    .5K
*                     |          /
*                     |          \
*                     |____ \____|
*                     |   SW1    |
*                     |          \
*                     |          /   
*                     |          \    .5K
*                     |          /
*                     |          \
*                     |____ \____|
*                     |   SW2    |
*                     |          |
*                     |          \
*                     |          /   
*                     |          \    .5K
*                     |          /
*                     |          \
*                     |____ \____|
*                     |   SW3    |
*                     |          |
*                     |          \
*                     |          /   
*                     |          \    .5K
*                     |          /
*                     |          \
*                     |____ \____|
*                     |   SW4    |
*                     |          |
*                     |          \
*                     |          /   
*                     |          \    .5K
*                     |          /
*                     |          \
*                     |          |
*                     |____ \____|
*                         SW5    |
*                                |
*                                |
*                              _____   
*                               ___     ground
*                                _
*
*/

int buttonPushed(int pinNum) {
int val = 0;         // variable to store the read value
digitalWrite((14+pinNum), HIGH); // enable the 20k internal pullup
val = analogRead(pinNum);   // read the input pin

#ifdef DEBUG_ON
 Serial.println(val);
 analogWrite(ledPin, val/4); // analog input 0-1023 while analogWrite 0-255
#endif
// we don't use the upper position because that is the same as the
// all-open switch value when the internal 20K ohm pullup is enabled.
//if( val >= 923 and val <= 1023 )
//  Serial.println("switch 0 pressed/triggered");
if( val >= 780 and val <= 880 ) {  // 830
 #ifdef DEBUG_ON
 Serial.println("switch 1 pressed/triggered");
 #endif
 return 1;
}
else if ( val >= (630-ERROR_WINDOW) and val <= (630+ERROR_WINDOW) ) { // 630
 #ifdef DEBUG_ON
 Serial.println("switch 2 pressed/triggered");
 #endif
 return 2;
}
else if ( val >= (430-ERROR_WINDOW) and val <= (430+ERROR_WINDOW) ) { // 430
 #ifdef DEBUG_ON
 Serial.println("switch 3 pressed/triggered");
 #endif
 return 3;
}
else if ( val >= (230-ERROR_WINDOW) and val <= (230+ERROR_WINDOW) ) { // 230
 #ifdef DEBUG_ON
 Serial.println("switch 4 pressed/triggered");
 #endif
 return 4;
}
else if( val >= 0 and val <= (20+ERROR_WINDOW) )  {
 #ifdef DEBUG_ON
 Serial.println("switch 5 pressed/triggered");    
 #endif
 return 5;
}
else
 return 0;  // no button found to have been pushed
}
