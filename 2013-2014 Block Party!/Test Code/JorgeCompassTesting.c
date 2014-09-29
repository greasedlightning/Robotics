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
#pragma config(Sensor, S3,     HTMC,                sensorI2CCustom)

#include "JoystickDriver.c"
#include "/hitechnic-compass.h"

task main()
{
  int _target = HTMCsetTarget(HTMC);
  int turnTarget=90;
	while(true){
		nxtDisplayCenteredTextLine(1,"Compass:");
		nxtDisplayCenteredTextLine(2,"%f",SensorValue[S3]);
		if(turnTarget==HTMCreadRelativeHeading(HTMC)){
			break;
		}
		motor[driveLeftFront] = 50;
		motor[driveLeftBack] = 50;
		motor[driveRightFront] = -50;
		motor[driveRightBack] = -50;
	}
}
