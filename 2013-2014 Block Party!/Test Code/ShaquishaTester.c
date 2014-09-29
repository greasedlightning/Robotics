#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Motor,  mtr_S1_C1_1,   	rightMotor,tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,  		leftMotor,   		tmotorNormal, openLoop, reversed)

task main()
{bFloatDuringInactiveMotorPWM=true;
	//while(nNxtButtonPressed==-1){}
while(true){
	for(int i=10;i<100;i++){
		motor[rightMotor]=i;
		motor[leftMotor]=i;
		wait1Msec(10);
	}
	motor[rightMotor]=100;
	motor[leftMotor]=100;
	wait1Msec(2000);
	while(nMotorRunState[leftMotor]==runStateRampDown){
	motor[rightMotor]=0;
	motor[leftMotor]=0;
}
	motor[rightMotor]=-100;
	motor[leftMotor]=-100;
	wait1Msec(2800);
}
}
