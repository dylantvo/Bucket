// This is the UNO Robotics Coordinate Motion Library. It includes the basic and advanced motion libraries, adapts them,
// and includes functions that allow for coordinate based motion.

// Written by Dylan Vo, March 2020
// Version 1.1

#include "constants.h"
#include "MaxbotixUltrasonics.h"


// BASIC MOTION
// use mov to move forward, reverse, strafe-left, and strafe-right
void mov(byte, float, long);
// use turn to rotate left and rotate right
void turn(byte, float, long);

// ADVANCED MOTION
// use these functions to perform variable speed motion, acceleration, and interrupt based motion
void vars( byte, float, long, float, byte, byte);
void acceleration(byte, float, long, int);
void ctc3_setup();
void ctc4_setup();
// use varsInt to move forward and reverse using timers. Using this function instead of mov lets you read sensor data as you move forward or reverse
void varsInt(byte, float, long, float, byte, byte);
// use varsIntTurn to rotate left and right using timers. Using this function instead of turn lets you read sensor data as you rotate
void varsIntTurn(byte, float, long, float, byte, byte);

// COORDINATE MOTION
// use these functions to move the robot in a coordinate plane overlaid on the field
void updateFacingAngle(byte, float);
void updateXandY(byte, float);
// use angleFromMe to compute the angle needed to rotate to face a coordinate
float angleFromMe(float, float);
// use distFromMe to compute the distance to a coordinate from the robot
float distFromMe(float, float);
// use orient to rotate towards a target heading
void orient(float, long);
// use lookAt to look at a target location
void lookAt(float, float, long);
// use GO to look at a target location and move to that location
void GO(float, float, long);
// use printLoc to print the robots location data for debugging
void printLoc();

// BASIC MOTION
void mov(byte dir, float dist, long del) {
  PORTL = dir;
  updateXandY(dir, dist);
  float stepf = dist * steps_per_inch;
  long steps = stepf;
  for (long i = 0; i < steps; i++) {
    delayMicroseconds(del);
    PORTL ^= allMotors; //toggle the step signal to continue moving
  }
  Serial.print("I used mov() to travel ");
  Serial.print(dist);
  Serial.println(" inches");
}

void movBetter(byte dir, float dist, long del) {
  PORTL = dir;
  updateXandY(dir, dist);
  float stepf = dist * steps_per_inch;
  long steps = stepf;
  for (int i = 0; i < steps * 0.9; i++) {
    del = del - 1;
    if (del <= 250) {
      del = 250;
    }
    delayMicroseconds(del);
    PORTL ^= allMotors;
  }
  for (int k = 0; k < steps * 0.1; k++) {
    del = del + 2;
    delayMicroseconds(del);
    PORTL ^= allMotors;
  }

}//end mov

void turn( byte dir, float dist, long del) {
  PORTL = dir;
  updateFacingAngle(dir, dist);
  float stepf = dist * steps_per_degree;
  long steps = stepf;
  for (long i = 0; i < steps; i++) {
    delayMicroseconds(del);
    PORTL ^= allMotors; //toggle the step signal to continue moving
  }
  Serial.print("I used turn() to rotate ");
  Serial.print(dist);
  Serial.println(" degrees.");

}

void turnBetter(byte dir, float dist, long del) {
  PORTL = dir;
  updateFacingAngle(dir, dist);
  float stepf = dist * steps_per_degree;
  long steps = stepf;
  for (int i = 0; i < steps * 0.9; i++) {
    del = del - 1;
    if (del <= 200) {
      del = 200;
    }
    delayMicroseconds(del);
    PORTL ^= allMotors;
  }
  for (int k = 0; k < steps * 0.1; k++) {
    del = del + 2;
    delayMicroseconds(del);
    PORTL ^= allMotors;
  }

}//end mov

void turnInsane(byte dir, float dist, long del) {
  PORTL = dir;
  updateFacingAngle(dir, dist);
  float stepf = dist * steps_per_degree;
  long steps = stepf;
  for (int i = 0; i < steps * 0.9; i++) {
    del = del - 1;
    if (del <= 175) {
      del = 175;
    }
    delayMicroseconds(del);
    PORTL ^= allMotors;
  }
  for (int k = 0; k < steps * 0.1; k++) {
    del = del + 2;
    delayMicroseconds(del);
    PORTL ^= allMotors;
  }

}//end mov

