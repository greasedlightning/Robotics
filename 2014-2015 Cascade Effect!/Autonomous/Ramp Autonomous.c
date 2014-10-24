#pragma config(Hubs,   S1, HTMotor, HTServo, HTMotor, none)
#pragma config(Motor,  mtr_S1_C1_1,     driveRight,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     lift,  tmotorNormal, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    fieldGrabberRight, tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    fieldGrabberLeft, tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    fieldRoller, tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    frontBridge, tServoStandard)
#pragma config(Motor,  mtr_S1_C3_1,     driveLeft,  tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     intake, tmotorNormal, openLoop)
#pragma config(Sensor, S2,              sonarSensor,       sensorSONAR)

#include "JoystickDriver.c"

#define open 0
#define grab 100
#define closed 105
#define inside 215
#define _threshold 20


void allStop(){
	motor[driveLeft] = 0;
	motor[driveRight] = 0;
	motor[lift] = 0;
	motor[intake] = 0;
	servo[frontBridge] = 127;
}

void lowerBridge(){
	servo[fieldRoller] = 256;
}

void raiseBridge(){
	servo[fieldRoller] = 0;
}

void stopBridge(){
	servo[fieldRoller] = 127;
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

void rightStickDown(){
		servo[fieldGrabberRight] = 255-inside;
}

void rightStickUp(){
		servo[fieldGrabberRight] = 255-open;

}

void leftStickDown(){
		servo[fieldGrabberLeft] = inside-15;
}

void leftStickUp(){
		servo[fieldGrabberLeft] = open;
}

void sticksDown(){
		servo[fieldGrabberLeft] = inside-15;
		servo[fieldGrabberRight] = 255-inside;
}

void sticksUp(){
		servo[fieldGrabberLeft] = open;
		servo[fieldGrabberRight] = 255-open;
}


void init(){
	sticksUp();
	servo[fieldRoller] = 127;
	servo[frontBridge] = 255;
}

task main()
{
	init();
	//waitForStart();
	backward(20);
	wait1Msec(2500); //Comes off ramp
	allStop();
	//Set up to find rolling goal
	left(100);
	wait1Msec(250);
	allStop();
	wait1Msec(1000);
	while(SensorValue[sonarSensor]>60){ //scan right until see tube
		right(50);
	}
	right(50);
	PlaySound(soundBlip);
	wait1Msec(50);
	allStop();
	PlaySound(soundBlip);
	wait1Msec(750);
	allStop();
	backward(20); //Drive towards goal
	PlaySound(soundBlip);
	wait1Msec(750);
	allStop();
	for(int i=0;i<3;i++){ //Grab goal with left stick
		leftStickDown();
	}
	PlaySound(soundBlip);
	wait1Msec(1000);
	allStop();
	right(50);
	PlaySound(soundBlip);
	wait1Msec(500);
	allStop();
	for(int i=0;i<3;i++){ //Grab goal with right stick after rotating in place
		rightStickDown();
	}
	allStop();
	PlaySound(soundUpwardTones);
	wait1Msec(1000); //Done Grabbing

	//DO WHATEVER DEPENDING ON CENTER ROTATION
	forward(50);
	wait1Msec(750);
	allStop();
	wait1Msec(750);
	right(100);
	wait1Msec(600);
	allStop();
	wait1Msec(750);
	forward(100);
	wait1Msec(500);
}
