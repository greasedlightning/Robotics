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
#pragma config(Sensor, S4,     HTIRS,           sensorI2CCustom)

//TODO: Change end of crate check
//			Calibrate compass sensor

#include "/hitechnic-irseeker-v2.h"
#include "/hitechnic-compass.h"
#include "JoystickDriver.c"

#define fullHeight 4.5
#define cubeDropperUp 0
#define cubeDropperDown 135
#define cubeLifterStop 128
#define cubeLifterUp 255
#define cubeLifterDown 0
int dcS1,dcS2,dcS3,dcS4,dcS5,compassHeading;
float time,time2,wait=0.0; //Seconds
bool irFound,cubeRaised;

void init(){
 				HTMCsetTarget(HTCOMPASS);
        bFloatDuringInactiveMotorPWM = false;
        irFound=cubeRaised=false;
        servo[cubeDropper] = cubeDropperUp;
        servo[cubeLifter] = cubeLifterStop;
}

//Movement Functions
void forward(int powerLevel,float seconds){
        motor[driveLeft] = powerLevel;
        motor[driveRight] = powerLevel;
        wait1Msec(seconds*1000);
        motor[driveLeft] = 0;
        motor[driveRight] = 0;
}
void forward(int powerLevel){
        motor[driveLeft] = powerLevel;
        motor[driveRight] = powerLevel;
}

void reverse(int powerLevel,float seconds){
        motor[driveLeft] = -powerLevel;
        motor[driveRight] = -powerLevel;
        wait1Msec(seconds*1000);
        motor[driveLeft] = 0;
        motor[driveRight] = 0;
}
void reverse(int powerLevel){
        motor[driveLeft] = -powerLevel;
        motor[driveRight] = -powerLevel;
}

void left(int powerLevel,float seconds){
        motor[driveLeft] = -powerLevel;
        motor[driveRight] = powerLevel;
        wait1Msec(seconds*1000);
        motor[driveLeft] = 0;
        motor[driveRight] = 0;
}
void left(int powerLevel){
        motor[driveLeft] = -powerLevel;
        motor[driveRight] = powerLevel;
}

void compassLeft(int powerLevel, int targetCompassHeading){
	while(abs(compassHeading-targetCompassHeading)>2){
		left(powerLevel);
	}
  motor[driveLeft] = 0;
  motor[driveRight] = 0;
}

void right(int powerLevel,float seconds){
        motor[driveLeft] = powerLevel;
        motor[driveRight] = -powerLevel;
        wait1Msec(seconds*1000);
        motor[driveLeft] = 0;
        motor[driveRight] = 0;
}
void right(int powerLevel){
        motor[driveLeft] = powerLevel;
        motor[driveRight] = -powerLevel;
}

void driveStop(){
        motor[driveLeft] = 0;
        motor[driveRight] = 0;
}
void allStop(){
        motor[primaryCubeIntake]=0;
				motor[robotLifterRight]=0;
				motor[robotLifterLeft]=0;
        motor[driveLeft]=0;
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

void raiseArms(float seconds){
				motor[robotLifterRight]=100;
				motor[robotLifterLeft]=100;
        wait1Msec(seconds*1000);
				motor[robotLifterRight]=0;
				motor[robotLifterLeft]=0;
}

void lowerArms(float seconds){
				motor[robotLifterRight]=-100;
				motor[robotLifterLeft]=-100;
        wait1Msec(seconds*1000);
				motor[robotLifterRight]=0;
				motor[robotLifterLeft]=0;
}

void DEBUG(){
        nxtDisplayCenteredTextLine(1,"IRSENSOR");
        nxtDisplayCenteredTextLine(2,"%i",dcS1);
        nxtDisplayCenteredTextLine(3,"%i",dcS2);
        nxtDisplayCenteredTextLine(4,"%i",dcS3);
        nxtDisplayCenteredTextLine(5,"%i",dcS4);
        nxtDisplayCenteredTextLine(6,"%i",dcS5);
}

task raiseCubeLifter(){
	raiseCube(fullHeight);
	cubeRaised=true;
  StopTask(raiseCubeLifter);
}

task raiseArmsTask(){
        raiseArms(1);
        StopTask(raiseArmsTask);
}

task initiateCubePosition(){
        intakeOn(2);
        StartTask(raiseCubeLifter);
        StopTask(initiateCubePosition);
}

task lowerCubeTask(){
				lowerCube(fullHeight);
				dropCube();
}

task main()
{
	int powerLevel = 100;
	while(true){
		getJoystickSettings(joystick);
		if(joy1Btn(1))
			powerLevel = 20;
		else if(joy1Btn(2))
			powerLevel = 40;
		else if(joy1Btn(3))
			powerLevel = 60;
		else if(joy1Btn(4))
			powerLevel = 80;
		else if(joy1Btn(5))
			powerLevel = 100;

		if(joystick.joy1_y1>0){
			raiseCube(.1);
		}else if(joystick.joy1_y1<0){
			lowerCube(.1);
		}else if(joystick.joy1_x1>0){
			right(powerLevel);
		}else if(joystick.joy1_x1<0){
			left(powerLevel);
		}else{
			driveStop();
		}
	}

}
