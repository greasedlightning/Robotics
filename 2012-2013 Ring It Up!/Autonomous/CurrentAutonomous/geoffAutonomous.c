#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Motor,  mtr_S1_C1_1,     motorLeftFront,   tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorLeftBack,   tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     ramp,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     arms,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     motorRightFront,   tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     motorRightBack,   tmotorNormal, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    clawLeft,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    clawRight,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    rampLock,               tServoStandard)
#pragma config(Sensor, S2,     LEGOLS,              sensorLightInactive)
#pragma config(Sensor, S3,     HTSPB,                sensorI2CCustom9V)
#pragma config(Sensor, S4,     HTSMUX,              sensorI2CCustom)


#include "JoystickDriver.c"
#include "/hitechnic-sensormux.h"
#include "/lego-light.h"
#include "/lego-touch.h"
#include "/hitechnic-superpro.h"
#include "/hitechnic-irseeker-v2.h"

#define OPEN 100
#define CLOSED 7
#define BLACK 0
//Field Values of White ADJUST AS NEEDED
#define ARM_UP_WHITE 220
#define WHITE 425

//const tMUXSensor HTIRS2 = msensor_S1_1;

//Define Variables
int acS1, acS2, acS3, acS4, acS5;

//Movement Functions
void forward(int powerLevel=100)
{
	motor[motorLeftFront]=powerLevel;
	motor[motorLeftBack]=powerLevel;
	motor[motorRightFront]=powerLevel;
	motor[motorRightBack]=powerLevel;
}

void forwardUntil(int powerLevel,int color)
{
	while(true)
	{
		if(color==0){
			if(SensorValue[LEGOLS]<425)
				break;
	  }
		else{
			if(SensorValue[LEGOLS]>color)
				break;
		}
		forward(powerLevel);
		nxtDisplayCenteredTextLine(1,"%i",SensorValue[LEGOLS]);
	}
}

void reverse(int powerLevel=100)
{
	motor[motorLeftFront]=-powerLevel;
	motor[motorLeftBack]=-powerLevel;
	motor[motorRightFront]=-powerLevel;
	motor[motorRightBack]=-powerLevel;
}

void reverseUntil(int powerLevel,int color)
{
	while(true)
	{
		if(color==0){
			if(color<425)
				break;
		}
		else{
			if(SensorValue[LEGOLS]>color)
				break;
		}
		reverse(powerLevel);
	}
}

void pivotRight(int powerLevel=100)
{
	motor[motorLeftFront]=powerLevel;
	motor[motorLeftBack]=powerLevel;
	motor[motorRightFront]=-powerLevel;
	motor[motorRightBack]=-powerLevel;
}

void pivotRightUntil(int powerLevel,int color)
{
	while(true)
	{
		if(color==0){
			if(color<425)
				break;
		}
		else{
			if(SensorValue[LEGOLS]>color)
				break;
		}
		pivotRight(powerLevel);
	}
}

void pivotLeft(int powerLevel=100)
{
	motor[motorLeftFront]=-powerLevel;
	motor[motorLeftBack]=-powerLevel;
	motor[motorRightFront]=powerLevel;
	motor[motorRightBack]=powerLevel;
}

void pivotLeftUntil(int powerLevel,int color)
{
	while(true)
	{
		if(color==0){
			if(color<425)
				break;
		}
		else{
			if(SensorValue[LEGOLS]>color)
				break;
		}
		pivotLeft(powerLevel);
	}
}

void right(int powerLevel=100)
{
	motor[motorLeftFront]=powerLevel;
	motor[motorLeftBack]=powerLevel;
	motor[motorRightFront]=0;
	motor[motorRightBack]=0;
}

void rightUntil(int powerLevel,int color)
{
	while(true)
	{
		if(color==0){
			if(color<425)
				break;
		}
		else{
			if(SensorValue[LEGOLS]>color)
				break;
		}
		right(powerLevel);
	}
}

void left(int powerLevel=100)
{
	motor[motorLeftFront]=0;
	motor[motorLeftBack]=0;
	motor[motorRightFront]=powerLevel;
	motor[motorRightBack]=powerLevel;
}

void leftUntil(int powerLevel,int color)
{
	while(true)
	{
		if(color==0){
			if(color<425)
				break;
		}
		else{
			if(SensorValue[LEGOLS]>color)
				break;
		}
		left(powerLevel);
	}
}

//Claw Functions
void openClaw(){
	servo[clawLeft]=OPEN;
	servo[clawRight]=210-OPEN;
}

void closeClaw(){
	servo[clawLeft]=CLOSED;
	servo[clawRight]=210-CLOSED;
}

//Arm Functions
void armUp(int powerLevel=100)
{
	motor[arms]=powerLevel;
}

void armDown(int powerLevel=100)
{
	motor[arms]=-powerLevel;
}

void armUpUntil(int powerLevel,int encoderVal)
{
	while(nMotorEncoder[arms]>encoderVal)
		motor[arms]=powerLevel;
}

void armDownUntil(int powerLevel,int encoderVal)
{
	while(nMotorEncoder[arms]<encoderVal)
		motor[arms]=-powerLevel;
}


//Stop All
void stop()
{
	motor[motorLeftFront]=0;
	motor[motorLeftBack]=0;
	motor[motorRightFront]=0;
	motor[motorRightBack]=0;
	motor[ramp]=0;
	motor[arms]=0;
}

//Initiate Robot
void init(){
	//Initiate SuperPro pinout
  HTSPBsetupIO(HTSPB, 0xFF); //Set all digital i/o as outputs

	//Initiate Variables
	acS1 = acS2 = acS3 = acS4 = acS5 = 0;

	//Initiate Motors
	nMotorEncoder[arms]=0;
	motor[motorLeftFront]=0;
	motor[motorRightFront]=0;
	motor[motorRightFront]=0;
	motor[motorRightBack]=0;
	motor[ramp]=0;
	motor[arms]=0;

	//Initiate Servos
	closeClaw();

	//Initiate Sensor
	LSsetActive(LEGOLS);
}

task main()
{
	init(); //Initiate the robot

	waitForStart(); //Needed for FTC competition

	forwardUntil(20,WHITE); //First reach white line

	stop();

	forwardUntil(100,BLACK); //Move slightly passed line

	stop();

	armUpUntil(30,-1400); //Raise the arm to first rack lvl

	stop();

	pivotRightUntil(80,ARM_UP_WHITE); //Rotate to line up with white line

	stop();

	pivotRightUntil(100,BLACK); //Rotate past white line

	stop();

	pivotRight(20); //Micro adjustment(hardcoded)
	wait1Msec(500);

	stop();

	pivotLeftUntil(60,ARM_UP_WHITE); //Rotate back to white line(increase accuracy)

	stop();

	forward(20); //Move forward for 1.7 seconds(hardcoded)
	wait1Msec(1700);

	stop();

	openClaw(); //...Open Claw

	armDownUntil(20,-50); //Lower the arm to have the ring supported by rack

	stop();

	reverse(20); //Back up to let alliance partner score and get closer position to dispenser
	wait1Msec(3000);

	stop();

}
