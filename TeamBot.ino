#include <Servo.h>

int enablePin2 = 6;
int enablePin1 = 9;

int in1Pin2 = 7;
int in2Pin2 = 8;

int in1Pin1 = 10;
int in2Pin1 = 11;

const int trigPin = 3;
const int echoPin = 2;

const int IRpin = A0;
int distance = 0;

int count = 1;
int speed = 255 ;
boolean past = 0 ;
boolean present = 0;

const int serPin = 4;
Servo myservo;
bool stepbool = true;
int serPos = 90;
int nstep = 30;
int startDeg = 60;
int stopDeg = 120;
//int potPin = 0;
 
void setup()
{
  pinMode(in1Pin2, OUTPUT);
  pinMode(in2Pin2, OUTPUT);
  pinMode(enablePin2, OUTPUT);
  pinMode(in1Pin1, OUTPUT);
  pinMode(in2Pin1, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  pinMode(echoPin, INPUT);
  //pinMode(stopPin, INPUT);
  Serial.begin(9600);
  myservo.write(serPos);
  myservo.attach(serPin);
  
}
 
long microsecondsToInches(long microseconds){
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds){
  return microseconds / 29 / 2;
}

void loop(){
  //delay(1000);
  if (stepbool){
    serPos += nstep;
  }else{
     serPos -= nstep;
  }
  if (serPos==stopDeg){
    stepbool = false;
  }
  if (serPos==startDeg){
    stepbool = true;
  }
  myservo.write(serPos);

  //irIN: ir sensor reading (value from 0 to 1024)
  //irLimit: Limit at which the edge of the table has been encountered
  
  long duration, inches, cm, irIN, irLimit = 200;  
  //irIN = analogRead(IRpin);
  irIN = irRead();
  Serial.println("IR sensor: " + String(irIN));

  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  //delay(100);
  
  Serial.println(speed);
  
  if ( cm < 15){
     stopMotor();
     myservo.write(90);
     delay(1000);
     left(speed);
     delay(1550);
   }else{
  forward(speed);
  }
  
  if (irIN < irLimit){
     stopMotor();
     delay(1000);
     left(speed);
     delay(1550);
   }else{
  forward(speed);
   }  
}
 
void setMotor(int speed, boolean reverse,int enablePinSel,int motorPin1, int motorPin2){
  analogWrite(enablePinSel, speed);
  digitalWrite(motorPin1, ! reverse);
  digitalWrite(motorPin2, reverse);
}

void stopMotor(){
  setMotor(0,true,enablePin2,in1Pin2,in2Pin2);
  setMotor(0,true,enablePin1,in1Pin1,in2Pin1);
}
void forward(int speed){
   setMotor(speed, false,enablePin1,in1Pin1,in2Pin1);
  setMotor(speed, false,enablePin2,in1Pin2,in2Pin2);
}

void reverse(int speed){ 
  setMotor(speed, true,enablePin1,in1Pin1,in2Pin1);
  setMotor(speed, true,enablePin2,in1Pin2,in2Pin2);
}

void right(int speed){
  setMotor(speed, true,enablePin1,in1Pin1,in2Pin1);
  setMotor(speed, false,enablePin2,in1Pin2,in2Pin2);
}

void left(int speed){
  setMotor(speed, false,enablePin1,in1Pin1,in2Pin1);
  setMotor(speed, true,enablePin2,in1Pin2,in2Pin2);
}

// IR Sensor, model number: GP2Y0A41SK0F
// IR Sensor, range: 4 to 30 cm
// IR Sensor, measuring cycle: 16.5 ms
// IR Sensor, output: Anolog

// Take multiple readings, and average them out to reduce false readings
int irRead() {
  int averaging = 0;             //  Holds value to average readings

  // Get a sampling of 5 readings from sensor
  for (int i=0; i<5; i++) {
    distance = analogRead(IRpin);
    averaging = averaging + distance;
    delay(25);      // Wait 55 ms between each read
                    // According to datasheet time between each read
                    //  is -38ms +/- 10ms. Waiting 55 ms assures each
                    //  read is from a different sample
  }
  distance = averaging / 5;      // Average out readings
  return(distance);              // Return value
}
