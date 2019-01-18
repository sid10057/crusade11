

int motorA1 = 4;
int motorA2 = 3;

int motorB1 = 6;
int motorB2 = 5;
char i;
int pwm1 = 9;
int pwm2 = 10;
int LED = 12;

void backward()
{
  Serial.println("Motion Backward");
  analogWrite(pwm1, 100);
  analogWrite(pwm2,103);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorA1, HIGH);


 digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);


}
void forward()
{
  Serial.println("Motion Forward");
  analogWrite(pwm1, 100);
  analogWrite(pwm2, 103);
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);


 digitalWrite(motorB2, LOW);
  digitalWrite(motorB1, HIGH);
  //ledblink();


}
void ledblink()
{ Serial.print("Blink");

  digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);
 
 
}

void left()
{
  Serial.println("Motion Left");
  analogWrite(pwm1, 100);
  analogWrite(pwm2, 103);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorA1, HIGH);

  digitalWrite(motorB2, LOW);
  digitalWrite(motorB1, HIGH);
  /*ledlow();*/


}
void right()
{
  Serial.println("Motion Right");


  analogWrite(pwm1, 100);
  analogWrite(pwm2, 103);
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);

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
 // delay(3000);
}



void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);

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
      Serial.println("Forward");
   /* delay(1000);
      stope();*/
    }




    else if (i == 'B')
    {
      backward();
    delay(500);
      stope();
    } 

    else if (i == 'L')
    {
      left();
      delay(300);
     stope();
    }
    else if (i == 'R')
    {
      right();
      delay(300);
      stope();
    }
    else if (i == 'S')
      stope();
    else if (i == 'K')
    {
      ledblink();
    }

    else {
      Serial.println("invalid command");
      }
   
  }

}
