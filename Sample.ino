int motorA1=4;
int motorA2=3;
int motorB1=6;
int motorB2=5;
 char i;
int pwm1=9;
int pwm2=10;
//int LED= ;
void forward()
{
  Serial.println("Motion Forward");
  analogWrite(pwm1,255);
  
  analogWrite(pwm2,255);
  digitalWrite(motorA1,HIGH);
  digitalWrite(motorA2,LOW);
  
   digitalWrite(motorB1,HIGH);
  digitalWrite(motorB2,LOW);
  delay(3000);
  
}
void backward()
{
  Serial.println("Motion Backward");
    analogWrite(pwm1,255);
  analogWrite(pwm2,255);
  digitalWrite(motorA1,LOW);
  digitalWrite(motorA2,HIGH);
  
   digitalWrite(motorB1,LOW);
  digitalWrite(motorB2,HIGH);
  delay(3000);
  
}
void left()
{
  Serial.println("Motion Left");
    analogWrite(pwm1,255);
  analogWrite(pwm2,255);
  digitalWrite(motorA1,LOW);
  digitalWrite(motorA2,HIGH);
  
   digitalWrite(motorB1,HIGH);
  digitalWrite(motorB2,LOW);
  delay(3000);
  
}
void right()
{
  Serial.println("Motion Right");
    analogWrite(pwm1,255);
  analogWrite(pwm2,255);
  digitalWrite(motorA1,HIGH);
  digitalWrite(motorA2,LOW);
  
   digitalWrite(motorB1,LOW);
  digitalWrite(motorB2,HIGH);
  delay(3000);
  
}
void stop()
{
  Serial.println("Stop");
    analogWrite(pwm1,128);
  analogWrite(pwm2,255);
  digitalWrite(motorA1,LOW);
  digitalWrite(motorA2,LOW);
  
   digitalWrite(motorB1,LOW);
  digitalWrite(motorB2,LOW);
  delay(3000);
}

/*void BLINK()
{
  digitalWrite(LED,HIGH);
  delay(2000);
  digitalWrite(LED,LOW);
  
}*/

void setup()
{
  // put your setup code here, to run once:
   Serial.begin(9600);
  pinMode(motorA1,OUTPUT);
  pinMode(motorA2,OUTPUT);
  pinMode(motorB1,OUTPUT);
  pinMode(motorB2,OUTPUT);
  /*pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);*/
  
}

void loop()
{
  // put your main code here, to run repeatedly:
 
  
  
  if(Serial.available()>0)
  {i = Serial.read();
    if(i=='F')
       forward();
       else if(i=='B')
       backward();
       else if(i=='L')
       left();
       else if(i=='R')
       right();
         else if(i=='S')
         stop();
         
         else Serial.println("invalid command");
       }
  
}
