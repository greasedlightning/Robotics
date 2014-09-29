#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S2, HTServo,  HTServo,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     leftb,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     leftf,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     rightb,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     rightf,        tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S2_C1_1,    cubedropper,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    cubelifter,               tServoStandard)

#include "JoystickDriver.c"
	void forward(int sec){
	motor[leftb]=60;
	motor[leftf]=60;
	motor[rightb]=60;
	motor[rightf]=60;
	wait10Msec(sec);
	motor[leftb]=0;
	motor[leftf]=0;
	motor[rightb]=0;
	motor[rightf]=0;
}

void turnleft(int sec){
	motor[leftb]=-100;
	motor[leftf]=-100;
	motor[rightb]=100;
	motor[rightf]=100;
	wait10Msec(sec);// divide by 100 to get #of sec
	motor[leftb]=0;
	motor[leftf]=0;
	motor[rightb]=0;
	motor[rightf]=0;
}

void turnright(int sec){
	motor[leftb]=60;
	motor[leftf]=60;
	motor[rightb]=-60;
	motor[rightf]=-60;
	wait10Msec(sec);
	motor[leftb]=0;
	motor[leftf]=0;
	motor[rightb]=0;
	motor[rightf]=0;
}

void backward(int sec){
	motor[leftb]=-60;
	motor[leftf]=-60;
	motor[rightb]=-60;
	motor[rightf]=-60;
	wait10Msec(sec);
	motor[leftb]=0;
	motor[leftf]=0;
	motor[rightb]=0;
	motor[rightf]=0;
}
task main()//when cube in ramp on right turn not tested
{
waitForStart();
	intakeOn(2);
servo[cubedropper]=50;
servo[cubelifter]=128;
servo[cubedropper]=50;
servo[cubelifter]=255;
wait10Msec(400);
backward(65);
servo[cubelifter]=255;
servo[cubedropper]=145;
forward(60);
//ramp
turnright(200);//150 turns about 90 75 is about 30
backward(170);
turnleft(140);
backward(210);
servo[cubelifter]=0;
wait10Msec(400);
}
