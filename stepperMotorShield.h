// This is the UNO Robotics stepperMotorShield Library to control 4 stepper motors with an Arduino Mega.
// The stepperMotorShield was designed by Michael Ceraso in 2019: https://cadlab.io/project/2003/master/files:
// The benefits of a Shield over a separate stepperMotorControllerBoard include:
// 1. Smaller Footprint: The Shield plugs into the Arduino and takes up less physical area
// 2. Less Wires: The wires to connect the Stepper Motor Controllers to the Arduino are embedded in the shield
// 3. Programmable Functions: More Features of the Drivers Are Unlocked By Connecting their Pins to the Arduino Through the Shield
// Regarding (3.), the intention of this library is to define the functions that unlock the drivers' functions.
// The shield is compatible with every Stepper Motor Driver Compact Carrier from Pololu, and currently has functions defined for:
// (a.) TB67S249FTG Stepper Motor Driver Compact Carrier: https://www.pololu.com/product/3096.

// Written by Dylan Vo, March 2020
// Version 1.1

// STEPPER MOTOR SHIELD CONSTANTS
const byte EN = 32;
const byte DM0 = 33;
const byte DM1 = 34;
const byte DM2 = 35;
const byte RST = 36;
const byte AGC = 37;

// STEPPER MOTOR SHIELD
// Use these functions to turn motors on and off to save power and to reduce noise in the lab.
void stepperMotorShieldInit();
void turn_motors_on();
void turn_motors_off();
void set_standby(); // functionally equivalent to turn_motors_off()
void set_full_step();
void set_noncircularhalf_step();
void set_circularhalf_step();
void set_quarter_step(); // standard UNO Robotics Club step mode
void set_eighth_step();
void set_sixteenth_step();
void set_thirtysecond_step();
void reset_motors();
void activate_agc();
void deactivate_agc();

// STEPPER MOTOR SHIELD
// Set the outputs of the stepperMotorShield to activate the library functions
void stepperMotorShieldInit() {
  pinMode(EN, OUTPUT);
  pinMode(DM0, OUTPUT);
  pinMode(DM1, OUTPUT);
  pinMode(DM2, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(AGC, OUTPUT);
}

// You can disable the board by driving ENABLE high (it can be connected directly to a logic high voltage between 2 V and 5.5 V,
// such as the driver’s own VCC output, or it can be dynamically controlled via connections to digital outputs of an MCU).
void turn_motors_on() {
  digitalWrite(EN, LOW);
}

void turn_motors_off() {
  digitalWrite(EN, HIGH);
}

// The resolution (step size) selector inputs (DMODE0, DMODE1, and DMODE2) enable selection from the seven step resolutions according to the table below.
// These three pins have internal 100 kΩ pull-down resistors, so the driver defaults to standby mode when these inputs are left disconnected;
// at least one DMODE pin must be driven high to select a step resolution and allow the driver to operate. For the microstep modes to function correctly,
// the current limit must be set low enough (see below) so that current limiting gets engaged. Otherwise, the intermediate current levels will not be correctly
// maintained, and the motor will skip microsteps.
void set_standby() { // functionally equivalent to turn_motors_off()
  digitalWrite(DM0, LOW);
  digitalWrite(DM1, LOW);
  digitalWrite(DM2, LOW);
}

void set_full_step() {
  digitalWrite(DM0, LOW);
  digitalWrite(DM1, LOW);
  digitalWrite(DM2, HIGH);
}

void set_noncircularhalf_step() {
  digitalWrite(DM0, LOW);
  digitalWrite(DM1, HIGH);
  digitalWrite(DM2, LOW);
}

void set_circularhalf_step() {
  digitalWrite(DM0, HIGH);
  digitalWrite(DM1, LOW);
  digitalWrite(DM2, LOW);
}

void set_quarter_step() { // standard UNO Robotics Club step mode
  digitalWrite(DM0, LOW);
  digitalWrite(DM1, HIGH);
  digitalWrite(DM2, HIGH);
}

void set_eighth_step() {
  digitalWrite(DM0, HIGH);
  digitalWrite(DM1, LOW);
  digitalWrite(DM2, HIGH);
}

void set_sixteenth_step() {
  digitalWrite(DM0, HIGH);
  digitalWrite(DM1, HIGH);
  digitalWrite(DM2, LOW);
}

void set_thirtysecond_step() {
  digitalWrite(DM0, HIGH);
  digitalWrite(DM1, HIGH);
  digitalWrite(DM2, HIGH);
}

// When the RESET pin is driven low, the driver resets its internal electrical angle (the state in the translator table that it is outputting)
// to an initial value of 45°. This corresponds to +100% of the current limit on both coils in full step and non-circular half step modes,
// and +71% on both coils in other microstep modes. Note that, unlike the reset pin on many other stepper drivers, the RESET pin on the TB67S2x9FTG
// compact carrier does not disable the motor outputs when it is asserted: when RESET is low, the driver will continue supplying current to the motor,
// but it will not respond to step inputs on the CLK pin.
void reset_motors() {
  digitalWrite(RST, LOW);
  delay(3); // arbitrary delay
  digitalWrite(RST, HIGH);
}

// On our compact carrier, the AGC pin can be pulled high to enable Active Gain Control with the bottom current limit set to 60%.
// If the 60% setting is too aggressive, you can raise the bottom current limit to 80% by shorting across the two pads on the CL0 jumper
// on the back-side of the board as shown in the picture below.
void activate_agc() {
  digitalWrite(AGC, HIGH);
}

void deactivate_agc() {
  digitalWrite(AGC, LOW);
}
