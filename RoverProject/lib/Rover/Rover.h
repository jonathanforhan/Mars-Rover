#ifndef ROVER_H
#define ROVER_H

#include<Servo.h>
#include<IRremote.h>
#include<Arduino.h>

class Rover
{
private:
	void ServoCmd(int degree);
	void Init();
	void Turn(int degree);
	void Cruise(int distance);
	void DataCheck();
	void ExecuteNav();

	char* data_arr;
	int* packet_size = new int;
	bool starter = false;
	bool IRon = false;

	int L_Throttle = 3; int R_Throttle = 2;
	int L_Dir_A = 5;	int L_Dir_B = 4;
	int R_Dir_A = 7;	int R_Dir_B = 6;
	int IRPin = 10;

	Servo L_Servo;
	Servo R_Servo;

	IRrecv* IRinput = new IRrecv(IRPin);
	decode_results results;
public:
	Rover(const int packet_size);
	~Rover();

	void ServoInit(int a, int b);

	void Run();
};

#endif