#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Motor,  mtr_S1_C1_1,     motorLeftFront,   tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorLeftBack,   tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     ramp,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     arms,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     motorRightFront,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorRightBack,   tmotorNormal, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    clawLeft,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    clawRight,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    button,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_4,    button2,               tServoStandard)
#pragma config(Sensor, S3,     LEGOLS,              sensorLightInactive)
#pragma config(Sensor, S4,     HTSMUX,              sensorI2CCustom)

#include "JoystickDriver.c"
#include "/hitechnic-sensormux.h"
#include "/lego-light.h"
#include "/lego-touch.h"
#include "/hitechnic-force.h"

#define threshold 25

const tMUXSensor HTFORCE1 = msensor_S4_3;
const tMUXSensor HTFORCE2 = msensor_S4_4;

bool rampLocked,speedToggleOne,speedToggleTwo,touched,weighted,on,prevOn;
float speedContOne,speedContTwo;
int clawVal,force1,force2;

void init(){

	//Initiate Variables
	rampLocked=speedToggleOne=speedToggleTwo=touched=weighted=on=prevOn=false;
	speedContOne=speedContTwo=1;
	clawVal=7;

	LSsetActive(LEGOLS);

	//Initiate Motors
	motor[motorLeftFront]=0;
	motor[motorRightFront]=0;
	motor[motorRightFront]=0;
	motor[motorRightBack]=0;
	motor[ramp]=0;
	motor[arms]=0;

	//Initiate Servos
	servo[clawLeft]=clawVal;
	servo[clawRight]=215-clawVal;
	servo[button]=140;
	servo[button2]=120;

}

void allStop(){
	eraseDisplay();
	motor[motorLeftFront] = 0;
	motor[motorLeftBack] = 0;
	motor[motorRightFront] = 0;
	motor[motorRightBack] = 0;
	motor[ramp] = 0;
	motor[arms] = 0;
	nxtDisplayCenteredBigTextLine(0,"Blutetooth");
	nxtDisplayCenteredBigTextLine(3,"Disconnected!");
}

void updateSensors()
{
	force1=HTFreadSensor(HTFORCE1);
	force2=HTFreadSensor(HTFORCE2);
}

void joystickControllerOne()
{

		getJoystickSettings(joystick);

		//Fine Control
		if(joy1Btn(1)){
			while(joy1Btn(1)){}
			speedToggleOne=!speedToggleOne;
		}
		if(speedToggleOne)
			speedContOne=.5;
		else
			speedContOne=1;

		//Drive Train Control
		if(abs(joystick.joy1_x1)>80){
			motor[motorLeftFront]=joystick.joy1_x1*speedContOne;
			motor[motorLeftBack]=joystick.joy1_x1*speedContOne;
			motor[motorRightFront]=-joystick.joy1_x1*speedContOne;
			motor[motorRightBack]=-joystick.joy1_x1*speedContOne;
		}
		else if(abs(joystick.joy1_y1)>threshold){
			motor[motorLeftFront]=joystick.joy1_y1*speedContOne;
			motor[motorLeftBack]=joystick.joy1_y1*speedContOne;
			motor[motorRightFront]=joystick.joy1_y1*speedContOne;
			motor[motorRightBack]=joystick.joy1_y1*speedContOne;
		}
		else{
			motor[motorLeftFront]=0;
			motor[motorLeftBack]=0;
			motor[motorRightFront]=0;
			motor[motorRightBack]=0;
		}


		//Ramp Lock
		if(joy1Btn(4))
			rampLocked=!rampLocked;

		//Ramp Control
		if(joy1Btn(6))
			motor[ramp]=50*speedContOne;
		else if(joy1Btn(5))
			motor[ramp]=-75*speedContOne;
		else
			motor[ramp]=0;

}

void joystickControllerTwo()
{

		getJoystickSettings(joystick);

		//Fine Control
		if(joy2Btn(1)){
			while(joy2Btn(1)){}
			speedToggleTwo=!speedToggleTwo;
		}
		if(speedToggleTwo)
			speedContTwo=.5;
		else
			speedContTwo=1;

		/*//Arm Controll
		if(joy2Btn(5))
			motor[arms] = -100*speedContTwo;
		else if(joy2Btn(6))
			motor[arms] = 100*speedContTwo;
		else
			motor[arms] = 0;
*/
		//Claw Control
			//Position Control
		if(joy2Btn(4))
			clawVal=7;
		if(joy2Btn(3))
			clawVal=100;
			//Fine Control
		if(joy2Btn(7)&&time1[T1]%10==0)
			clawVal++;
		if(joy2Btn(8)&&time1[T1]%10==0)
			clawVal--;

		//Update Servos//////////////
		servo[clawLeft]=clawVal;
		servo[clawRight]=210-clawVal;
		/////////////////////////////7
}

void checkWeightedRing()
{
	//Weighted Ring Checker
	  if(force1<1000)
	  	force1+=1000;
	 	if(force2<1000)
	 		force2+=1000;
	 	if(force1>1450&&force2>1300||force2>1500&&force1>1400){
			if(time1[T1]>350){
				ClearTimer(T1);
				on=!on;
			}
	 		if(on!=prevOn){
	 			if(on)
					servo[button2]=0;
				else
					servo[button2]=120;
			}
	 	}
	 	else{
			servo[button2]=110;
	 	}
		if(force1>1455){
			servo[button]=150;
		}
		else if(force2>1460){
			servo[button]=150;
		}
		else{
			servo[button]=140;
	 	}
	 	prevOn=on;
}

void debug()
{
	nxtDisplayCenteredTextLine(0,"Force1:%i",force1);
	nxtDisplayCenteredTextLine(1,"Force2:%i",force2);
	nxtDisplayCenteredTextLine(2,"Light:%i",SensorValue[LEGOLS]);
	nxtDisplayCenteredTextLine(3,"Time:%i",time1[T1]);
	nxtDisplayCenteredTextLine(4,"ON?%i",on);
	nxtDisplayCenteredTextLine(5,"");
	nxtDisplayCenteredTextLine(6,"");
	nxtDisplayCenteredTextLine(7,"");
}

task main()
{
	init();

	waitForStart();

	ClearTimer(T1);
	while(true){
		if(true){
	  	//Update Sensors
			updateSensors();

			//Check weighted Ring
			checkWeightedRing();

   	 	//Joystick Control
			joystickControllerOne();
			joystickControllerTwo();

			debug();
		}
		else{
			allStop();
		}
	}
}
