// Written by Dylan Vo, March 2020

#include "stepperMotorShield.h"
#include "coordinateMotion.h"
#include "bucket_compass.h"
#include "mouth.h"

void setup(void) {
  // INITIALIZATION:
  Serial.begin(9600); //init serial monitor
  CMPS12.begin(9600); //init compass

  // Setup basic motion:
  DDRL = B11111111;

  // Setup STEPPER MOTOR SHIELD:
  stepperMotorShieldInit();
  set_quarter_step();
  activate_agc();
  turn_motors_off();
  reset_motors();

  // Setup the mouth
  mouthInit();

  // initialize compass values:
  delay(50);
  startingHeading = getAbsoluteHeading(); //calibrate compass
  Serial.print("I got the starting heading: ");
  Serial.println(startingHeading);
  facingAngle = getAverageHeading(); //sync facingAngle and compass
  Serial.print("I got the heading: ");
  Serial.println(facingAngle);

  // Optional functions to reset the compass... Doesnt seem to work:
  // Serial.println(getCalibrationState(), BIN);
  // deleteCalibrationByte();
  // Serial.println(getCalibrationState(), BIN);

  // WRITE YOUR MAIN PROGRAM HERE:
  // Decide your starting location
  // code needed
turn_motors_on();
  // Start your mouth and turn motors on
  int mouthSpeed = 45;   // Set the max speed of your mouth
  mouth_start(mouthSpeed, 2000); // first arg is % of max speed; second arg is ramp up time

  // Set some constants for speed of motion and delay between motions
  int s = 600; // smaller numbers = faster robot. minimum is about 500 w/o acceleration
  int d = 100; // delay in milliseconds

  // Sweep the field
  // compass or wall following code needed
  // test acceleration code

  // test_vision();


  
    GO_fast(20, 18, s);
    delay(d);

    GO_fast(20, 60, s);
    delay(d);
    
    GO_safe(20, 124, s);
    delay(d);

    GO_fast(36, 124, s);
    delay(d);

    GO_fast(36, 84, s);
    delay(d);

    GO_safe(36, 24, s);
    delay(d);

    GO_fast(60, 24, s);
    delay(d);

    GO_fast(60, 60, s);
    delay(d);

    GO_safe(60, 124, s);
    delay(d);

    GO_fast(84, 124, s);
    delay(d);

    GO_fast(84, 84, s);
    delay(d);

    GO_safe(84, 24, s);
    delay(d);

    GO_fast(108, 24, s);
    delay(d);

    GO_fast(108, 60, s);
    delay(d);

    GO_safe(108, 124, s);
    delay(d);

    GO_fast(124, 124, s);
    delay(d);

    GO_fast(124, 84, s);
    delay(d);

    GO_fast(124, 24, s);
    delay(d);

  /*
  turn(rotl, 90, s);  // these lines are the same as approach_and_avoid function in MaxbotixUltrasonics.h, but the function doesn't work when called for some reason.
  delay(50);
  mov(fwd, 12, s);
  delay(50);
  turn(rotr, 90, s);
  delay(50);
  mov(fwd, 12, s);
  delay(100);
  avoid();*/
  // Turn your mouth off when you're done eating:
  mouth_stop(mouthSpeed, 2000);

  // Go to the trash square, orient to dump, and dump
  // code needed
  // GO_fast(50, 126, s);
  //delay(d);
  //orientBetter(180,s);
  // Meme on the competition
  // code needed
  //GO_fast(48, 48, s);
  //turnInsane(rotl,720,s);
  // Turn your motors off when you're done with a round
  turn_motors_off();
}

void loop(void) {
  // CODE TO RUN FOREVER
  //debug_sensors();

}
