#include <Stepper.h>
#include <HX711.h>

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

	const int rotPerMin = 10;

void setup()
{
	Serial.begin(9600);

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
}

void loop()
{
	delay(500);
	#pragma region [Weight sensor]

	switch (ReadMarble())
	{
	case Marble::Plastic:
		Serial.println("Plastic");
		stepperWeight.step(683);
		break;

	case Marble::Metal:
		Serial.println("Metal");
		stepperWeight.step(-683);
		break;
	
	default:
		Serial.println("Read nothing :(");
		break;
	}
	#pragma endregion
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
