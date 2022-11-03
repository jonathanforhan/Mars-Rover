#include<Rover.h>

void Rover::ServoCmd(int degree)
{
	L_Servo.write(degree);
	R_Servo.write(degree);
}

void Rover::Turn(int degree)
{
	if (degree > 90)
	{
		ServoCmd(120);
		analogWrite(R_Throttle, 162);
		analogWrite(L_Throttle, 255);
		delay(20 * (degree - 90));
	}
	else if (degree < 90)
	{
		ServoCmd(60);
		analogWrite(R_Throttle, 255);
		analogWrite(L_Throttle, 162);
		delay(20 * ( - 1 * (degree - 90)));
	}
	ServoCmd(90);
}

void Rover::Cruise(int distance)
{
	analogWrite(R_Throttle, 255);
	analogWrite(L_Throttle, 255);
	delay(distance * 50);
	analogWrite(R_Throttle, 100);
	analogWrite(L_Throttle, 100);
	delay(100);
}

void Rover::Init()
{
	ServoCmd(90);
	digitalWrite(R_Dir_A, HIGH);
	digitalWrite(R_Dir_B, LOW);
	digitalWrite(L_Dir_A, HIGH);
	digitalWrite(L_Dir_B, LOW);
	analogWrite(R_Throttle, 0);
	analogWrite(L_Throttle, 0);

	this->IRinput->enableIRIn();
}

void Rover::SerialToEEPROM(unsigned int packet_size)
{
	uint8_t holder[packet_size];
	if (Serial.available())
	{
		int n = Serial.readBytes(holder, packet_size);
		for (int i = 0; i < n; i++)
		{
			EEPROM.write(i, holder[i]);
		}
	}
}

void Rover::ReadEEPROM()
{
	for (int i = 0; i < packet_size; i++)
	{
		nav_data[i] = EEPROM.read(i);
	}
}

void Rover::CheckIR()
{
	if (IRinput->decode(&results) && nav_data[packet_size - 1] == 5)
	{
		this->IRon = true;
	}
	else if (IRinput->decode(&results))
	{
		this->IRinput->resume();
	}
}

void Rover::ExecuteNav()
{
	if (IRon == true && nav_data[packet_size - 1] == 5)
	{
		for (int i = 0; i < packet_size - 1; i += 2)
		{
			Turn(nav_data[i]);
			Cruise(nav_data[i + 1]);
		}
		analogWrite(R_Throttle, 0);
		analogWrite(L_Throttle, 0);
		EEPROM.write(packet_size - 1, 0);
		nav_data[packet_size - 1] = 0;
		this->IRon = false;
	}
}

Rover::Rover(const int p_size)
{
	nav_data = new uint8_t[p_size];
	this->packet_size = p_size;

	Init();
}

Rover::~Rover()
{
	delete[] nav_data;
	delete& IRinput;
}

void Rover::ServoInit(int a, int b)
{
	L_Servo.attach(a);
	R_Servo.attach(b);
}

void Rover::Run()
{
	SerialToEEPROM(packet_size);
	ReadEEPROM();
	CheckIR();
	ExecuteNav();
}