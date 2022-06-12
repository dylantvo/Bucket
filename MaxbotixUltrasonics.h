// LAST UPDATED MARCH 4, 2020


const int FrontLeft = A0;               // analog pin for one ultrasonic sensor
const int FrontRight = A1;
const int triggerPin = 23;
int anVoltFL, anVoltFR;                 // analog voltage inputs from front left and front right sensors
float inchesFL, inchesFR;               // output of front left and front right sensors
const float calScale = 0.5;             // calibration scale value to convert anVolt to inches, calibrated using read_sensor_avg
const float calOffset = 0.4847;         // offset value for calibration, calibrated using read_sensor_avg
const float calScaleMode = 0.4948;      // calibration scale value using calibration data gathered from read_sensor_mode
const float calOffsetMode = 2.0088;     // offset from using read_sensor_mode
//const float calScaleMode = 0.4734;    // untested calibration values from extending data gathered from read_sensor_mode
//const float calOffsetMode = 2.9474;
const int anReads = 5;                  // anReads is the amount of analog reads will be performed before we find the average, median, or mode of the set
int anVoltArrFL[anReads];               // used in read_sensor_mode and read_sensor_median as an array of size anReads.
int anVoltArrFR[anReads];
int printMedian(int a[]);               // must prototype, as this function is used in read_sensors
int find_max(int arr[]);


/////////////////////////////////////////////////////////////
void start_sensor() {
  digitalWrite(triggerPin, HIGH);
  delay(1);
  digitalWrite(triggerPin, LOW);
}

void read_sensors() {                                     // use to read sensors
  for (int i = anReads; i > 0; i--) {                     // loop creates an array of analog voltage readings so the program can later find the median
    anVoltArrFL[i] = analogRead(FrontLeft);               // stores analog values into anVoltArrFL and anVoltArrFR
    anVoltArrFR[i] = analogRead(FrontRight);
    delay(50);
  }
  anVoltFL = printMedian(anVoltArrFL);                    // sets analog voltages equal to the median within array by using function printMedian
  anVoltFR = printMedian(anVoltArrFR);
  inchesFL = (anVoltFL * calScaleMode) + calOffsetMode;   // scales anVolt using calibration scaling factors
  inchesFR = (anVoltFR * calScaleMode) + calOffsetMode;
  delay(50);
}

void print_range() {                      // prints data
  Serial.print("FL");
  Serial.print(" ");
  Serial.print(inchesFL);
  Serial.print("in");
  Serial.print(" ");
  Serial.print(" ");
  Serial.print("FR");
  Serial.print(" ");
  Serial.print(inchesFR);
  Serial.print("in");
  Serial.println();
}

void debug_sensors() {                    // function used to debug sensors by reading the sensors and printing the data in serial monitor
  start_sensor();
  read_sensors();
  print_range();
  delay(50);
}

int s;
int lb;                                   // distance from litterbug