// ADVANCED MOTION
void vars( byte dir, float dist, long del, float ratio, byte master, byte slave) {
  PORTL = dir;
  float stepf = dist * steps_per_inch;
  long steps = stepf;

  long masterCount = 0;
  long slaveCount = 0;
  long stepCount = 0;

  float temp = del * ratio;
  long slaveDelay =  temp;

  while (stepCount < steps) {
    if (masterCount > del) {
      PORTL ^= master;//Step masterwheels
      //        PORTL ^= master;//Step masterwheels
      masterCount = 0;
      stepCount++;
    }
    if (slaveCount > slaveDelay) {
      PORTL ^= slave;//step slave wheels
      //     PORTL ^= slave;//step slave wheels
      slaveCount = 0;//rest
    }
    masterCount++;
    slaveCount++;
  }
}

void acceleration(byte dir, float dist, long del, int N) {
  updateXandY(dir, dist);
  float total_dis = N * (N + 1) / 2 * 3 * start_dis; // check commanded distance and number of steps
  if (total_dis > dist) {
    float m = sqrt((dist / start_dis) * 2 / 3); // correct if necessary
    N = m;
  }

  float mid_dis = dist - start_dis * 3 * N * (N + 1) / 2;

  for (int i = 1; i <= N; i++) {
    vars(dir, start_dis * i, del / i, straight, rightmotors, leftmotors);
  }

  vars(dir, mid_dis, del / N, straight, rightmotors, leftmotors);

  for (int i = N; i > 0; i--) {
    vars(dir, start_dis * 2 * i, del / i, straight, rightmotors, leftmotors);
  }
  Serial.print("I used acceleration() to travel ");
  Serial.print(dist);
  Serial.print("inches");
}

//3: Slave
ISR(TIMER3_COMPA_vect) { // timer compare ISR
  if (steps > 0) {
    PORTL ^= slaveWheels;
    //    PORTL ^= slaveWheels;
  }
  steps_counter++;
}

//4: Master
ISR(TIMER4_COMPA_vect) { // timer compare ISR
  if (steps > 0) {
    PORTL ^= masterWheels;
    //    PORTL ^= masterWheels;
  }

  steps--;
}

void ctc3_setup() {
  noInterrupts();
  TCCR3A = 0;  // clear counter control register
  TCCR3B = 0;
  TCNT3 = 0;
  // OCR3A = 16000; // compare match register – 1000 usecond delay
  // countCompareValue = delayinmicroseconds * 16
  // countCompareValue = 16000000 / prescaler / desired frequency
  TCCR3B |= (1 << WGM32); // count to compare mode
  TCCR3B |= (1 << CS30); // 1 prescaler
  TIMSK3 |= (1 << OCIE3A); // enable timer compare interrupt
  interrupts();
}

void ctc4_setup() {
  noInterrupts();
  TCCR4A = 0;  // clear counter control register
  TCCR4B = 0;
  TCNT4 = 0;

  OCR4A = 16000; // compare match register – 1000 usecond delay
  // countCompareValue = delayInMicroseconds * 16
  // countCompareValue = 16000000 / prescaler / desired frequency
  TCCR4B |= (1 << WGM42); // count to compare mode
  TCCR4B |= (1 << CS40); // 1 prescaler
  TIMSK4 |= (1 << OCIE4A); // enable timer compare interrupt
  interrupts();
}

void varsInt(byte dir, float dist, long del, float ratio, byte master, byte slave) {
  updateXandY(dir, dist);
  ctc3_setup();
  ctc4_setup();
  PORTL = dir;
  float stepf = dist * steps_per_inch;
  masterWheels = master;
  slaveWheels = slave;

  noInterrupts();
  //OCR4A = 16 * del - 1;//setup speed for master
  OCR4A = 16 * del;
  TCNT4 = 0;//reset
  float temp = del * ratio;
  long slaveDelay = temp;
  //OCR3A = slaveDelay * 16 - 1;//setup speed for slave
  OCR3A = slaveDelay * 16;
  TCNT3 = 0;//reset
  steps = stepf;
  steps_counter = 0;  //reset step counter
  interrupts();
}

