#pragma config(Hubs,   S1, HTMotor, HTServo, HTMotor, HTMotor)
#pragma config(Motor,  mtr_S1_C1_1,     driveRight,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     lift,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     driveLeft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     intake,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     liftMotor3,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    fieldGrabberRight,    tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    fieldGrabberLeft,     tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    scoopBridge,          tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    rampBridge,          tServoStandard)
#pragma config(Sensor, S2,              sonarSensor,       sensorSONAR)
#pragma config(Sensor, S3,     HTIRS2,              sensorI2CCustom)
#pragma config(Sensor, S4,     HTGYRO,              sensorAnalogInactive)

#include "JoystickDriver.c"
#include "/hitechnic-irseeker-v2.h"
#include "/hitechnic-gyro.h"


#define _open 0
#define _closed 205
#define _threshold 20

int acS1, acS2, acS3, acS4, acS5 = 0;

float exponentialJoystick(int joyVal){
	return (float)5.60015*pow(2.718281828,0.96781*(abs(joyVal)/40));
}

void sticksDown(){
	servo[fieldGrabberLeft] = _open;
	servo[fieldGrabberRight] = 255-_open;
}

void sticksUp(){
	servo[fieldGrabberLeft] = _closed-15;
	servo[fieldGrabberRight] = 255-_closed;
}

void retainBalls()
{
	servo[scoopBridge] = 0;
}

void releaseBalls()
{
	servo[scoopBridge] = 155;
}

void closeRamp()
{
	servo[rampBridge] = 0;
}

void openRamp()
{
	servo[rampBridge] = 100;
}

void allStop(){
	motor[driveLeft] = 0;
	motor[driveRight] = 0;
	motor[lift] = 0;
	motor[liftMotor3] = 0;
	motor[intake] = 0;
}

void raiseLift(int powerLevel){
	motor[lift] = abs(powerLevel);
}

void lowerLift(int powerLevel){
	motor[lift] = -abs(powerLevel);
}

void intakeIn(int powerLevel){
	motor[intake] = abs(powerLevel);
}

void intakeOut(int powerLevel){
	motor[intake] = -abs(powerLevel);
}

void forward(int powerLevel){
	motor[driveLeft] = abs(powerLevel);
	motor[driveRight] = -abs(powerLevel);
}

void backward(int powerLevel){
	motor[driveLeft] = -abs(powerLevel);
	motor[driveRight] = abs(powerLevel);
}

void left(int powerLevel){
	motor[driveLeft] = -abs(powerLevel);
	motor[driveRight] = -abs(powerLevel);
}

void right(int powerLevel){
	motor[driveLeft] = abs(powerLevel);
	motor[driveRight] = abs(powerLevel);
}


void init(){
	servo[fieldGrabberLeft] = _open;
	servo[fieldGrabberRight] = 255-_open;
	servo[scoopBridge] = 180;
	servo[rampBridge] = 0;
	nMotorEncoder[intake] = 0;
	openRamp();
}

void turn(int direction,int degrees,int powerLevel){//Dir:1 = right, 0 = left::Degrees

  float rotSpeed = 0;
  float heading = 0;
  // Calibrate the gyro, make sure you hold the sensor still
  HTGYROstartCal(HTGYRO);
  while(true){
	while (time1[T1] < 20)
      wait1Msec(1);

    // Reset the timer
    time1[T1]=0;

    // Read the current rotation speed
    rotSpeed = HTGYROreadRot(HTGYRO);

    // Calculate the new heading by adding the amount of degrees
    // we've turned in the last 20ms
    // If our current rate of rotation is 100 degrees/second,
    // then we will have turned 100 * (20/1000) = 2 degrees since
    // the last time we measured.
    heading += rotSpeed * 0.02;
    if(heading>degrees*(3.0/4.0))
    	powerLevel=powerLevel/3;
    if(direction==1)
    	right(powerLevel);
   	else if(direction==0)
   		left(powerLevel);

    if(abs(heading)>degrees)
    	break;
 }
}

task main()
{
	init();
	waitForStart();
	backward(100);
	wait1Msec(1900); //Comes off ramp
	allStop();
	wait1Msec(500);
}
