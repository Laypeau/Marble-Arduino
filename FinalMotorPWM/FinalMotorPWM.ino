#pragma region [Motor PWM]
	const int pinScrewMotor = 9; //Make sure it's a PWM pin
	int motorPWM = 150; //0-255
#pragma endregion

void  setup()
{
	#pragma region [Motor PWM]
	pinMode(pinScrewMotor, OUTPUT);
	#pragma endregion
}

void loop()
{
	#pragma region [Motor PWM]
	analogWrite(pinScrewMotor, motorPWM);
	#pragma endregion

	if(Serial.available())
	{
		char temp = Serial.read();
		if(temp == 'a') motorPWM += 5;
		else if(temp == 'z') motorPWM -= 5;
	}
}