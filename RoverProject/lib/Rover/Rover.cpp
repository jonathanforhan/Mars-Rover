#include<Rover.h>

Rover::Rover(const int p_size)
{
	data_arr = new char[p_size];
    *packet_size = p_size;

    Init();
}

Rover::~Rover()
{
    delete[] data_arr;
    delete &packet_size;
    delete &IRinput;
}

void Rover::ServoInit(int a, int b)
{
    L_Servo.attach(a);
    R_Servo.attach(b);
}

void Rover::ServoCmd(int degree)
{
    L_Servo.write(degree);
    R_Servo.write(degree);
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

    for (int i = 0; i < *packet_size; i++)
    {
        data_arr[i] = 0;
    }
}

void Rover::Turn(int degree)
{
    if (degree > 90)
    {
        ServoCmd(120);
        analogWrite(R_Throttle, 162);
        analogWrite(L_Throttle, 255);
        delay(50 * (degree - 90));
    }
    else if (degree < 90)
    {
        ServoCmd(60);
        analogWrite(R_Throttle, 255);
        analogWrite(L_Throttle, 162);
        delay(50 * (-1 * (degree - 90)));
    }

    ServoCmd(90);
}

void Rover::Cruise(int distance)
{
    analogWrite(R_Throttle, 255);
    analogWrite(L_Throttle, 255);
    delay(distance * 100);
    analogWrite(R_Throttle, 100);
    analogWrite(L_Throttle, 100);
    delay(100);
}

void Rover::DataCheck()
{
    if (Serial.available())
    {
        String data = Serial.readString();
        for (int i = 0; i < *packet_size; i++)
        {
            data_arr[i] = data.charAt(i);
        }
        if (data_arr[*packet_size - 1] == 5)
        {
            starter = true;
        }
    }
    if (IRinput->decode(&results) && starter)
    {
        IRon = true;
    }
    else if (IRinput->decode(&results))
    {
        this->IRinput->resume();
    }
}


void Rover::ExecuteNav()
{
    if (starter && IRon)
    {
        for (int i = 0; i < *packet_size - 1; i += 2)
        {
            Turn(data_arr[i]);
            Cruise(data_arr[i + 1]);
        }
        analogWrite(R_Throttle, 0);
        analogWrite(L_Throttle, 0);
        data_arr[*packet_size - 1] = 0;
        starter = false;
        IRon = false;
    }
}

void Rover::Run()
{
    DataCheck();
    ExecuteNav();
}