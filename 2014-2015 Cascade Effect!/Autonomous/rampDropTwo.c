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

#include "/SandyDriver.c"

task main()
{
	init();
	waitForStart();
	driveSonar(1,25,80);
	allStop();
	wait1Msec(100);
	backward(20);
	wait1Msec(700);
	sticksDown();
	wait1Msec(250);
	allStop();
	wait1Msec(100);
  	raiseLift(100);
  	time1[T1]=0;
  	while (nMotorEncoder[intake] < 410 && time1[T1] < 2500) //while the encoder wheel turns one revolution
  	{
  		times = time1[T1];
  	}
	allStop();
	wait1Msec(500);
	//if(time1[T1]>2500)
	//	while(true){
	//		nxtDisplayCenteredBigTextLine(1,":(");
	//	}
	releaseBalls();
	allStop();
	wait1Msec(3000);
	retainBalls();
	allStop();
	wait1Msec(750);
  	lowerLift(80);
  	while (nMotorEncoder[intake] > 220) //while the encoder wheel turns one revolution
  	{
  	}
	allStop();
	wait1Msec(500);
	releaseAutoBall();
	wait1Msec(500);
	motor[intake] = 100;
	wait1Msec(1000);
	motor[intake] = 0;
	turn(0,180,100);
	allStop();
	wait1Msec(100);
	drive(1,1,75);
	sticksUp();
	drive(0,1,75);
	time1[T1]=0;
	while(SensorValue[sonarSensor]>30||time1[T1]<1200){
		left(50);
	}
	while(SensorValue[sonarSensor]<200){
		left(50);
	}
	allStop();
	wait1Msec(100);
	turn(0,25,70);
	allStop();
	wait1Msec(100);
	driveSonar(1,25,50);
	backward(30);
	wait1Msec(700);
	sticksDown();
	wait1Msec(250);
	allStop();
	raiseLift(100);
  	while (nMotorEncoder[intake] < 1000) //while the encoder wheel turns one revolution
  	{
  	}
	allStop();
	wait1Msec(500);
	releaseBalls();
	allStop();
	wait1Msec(3000);
	retainBalls();
	allStop();
	wait1Msec(750);
  	lowerLift(80);
  	while (nMotorEncoder[intake] > 740) //while the encoder wheel turns one revolution
  	{
  	}
	allStop();
	wait1Msec(500);
}
