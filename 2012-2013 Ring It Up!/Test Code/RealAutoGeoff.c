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
#pragma config(Sensor, S2,     LEGOSONAR1,              sensorSONAR)
#pragma config(Sensor, S3,     LEGOSONAR2,              sensorSONAR)

#include "/hitechnic-sensormux.h"
#include "/lego-light.h"
#include "/lego-touch.h"
#include "/hitechnic-superpro.h"
#include "/hitechnic-irseeker-v2.h"

#define threshold 25
#define OPEN 100
#define CLOSED 7
#define BLACK 0
//Field Values of White ADJUST AS NEEDED
#define ARM_UP_WHITE 220
#define WHITE 425

int s1,s2,rightSide,leftSide,targetRightSide,targetLeftSide;

//Movement Functions
void forward(int powerLevel)
{
	motor[motorLeftFront]=powerLevel;
	motor[motorLeftBack]=powerLevel;
	motor[motorRightFront]=powerLevel;
	motor[motorRightBack]=powerLevel;
}

void reverse(int powerLevel)
{
	motor[motorLeftFront]=-powerLevel;
	motor[motorLeftBack]=-powerLevel;
	motor[motorRightFront]=-powerLevel;
	motor[motorRightBack]=-powerLevel;
}

void pivotRight(int powerLevel)
{
	motor[motorLeftFront]=powerLevel;
	motor[motorLeftBack]=powerLevel;
	motor[motorRightFront]=-powerLevel;
	motor[motorRightBack]=-powerLevel;
}

void pivotLeft(int powerLevel)
{
	motor[motorLeftFront]=-powerLevel;
	motor[motorLeftBack]=-powerLevel;
	motor[motorRightFront]=powerLevel;
	motor[motorRightBack]=powerLevel;
}

void left(int powerLevel)
{
	motor[motorLeftFront]=powerLevel;
	motor[motorLeftBack]=powerLevel;
	motor[motorRightFront]=0;
	motor[motorRightBack]=0;
}

void right(int powerLevel)
{
	motor[motorLeftFront]=0;
	motor[motorLeftBack]=0;
	motor[motorRightFront]=powerLevel;
	motor[motorRightBack]=powerLevel;
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
void armUp(int powerLevel)
{
	motor[arms]=powerLevel;
}

void armDown(int powerLevel)
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

void updateVariables(){
		s1 = SensorValue[LEGOSONAR1];
		s2 = SensorValue[LEGOSONAR2];

	targetRightSide = s1+(s1-s2);
	targetLeftSide = s2+(s2-s1);

	if(rightSide<targetRightSide)
		rightSide++;
	else if(rightSide>targetRightSide)
		rightSide--;
	if(leftSide<targetLeftSide)
		leftSide++;
	else if(leftSide>targetLeftSide)
		leftSide--;
}

//Initiate Robot
void init(){
	//Initiate Variables
	s1 = s2 = leftSide = rightSide = targetRightSide = targetLeftSide = 0;

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
}

void debug(){
	nxtDisplayCenteredTextLine(0,"SONAR");
	nxtDisplayCenteredTextLine(1,"%i %i",s1,s2);
}

task main()
{
	while(true){
		debug();

		updateVariables();
		if(s1<threshold*1.3&&s2<threshold*1.3){
			nxtDisplayTextLine(5,"REVERSE");
			debug();
			reverse(50);
			wait1Msec(1000);
			debug();
			pivotLeft(50);
			wait1Msec(500);
		}
		else if(s1<threshold){
			while(s1<threshold*3){
				debug();
				updateVariables();
				pivotRight(100);
			}
		}
		else if(s2<threshold){
			while(s2<threshold*3){
				debug();
				updateVariables();
				pivotLeft(100);
			}
		}else{
			forward(35);
		}
	}
}
