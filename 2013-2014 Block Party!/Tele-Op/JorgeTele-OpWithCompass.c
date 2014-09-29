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
#pragma config(Sensor, S3,     HTCOMPASS,           sensorI2CCustom)

#define threshold 25

#include "JoystickDriver.c"
#include "/hitechnic-compass.h"

//Variables
bool intakeOn,joy2Btn_1_Pressed,down,calibrated,driveTrainOn;
int cubeLiftCount,heading,compassReading;
float compassTheta;
float cubeDropperPos,cubeLifterPos;

void init(){ //Initiate Robot(servo positions, intake bool)
	bFloatDuringInactiveMotorPWM = false;
	nNoMessageCounterLimit=500;
	cubeLiftCount=compassTheta=0;
	intakeOn = joy2Btn_1_Pressed = down = calibrated = driveTrainOn = false;
	cubeDropperPos=50;
	cubeLifterPos=128;
	servo[cubeDropper] = cubeDropperPos;
	servo[cubeLifter] = cubeLifterPos;
  HTMCsetTarget(HTCOMPASS);
}

void allStop(){
	motor[primaryCubeIntake]=0;
	motor[robotLifter]=0;
	motor[driveLeftBack]=0;
	motor[driveLeftFront]=0;
	motor[driveRightBack]=0;
	motor[driveRightFront]=0;
	motor[motorA]=0;
	motor[motorB]=0;
	servo[cubeLifter]=128;
	servo[cubeDropper]=ServoValue[cubeDropper];
}

void updateSensors(){
}

void joystickControllerOne() //Driver 1 Controls drive train and hang mechanism

{
	if(joy1Btn(6)) //Hanging Mecahnism Control
		motor[robotLifter] = 100;
	else if(joy1Btn(5))
		motor[robotLifter] = -100;
	else
		motor[robotLifter] = 0;

	if(joy1Btn(9)&&joy1Btn(10))
	{
		if(joystick.joy1_TopHat==1){}
	}

	//Get joystick compassHeading
	if(abs(joystick.joy1_y1)>70 || abs(joystick.joy1_x1)>70){ //Only pay attention to decisive joystick values
		heading=atan((float)joystick.joy1_y1/(float)joystick.joy1_x1)*180/PI;
		if(joystick.joy1_x1>=0){
				heading*=-1;
			if(joystick.joy1_y1>0)
				heading+=360;
		}
		if(joystick.joy1_x1<0){
			heading*=-1;
			heading+=180;
		}
	}
	compassTheta=heading;
	nxtDisplayCenteredTextLine(2,"%i",heading);
	//Drive train right stick forward/reverse left stick compassHeading of robot
	//if(abs(joystick.joy1_y2)>10){ //Forward/reverse overrides turning
	//	motor[driveLeftFront] = joystick.joy1_y2;
	//	motor[driveLeftBack] = joystick.joy1_y2;
	//	motor[driveRightFront] = joystick.joy1_y2;
	//	motor[driveRightBack] = joystick.joy1_y2;
	//}
	compassReading=HTMCreadRelativeHeading(HTCOMPASS);
	if(abs(compassTheta-HTMCreadRelativeHeading(HTCOMPASS))>20){ //3 degrees of leeway
		if(abs(compassTheta-HTMCreadRelativeHeading(HTCOMPASS))<abs(360-compassTheta-HTMCreadRelativeHeading(HTCOMPASS))){ //Decides which turn direction is most efficient
			motor[driveLeftFront] = -100+joystick.joy1_y2;
			motor[driveLeftBack] = -100+joystick.joy1_y2;
			motor[driveRightFront] = 100;
			motor[driveRightBack] = 100;
			driveTrainOn=true;
		}
		else{
			motor[driveLeftFront] = 100;
			motor[driveLeftBack] = 100;
			motor[driveRightFront] = -100+joystick.joy1_y2;
			motor[driveRightBack] = -100+joystick.joy1_y2;
			driveTrainOn=true;
		}
	}else if(abs(joystick.joy1_y2)>threshold){
		motor[driveLeftFront] = joystick.joy1_y2;
		motor[driveLeftBack] = joystick.joy1_y2;
		motor[driveRightFront] = joystick.joy1_y2;
		motor[driveRightBack] = joystick.joy1_y2;
	}
	else{
		motor[driveLeftFront] = 0;
		motor[driveLeftBack] = 0;
		motor[driveRightFront] = 0;
		motor[driveRightBack] = 0;
		driveTrainOn=false;
	}
}

void joystickControllerTwo() //Driver 2 controls cube intake and cube lifter
{
	if(cubeLiftCount<0) //Don't let the count go negative
		cubeLiftCount=0;

	if(joy2Btn(4)){ //Cube dropper Up
		cubeDropperPos=50;
		down=false;
	}
	else if(joy2Btn(3)){ //Cube dropper Down
		cubeDropperPos=145;
		down=true;
		ClearTimer(T2);
	}

	//Continous rotation servo, 0=reverse, 128=stop, 255=forward
	if(joy2Btn(7)){ //Cube lifter Down
		cubeLifterPos=0;
		cubeLiftCount--;
	}
	else if(joy2Btn(8)&&cubeLiftCount<2400){ //Cube lifter Up
		cubeLifterPos=255;
		cubeLiftCount++;
	}
	else if(cubeLiftCount>0&&(down||cubeDropperPos==145)&&time1[T2]>1250){ //Autonomously reset cubeLifter
		if(cubeLiftCount<1000) //Reset cubeDropper when at bottom
			cubeDropperPos=50;
		cubeLifterPos=0;
		cubeLiftCount--;
		down=true;
		if(cubeLiftCount<200)
			down=false;
	}
	else //Stop
		cubeLifterPos=128;

	if(joy2Btn(1)&&!joy2Btn_1_Pressed){ //Intake toggle on if last tick button 1 was Up
		intakeOn=!intakeOn;
		joy2Btn_1_Pressed=true;
	}
	if(!joy2Btn(1))
		joy2Btn_1_Pressed=false;

	if(joy2Btn(2)){ //Reverse intake. Overrides intake forward
		motor[primaryCubeIntake] = -35;
		intakeOn = false;
		motor[motorA] = -100;
		motor[motorB] = -100;
	}
	else if(intakeOn){ //Intake forward if toggle on
		motor[primaryCubeIntake] = 36;
		motor[motorA] = 100;
		motor[motorB] = 100;
	}
	else{ //Stop intake
		motor[primaryCubeIntake] = 0;
		motor[motorA] = 0;
		motor[motorB] = 0;
	}

	//Set servo positions
	servo[cubeDropper] = cubeDropperPos;
	servo[cubeLifter] = cubeLifterPos;
}

task main(){

	init();

	waitForStart();

	ClearTimer(T1);
	int heading=0;
	while(true){
		if(bDisconnected){ //Stop Robot if disconnected
			allStop();
		}else{
			joystickControllerOne();
			joystickControllerTwo();
		}
	}
}
