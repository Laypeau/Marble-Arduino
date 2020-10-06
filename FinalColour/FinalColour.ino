#include <Stepper.h>
#include <HX711.h>

#pragma region [Colour sensor]
	enum Colour{Red, Green, Blue, Off};
  const int pinLedRed = 10;
  const int pinLedGreen = 11;
  const int pinLedBlue = 12;
  const int pinLedLdrIn = A5; //analog pin

	int ambientRed = 0;
	int ambientGreen = 0;
	int ambientBlue = 0;

  const int pinLedStepper1 = A0;
  const int pinLedStepper2 = A1;
  const int pinLedStepper3 = A2;
  const int pinLedStepper4 = A3;
	Stepper stepperLed = Stepper(2048, pinLedStepper1, pinLedStepper3, pinLedStepper2, pinLedStepper4);
#pragma endregion

	const int rotPerMin = 10;

void setup()
{
	#pragma region [Colour sensor]
	pinMode(pinLedRed, OUTPUT);
	pinMode(pinLedGreen, OUTPUT);
	pinMode(pinLedBlue, OUTPUT);
	pinMode(pinLedLdrIn, INPUT);
	stepperLed.setSpeed(rotPerMin);

	ambientRed = ReadAverage(Colour::Red, 10);
	ambientGreen = ReadAverage(Colour::Green, 10);
	ambientBlue = ReadAverage(Colour::Blue, 10);
	#pragma endregion

	Serial.begin(9600);
}

void loop()
{
	delay(10);
	#pragma region [Colour sensor]
	int r = ReadAverage(Colour::Red, 10) - ambientRed;
	int g = ReadAverage(Colour::Green, 10) - ambientGreen;
	int b = ReadAverage(Colour::Blue, 10) - ambientBlue;
	Serial.print("Reading {R: ");
	Serial.print(r);
	Serial.print(", G: ");
	Serial.print(g);
	Serial.print(", B: ");
	Serial.print(b);
	Serial.println("}");

	if (r > 200)
	{
		stepperLed.step(2048);
	}
	else
	{
		stepperLed.step(-2048);
	}
	#pragma endregion
}

int ReadAverage(Colour colour, int iterations)
{
	int total = 0;
	for (int i = 0; i < iterations; i++)
	{
		SetColour(colour);
		delay(25);
		total += analogRead(pinLedLdrIn);
	}
	SetColour(Colour::Off);
	total = total/iterations;
	return total;
}

void SetColour(Colour colour)
{
	switch (colour)
	{
	case Colour::Red:
		digitalWrite(pinLedRed, HIGH);
		digitalWrite(pinLedGreen, LOW);
		digitalWrite(pinLedBlue, LOW);
		break;

	case Colour::Green:
		digitalWrite(pinLedRed, LOW);
		digitalWrite(pinLedGreen, HIGH);
		digitalWrite(pinLedBlue, LOW);
		break;

	case Colour::Blue:
		digitalWrite(pinLedRed, LOW);
		digitalWrite(pinLedGreen, LOW);
		digitalWrite(pinLedBlue, HIGH);
		break;
	
	default:
		digitalWrite(pinLedRed, LOW);
		digitalWrite(pinLedGreen, LOW);
		digitalWrite(pinLedBlue, LOW);
		break;
	}
}
