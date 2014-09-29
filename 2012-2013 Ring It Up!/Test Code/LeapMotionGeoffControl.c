#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Motor,  mtr_S1_C1_1,     motorFLeft,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorBLeft,   tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     motorFRight,   tmotorNormal, openLoop,reversed)
#pragma config(Motor,  mtr_S1_C2_2,     motorBRight,   tmotorNormal, openLoop)

#include "JoystickDriver.c"

#define threshold 5

void forward(int powerLevel){
	motor[motorFRight] = powerLevel;
	motor[motorBRight] = powerLevel;
	motor[motorFLeft] = powerLevel;
	motor[motorBLeft] = powerLevel;
}
void reverse(int powerLevel){
	motor[motorFRight] = -powerLevel;
	motor[motorBRight] = -powerLevel;
	motor[motorFLeft] = -powerLevel;
	motor[motorBLeft] = -powerLevel;
}
void left(int powerLevel){
	motor[motorFRight] = -powerLevel;
	motor[motorBRight] = -powerLevel;
	motor[motorFLeft] = powerLevel;
	motor[motorBLeft] = powerLevel;
}
void right(int powerLevel){
	motor[motorFRight] = powerLevel;
	motor[motorBRight] = powerLevel;
	motor[motorFLeft] = -powerLevel;
	motor[motorBLeft] = -powerLevel;
}
void stop(){
	motor[motorFRight] = 0;
	motor[motorBRight] = 0;
	motor[motorFLeft] = 0;
	motor[motorBLeft] = 0;
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
			forward(powerLevel);
		}else if(joystick.joy1_y1<0){
			reverse(powerLevel);
		}else if(joystick.joy1_x1>0){
			right(powerLevel);
		}else if(joystick.joy1_x1<0){
			left(powerLevel);
		}else{
			stop();
		}
	}
}
