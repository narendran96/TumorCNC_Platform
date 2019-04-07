// TumorCNC stewart platform tester

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<Servo.h>
#include<Wire.h>ss

//include IMU --> measure angle before and after servo rotation and print
#define I2C_address 0b1101000

// Global variables
// Angular Translation - Converted to radians from input
double xa = 0*(3.141592653589/180);
double ya = 0*(3.141592653589/180);
double za = 0*(3.141592653589/180);
double xt = 0*25.4;
double yt = 0*25.4;
double zt = 0*25.4;
float accX; float accY; float accZ;
float gyroX; float gyroY; float gyroZ;
float XAng, YAng, ZAng;
float diffTime, curTime, prevTime;

Servo ser_1;
Servo ser_2;
Servo ser_3;
Servo ser_4;
Servo ser_5;
Servo ser_6;

// Constant Variables
/*

x
^
|
|
|_ _ _ _ _ _ > y

Servo 1 is top right corner when looking from laser perspective

*/

const double s = 5.225 * 25.4; // rod length (mm)
const double a = 1.8 * 25.4; // arm length (mm)
const double h0 = 5 * 25.4; // home height (mm)
const double bi[6][3] = {{94.39, 54.45, 0}, {0, 108.5, 0}, {-94.15, 54.17, 0}, {-94.15, -54.17, 0}, {0, -108.5, 0}, {94.39, -54.45, 0}}; // Position of base anchors (mm)
const double pi[6][3] = {{50.95, 56.41, 0}, {23.38, 72.33, 0}, {-74.33, 15.92, 0}, {-74.33, -15.92, 0}, {23.38, -72.33, 0}, {50.95, -56.41, 0}}; // position of platform anchors (mm)
const double beta[6] = {270.0, 210.0, 30.0, 330.0, 150.0, 90.0}; // angles of servos w.r.t x axis !!!!!
const double servo_0 = 90; // Set servo @ 90 deg as home .write(90) this makes alpha = 0
const double alpha0 = 0;
char command;

void setup() {
  //Wire.begin();
  //IMU_setup();
  ser_1.attach(6);
  ser_2.attach(5);
  ser_3.attach(3);
  ser_4.attach(11);
  ser_5.attach(10);
  ser_6.attach(9);
  ser_1.write(servo_0);
  ser_2.write(servo_0);
  ser_3.write(servo_0);
  ser_4.write(servo_0);
  ser_5.write(servo_0);
  ser_6.write(servo_0);
  Serial.begin(115200);
}

