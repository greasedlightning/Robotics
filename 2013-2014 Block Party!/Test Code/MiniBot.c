#pragma config(Hubs,  S1, HTMotor,  none,  none,  none)
#pragma config(Motor,  mtr_S1_C1_1,     leftDrive,tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     rightDrive,tmotorNormal, openLoop,reversed)

#include "JoystickDriver.c"

task main()
{
	while(true){
		if(abs(joystick.joy1_y1)>25)
			motor[leftDrive] = joystick.joy1_y1;
		else
			motor[leftDrive] = 0;
		if(abs(joystick.joy1_y2)>25)
			motor[rightDrive] = joystick.joy1_y2;
		else
			motor[rightDrive] = 0;
	}
}
