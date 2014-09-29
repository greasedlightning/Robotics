#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Motor,  mtr_S1_C1_1,     motorRight,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorLeft,   tmotorNormal, openLoop, reversed)

#include "JoystickDriver.c"

#define threshold 5

void forward(int powerLevel){
	motor[motorRight] = powerLevel;
	motor[motorLeft] = powerLevel;
}
void reverse(int powerLevel){
	motor[motorRight] = -powerLevel;
	motor[motorLeft] = -powerLevel;
}
void left(int powerLevel){
	motor[motorRight] = -powerLevel;
	motor[motorLeft] = powerLevel;
}
void right(int powerLevel){
	motor[motorRight] = powerLevel;
	motor[motorLeft] = -powerLevel;
}
void stop(){
	motor[motorRight] = 0;
	motor[motorLeft] = 0;
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
