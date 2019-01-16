


int motorA1 = 4;
int motorA2 = 3;

int motorB1 = 6;
int motorB2 = 5;
char i;
int pwm1 = 9;
int pwm2 = 10;
int LED = 12;

void left()
{
  Serial.println("Motion LEFT");
  analogWrite(pwm1, 80);
  analogWrite(pwm2, 82);
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);

  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);


}
void right()
{
  Serial.println("Motion Right");
  analogWrite(pwm1, 80);
  analogWrite(pwm2, 82);
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);

  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
  //ledblink();


}
void ledblink()
{ Serial.print("Blink");

  digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);
 
 
}

void backward()
{
  Serial.println("Motion Backward");
  analogWrite(pwm1, 80);
  analogWrite(pwm2, 82);
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);

  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
  /*ledlow();*/


}
void forward()
{
  Serial.println("Motion Forward");


  analogWrite(pwm1, 80);
  analogWrite(pwm2, 82);
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);

  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);




}
void stope()
{
  Serial.println("Stop");
  analogWrite(pwm1, 128);
  analogWrite(pwm2, 255);
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);

  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
  delay(3000);
}



void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  pinMode(LED, OUTPUT);
  /* digitalWrite(LED,LOW);*/


}

void loop()
{
  // put your main code here, to run repeatedly:


  if (Serial.available() > 0)
  {
    i = Serial.read();
    if (i == 'F')
    {

      forward();
      delay(1000);
      stope();
    }




    else if (i == 'B')
    {
      backward();
      delay(1000);
      stope();
    }

    else if (i == 'L')
    {
      left();
      delay(1000);
      stope();
    }
    else if (i == 'R')
    {
      right();
      delay(1000);
      stope();
    }
    else if (i == 'S')
      stope();
    else if (i == 'K')
    {
      ledblink();
    }

    else Serial.println("invalid command");
  }

}
