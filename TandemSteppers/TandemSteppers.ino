#include <Stepper.h>
#include <AccelStepper.h>

int analogIn = A7;
const int stepsPerRev = 2048;
const int rotPerMin = 10; //Single 5v stepper has max RPM of ~10-15

int MotorInterfaceType = 4;

#pragma region 
int in1A = 2;
int in2A = 3;
int in3A = 4;
int in4A = 5;
Stepper stepperLed = Stepper(stepsPerRev, in1A, in3A, in2A, in4A);
int posA = 0;

int in1B = 6;
int in2B = 7;
int in3B = 8;
int in4B = 9;
Stepper stepperB = Stepper(stepsPerRev, in1B, in3B, in2B, in4B);
int posB = 0;
#pragma endregion

void setup()
{
	pinMode(analogIn, INPUT);
	Serial.begin(9600);

	stepperLed.setSpeed(rotPerMin);
	stepperB.setSpeed(rotPerMin);
}

void loop()
{
	int calculated = ((float)analogRead(analogIn) / (float)1024) * stepsPerRev; //1024 is analog max

	if (posA != calculated)
	{
		stepperLed.step(posA - calculated);
	}
	posA = calculated;

	
	if (posB != stepsPerRev - calculated)
	{
		stepperB.step(posB - calculated);
	}
	posB = calculated;

	Serial.println(calculated);
}
