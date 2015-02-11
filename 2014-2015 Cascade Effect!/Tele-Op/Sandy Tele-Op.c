#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  HTMotor)
#pragma config(Motor,  mtr_S1_C1_1,     driveRight,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     lift,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     driveLeft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     intake,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     liftMotor3,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    fieldGrabberRight,    tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    fieldGrabberLeft,     tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    scoopBridge,          tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    rampBridge,          tServoStandard)
#pragma config(Sensor, S4,     HTGYRO,              sensorAnalogInactive)

#include "JoystickDriver.c"
#include "/hitechnic-gyro.h"

#define _open 0
#define _closed 205
#define _threshold 20

void init(){
	servo[fieldGrabberLeft] = _open;
	servo[fieldGrabberRight] = 255-_open;
	servo[rampBridge] = 0;
	nMotorEncoder[intake] = 0;
	servo[scoopBridge] = 155;
}

float exponentialJoystick(int joyVal){
	return (float)5.60015*pow(2.718281828,0.96781*(abs(joyVal)/40));
}

void sticksDown(){
	servo[fieldGrabberLeft] = _open;
	servo[fieldGrabberRight] = 255-_open;
}

void sticksUp(){
	servo[fieldGrabberLeft] = _closed-15;
	servo[fieldGrabberRight] = 255-_closed;
}

void retainBalls()
{
	servo[scoopBridge] = 155;
}

void releaseBalls()
{
	servo[scoopBridge] = 10;
}

void closeRamp()
{
	servo[rampBridge] = 0;
}

void openRamp()
{
	servo[rampBridge] = 100;
}

void allStop(){
	motor[driveLeft] = 0;
	motor[driveRight] = 0;
	motor[lift] = 0;
	motor[liftMotor3] = 0;
	motor[intake] = 0;
}


float heading = 0;

void drive(int direction,int powerLevel){//Dir:0 = forward, 1 = reverse:
  //Accurately drive in a straigt line using GYRO
	int dir = direction==0?1:-1;
	if(direction==0){
		motor[driveLeft] = (powerLevel+powerLevel*.15*heading)*dir;
		motor[driveRight] = -(powerLevel-powerLevel*.15*heading)*dir;
	}else{
		motor[driveLeft] = (powerLevel-powerLevel*.15*heading)*dir;
		motor[driveRight] = -(powerLevel+powerLevel*.15*heading)*dir;
	}
}

//Joystick one controls the drive train and goal grabbers
void joystickOne(){

	getJoystickSettings(joystick);
	if(joystick.joy1_TopHat==-1)
		heading = 0;
	if(joystick.joy1_TopHat==0){
		drive(0,100);
		}else if(joystick.joy1_TopHat==4){
		drive(1,100);
	}
	else{
		if(abs(joystick.joy1_y1)>_threshold)//Check joystick not in deadzone
		{
			motor[driveLeft] = joystick.joy1_y1/abs(joystick.joy1_y1)*exponentialJoystick(joystick.joy1_y1);
		}
		else
		{
			motor[driveLeft] = 0;
		}
		if(abs(joystick.joy1_y2)>_threshold)
		{
			motor[driveRight] = -joystick.joy1_y2/abs(joystick.joy1_y2)*exponentialJoystick(joystick.joy1_y2);
		}
		else
		{
			motor[driveRight] = 0;
		}
	}


	if(joy1Btn(8)||joy1Btn(7))//Grab rolling goal
	{
		sticksDown();
	}

	if(joy1Btn(6)||joy1Btn(5))//Release rolling goal
	{
		sticksUp();
	}

}

//Joystick two controls the lift, scoop bridge, and intake
void joystickTwo()
{

	getJoystickSettings(joystick);

	if(joy2Btn(1))//Intake reverse
	{
		motor[intake] = -60;
	}
	else if(joy2Btn(3))//Intake forward
	{
		motor[intake] = 80;
	}
	else{
		motor[intake] = 0;
	}
	if(joy2Btn(2))//Ramp bridge down
	{
		openRamp();
	}
	else if(joy2Btn(4))//Ramp bridge up
	{
		closeRamp();
	}

	if(joy2Btn(5))//Lift down
	{
		motor[lift] = 70;
		motor[liftMotor3] = -70;
	}
	else if(joy2Btn(6))//Lift ups
	{
		motor[lift] = -100;
		motor[liftMotor3] = 100;
	}
	else
	{
		motor[lift] = 0;
		motor[liftMotor3] = 0;
	}

	if(joy2Btn(7))//Raise scoop bridge
	{
		retainBalls();
	}
	else if(joy2Btn(8))//Release balls
	{
		releaseBalls();
	}

}

task main()
{

	init();

	HTGYROstartCal(HTGYRO);
	waitForStart();
	float rotSpeed;

	while(true)
	{
		joystickOne();
		joystickTwo();

		nxtDisplayCenteredBigTextLine(1,"%i",heading);
		nxtDisplayCenteredTextLine(2,"Intake:%i",nMotorEncoder[intake]);

		while (time1[T1] < 20)
			wait1Msec(1);
		// Reset the timer
		time1[T1]=0;

		// Read the current rotation speed
		rotSpeed = HTGYROreadRot(HTGYRO);

		// Calculate the new heading by adding the amount of degrees
		// we've turned in the last 20ms
		// If our current rate of rotation is 100 degrees/second,
		// then we will have turned 100 * (20/1000) = 2 degrees since
		// the last time we measured.
		heading += rotSpeed * 0.02;

		wait1Msec(5);//Debouncing
	}
}
