#pragma config(Hubs,   S1, HTMotor, HTServo, HTMotor, none)
#pragma config(Motor,  mtr_S1_C1_1,     driveRight,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     lift,  tmotorNormal, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    fieldGrabberRight, tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    fieldGrabberLeft, tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    fieldRoller, tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    frontBridge, tServoStandard)
#pragma config(Motor,  mtr_S1_C3_1,     driveLeft,  tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     intake, tmotorNormal, openLoop)

#include "JoystickDriver.c"

#define open 0
#define grab 100
#define closed 105
#define inside 215
#define _threshold 20

bool intakeOn = false;

void init(){
	servo[fieldGrabberLeft] = 265;//inside-15;
	servo[fieldGrabberRight] = 235-inside;
	servo[fieldRoller] = 127;
	servo[frontBridge] = 255;
}

void allStop(){
	motor[driveLeft] = 0;
}

float exponentialJoystick(int joyVal){
	return (float)5.60015*pow(2.718281828,0.96781*(abs(joyVal)/40));
}

void joystickOne(){

	if(abs(joystick.joy1_y2)>_threshold){
		motor[driveRight] = -joystick.joy1_y2/abs(joystick.joy1_y2)*exponentialJoystick(joystick.joy1_y2);
	}
	else{
		motor[driveRight] = 0;
	}
	if(abs(joystick.joy1_y1)>_threshold){
		motor[driveLeft] = joystick.joy1_y1/abs(joystick.joy1_y1)*exponentialJoystick(joystick.joy1_y1);
	}
	else{
		motor[driveLeft] = 0;
	}

	if(joy1Btn(7)){
		servo[fieldRoller] = 0;
	}else if(joy1Btn(8)){
		servo[fieldRoller] = 256;
	}else{
		servo[fieldRoller] = 127;
	}
	if(joy1Btn(11))
		servo[frontBridge] = 0;
	else if(joy1Btn(12))
		servo[frontBridge] = 255;

	if(joy1Btn(5))
		motor[lift] = -20;
	else if(joy1Btn(6))
		motor[lift] = 100;
	else
		motor[lift] = 0;

	if(joy1Btn(1)){
		servo[fieldGrabberLeft] = inside-15;
		servo[fieldGrabberRight] = 255-inside;
	}
	else if(joy1Btn(2)){
		servo[fieldGrabberLeft] = open;
		servo[fieldGrabberRight] = 255-open;
	}
	else if(joy1Btn(3)){
		servo[fieldGrabberLeft] = closed;
		servo[fieldGrabberRight] = 255-closed;
	}
	if(joy1Btn(10)){
		while(joy1Btn(10)){}
		intakeOn = !intakeOn;
	}if(joy1Btn(9))
	{
		motor[intake] = -100;
		intakeOn = false;
	}
	else if(intakeOn){
		motor[intake] = 100;
	}
	else{
	 	motor[intake] = 0;
	}
}

void joyStickTwo(){

}

task main()
{
	init();

	waitForStart();

	while(true){
		joystickOne();
	}
}