void varsIntTurn(byte dir, float dist, long del, float ratio, byte master, byte slave) {
  ctc3_setup();
  ctc4_setup();
  PORTL = dir;
  updateFacingAngle(dir, dist); //update global Facing Angle
  float stepf = dist * steps_per_degree;
  masterWheels = master;
  slaveWheels = slave;

  noInterrupts();
  //OCR4A = 16 * del - 1;//setup speed for master
  OCR4A = 16 * del;
  TCNT4 = 0;//reset
  float temp = del * ratio;
  long slaveDelay = temp;
  //OCR3A = slaveDelay * 16 - 1;//setup speed for slave
  OCR3A = slaveDelay * 16;
  TCNT3 = 0;//reset
  steps = stepf;
  steps_counter = 0;  //reset step counter
  interrupts();

}

// COORDINATE MOTION
void updateFacingAngle(byte dir, float dist) { //update Global facingAngle based on turn inputs
  Serial.print("facingAngle updated from ");
  Serial.print(facingAngle);

  if (dir == rotl) {
    facingAngle += dist;
  }
  else if (dir == rotr) {
    facingAngle -= dist;
  }
  while (facingAngle <= -180) { //see if you can replace these with facingAngle = fixDegrees(facingAngle);
    facingAngle += 360;
  }
  while (facingAngle > 180) {
    facingAngle -= 360;
  }
  Serial.print(" to ");
  Serial.println(facingAngle);
}

void updateXandY(byte dir, float dist) { //fupdate Global xPos and yPos based on mov inputs
  float convertedAngle, dx, dy;

  Serial.print("X and Y updated from ");
  Serial.print(xPos);
  Serial.print(", ");
  Serial.print(yPos);

  if (dir == fwd) {
    dx = -dist * sin(facingAngle * DEG_TO_RAD); //phase shift correction using trig identities
    dy = dist * cos(facingAngle * DEG_TO_RAD);
    xPos += dx;
    yPos += dy;
  }
  else if (dir == strr) {
    convertedAngle = facingAngle - 90.0; //phase shift correction
    dx = -dist * sin(convertedAngle * DEG_TO_RAD);
    dy = dist * cos(convertedAngle * DEG_TO_RAD);
    xPos += dx;
    yPos += dy;
  }
  else if (dir == strl) {
    convertedAngle = facingAngle + 90.0; //phase shift correction
    dx = -dist * sin(convertedAngle * DEG_TO_RAD);
    dy = dist * cos(convertedAngle * DEG_TO_RAD);
    xPos += dx;
    yPos += dy;
  }
  else if (dir == rev) {
    convertedAngle = facingAngle + 180.0; //phase shift correction
    dx = -dist * sin(convertedAngle * DEG_TO_RAD);
    dy = dist * cos(convertedAngle * DEG_TO_RAD);
    xPos += dx;
    yPos += dy;
  }

  Serial.print(" to ");
  Serial.print(xPos);
  Serial.print(", ");
  Serial.println(yPos);
}

float angleFromMe(float x, float y) {
  float angleRadians;
  float newX = x - xPos;
  float newY = y - yPos;
  if (newX == 0) { //new correction algorithm to fix divide by 0 error
    if (newY > 0) {
      angleRadians = 0;
    }
    else {
      angleRadians = pi;
    }
  }
  else {
    angleRadians = atan(newY / newX);
  }
  float angleDegrees = angleRadians * RAD_TO_DEG;
  if (newX < 0) { //correction algoritm to pet your angle in phase with north == 0 degrees
    angleDegrees += 90;
  }
  if (newX > 0) {
    angleDegrees -= 90;
  }
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(" is heading ");
  Serial.print(angleDegrees);
  Serial.println("degrees from me.");

  return angleDegrees;
}

float distFromMe(float x, float y) {
  float newX = x - xPos;
  float newY = y - yPos;
  float dist = sqrt(sq(newX) + sq(newY));
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(" is ");
  Serial.print(dist);
  Serial.println(" inches from me.");
  return dist;
}

void orient(float dist, long del) { //works perfect
  Serial.print("I'm looking at heading ");
  Serial.print(facingAngle);
  Serial.println(" degrees.");
  float diff = facingAngle - dist;
  while (diff > 180) {
    diff -= 360;
  }
  while (diff < -180) {
    diff += 360;
  }
  if (diff > 0) {
    turn(rotr, diff, del);
    Serial.print("I wanted to orient to ");
    Serial.print(dist);
    Serial.print(", so I rotated right ");
    Serial.print(diff);
    Serial.println(" degrees.");
  }
  if (diff < 0) {
    turn(rotl, abs(diff), del);
    Serial.print("I wanted to orient to ");
    Serial.print(dist);
    Serial.print(", so I rotated left ");
    Serial.print(diff);
    Serial.println(" degrees.");
  }
}

