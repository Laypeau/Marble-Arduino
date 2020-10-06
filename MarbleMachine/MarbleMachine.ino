#include <Stepper.h>
#include <HX711.h>

// Each region contains code relating to a single sensor/device
// Remember to adjust redThreshold for the room lighting

#pragma region [Colour sensor]
	enum Colour{Red, Green, Blue, Off};
	const int pinLedRed = 10;
	const int pinLedGreen = 11;
	const int pinLedBlue = 12;
	const int pinLedLdrIn = A5; //Make sure it's an analog pin

	int ambientRed = 0;
	int ambientGreen = 0;
	int ambientBlue = 0;

	int redThreshold = 200; //Adjust red threshold for accordingly

	const int pinLedStepper1 = A0;
	const int pinLedStepper2 = A1;
	const int pinLedStepper3 = A2;
	const int pinLedStepper4 = A3;
	Stepper stepperLed = Stepper(2048, pinLedStepper1, pinLedStepper3, pinLedStepper2, pinLedStepper4);
#pragma endregion

#pragma region [Weight sensor]
 	const float weightCalibration = 10100; //Regular marble ~1.0, metal marble ~3.1
	enum Marble{Plastic, Metal, Nothing};
	const int pinWeightDout = 4;
	const int pinWeightClk = 5;
	HX711 scale;

	const int pinWeightStepper1 = 6;
	const int pinWeightStepper2 = 7;
	const int pinWeightStepper3 = 8;
	const int pinWeightStepper4 = 9;
	Stepper stepperWeight = Stepper(2048, pinWeightStepper1, pinWeightStepper3, pinWeightStepper2, pinWeightStepper4);
#pragma endregion

#pragma region [Motor PWM]
	const int pinScrewMotor = 3; //Make sure it's a PWM pin
	int motorPWM = 150; //0-255
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

	#pragma region [Weight sensor]
	scale.begin(pinWeightDout, pinWeightClk);
	scale.set_scale(weightCalibration);
	scale.tare();

	pinMode(pinWeightStepper1, OUTPUT);
	pinMode(pinWeightStepper2, OUTPUT);
	pinMode(pinWeightStepper3, OUTPUT);
	pinMode(pinWeightStepper4, OUTPUT);

	stepperWeight.setSpeed(rotPerMin);
	#pragma endregion

	#pragma region [Motor PWM]
	pinMode(pinScrewMotor, OUTPUT);
	#pragma endregion

	Serial.begin(9600);
}

void loop()
{
	#pragma region [Colour sensor]
	int r = ReadAverage(Colour::Red, 10) - ambientRed;
	int g = ReadAverage(Colour::Green, 10) - ambientGreen;
	int b = ReadAverage(Colour::Blue, 10) - ambientBlue;
	Serial.print("Colour sensor reading: {R: ");
	Serial.print(r);
	Serial.print(", G: ");
	Serial.print(g);
	Serial.print(", B: ");
	Serial.print(b);
	Serial.println("}");

	if (r > redThreshold) //Adjust red threshold for accordingly
	{
		stepperLed.step(2048);
	}
	else
	{
		stepperLed.step(-2048);
	}
	#pragma endregion

	#pragma region [Weight sensor]
	switch (ReadMarble())
	{
	case Marble::Plastic:
		Serial.println("Weight sensor reading {Plastic}");
		stepperWeight.step(683);
		break;

	case Marble::Metal:
		Serial.println("Weight sensor reading {Metal}");
		stepperWeight.step(-683);
		break;
	
	default:
		Serial.println("Weight sensor reading {Nothing :(}");
		break;
	}
	#pragma endregion

	#pragma region [Motor PWM]
	analogWrite(pinScrewMotor, motorPWM);
	#pragma endregion
}

int ReadAverage(Colour colour, int iterations)
{
	int total = 0;
	for (int i = 0; i < iterations; i++)
	{
		SetColour(colour);
		delay(1);
		total += analogRead(pinLedLdrIn);
	}
	SetColour(Colour::Off);
	total = total/iterations;
	return total;
}

void SetColour(Colour colour)
{
	Serial.println(colour);
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

Marble ReadMarble()
{
	float reading = scale.get_units();
	Serial.print(reading);
	Serial.print(": ");
	if (reading >= 2.0f)
	{
		return Marble::Metal;
	}
	else if (reading >= 0.5f)
	{
		return Marble::Plastic;
	}
	else
	{
		return Marble::Nothing;
	}
}
