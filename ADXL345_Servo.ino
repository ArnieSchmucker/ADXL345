
#include <Wire.h>
#include <Servo.h>

Servo myServo;
int servoPin = 9;

#define accel_module (0x53)

byte values[6];
char output[512];
double theta = 0;
int indexMax = 4;
int index = 0;
double indexTheta[4] = {0,0,0,0};
double sumTheta = 0;
int pos = 0;

void setup() {
  myServo.attach(servoPin);
  initializeChip();
}

void loop() { 
  for (index = 0; index <= indexMax; index++)
  {
    readI2C();
    indexTheta[index] = theta;
    sumTheta = sumTheta + indexTheta[index];
    index++;
    if (index > indexMax)
    {
      index = 0;
    }
  }

    sumTheta = sumTheta*57.3/indexMax;
   
    pos = sumTheta;
 
    pos = map(pos, -90, 90, 0, 179);
    pos = constrain(pos, 0, 179);
    //Serial.println(sumTheta);
    //Serial.println(pos);
    //delay(10);
    sumTheta = 0;
    
    myServo.write(pos);
    delay(15);
}

void initializeChip()
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("ADXL345_I2C");
  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(0);
  Wire.endTransmission();
  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(16);
  Wire.endTransmission();
  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.endTransmission();
}

void readI2C()
{
  int xyzregister = 0x32;
  int x, y, z;
  Wire.beginTransmission(accel_module);
  Wire.write(xyzregister);
  Wire.endTransmission();
  Wire.beginTransmission(accel_module);
  Wire.requestFrom(accel_module, 6);
  

  int i=0;
  while(Wire.available())
  {
    values[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();

  x = (((int)values[1]) << 8) | values[0];
  y = (((int)values[3]) << 8) | values[2];
  z = (((int)values[5]) << 8) | values[4];

  sprintf(output, "%d %d %d", x, y, z);
  //Serial.print(output);
  //Serial.write(10);
  //Pitch about y axis
  theta = atan(-y/sqrt(pow(x,2)+pow(z,2)));

}

