#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S2, HTServo,     none,     none,  none)
#pragma config(Motor,  mtr_S1_C1_1,     primaryCubeIntake,tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     flagSpinner,                   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     robotLifterRight,          tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     driveLeft,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     robotLifterLeft,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     driveRight,  tmotorNormal, openLoop, reversed)
#pragma config(Servo,  srvo_S2_C1_1,    cubeDropper,  tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    cubeLifter,   tServoStandard)
#pragma config(Sensor, S3,     HTCOMPASS,           sensorI2CCustom)
#pragma config(Sensor, S4,     HTIRSENSOR,           sensorI2CCustom)

#include "/hitechnic-compass.h"
#include "JoystickDriver.c"

#define cubeDropperUp 15
#define cubeDropperDown 112
#define CRStop 128
#define CRForward 255
#define CRReverse 0

task main()
{
	servo[cubeDropper] = cubeDropperDown;
	while(true){
		nxtDisplayCenteredBigTextLine(1,"%i",nNxtButtonPressed);
		if(nNxtButtonPressed==1){
			servo[cubeDropper] = cubeDropperUp;
			motor[robotLifterLeft] = 100;
			motor[robotLifterRight] = 100;
			servo[cubeLifter] = 255;

		}
		else if(nNxtButtonPressed==2){
			servo[cubeDropper] = cubeDropperDown;
			motor[robotLifterLeft] = -100;
			motor[robotLifterRight] = -100;
			servo[cubeLifter] = 0;
		}else{
			motor[robotLifterLeft] = 0;
			motor[robotLifterRight] = 0;
			servo[cubeLifter] = 128;
		}if(nNxtButtonPressed==3){
			motor[primaryCubeIntake] = 100;
			motor[flagSpinner] = 100;
			motor[motorA] = 100;
			motor[motorB] = 100;
			motor[motorC] = 100;
		}
		else{
			motor[primaryCubeIntake] = 0;
			motor[flagSpinner] = 0;
			motor[motorA] = 0;
			motor[motorB] = 0;
			motor[motorC] = 0;
		}
	}
}
