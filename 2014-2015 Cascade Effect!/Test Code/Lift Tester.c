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
#pragma config(Servo,  srvo_S1_C2_5,    autoBallRelease,          tServoStandard)
#pragma config(Sensor, S4,     HTGYRO,              sensorAnalogInactive)



void retainBalls()
{
	servo[scoopBridge] = 137;
}

void releaseBalls()
{
	servo[scoopBridge] = 10;
}

void allStop(){
	motor[driveLeft] = 0;
	motor[driveRight] = 0;
	motor[lift] = 0;
	motor[liftMotor3] = 0;
	motor[intake] = 0;
}

void raiseLift(int powerLevel){
		motor[lift] = powerLevel;
		motor[liftMotor3] = -powerLevel;
}

void lowerLift(int powerLevel){
		motor[lift] = -powerLevel;
		motor[liftMotor3] = powerLevel;
}

task main()
{
	retainBalls();
	int speed = 100;
	int lastState = -1;

	while(true){
		nxtDisplayCenteredTextLine(1,"Speed: %i",speed);

		if (nNxtButtonPressed == 1) //Forward
         {
           raiseLift(100);
         }

      else if (nNxtButtonPressed == 2) //Backward
         {
           lowerLift(100);
         }
         else {
           allStop();
         }

      if (nNxtButtonPressed == 3 && lastState!=3) //Change Speed
         {
           if(speed==100){
             speed = 50;
             retainBalls();
           }
           else if(speed == 50){
             speed = 100;
             releaseBalls();
           }
         }
         lastState = nNxtButtonPressed;
      wait1Msec(1);
	}

}