/*void avoid() {                            // function that scans in front of the robot and checks if the litterbug is in its way
  // if the litterbug is in the way, the robot will change its route to avoid the litterbug
  int lowerlim = 1;                       // lower limit of the sensor readings that the robot will accept as valid data (to prevent 0 inch readings)
  int upperlim = 75;                      // upper limit (to prevent huge readings)
  int direct = rotl;                      // direction of the first turn (initialized to turn left to be safe, this value change in the function)
  int opdirect = rotr;                    // direction of the second turn (will always be the opposite direction from the first turn)
  int inches = 0;                         // inches is the distance the robot will go if it sees the litterbug
  s = 600;

  turn_motors_off();                      // MUST TURN MOTORS OFF TO READ THE SENSORS!!!!!!!!!!!!!
  read_sensors();                         // get the sensor readings
  turn_motors_on();                       // turn the motors back on

  if (inchesFL >= lowerlim && inchesFL < upperlim ||
      inchesFR >= lowerlim && inchesFR < upperlim) {     // if the sensors see anything within the limits
    if (inchesFR <= inchesFL) {                          // if the front right sensor is closer to the litterbug than the front left sensor
      direct = rotl;                                     // the first turn will be left
      opdirect = rotr;                                   // the second turn will be right
      inches = inchesFR;                                 // set the distance we will travel forward to the smaller distance to the litterbug
    }
    else {
      direct = rotr;                                     // opposite case for if the front left sensor is closer to the litterbug
      opdirect = rotl;
      inches = inchesFL;
    }
    turn(direct, 90, s);                                 // turn the direction passed from previous if else statements
    delay(50);
    mov(fwd, 20, s);                                     // move 20 inches away so the robot is no longer in the path towards the litterbug
    delay(50);
    turn(opdirect, 90, s);                               // turn again to face the direction of the litterbug
    delay(50);
    mov(fwd, inches, s);                                 // move forward using the distance passed from the sensor reading
    delay(100);
  }
  else {
    mov(fwd, 30, s);                                     // if the robot doesn't see the litterbug then move forward 30 inches
    delay(100);
  }
}

void coordAvoid(float x, float y, long del) {
  int lowerlim = 1;                       // lower limit of the sensor readings that the robot will accept as valid data (to prevent 0 inch readings)
  int upperlim = 75;                      // upper limit (to prevent huge readings)
  int inches = 0;                         // inches is the distance the robot will go if it sees the litterbug
  int shift = 36;
  int xBug, yBug, xDes, yDes;
  s = 600;

  turn_motors_off();                      // MUST TURN MOTORS OFF TO READ THE SENSORS!!!!!!!!!!!!!
  read_sensors();                         // get the sensor readings
  turn_motors_on();                       // turn the motors back on

  if (inchesFL >= lowerlim && inchesFL < upperlim ||
      inchesFR >= lowerlim && inchesFR < upperlim) {     // if the sensors see anything within the limits
    if (inchesFR <= inchesFL) {                          // if the front right sensor is closer to the litterbug than the front left sensor
      inches = inchesFR;
    }
    else {
      inches = inchesFL;                                 // minimum distance from litterbug
    }
    xBug = x;
    yBug = y + inches + 12;

    xDes = xBug - shift;
    yDes = yBug;
    if (xDes <= 0 || xDes >= 144) {
      xDes = x + shift;
    }
    GO_fast(xDes, yDes, s);
    delay(75);
  }
}
*/



/////////////////////////////////////////////////
/////////////////////////////////////////////////
/*Median and mode calculations*/
int printMode(int a[])      // printMode takes an array of size anReads as its input and returns the mode of the array
{
  int b[anReads];

  int maxval = find_max(a);
  int t = maxval + 1;
  int count[t];
  for (int i = 0; i < t; i++)
    count[i] = 0;

  for (int i = 0; i < anReads; i++)
    count[a[i]]++;

  int mode = 0;
  int k = count[0];
  for (int i = 1; i < t; i++)
  {
    if (count[i] > k)
    {
      k = count[i];
      mode = i;
    }
  }

  for (int i = 1; i < t; i++)
    count[i] = count[i] + count[i - 1];

  for (int i = 0; i < anReads; i++)
  {
    b[count[a[i]] - 1] = a[i];
    count[a[i]]--;
  }
  return mode;                  // returns mode of the array
}

int printMedian(int a[])                    // printMedian takes input array of size anReads and returns the median
{
  int b[anReads];

  int maxval = find_max(a);
  int t = maxval + 1;
  int count[t];
  for (int i = 0; i < t; i++)
    count[i] = 0;

  for (int i = 0; i < anReads; i++)
    count[a[i]]++;

  int mode = 0;
  int k = count[0];
  for (int i = 1; i < t; i++)
  {
    if (count[i] > k)
    {
      k = count[i];
      mode = i;
    }
  }

  for (int i = 1; i < t; i++)
    count[i] = count[i] + count[i - 1];

  for (int i = 0; i < anReads; i++)
  {
    b[count[a[i]] - 1] = a[i];
    count[a[i]]--;
  }
  float median;
  if (anReads % 2 != 0) {
    median = b[anReads / 2];
  }
  else {
    median = (b[(anReads - 1) / 2] +
              b[(anReads / 2)]) / 2.0;
  }
  return median;                          // returns median of input array
}


int find_max(int arr[]) {
  int maxval = 0;                         // initialize maximum value of array to be 0
  for (int i = anReads; i > 0; i--) {     // input array is of size anReads, so loop anReads amount of times
    if (arr[i] > maxval) {                // if the present element of the array is greater than maxval
      maxval = arr[i];                    // then set maxval equal to the element
    }
  }
  return maxval;                          // returns the maximum value maxval
}