void orientBetter(float dist, long del) { //works perfect
  Serial.print("I'm looking at heading ");
  Serial.print(facingAngle);
  Serial.println(" degrees.");
  float diff = facingAngle - dist;
  while (diff > 180) {
    diff -= 360;
  }
  while (diff < -180) {
    diff += 360;
  }
  if (diff > 0) {
    turnBetter(rotr, diff, del);
    Serial.print("I wanted to orient to ");
    Serial.print(dist);
    Serial.print(", so I rotated right ");
    Serial.print(diff);
    Serial.println(" degrees.");
  }
  if (diff < 0) {
    turnBetter(rotl, abs(diff), del);
    Serial.print("I wanted to orient to ");
    Serial.print(dist);
    Serial.print(", so I rotated left ");
    Serial.print(diff);
    Serial.println(" degrees.");
  }
}

void lookAt(float x, float y, long del) { //works perfect
  float angleDegrees = angleFromMe(x, y);
  orient(angleDegrees, del);
}

void lookAtBetter(float x, float y, long del) { //works perfect
  float angleDegrees = angleFromMe(x, y);
  orientBetter(angleDegrees, del);
}

void GO(float x, float y, long del) { //trying to GO to your current location orients may behave oddly but it doesn't cause any huge problems rn
  Serial.print("I want to GO() to ");
  Serial.print(x);
  Serial.print(", ");
  Serial.println(y);

  lookAt(x, y, del);
  delay(500);
  float dist = distFromMe(x, y);
  mov(fwd, dist, del);
  delay(500);
}

void GO_fast(float x, float y, long del) { //trying to GO to your current location orients may behave oddly but it doesn't cause any huge problems rn
  Serial.print("I want to GO() to ");
  Serial.print(x);
  Serial.print(", ");
  Serial.println(y);

  lookAtBetter(x, y, del);
  delay(100);
  float dist = distFromMe(x, y);
  movBetter(fwd, dist, del);
  delay(75);
}
void coordAvoid(float x, float y, long del) {
  int lowerlim = 1;                       // lower limit of the sensor readings that the robot will accept as valid data (to prevent 0 inch readings)
  int upperlim = 30;                      // upper limit (to prevent huge readings)
  int inches = 0;                         // inches is the distance the robot will go if it sees the litterbug
  int shift = 48;
  int xBug, yBug, xDes, yDes;
  s = 600;

  turn_motors_off();                      // MUST TURN MOTORS OFF TO READ THE SENSORS!!!!!!!!!!!!!
  read_sensors();                         // get the sensor readings
  turn_motors_on();                       // turn the motors back on

  if (inchesFL >= lowerlim && inchesFL < upperlim ||
      inchesFR >= lowerlim && inchesFR < upperlim) {     // if the sensors see anything within the limits
    if (inchesFL <= inchesFR) {                          // if the front right sensor is closer to the litterbug than the front left sensor
      inches = inchesFL;
    }
    else {
      inches = inchesFR;                                 // minimum distance from litterbug
    }
    xBug = x;
    if (facingAngle == 0) {
      yBug = yPos + inches;
    }

    else {
      yBug = yPos - inches;
    }

    xDes = xBug - shift;
    yDes = yBug;
    if (xDes <= 0 || xDes >= 144) {
      xDes = xBug + shift;
    }
    GO_fast(xDes, yDes, s);
    delay(75);
  }
}

void GO_safe(float x, float y, long del) { //trying to GO to your current location orients may behave oddly but it doesn't cause any huge problems rn
  Serial.print("I want to GO() to ");
  Serial.print(x);
  Serial.print(", ");
  Serial.println(y);

  lookAtBetter(x, y, del);
  delay(100);
  float dist = distFromMe(x, y);
  coordAvoid(x, y, del);
  GO_fast(x, y, del);
  delay(75);
}

void printLoc() {
  Serial.print("xPos: ");
  Serial.print(xPos);
  Serial.print("\t");
  Serial.print("yPos: ");
  Serial.print(yPos);
  Serial.print("\t");
  Serial.print("facingAngle: ");
  Serial.println(facingAngle);
}
