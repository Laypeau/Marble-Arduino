int ledR = 2;
int ledG = 3;
int ledB = 4;

void setup()
{
	pinMode(ledR, OUTPUT);
	pinMode(ledG, OUTPUT);
	pinMode(ledB, OUTPUT);
	
	ledR = LOW;
	ledG = LOW;
	ledB = LOW;
}

void loop()
{
	ledR = HIGH;
	ledG = LOW;
	ledB = LOW;
	delay(10000);

	ledR = LOW;
	ledG = HIGH;
	ledB = LOW;
	delay(10000);
	
	ledR = LOW;
	ledG = LOW;
	ledB = HIGH;
	delay(10000);
}