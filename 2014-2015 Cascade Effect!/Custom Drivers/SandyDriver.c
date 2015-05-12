#include "JoystickDriver.c"
#include "/hitechnic-irseeker-v2.h"
#include "/hitechnic-gyro.h"
#include "SandyDriver.h"

#define _open 0
#define _closed 205
#define _threshold 20

int times=0;

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
	servo[scoopBridge] = 137;
}

void releaseBalls()
{
	servo[scoopBridge] = 0;
}

void releaseAutoBall(){
	servo[autoBallRelease] = 125;
}

void retainAutoBall(){
	servo[autoBallRelease] = 8;
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
		motor[liftMotor3] = powerLevel;
}

void lowerLift(int powerLevel){
		motor[lift] = -powerLevel;
		motor[liftMotor3] = -powerLevel;
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

void driveSonar(int direction,float distance,int powerLevel){//Dir:distance(centimeters):1 = reverse, 0 = forward:
	//Drive in a straight line GYRO
	float rotSpeed = 0;
	float heading = 0;
	int dir = direction==0?1:-1;
	// Calibrate the gyro, make sure you hold the sensor still
	HTGYROstartCal(HTGYRO);
	time1[T2] = 0;
	while(SensorValue[sonarSensor] > distance){
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

void init(){
	servo[fieldGrabberLeft] = _open;
	servo[fieldGrabberRight] = 255-_open;
	nMotorEncoder[intake] = 0;
	servo[scoopBridge] = 155;
	sticksUp();
	retainAutoBall();
	HTGYROstartCal(HTGYRO);
}