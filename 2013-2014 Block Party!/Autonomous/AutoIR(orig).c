#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S2, HTServo,     none,     none,  none)
#pragma config(Motor,  mtr_S1_C1_1,   	primaryCubeIntake,tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,  		robotLifter,   		tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     driveLeftBack,  	tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     driveLeftFront,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     driveRight,   tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     flagSpinner,  tmotorNormal, openLoop, reversed)
#pragma config(Servo,  srvo_S2_C1_1,    cubeDropper,  tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    cubeLifter,   tServoStandard)
#pragma config(Sensor, S3,     HTCOMPASS,           sensorI2CCustom)
#pragma config(Sensor, S4,     HTIRS,           sensorI2CCustom)

#include "/hitechnic-irseeker-v2.h"
#include "/hitechnic-compass.h"
#include "JoystickDriver.c"

#define cubeDropperUp 0
#define cubeDropperDown 120
#define cubeLifterStop 128
#define cubeLifterUp 255
#define cubeLifterDown 0
int dcS1,dcS2,dcS3,dcS4,dcS5;
float time;

void init(){
	bFloatDuringInactiveMotorPWM = false;
	//nNoMessageCounterLimit=500;
	servo[cubeDropper] = cubeDropperUp;
	servo[cubeLifter] = cubeLifterStop;
}

//Movement Functions
void forward(int powerLevel,float seconds){
	motor[driveLeftBack] = powerLevel;
	motor[driveLeftFront] = powerLevel;
	motor[driveRight] = powerLevel;
	wait1Msec(seconds*1000);
	motor[driveLeftBack] = 0;
	motor[driveLeftFront] = 0;
	motor[driveRight] = 0;
}
void forward(int powerLevel){
	motor[driveLeftBack] = powerLevel;
	motor[driveLeftFront] = powerLevel;
	motor[driveRight] = powerLevel;
}

void reverse(int powerLevel,float seconds){
	motor[driveLeftBack] = -powerLevel;
	motor[driveLeftFront] = -powerLevel;
	motor[driveRight] = -powerLevel;
	wait1Msec(seconds*1000);
	motor[driveLeftBack] = 0;
	motor[driveLeftFront] = 0;
	motor[driveRight] = 0;
}
void reverse(int powerLevel){
	motor[driveLeftBack] = -powerLevel;
	motor[driveLeftFront] = -powerLevel;
	motor[driveRight] = -powerLevel;
}

void left(int powerLevel,float seconds){
	motor[driveLeftBack] = -powerLevel;
	motor[driveLeftFront] = -powerLevel;
	motor[driveRight] = powerLevel;
	wait1Msec(seconds*1000);
	motor[driveLeftBack] = 0;
	motor[driveLeftFront] = 0;
	motor[driveRight] = 0;
}
void left(int powerLevel){
	motor[driveLeftBack] = -powerLevel;
	motor[driveLeftFront] = -powerLevel;
	motor[driveRight] = powerLevel;
}

void right(int powerLevel,float seconds){
	motor[driveLeftBack] = powerLevel;
	motor[driveLeftFront] = powerLevel;
	motor[driveRight] = -powerLevel;
	wait1Msec(seconds*1000);
	motor[driveLeftBack] = 0;
	motor[driveLeftFront] = 0;
	motor[driveRight] = 0;
}
void right(int powerLevel){
	motor[driveLeftBack] = powerLevel;
	motor[driveLeftFront] = powerLevel;
	motor[driveRight] = -powerLevel;
}

void allStop(){
	motor[primaryCubeIntake]=0;
	motor[robotLifter]=0;
	motor[driveLeftBack]=0;
	motor[driveLeftFront]=0;
	motor[driveRight]=0;
	motor[flagSpinner]=0;
	motor[motorA]=0;
	motor[motorB]=0;
	servo[cubeLifter]=128;
	servo[cubeDropper]=ServoValue[cubeDropper];
}

//Secondary Actions
void dropCube(){
	servo[cubeDropper] = cubeDropperDown;
}

void cubeUp(){
	servo[cubeDropper] = cubeDropperUp;
}

void raiseCube(float seconds){
	servo[cubeLifter]=255;
	wait1Msec(seconds*1000);
	servo[cubeLifter]=128;
}

void lowerCube(float seconds){
	servo[cubeLifter]=0;
	wait1Msec(seconds*1000);
	servo[cubeLifter]=128;
}

void intakeOn(float seconds){
	motor[primaryCubeIntake] = 100;
	wait1Msec(seconds*1000);
	motor[primaryCubeIntake] = 0;
}

task main()
{
	waitForStart();
	//wait1Msec(6000);
	init();

	intakeOn(2);

	//waitForStart();

	//forward(60,.5);
	//right(100,1);
	//HTIRS2readAllDCStrength(HTIRS, dcS1, dcS2, dcS3, dcS4, dcS5); //Take IR reading
	HTIRS2readAllDCStrength(HTIRS, dcS1, dcS2, dcS3, dcS4, dcS5); //Take IR reading
	ClearTimer(T1);
	while(true){
		reverse(30);
		HTIRS2readAllDCStrength(HTIRS, dcS1, dcS2, dcS3, dcS4, dcS5); //Take IR reading
		nxtDisplayCenteredTextLine(1,"IRSENSOR");
		nxtDisplayCenteredTextLine(2,"%i",dcS1);
		nxtDisplayCenteredTextLine(3,"%i",dcS2);
		nxtDisplayCenteredTextLine(4,"%i",dcS3);
		nxtDisplayCenteredTextLine(5,"%i",dcS4);
		nxtDisplayCenteredTextLine(6,"%i",dcS5);
		if(dcS3>50)
			break;
		if(time1[T1]/1000>7)
			goto end;
	}
	//reverse(30,1);
	time=time1[T1];
	left(100,1.4);
	raiseCube(4.5);
	reverse(30,.7);
	dropCube();
	wait1Msec(1000);
	cubeUp();
	forward(30,.65);
	right(100,1.5);
	forward(30,(time/1000));
	//Ramp
	right(100,.8);
	forward(100,1);
	right(100,1);
	forward(100,1);
	right(100,1.1);
	forward(100,1.5);
	lowerCube(4.5);
	end:;
	allStop();
}