void loop() {

  // Stall
  while(!Serial.available()){}
  // Inputs

  while(Serial.available()>0){
      command = Serial.read();
      if(command == 'R'){
        xa = (double) Serial.parseFloat()*(3.141592653589/180); Serial.println(xa);
        ya = (double) Serial.parseFloat()*(3.141592653589/180); Serial.println(ya);
        za = (double) Serial.parseFloat()*(3.141592653589/180); Serial.println(za);
      }
      else if(command == 'T'){
        xt = (double) Serial.parseFloat()*(25.4);
        yt = (double) Serial.parseFloat()*(25.4);
        zt = (double) Serial.parseFloat()*(25.4);
      }
      else if(command == 'D'){
        xa = (double) Serial.parseFloat()*(3.141592653589/180); Serial.println(xa);
        ya = (double) Serial.parseFloat()*(3.141592653589/180); Serial.println(ya);
        za = (double) Serial.parseFloat()*(3.141592653589/180); Serial.println(za);
        xt = (double) Serial.parseFloat()*(25.4);
        yt = (double) Serial.parseFloat()*(25.4);
        zt = (double) Serial.parseFloat()*(25.4);
      }
      else if(command == 'r'){
        xa=ya=za=xt=yt=zt=0;
      }
  }
  command = (char) 0;

  // Values for transformation
  double R[3][3] = {{cos(za)*cos(ya), -sin(za)*cos(xa)+cos(za)*sin(ya)*sin(xa), sin(za)*sin(xa)+cos(za)*sin(ya)*cos(xa)},
    {sin(za)*cos(ya), cos(za)*cos(xa)+sin(za)*sin(ya)*sin(xa), -cos(za)*sin(xa)+sin(za)*sin(ya)*cos(xa)},
    {-sin(ya), cos(ya)*sin(xa), cos(ya)*cos(xa)}};
  double li[6][3] = {0}; // leg vector "pi" - "bi"
  double leg_length[6] = {0}; // length of li
  double T[3] = {xt, yt, zt+h0};
  double alpha[6] = {0};
  double rot[6] = {0};

  // Matrix multiplication to find leg vector li
  for(int i=0; i<6; i++){
    for(int k=0; k<3; k++){
      for(int z=0; z<3; z++){
        li[i][k] = li[i][k] + R[k][z]*pi[i][z];
      }
      li[i][k] = li[i][k]+T[k]-bi[i][k];
    }
  }
  // Leg length and alpha calculation
  for(int i=0;i<6;i++){
    leg_length[i] = sqrt(li[i][0]*li[i][0] + li[i][1]*li[i][1] + li[i][2]*li[i][2]);
    double L = leg_length[i]*leg_length[i] - (s*s - a*a);
    double M = 2*a*(li[i][2]);
    double N = 2*a*(cos(beta[i]*3.141592653589/180)*(li[i][0]) + sin(beta[i]*3.141592653589/180)*(li[i][1]));
    alpha[i] = asin(L/sqrt(M*M+N*N)) - atan(N/M);
    rot[i] = constrain((alpha[i] - alpha0)*180/3.141592653589, -45, 45);
  }

  // Control servo for each arm
  /*prevTime = curTime;
  curTime = micros();
  diffTime = curTime - prevTime;

  gyroAng();*/
  ser_1.write(90+rot[0]); // +
  ser_2.write(90-rot[1]); // -
  ser_3.write(90+rot[2]); // +
  ser_4.write(90-rot[3]); // -
  ser_5.write(90+rot[4]); // +
  ser_6.write(90-rot[5]); // -
  /*prevTime = curTime;
  curTime = micros();
  diffTime = curTime - prevTime;
  gyroAng();*/
}

void IMU_setup(){
  Wire.beginTransmission(I2C_address); // connects MPU
  Wire.write(0x6B); // PU management
  Wire.write(0b00000000); // Taking MPU off sleep mode
  Wire.endTransmission();
  // Set up the Gyro data
  Wire.beginTransmission(I2C_address);
  Wire.write(0x1B);
  Wire.write(0x00000000);
  Wire.endTransmission();
  // Accelerometer setup
  Wire.beginTransmission(I2C_address);
  Wire.write(0x1C);
  Wire.write(0x00000000);
  Wire.endTransmission();
}

void gyroAng(){
  Wire.beginTransmission(I2C_address);
  Wire.write(0x43);
  Wire.endTransmission();
  // Request 6 registers starting from 0x43 from MPU
  Wire.requestFrom(I2C_address, 6);
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read();
  gyroY = Wire.read()<<8|Wire.read();
  gyroZ = Wire.read()<<8|Wire.read();
  // Need to scale down gyro 16 bit 2's compliment value to 250 deg/s range

  gyroX /= 131.0;
  gyroX = map(gyroX, -32768, 32767, -250, 250);
  gyroY /= 131.0;
  gyroY = map(gyroY, -32768, 32767, -250, 250);
  gyroZ /= 131.0;
  gyroZ = map(gyroZ, -32768, 32767, -250, 250);
  XAng = (float) XAng + gyroX*diffTime/1000; Serial.println(XAng);
  YAng = (float) YAng + gyroY*diffTime/1000; Serial.println(YAng);
  ZAng = (float) ZAng + gyroZ*diffTime/1000; Serial.println(ZAng);

}
