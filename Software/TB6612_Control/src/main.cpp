#include <Arduino.h>

#define MOTOR_IN_1 4
#define MOTOR_IN_2 5

// put function declarations here:
void forward()
{
  digitalWrite(MOTOR_IN_1, HIGH);
  digitalWrite(MOTOR_IN_2, LOW);
}

void backward()
{
  digitalWrite(MOTOR_IN_1, LOW);
  digitalWrite(MOTOR_IN_2, HIGH);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR_IN_1, OUTPUT);
  pinMode(MOTOR_IN_2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(MOTOR_IN_2, HIGH);
  /*
  forward();
  delay(1000);
  backward();
  delay(1000);
  */
}

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
//}
