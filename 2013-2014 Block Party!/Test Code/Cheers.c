#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S2, HTServo,     none,     none,  none)
#pragma config(Motor,  mtr_S1_C1_1,     primaryCubeIntake,tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     flagSpinner,                   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     robotLifterRight,          tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     driveLeft,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     robotLifterLeft,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     driveRight,  tmotorNormal, openLoop, reversed)
#pragma config(Servo,  srvo_S2_C1_1,    cubeDropper,  tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    cubeLifter,   tServoStandard)
#pragma config(Sensor, S3,     soundSensor,         sensorSoundDB)
#pragma config(Sensor, S4,     HTIRSENSOR,           sensorI2CCustom)

#include "/hitechnic-compass.h"
#include "JoystickDriver.c"

#define cubeDropperUp 15
#define cubeDropperDown 112
#define CRStop 128
#define CRForward 255
#define CRReverse 0

//Variables
bool intakeOn,joy2Btn_1_Pressed,down;
int cubeLiftCount;

task main()
{
	while(true){
	nxtDisplayCenteredBigTextLine(1,"%i",SensorValue[S3]);
}



}
