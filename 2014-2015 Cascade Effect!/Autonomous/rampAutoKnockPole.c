#pragma config(Hubs,   S1, HTMotor, HTServo, HTMotor, HTMotor)
#pragma config(Motor,  mtr_S1_C1_1,     driveRight,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     lift,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     driveLeft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     intake,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     liftMotor3,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    fieldGrabberRight,    tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    fieldGrabberLeft,     tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    scoopBridge,          tServoStandard)
#pragma config(Servo,  srvo_S1_C2_5,    autoBallRelease,      tServoStandard)
#pragma config(Sensor, S2,              sonarSensor,       sensorSONAR)
#pragma config(Sensor, S3,     HTIRS2,              sensorI2CCustom)
#pragma config(Sensor, S4,     HTGYRO,              sensorAnalogInactive)

#include "SandyDriver.c"

int acS1, acS2, acS3, acS4, acS5 = 0;
float timer;

task main()
{
	init();
	waitForStart();
	drive(1,1.9,30);
	allStop();
	wait1Msec(500);
	turn(1,90,50);
	allStop();
	wait1Msec(700);
	bFloatDuringInactiveMotorPWM = true;
	drive(0,1.0,50);
	bFloatDuringInactiveMotorPWM = false;
	allStop();
	wait1Msec(750);
	//DO WHATEVER DEPENDING ON CENTER ROTATION
	determineRotation:
	HTIRS2readAllACStrength(HTIRS2, acS1, acS2, acS3, acS4, acS5 );
	int avg1=acS1,avg2,avg3,avg4,avg5;
	for(int j=0;j<10;++j){
		HTIRS2readAllACStrength(HTIRS2, acS1, acS2, acS3, acS4, acS5 );
		avg1=(avg1+acS1)/2;
		avg2=(avg2+acS2)/2;
		avg3=(avg3+acS3)/2;
		avg4=(avg4+acS4)/2;
		avg5=(avg5+acS5)/2;
	}
	if(acS2>16||acS3>30){ //Center is in rotation 1
		nxtDisplayCenteredTextLine(1,"Rot1");
		turn(0,90,70);
		allStop();
		wait1Msec(500);
		drive(0,.98,50);
		allStop();
		wait1Msec(500);
		turn(1,80,50);
		allStop();
		wait1Msec(500);
		drive(0,1.5,100);
		allStop();
		wait1Msec(500);
	}
	else{
		turn(1,25,50);
		allStop();
		wait1Msec(500);
		time1[T1]=0;
		bool irfound = false;
		timer = 0.0;
		while(time1[T1] < 700){
			HTIRS2readAllACStrength(HTIRS2, acS1, acS2, acS3, acS4, acS5 );
			forward(50);
			if(acS2>15){
				irfound = true;
				timer = time1[T1]/1000+.3;
				break;
			}
		}
		allStop();
		wait1Msec(500);
		if(irfound){ //Center is in rotation 2
			nxtDisplayCenteredTextLine(1,"Timer:%f",timer);
			nxtDisplayCenteredTextLine(1,"Rot2:%d",acS2);
			drive(1,timer,50);
			allStop();
			wait1Msec(500);
			turn(0,90,80);
			allStop();
			wait1Msec(500);
			drive(0,1.5,100);
			allStop();
			wait1Msec(500);
		}
		else{ //Center is in rotation 3
			nxtDisplayCenteredTextLine(1,"Rot3");
			drive(1,.7,50);
			allStop();
			wait1Msec(500);
			turn(1,125,50);
			while(SensorValue[sonarSensor]>20){
				nxtDisplayCenteredBigTextLine(1,"GOGOGOGOG");
				backward(30);
			}
			allStop();
			wait1Msec(750);
			turn(0,65,100);
			allStop();
			wait1Msec(500);
			drive(1,1.5,100);
			allStop();
			wait1Msec(1000);
		}
	}
}
