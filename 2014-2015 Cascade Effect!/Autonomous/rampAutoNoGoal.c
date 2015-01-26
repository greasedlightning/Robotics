#pragma config(Hubs,   S1, HTMotor, HTServo, HTMotor, none)
#pragma config(Motor,  mtr_S1_C1_1,     driveRight,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     lift,  tmotorNormal, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    fieldGrabberRight, tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    fieldGrabberLeft, tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    scoopBridge, tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    frontBridge, tServoStandard)
#pragma config(Motor,  mtr_S1_C3_1,     driveLeft,  tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     intake, tmotorNormal, openLoop)
#pragma config(Sensor, S2,              sonarSensor,       sensorSONAR)
#pragma config(Sensor, S3,     HTIRS2,              sensorI2CCustom)

#include "JoystickDriver.c"
#include "/hitechnic-irseeker-v2.h"

#define open 0
#define grab 100
#define closed 105
#define inside 215
#define _threshold 20

int acS1, acS2, acS3, acS4, acS5 = 0;

void allStop(){
	motor[driveLeft] = 0;
	motor[driveRight] = 0;
	motor[lift] = 0;
	motor[intake] = 0;
	servo[frontBridge] = 127;
	servo[scoopBridge] = 127;
}

void lowerBridge(){
	servo[scoopBridge] = 256;
}

void raiseBridge(){
	servo[scoopBridge] = 0;
}

void stopBridge(){
	servo[scoopBridge] = 127;
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
	servo[scoopBridge] = 127;
	servo[frontBridge] = 100;
	nMotorEncoder[intake] = 0;

}

task main()
{
	init();

	//sticksDown();
	//goto determineRotation;
	servo[frontBridge] = 127;
	waitForStart();
	backward(20);
	wait1Msec(1900); //Comes off ramp
	allStop();
	wait1Msec(500);
	right(100);
	wait1Msec(600);
	allStop();
	wait1Msec(700);
	forward(50);
	wait1Msec(750);
	allStop();
	wait1Msec(750);
	//DO WHATEVER DEPENDING ON CENTER ROTATION
	determineRotation:
	HTIRS2readAllACStrength(HTIRS2, acS1, acS2, acS3, acS4, acS5 );
	int avg1=acS1,avg2,avg3,avg4,avg5;
	for(int j=0;j<10;++j){
		HTIRS2readAllACStrength(HTIRS2, acS1, acS2, acS3, acS4, acS5 );
		avg1=(avg1+acS1)/2;
		avg2=(avg2+acS2)/2;
		avg3=(avg3+acS3)/2;
		avg4=(avg4+acS4)/2;
		avg5=(avg5+acS5)/2;
	}
	if(avg2>16||avg3>30){ //Center is in rotation 1
		nxtDisplayCenteredTextLine(1,"Rot1");
		forward(50);
		wait1Msec(100);
		left(100);
		wait1Msec(600);
		allStop();
		wait1Msec(750);
		forward(100);
		wait1Msec(600);
		allStop();
		wait1Msec(750);
		right(100);
		wait1Msec(550);
		allStop();
		forward(100);
		wait1Msec(750);
		StopAllTasks();
	}
	else{
		right(100);
		wait1Msec(300);
		allStop();
		for(int j=0;j<10;++j){
		HTIRS2readAllACStrength(HTIRS2, acS1, acS2, acS3, acS4, acS5 );
		avg1=(avg1+acS1)/2;
		avg2=(avg2+acS2)/2;
		avg3=(avg3+acS3)/2;
		avg4=(avg4+acS4)/2;
		avg5=(avg5+acS5)/2;
	}
	 if(avg3>15){ //Center is in rotation 2
		nxtDisplayCenteredTextLine(1,"Rot2:%d",avg3);
		forward(100);
		wait1Msec(300);
		allStop();
		wait1Msec(750);
		left(100);
		wait1Msec(600);
		allStop();
		wait1Msec(500);
		forward(100);
		wait1Msec(750);
	}
	else{ //Center is in rotation 3
		nxtDisplayCenteredTextLine(1,"Rot3");
		right(100);
		wait1Msec(100);
		forward(100);
		wait1Msec(800);
		allStop();
		wait1Msec(750);
		left(100);
		wait1Msec(900);
		allStop();
		wait1Msec(500);
		forward(100);
		wait1Msec(750);
	}
}
}
