#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S2, HTServo,     none,     none,  none)
#pragma config(Motor,  mtr_S1_C1_1,     primaryCubeIntake,tmotorNormal, openLoop)
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

#define cubeDropperUp 15
#define cubeDropperDown 112
#define CRStop 128
#define CRForward 255
#define CRReverse 0

//Variables
bool intakeOn,joy2Btn_1_Pressed,down;
int cubeLiftCount;

void init(){ //Initiate Robot(servo positions, intake bool)
        bFloatDuringInactiveMotorPWM = false;
        nNoMessageCounterLimit=500;
        cubeLiftCount=0;
        intakeOn = joy2Btn_1_Pressed = down = false;
        servo[cubeDropper] = cubeDropperUp;
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

{/*
        if(joy1Btn(1))
                motor[flagSpinner]=100;
        else if(joy1Btn(3))
                motor[flagSpinner]=-100;
        else if(joy1Btn(4))
        	motor[flagSpinner] = 5;
        else if(joy1Btn(2))
        	motor[flagSpinner] = -5;
        else
                motor[flagSpinner]=0;

        if(joy1Btn(6)){ //Hanging Mecahnism Control
       		motor[robotLifterRight]=100;
        	motor[robotLifterLeft]=100;
      	}else if(joy1Btn(5)){
       		motor[robotLifterRight]=-100;
        	motor[robotLifterLeft]=-100;
      	}else{
       		motor[robotLifterRight]=0;
        	motor[robotLifterLeft]=0;
      	}*/

        //Accidental Mode Switch fix
        if(joystick.joy1_TopHat==0){
                motor[driveLeft] = 100;
        }else if(joystick.joy1_TopHat==4){
                motor[driveLeft] = -100;
        }else if(abs(joystick.joy1_y1)>10){ //Drive train control(tank drive)
        			if(joystick.joy1_y1<0&&joystick.joy1_y2<0){
                PlaySound(soundBeepBeep);
              }else
              	ClearSounds();

              motor[driveLeft] = exponentialJoystick(joystick.joy1_y1)*(abs(joystick.joy1_y1)/joystick.joy1_y1);
        }
        else{
                motor[driveLeft] = 0;
        }if(abs(joystick.joy1_y2)>10){
        			if(joystick.joy1_y1<0&&joystick.joy1_y2<0){
                PlaySound(soundBeepBeep);
              }else
              	ClearSounds();

                motor[driveRight] = exponentialJoystick(joystick.joy1_y2)*(abs(joystick.joy1_y2)/joystick.joy1_y2);
        }else{
                motor[driveRight] = 0;
        }
}

void joystickControllerTwo() //Driver 2 controls cube intake and cube lifter
{/*
        if(cubeLiftCount<0) //Don't let the count go negative
                cubeLiftCount=0;

        if(joy2Btn(4)){ //Cube dropper Up
                servo[cubeDropper]=cubeDropperUp;
                down=false;
                PlaySound(soundFastUpwardTones);
        }
        else if(joy2Btn(3)){ //Cube dropper Down
                servo[cubeDropper]=cubeDropperDown;
                down=true;
                ClearTimer(T2);
                PlaySound(soundDownwardTones);
        }else
        	ClearSounds();

        //Continous rotation servo, 0=reverse, 128=stop, 255=forward
        if(joy2Btn(7)){ //Cube lifter Down
                servo[cubeLifter]=CRReverse;
                cubeLiftCount--;
        }
        else if(joy2Btn(8)){ //Cube lifter Up
                servo[cubeLifter]=CRForward;
                cubeLiftCount++;
        }
       /*else if(cubeLiftCount>0&&(down||cubeDropperPos==100)&&time1[T2]>1250){ //Autonomously reset cubeLifter(currently unused)
                if(cubeLiftCount<50) //Reset cubeDropper when at bottom
                        cubeDropperPos=0;
                cubeLifterPos=0;
                cubeLiftCount--;
                down=true;
                if(cubeLiftCount<50)
                        down=false;
        }*//*
        else{ //Stop
                servo[cubeLifter]=CRStop;
            }

        if(joy2Btn(1)&&!joy2Btn_1_Pressed){ //Intake toggle on if last tick button 1 was Up
        				intakeOn=!intakeOn;
                joy2Btn_1_Pressed=true;
        }
        if(!joy2Btn(1))
                joy2Btn_1_Pressed=false;
        if(joystick.joy2_TopHat==4)
                motor[primaryCubeIntake] = 100;
        else if(joy2Btn(2)){ //Reverse intake. Overrides intake forward
                //HTMCstopCal(HTCOMPASS);
                motor[primaryCubeIntake] = 100;
                intakeOn = false;
                motor[motorA] = -100;
                motor[motorB] = -100;
        }
        else if(intakeOn){ //Intake forward if toggle on

                motor[primaryCubeIntake] = -100;
                motor[motorA] = 100;
                motor[motorB] = 100;
        }
        else{ //Stop intake
                motor[primaryCubeIntake] = 0;
                motor[motorA] = 0;
                motor[motorB] = 0;
        }*/
}
void BeethovenFifth()
{
	//  Piano Player II
	//    rest);
	wait10Msec(75);
	PlayTone(784,15);	wait10Msec(19);
	PlayTone(784,15);	wait10Msec(19);
	PlayTone(784,15);	wait10Msec(19);
	PlayTone(622,38);	wait10Msec(38);
	//    rest);
	wait10Msec(38);
	PlayTone(698,15);	wait10Msec(19);
	PlayTone(698,15);	wait10Msec(19);
	PlayTone(698,15);	wait10Msec(19);
	PlayTone(587,38);	wait10Msec(38);
	//    rest);
	wait10Msec(75);
	PlayTone(784,15);	wait10Msec(19);
	PlayTone(784,15);	wait10Msec(19);
	PlayTone(784,15);	wait10Msec(19);
	PlayTone(622,15);	wait10Msec(19);
	PlayTone(831,15);	wait10Msec(19);
	PlayTone(831,15);	wait10Msec(19);
	PlayTone(831,15);	wait10Msec(19);
	PlayTone(784,15);	wait10Msec(19);
	PlayTone(1244,15);	wait10Msec(19);
	PlayTone(1244,15);	wait10Msec(19);
	PlayTone(1244,15);	wait10Msec(19);
	PlayTone(1047,38);	wait10Msec(38);
	//    rest);
	wait10Msec(19);
	PlayTone(784,15);	wait10Msec(19);
	PlayTone(784,15);	wait10Msec(19);
	PlayTone(784,15);	wait10Msec(19);
	PlayTone(587,15);	wait10Msec(19);
	PlayTone(831,15);	wait10Msec(19);
	PlayTone(831,15);	wait10Msec(19);
	PlayTone(831,15);	wait10Msec(19);
	PlayTone(784,15);	wait10Msec(19);
	PlayTone(1397,15);	wait10Msec(19);
	PlayTone(1397,15);	wait10Msec(19);
	PlayTone(1397,15);	wait10Msec(19);
	PlayTone(1175,38);	wait10Msec(38);
	//    rest);
	wait10Msec(19);
	PlayTone(1568,15);	wait10Msec(19);
	PlayTone(1568,15);	wait10Msec(19);
	PlayTone(1397,15);	wait10Msec(19);
	PlayTone(1244,19);	wait10Msec(19);
	//    rest);
	wait10Msec(19);
	PlayTone(1175,15);	wait10Msec(19);
	PlayTone(1568,15);	wait10Msec(19);
	PlayTone(1568,15);	wait10Msec(19);
	PlayTone(1397,15);	wait10Msec(19);
	PlayTone(1244,19);	wait10Msec(19);
	//    rest);
	wait10Msec(19);
	PlayTone(1175,15);	wait10Msec(19);
	PlayTone(1568,15);	wait10Msec(19);
	PlayTone(1568,15);	wait10Msec(19);
	PlayTone(1397,15);	wait10Msec(19);
	PlayTone(1244,19);	wait10Msec(19);
	//    rest);
	wait10Msec(38);
	PlayTone(1047,19);	wait10Msec(19);
	//    rest);
	wait10Msec(38);
	PlayTone(784,38);	wait10Msec(38);
	return;
}


void HallOfMountainKing()
{
	wait10Msec(8);  // Rest
	PlayTone(220,12);  wait10Msec(15);
	PlayTone(247,12);  wait10Msec(15);
	PlayTone(262,12);  wait10Msec(15);
	PlayTone(294,12);  wait10Msec(15);
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(262,12);  wait10Msec(15);
	PlayTone(330,24);  wait10Msec(30);
	PlayTone(311,12);  wait10Msec(15);
	PlayTone(247,12);  wait10Msec(15);
	PlayTone(311,24);  wait10Msec(30);
	PlayTone(294,12);  wait10Msec(15);
	PlayTone(233,12);  wait10Msec(15);
	PlayTone(294,24);  wait10Msec(30);
	PlayTone(220,12);  wait10Msec(15);
	PlayTone(247,12);  wait10Msec(15);
	PlayTone(262,12);  wait10Msec(15);
	PlayTone(294,12);  wait10Msec(15);
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(262,12);  wait10Msec(15);
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(440,12);  wait10Msec(15);
	PlayTone(392,12);  wait10Msec(15);
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(262,12);  wait10Msec(15);
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(392,48);  wait10Msec(60);
	wait10Msec(8);  // Rest
	PlayTone(220,12);  wait10Msec(15);
	PlayTone(247,12);  wait10Msec(15);
	PlayTone(262,12);  wait10Msec(15);
	PlayTone(294,12);  wait10Msec(15);
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(262,12);  wait10Msec(15);
	PlayTone(330,24);  wait10Msec(30);
	PlayTone(311,12);  wait10Msec(15);
	PlayTone(247,12);  wait10Msec(15);
	PlayTone(311,24);  wait10Msec(30);
	PlayTone(294,12);  wait10Msec(15);
	PlayTone(233,12);  wait10Msec(15);
	PlayTone(294,24);  wait10Msec(30);
	PlayTone(220,12);  wait10Msec(15);
	PlayTone(247,12);  wait10Msec(15);
	PlayTone(262,12);  wait10Msec(15);
	PlayTone(294,12);  wait10Msec(15);
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(262,12);  wait10Msec(15);
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(440,12);  wait10Msec(15);
	PlayTone(392,12);  wait10Msec(15);
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(262,12);  wait10Msec(15);
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(392,48);  wait10Msec(60);
	wait10Msec(8);  // Rest
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(370,12);  wait10Msec(15);
	PlayTone(415,12);  wait10Msec(15);
	PlayTone(440,12);  wait10Msec(15);
	PlayTone(494,12);  wait10Msec(15);
	PlayTone(415,12);  wait10Msec(15);
	PlayTone(494,24);  wait10Msec(30);
	PlayTone(523,12);  wait10Msec(15);
	PlayTone(440,12);  wait10Msec(15);
	PlayTone(523,24);  wait10Msec(30);
	PlayTone(494,12);  wait10Msec(15);
	PlayTone(415,12);  wait10Msec(15);
	PlayTone(494,24);  wait10Msec(30);
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(370,12);  wait10Msec(15);
	PlayTone(415,12);  wait10Msec(15);
	PlayTone(440,12);  wait10Msec(15);
	PlayTone(494,12);  wait10Msec(15);
	PlayTone(415,12);  wait10Msec(15);
	PlayTone(494,24);  wait10Msec(30);
	PlayTone(523,12);  wait10Msec(15);
	PlayTone(440,12);  wait10Msec(15);
	PlayTone(523,24);  wait10Msec(30);
	PlayTone(494,48);  wait10Msec(60);
	wait10Msec(8);  // Rest
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(370,12);  wait10Msec(15);
	PlayTone(415,12);  wait10Msec(15);
	PlayTone(440,12);  wait10Msec(15);
	PlayTone(494,12);  wait10Msec(15);
	PlayTone(415,12);  wait10Msec(15);
	PlayTone(494,24);  wait10Msec(30);
	PlayTone(523,12);  wait10Msec(15);
	PlayTone(440,12);  wait10Msec(15);
	PlayTone(523,24);  wait10Msec(30);
	PlayTone(494,12);  wait10Msec(15);
	PlayTone(415,12);  wait10Msec(15);
	PlayTone(494,24);  wait10Msec(30);
	PlayTone(330,12);  wait10Msec(15);
	PlayTone(370,12);  wait10Msec(15);
	PlayTone(415,12);  wait10Msec(15);
	PlayTone(440,12);  wait10Msec(15);
	PlayTone(494,12);  wait10Msec(15);
	PlayTone(415,12);  wait10Msec(15);
	PlayTone(494,24);  wait10Msec(30);
	PlayTone(523,12);  wait10Msec(15);
	PlayTone(440,12);  wait10Msec(15);
	PlayTone(523,24);  wait10Msec(30);
	PlayTone(494,48);  wait10Msec(60);
	wait10Msec(8);  // Rest
	wait10Msec(8);  // Rest
	PlayTone(440,8);  wait10Msec(15);
	PlayTone(494,8);  wait10Msec(15);
	PlayTone(523,8);  wait10Msec(15);
	PlayTone(587,8);  wait10Msec(15);
	PlayTone(659,8);  wait10Msec(15);
	PlayTone(523,8);  wait10Msec(15);
	PlayTone(659,24);  wait10Msec(30);
	PlayTone(622,8);  wait10Msec(15);
	PlayTone(494,8);  wait10Msec(15);
	PlayTone(622,24);  wait10Msec(30);
	PlayTone(587,8);  wait10Msec(15);
	PlayTone(466,8);  wait10Msec(15);
	PlayTone(587,24);  wait10Msec(30);
	PlayTone(440,8);  wait10Msec(15);
	PlayTone(494,8);  wait10Msec(15);
	PlayTone(523,8);  wait10Msec(15);
	PlayTone(587,8);  wait10Msec(15);
	PlayTone(659,8);  wait10Msec(15);
	PlayTone(523,8);  wait10Msec(15);
	PlayTone(659,8);  wait10Msec(15);
	PlayTone(880,8);  wait10Msec(15);
	PlayTone(784,8);  wait10Msec(15);
	PlayTone(659,8);  wait10Msec(15);
	PlayTone(523,8);  wait10Msec(15);
	PlayTone(659,8);  wait10Msec(15);
	PlayTone(784,48);  wait10Msec(60);
	wait10Msec(8);  // Rest
	PlayTone(440,8);  wait10Msec(15);
	PlayTone(494,8);  wait10Msec(15);
	PlayTone(523,8);  wait10Msec(15);
	PlayTone(587,8);  wait10Msec(15);
	PlayTone(659,8);  wait10Msec(15);
	PlayTone(523,8);  wait10Msec(15);
	PlayTone(659,24);  wait10Msec(30);
	PlayTone(622,8);  wait10Msec(15);
	PlayTone(494,8);  wait10Msec(15);
	PlayTone(622,24);  wait10Msec(30);
	PlayTone(587,8);  wait10Msec(15);
	PlayTone(466,8);  wait10Msec(15);
	PlayTone(587,24);  wait10Msec(30);
	PlayTone(440,8);  wait10Msec(15);
	PlayTone(494,8);  wait10Msec(15);
	PlayTone(523,8);  wait10Msec(15);
	PlayTone(587,8);  wait10Msec(15);
	PlayTone(659,8);  wait10Msec(15);
	PlayTone(523,8);  wait10Msec(15);
	PlayTone(659,8);  wait10Msec(15);
	PlayTone(880,8);  wait10Msec(15);
	PlayTone(831,8);  wait10Msec(15);
	PlayTone(659,8);  wait10Msec(15);
	PlayTone(831,8);  wait10Msec(15);
	PlayTone(988,8);  wait10Msec(15);
	PlayTone(880,48); wait10Msec(60);
	return;
}

void FrereJacque()
{
	PlayTone(523,40);	wait10Msec(50);
	PlayTone(587,40);	wait10Msec(50);
	PlayTone(659,40);	wait10Msec(50);
	PlayTone(523,40);	wait10Msec(50);
	PlayTone(523,40);	wait10Msec(50);
	PlayTone(587,40);	wait10Msec(50);
	PlayTone(659,40);	wait10Msec(50);
	PlayTone(523,40);	wait10Msec(50);
	PlayTone(659,40);	wait10Msec(50);
	PlayTone(698,40);	wait10Msec(50);
	PlayTone(784,80);	wait10Msec(90);
	PlayTone(659,40);	wait10Msec(50);
	PlayTone(698,40);	wait10Msec(50);
	PlayTone(784,80);	wait10Msec(90);
	PlayTone(784,20);	wait10Msec(30);
	PlayTone(880,20);	wait10Msec(30);
	PlayTone(784,20);	wait10Msec(30);
	PlayTone(698,20);	wait10Msec(30);
	PlayTone(659,40);	wait10Msec(50);
	PlayTone(523,40);	wait10Msec(50);
	PlayTone(784,20);	wait10Msec(30);
	PlayTone(880,20);	wait10Msec(30);
	PlayTone(784,20);	wait10Msec(30);
	PlayTone(698,20);	wait10Msec(30);
	PlayTone(659,40);	wait10Msec(50);
	PlayTone(523,40);	wait10Msec(50);
	PlayTone(523,40);	wait10Msec(50);
	PlayTone(392,40);	wait10Msec(50);
	PlayTone(523,80);	wait10Msec(90);
	PlayTone(523,40);	wait10Msec(50);
	PlayTone(392,40);	wait10Msec(50);
	PlayTone(523,80);	wait10Msec(90);
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Separate task to actually play the three songs
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

task playSongs()
{
	//
	// Set the task to high priority so that there is no jitter in the playback of sounds
	//
	nSchedulePriority = kHighPriority;

	//
	// Play the three songs in sequential order
	//
	HallOfMountainKing();
	wait10Msec(50);

	BeethovenFifth();
	wait10Msec(50);

	FrereJacque();
	wait10Msec(50);

	StartTask(playSongs); // we're finished.
	return;
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
      nxtDisplayCenteredTextLine(2, "%i", HTMCreadRelativeHeading(HTCOMPASS));
                }
        }
}
