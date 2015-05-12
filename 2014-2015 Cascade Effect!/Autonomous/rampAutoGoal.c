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

float globalHeading = 0.0;

task keepHeading(){
	float rotSpeed = 0;
	// Calibrate the gyro, make sure you hold the sensor still
	while(true){
		while (time1[T3] < 20)
			wait1Msec(1);

		// Reset the timer
		time1[T3]=0;

		// Read the current rotation speed
		rotSpeed = HTGYROreadRot(HTGYRO);

		// Calculate the new heading by adding the amount of degrees
		// we've turned in the last 20ms
		// If our current rate of rotation is 100 degrees/second,
		// then we will have turned 100 * (20/1000) = 2 degrees since
		// the last time we measured.
		globalHeading += rotSpeed * 0.02;
		nxtDisplayCenteredTextLine(1,"GH:%f",globalHeading);
		wait1Msec(1);
		if(globalHeading>180)
			globalHeading = -180;
		else if(globalHeading<-180)
			globalHeading =  180;
	}
}

void turnToGlobalHeading(int targetHeading){
	int powerLevel = 70;
	int slow = 50;
	if(globalHeading>targetHeading){
		while(globalHeading>targetHeading){
			if(abs(globalHeading-targetHeading)<20)
				powerLevel=slow;
			right(powerLevel);
			nxtDisplayCenteredTextLine(2,"Get toL");
		}
	}
	else if(globalHeading<targetHeading){
		while(globalHeading<targetHeading){
			if(abs(globalHeading-targetHeading)<20)
				powerLevel=slow;
			left(powerLevel);
			nxtDisplayCenteredTextLine(2,"Get toR");
		}
	}
}

task raiseLiftWhile(){
  raiseLift(100);
  time1[T1]=0;
	while (nMotorEncoder[intake] < 415 && time1[T1] < 2800) //while the encoder wheel turns one revolution
  {
  	times = time1[T1];
  }
	allStop();
	wait1Msec(500);
	if(time1[T1]>2500)
		StopAllTasks();
}

task main()
{
	init();
	waitForStart();
	StartTask(keepHeading);
	StartTask(raiseLiftWhile);
	driveSonar(1,25,50);
	allStop();
	wait1Msec(100);
	backward(20);
	wait1Msec(700);
	sticksDown();
	wait1Msec(250);
	allStop();
	wait1Msec(300);
  //LIFT

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
	wait1Msec(100);
	turn(1,19,70);
	allStop();
	wait1Msec(100);
	drive(0,2.0,100);
	allStop();
	wait1Msec(100);
	turn(0,170,100);
	allStop();
	wait1Msec(100);
	sticksUp();
	drive(1,.3,100);
	allStop();
	wait1Msec(100);
	turn(0,180,100);
	allStop();
	wait1Msec(100);
	//while(SensorValue[sonarSensor]>40){
	//	backward(100);
	//}
	//turnToGlobalHeading(-15);
	allStop();
	wait1Msec(100);
	drive(1,1.3,100);
	allStop();
	wait1Msec(100);
	turnToGlobalHeading(-78);
	allStop();
	wait1Msec(100);
	while(SensorValue[sonarSensor]>25){
		backward(30);
	}
	allStop();
	wait1Msec(100);
	while(SensorValue[sonarSensor]<200){
		left(50);
	}
	allStop();
	wait1Msec(100);
	turn(0,18,100);
	allStop();
	wait1Msec(100);
	driveSonar(1,25,50);
	backward(30);
	wait1Msec(700);
	sticksDown();
	wait1Msec(250);
	allStop();
	turn(1,17,70);
	allStop();
	wait1Msec(100);
	drive(0,2.7,100);
	allStop();
	wait1Msec(100);
	turn(0,175,100);
	allStop();
	wait1Msec(50);
	sticksUp();
	drive(1,.8,100);
}
