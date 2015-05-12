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
bool stalledLift = false;

task main()
{
	init();
	waitForStart();
	drive(0,1.5,30);
	allStop();
	wait1Msec(500);
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
	if(acS3>50&&acS4>40){ //Center is in rotation 3 (s2-90,s3-70)
		nxtDisplayCenteredTextLine(1,"Rot3");
		//Drop Ball
		drive(1,.2,100);
		allStop();
		wait1Msec(200);
		turn(1,165,70);
		allStop();
		wait1Msec(200);
		while(SensorValue[sonarSensor]>21){
			backward(30);
		}
		wait1Msec(200);
		allStop();
		wait1Msec(500);
		drive(0,.3,30);
		allStop();
		wait1Msec(100);
		raiseLift(100);
		while (nMotorEncoder[intake] < 5000) //while the encoder wheel turns one revolution
		{
			if(stalledLift){
				while(true){
					allStop();
					nxtDisplayCenteredTextLine(1,"STALLED");
				}
			}
		}
		allStop();
		wait1Msec(500);
		raiseLift(100);
		while (nMotorEncoder[intake] < 5200) //while the encoder wheel turns one revolution
		{
		}
		allStop();
		wait1Msec(500);
		raiseLift(100);
		while (nMotorEncoder[intake] < 5500) //while the encoder wheel turns one revolution
		{
		}
		allStop();
		wait1Msec(500);
		releaseBalls();
		allStop();
		wait1Msec(2000);
		retainBalls();
		allStop();
		wait1Msec(750);
		releaseBalls();
		allStop();
		wait1Msec(2000);
		retainBalls();
		allStop();
		wait1Msec(750);
		lowerLift(80);
		while (nMotorEncoder[intake] > 1500) //while the encoder wheel turns one revolution
		{
		}
		allStop();
		wait1Msec(500);
		turn(1,165,70);
		allStop();
		wait1Msec(100);
		turn(1,80,50);
		allStop();
		wait1Msec(500);
		drive(0,.65,50);
		allStop();
		wait1Msec(500);
		turn(0,75,100);
		allStop();
		wait1Msec(500);
		drive(0,1.5,100);
		allStop();
		wait1Msec(500);
	}
	else if(acS3>40){ //Center is in rotation 2
		nxtDisplayCenteredTextLine(1,"Rot2");
		forward(50);
		wait1Msec(500);
		allStop();
		wait1Msec(200);
		while(acS2<20){
			HTIRS2readAllACStrength(HTIRS2, acS1, acS2, acS3, acS4, acS5 );
			nxtDisplayCenteredTextLine(1,"1: %i",acS1);
			nxtDisplayCenteredTextLine(2,"2: %i",acS2);
			nxtDisplayCenteredTextLine(3,"3: %i",acS3);
			nxtDisplayCenteredTextLine(4,"4: %i",acS4);
			nxtDisplayCenteredTextLine(5,"5: %i",acS5);
			right(40);
		}
		allStop();
		wait1Msec(500);
		drive(0,1.0,100);
		allStop();
		wait1Msec(500);
	}
	else{
		turn(1,45,70);
		allStop();
		wait1Msec(500);
		drive(0,1.3,50);
		allStop();
		wait1Msec(500);
		turn(1,30,70);
		allStop();
		wait1Msec(500);
		drive(1,1.2,100);
		allStop();
		wait1Msec(500);
	}
}
