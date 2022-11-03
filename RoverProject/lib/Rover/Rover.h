#ifndef ROVER_H
#define ROVER_H

#include<Servo.h>
#include<IRremote.h>
#include<Arduino.h>
#include<EEPROM.h>

class Rover
{
private:
	char* nav_data;
	int packet_size;
	bool IRon = false;

	int L_Throttle = 3; int R_Throttle = 2;
	int L_Dir_A = 5;    int L_Dir_B = 4;
	int R_Dir_A = 7;    int R_Dir_B = 6;
	int IRPin = 10;

	Servo L_Servo;
	Servo R_Servo;

	IRrecv* IRinput = new IRrecv(IRPin);
	decode_results results;

	void ServoCmd(int degree);
	void Turn(int degree);
	void Cruise(int distance);
	void Init();
	void SerialToEEPROM(unsigned int packet_size);
	void ReadEEPROM();
	void CheckIR();
	void ExecuteNav();
public:
	Rover(const int packet_size);
	~Rover();
	void ServoInit(int a, int b);

	void Run();
};

#endif