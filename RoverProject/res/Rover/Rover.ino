//Jonathan Forhan

/*
* this rover uses L293D chip
* ---------
* PMW -> L293D Pin
* 2   -> 9
* 3   -> 1
* 4   -> 7
* 5   -> 2
* 6   -> 15
* 7   -> 10
* ---------
* 10 -> IR receiver module
*/

#include <Rover.h>

Rover rover(9); // instantiate your rover obj with an int: packet_size of data from Serial, in this case 9

void setup()
{
  Serial.begin(9600); // Our GUI also runs 9600 baud
  rover.ServoInit(8, 9); // init servos on pins 8 & 9
}
void loop()
{
  rover.Run();
}