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
float globalHeading = 0.0;

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
	servo[scoopBridge] = 155;
}

void releaseBalls()
{
	servo[scoopBridge] = 0;
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
		motor[lift] = powerLevel;
		motor[liftMotor3] = -powerLevel;
}

void lowerLift(int powerLevel){
		motor[lift] = -powerLevel;
		motor[liftMotor3] = powerLevel;
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
	servo[scoopBridge] = 155;
	servo[rampBridge] = 0;
	nMotorEncoder[intake] = 0;
	openRamp();
	sticksUp();
	HTGYROstartCal(HTGYRO);
}

void drive(int direction,float time,int powerLevel){//Dir:time(seconds):1 = reverse, 0 = forward:
	//Drive in a straight line GYRO
	float rotSpeed = 0;
	float heading = 0;
	int dir = direction==0?1:-1;
	// Calibrate the gyro, make sure you hold the sensor still
	HTGYROstartCal(HTGYRO);
	time1[T2] = 0;
	while(time1[T2] < time*1000){
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

		if(direction==0){
			motor[driveLeft] = (powerLevel+powerLevel*.15*heading)*dir;
			motor[driveRight] = -(powerLevel-powerLevel*.15*heading)*dir;
		}else{
			motor[driveLeft] = (powerLevel-powerLevel*.15*heading)*dir;
			motor[driveRight] = -(powerLevel+powerLevel*.15*heading)*dir;
		}
	}
}

void turn(int direction,int degrees,int powerLevel){//Dir:1 = right, 0 = left::Degrees
	//Accurately turn with GYRO
	float rotSpeed = 0;
	float heading = 0;
	int slow = powerLevel/2;
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
		if(heading>degrees*3.0/4.0)
			powerLevel=slow;
		if(direction==1)
			right(powerLevel);
		else if(direction==0)
			left(powerLevel);

		if(abs(heading)>degrees)
			break;
	}
}

task keepHeading(){
	float rotSpeed = 0;
	// Calibrate the gyro, make sure you hold the sensor still
	while(true){
		while (time1[T3] < 20)
			wait1Msec(1);

		// Reset the timer
		time1[T3]=0;

		// Read the current rotation speed
		rotSpeed = HTGYROreadRot(HTGYRO);

		// Calculate the new heading by adding the amount of degrees
		// we've turned in the last 20ms
		// If our current rate of rotation is 100 degrees/second,
		// then we will have turned 100 * (20/1000) = 2 degrees since
		// the last time we measured.
		globalHeading += rotSpeed * 0.02;
		nxtDisplayCenteredTextLine(1,"GH:%f",globalHeading);
		wait1Msec(1);
		if(globalHeading>180)
			globalHeading = -180;
		else if(globalHeading<-180)
			globalHeading =  180;
	}
}

void turnToGlobalHeading(int targetHeading){
	int powerLevel = 70;
	int slow = 50;
	if(globalHeading>targetHeading){
		while(globalHeading>targetHeading){
			if(abs(globalHeading-targetHeading)<20)
				powerLevel=slow;
			right(powerLevel);
			nxtDisplayCenteredTextLine(2,"Get toL");
		}
	}
	else if(globalHeading<targetHeading){
		while(globalHeading<targetHeading){
			if(abs(globalHeading-targetHeading)<20)
				powerLevel=slow;
			left(powerLevel);
			nxtDisplayCenteredTextLine(2,"Get toR");
		}
	}
}

task main()
{
	init();
	waitForStart();
	StartTask(keepHeading);
	while(SensorValue[sonarSensor]>25){
		backward(30);
	}
	allStop();
	wait1Msec(100);
	backward(20);
	wait1Msec(700);
	sticksDown();
	wait1Msec(250);
	allStop();
	/*forward(30);
	wait1Msec(500);
	allStop();
	wait1Msec(500);
  raiseLift(100);
  while (nMotorEncoder[intake] < 2200) //while the encoder wheel turns one revolution
  {
  }
	allStop();
	wait1Msec(500);
	releaseBalls();
	allStop();
	wait1Msec(3000);
	retainBalls();
	allStop();
	wait1Msec(750);
  lowerLift(20);
  while (nMotorEncoder[intake] > 0) //while the encoder wheel turns one revolution
  {
  }
	allStop();
	wait1Msec(500);*/
	turn(1,19,70);
	allStop();
	wait1Msec(100);
	drive(0,2.0,100);
	allStop();
	wait1Msec(100);
	turn(0,170,100);
	allStop();
	wait1Msec(100);
	sticksUp();
	drive(1,.3,100);
	allStop();
	wait1Msec(100);
	turn(0,180,100);
	allStop();
	wait1Msec(500);
	//while(SensorValue[sonarSensor]>40){
	//	backward(100);
	//}
	//turnToGlobalHeading(-15);
	allStop();
	wait1Msec(100);
	drive(1,1.3,100);
	allStop();
	wait1Msec(100);
	turnToGlobalHeading(-78);
	allStop();
	wait1Msec(100);
	while(SensorValue[sonarSensor]>25){
		backward(30);
	}
	allStop();
	wait1Msec(100);
	while(SensorValue[sonarSensor]<200){
		left(50);
	}
	allStop();
	wait1Msec(100);
	turn(0,22,70);
	allStop();
	wait1Msec(100);
	while(SensorValue[sonarSensor]>25){
		backward(30);
	}
	backward(30);
	wait1Msec(700);
	sticksDown();
	wait1Msec(250);
	allStop();
	turn(1,18,70);
	allStop();
	wait1Msec(100);
	drive(0,2.7,100);
	allStop();
	wait1Msec(100);
	turn(0,175,100);
	allStop();
	wait1Msec(100);
	sticksUp();
	drive(1,.3,100);
}
