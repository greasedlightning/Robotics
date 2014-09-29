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

#define cubeDropperUp 0
#define cubeDropperDown 110
#define CRStop 128
#define CRForward 255
#define CRReverse 0

//Variables
bool intakeOn,joy1Btn_1_Pressed,down,on;
int cubeLiftCount,cubeDropperPos,cubeLifterPos;

void init(){ //Initiate Robot(servo positions, intake bool)
	bFloatDuringInactiveMotorPWM = false;
	nNoMessageCounterLimit=500;
	cubeLiftCount=cubeDropperPos=cubeLifterPos=0;
	intakeOn = joy1Btn_1_Pressed = down = on = false;
	servo[cubeDropper] = cubeDropperDown;
	servo[cubeLifter] = CRStop;
	//HTMCstartCal(HTCOMPASS);
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
	servo[cubeLifter]=CRStop;
	servo[cubeDropper]=ServoValue[cubeDropper];
}

float exponentialJoystick(int joyVal){
	return (float)pow((joyVal/12),2);
}

void updateSensors(){
}

void joystickControllerOne() //Driver 1 Controls drive train and hang mechanism
{
	if(joystick.joy1_x1>20)
	{
		motor[driveRight] = -100;
		motor[driveLeft] = 100;
	}else if(joystick.joy1_x1<-20)
	{
		motor[driveRight] = 100;
		motor[driveLeft] = -100;
	}
else 	if(joystick.joy1_y1>10)
	{
		motor[driveRight] = 100;
		motor[driveLeft] = 100;
	}else if(joystick.joy1_y1<-10)
	{
		motor[driveRight] = -100;
		motor[driveLeft] = -100;
	}else{

		motor[driveRight] = 0;
		motor[driveLeft] = 0;
}
	if(joy1Btn(1)&&!intakeOn&&!joy1Btn_1_Pressed){ //Intake toggle on if last tick button 1 was Up
		intakeOn=true;
		joy1Btn_1_Pressed=true;
	}
	else if(!joy1Btn(1)){
		joy1Btn_1_Pressed=false;
	}
	//if(joy1Btn(1)&&intakeOn)
	//	on=true;

	//if(on){
	//	if(joy1Btn(1)&&on){ //Reverse intake. Overrides intake forward
	//		//HTMCstopCal(HTCOMPASS);
	//		motor[primaryCubeIntake] = -35;
	//		motor[motorA] = -100;
	//		motor[motorB] = -100;
	//	}if(!joy1Btn(1)){
	//		on = false;
	//		intakeOn = false;
	//	}
	//}
	//if(intakeOn){ //Intake forward if toggle on
	//	motor[primaryCubeIntake] = 100;
	//	motor[motorA] = 100;
	//	motor[motorB] = 100;
	//}
	//else{ //Stop intake
	//	motor[primaryCubeIntake] = 0;
	//	motor[motorA] = 0;
	//	motor[motorB] = 0;
	//}
	if(abs(joystick.joy1_x2)>30){
		motor[primaryCubeIntake] = 100*((joystick.joy1_x2)/abs(joystick.joy1_x2));
		motor[motorA] = 100*((joystick.joy1_x2)/abs(joystick.joy1_x2);
		motor[motorB] = 100*((joystick.joy1_x2)/abs(joystick.joy1_x2);
	}
	else{ //Stop intake
		motor[primaryCubeIntake] = 0;
		motor[motorA] = 0;
		motor[motorB] = 0;
	}


	if(joy1Btn(5)){ //Hanging Mecahnism Control
		motor[robotLifterRight]=100;
		motor[robotLifterLeft]=100;
	}
	else if(joy1Btn(4)){
		motor[robotLifterRight]=-100;
		motor[robotLifterLeft]=-100;
	}
	else{
		motor[robotLifterRight]=0;
		motor[robotLifterLeft]=0;
	}
	if(joy1Btn(11)){ //Cube dropper Up
		servo[cubeDropper]=cubeDropperUp;
		down=false;
	}
	else if(joy1Btn(10)){ //Cube dropper Down
		servo[cubeDropper]=cubeDropperDown;
		down=true;
		ClearTimer(T2);
	}


	if(cubeLiftCount<0) //Don't let the count go negative
		cubeLiftCount=0;



	//Continous rotation servo, 0=reverse, 128=stop, 255=forward
	if(joy1Btn(3)){ //Cube lifter Down
		servo[cubeLifter]=CRReverse;
		cubeLiftCount--;
	}
	else if(joy1Btn(2)){ //Cube lifter Up
		servo[cubeLifter]=CRForward;
		cubeLiftCount++;
	}
	else if(cubeLiftCount>0&&(down||cubeDropperPos==100)&&time1[T2]>1250){ //Autonomously reset cubeLifter
		if(cubeLiftCount<50) //Reset cubeDropper when at bottom
			cubeDropperPos=0;
		cubeLifterPos=0;
		cubeLiftCount--;
		down=true;
		if(cubeLiftCount<50)
			down=false;
	}
	else //Stop
		servo[cubeLifter]=CRStop;

	if(joy1Btn(6))
		motor[flagSpinner]=100;
	else if(joy1Btn(7))
		motor[flagSpinner]=-100;
	else
		motor[flagSpinner]=0;

}

void joystickControllerTwo() //Driver 2 controls cube intake and cube lifter
{


}

task main(){

	init();

	waitForStart();

	ClearTimer(T1);

	while(true){
		if(bDisconnected){ //Stop Robot if disconnected
			allStop();
		}else{
			joystickControllerOne();
			joystickControllerTwo();
      nxtDisplayCenteredTextLine(2, "%i", HTMCreadHeading(HTCOMPASS));
		}
	}
}
