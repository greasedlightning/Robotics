#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S2, HTServo,     none,     none,  none)
#pragma config(Motor,  mtr_S1_C1_1,   	primaryCubeIntake,tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,  		robotLifter,   		tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     driveLeftBack,  	tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     driveLeftFront,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     driveRightBack,   tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     driveRightFront,  tmotorNormal, openLoop, reversed)
#pragma config(Servo,  srvo_S2_C1_1,    cubeDropper,  tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    cubeLifter,   tServoStandard)
#pragma config(Sensor, S3,     HTCOMPASS,           sensorI2CCustom)

#include "JoystickDriver.c"
#include "/hitechnic-compass.h"

//Variables
int heading;

task main()
{
  HTMCsetTarget(HTCOMPASS);
	int heading=90;
	while(abs(HTMCreadRelativeHeading(HTCOMPASS)-heading)>10){
		motor[driveLeftFront] = 100;
		motor[driveLeftBack]  = 100;
		motor[driveRightFront]= -100;
		motor[driveRightBack] = -100;
	}servo[cubeDropper]=140;
}
