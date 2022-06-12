volatile byte masterWheels, slaveWheels;
volatile long steps, steps_counter;

// Direction bytes
byte fwd = B00001010;
byte rev = B10100000;
byte rotr = B10101010;
byte rotl = B00000000;
byte strl = B00100010;
byte strr = B10001000;

// Motor bytes
byte allMotors = B01010101;
byte frontmotors = B01000100;
byte rearmotors = B00010001;
byte rightmotors = B01010000;
byte leftmotors = B00000101;
byte nowheels = B00000000;
byte l45 = B00010100;
byte r45 = B01000001;

// Measurements
const float straight = 1.000;
const float steps_per_inch = 132.2;
float steps_per_degree = 41.5514;
const float start_dis = .25;
const float pi = 3.14159;

// Coordinate Motion Global Variables
float facingAngle = 0.0;
float xPos = 36;
float yPos = 18;
