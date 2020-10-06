int motorPin = 9;
int increment = 5;
int amount = 150;

void setup() {

}

void loop(){
  analogWrite(motorPin,amount);
}
