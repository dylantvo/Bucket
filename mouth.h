const int IN1 = 3;
const int IN2 = 2;
const int ENA = 4;

//This corresponds to 3 times the mechanical time constant of the motor-robot system in ms.
double winduptime = 3000;

void mouthInit() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
}

void Motor1_Backward(int Speed)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, Speed);
}

void Motor1_Forward(int Speed)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, Speed);
}
void Motor1_Brake()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

double SmoothFunction(double);

// SmoothFunction is normalized to go from 0 to 1 between x=0 to x=1
double SmoothFunction(double x)
{

  double output;

  output = 1 / (1 + exp(-(10 * x - 5)));

  return output;
}

void mouth_start(int speed, double ramptime) {
  double i = 0;
  // Time step of 1 millisecond
  double ts = 1;
  // Placeholder value between 0 and 1 - must be scaled to be between 0 and 255 to become z
  double y = 0;
  // Value to be passed to the motor (Must be 8 bit integer)
  int z = 0;

  while (i <= ramptime)
  {
    // Smooth ramping up during wind-up transient
    y = SmoothFunction(i / ramptime);

    // Normalize y to between 0 and 255
    y = round(y * 255 * speed / 100);

    z = (int) y;
    Motor1_Forward(z);

    i += ts;
    delay(ts);
  }
}

void mouth_stop(int speed, double ramptime) { // VERY Iompnortant to suie same speeeed

  double i = 0;
  // Time step of 1 millisecond
  double ts = 1;
  // Placeholder value between 0 and 1 - must be scaled to be between 0 and 255 to become z
  double y = 0;
  // Value to be passed to the motor (Must be 8 bit integer)
  int z = 0;

  while (i <= ramptime)
  {


    y = SmoothFunction(-(i - ramptime) / ramptime);

    // Normalize y to between 0 and 255
    y = round(y * 255 * speed / 100);

    z = (int) y;
    Motor1_Forward(z);

    i += ts;
    delay(ts);
  }
}

// This function is meant to slowly ramp up the motor, to negate sharp input currents. It is not designed (yet) to mitigate wheel slippage
void Smooth_Forward(int speed, int duration)
{
  double i = 0;
  // Time step of 1 millisecond
  double ts = 1;
  // Placeholder value between 0 and 1 - must be scaled to be between 0 and 255 to become z
  double y = 0;
  // Value to be passed to the motor (Must be 8 bit integer)
  int z = 0;

  while (i < duration)
  {
    // Smooth ramping up during wind-up transient
    if (i <= winduptime)
    {
      y = SmoothFunction(i / winduptime);
    }

    // Smooth ramping down during wind-down transient
    else if (i >= duration - winduptime)
    {
      y = SmoothFunction(-(i - duration) / winduptime);
    }
    // Constant value during steady state
    else
    {
      y = 1;
    }

    // Normalize y to between 0 and 255
    y = round(y * 255 * speed / 100);

    z = (int) y;
    Motor1_Forward(z);

    i += ts;
    delay(ts);
  }

}

void Smooth_Backward(int speed, int duration)
{
  double i = 0;
  // Time step of 1 millisecond
  double ts = 1;
  // Placeholder value between 0 and 1 - must be scaled to be between 0 and 255 to become z
  double y = 0;
  // Value to be passed to the motor (Must be 8 bit integer)
  int z = 0;

  while (i < duration)
  {
    // Smooth ramping up during wind-up transient
    if (i <= winduptime)
    {
      y = SmoothFunction(i / winduptime);
    }

    // Smooth ramping down during wind-down transient
    else if (i >= duration - winduptime)
    {
      y = SmoothFunction(-(i - duration) / winduptime);
    }
    // Constant value during steady state
    else
    {
      y = 1;
    }

    // Normalize y to between 0 and 255
    y = round(y * 255 * speed / 100);

    z = (int) y;
    Motor1_Backward(z);
    i += ts;
    delay(ts);
  }

}
