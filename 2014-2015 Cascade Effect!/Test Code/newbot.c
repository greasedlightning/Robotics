#pragma config(Hubs,  S1, HTMotor,  none,  none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     driveLeft,              tmotorNone, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     driveRight,              tmotorNone, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Tele-Operation Mode Code Template
//
// This file contains a template for simplified creation of an tele-op program for an FTC
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of tele-op mode, you may want to perform some initialization on your robot
// and the variables within your program.
//
// In most cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void initializeRobot()
{
  // Place code here to sinitialize servos to starting positions.
  // Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.

  return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the tele-op robot operation. Customize as appropriate for
// your specific robot.
//
// Game controller / joystick information is sent periodically (about every 50 milliseconds) from
// the FMS (Field Management System) to the robot. Most tele-op programs will follow the following
// logic:
//   1. Loop forever repeating the following actions:
//   2. Get the latest game controller / joystick settings that have been received from the PC.
//   3. Perform appropriate actions based on the joystick + buttons settings. This is usually a
//      simple action:
//      *  Joystick values are usually directly translated into power levels for a motor or
//         position of a servo.
//      *  Buttons are usually used to start/stop a motor or cause a servo to move to a specific
//         position.
//   4. Repeat the loop.
//
// Your program needs to continuously loop because you need to continuously respond to changes in
// the game controller settings.
//
// At the end of the tele-op period, the FMS will autonmatically abort (stop) execution of the program.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	initializeRobot();

	waitForStart();   // wait for start of tele-op phase

	while (true)
	{
		///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////
		////                                                   ////
		////      Add your robot specific tele-op code here.   ////
		////                                                   ////
		///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////

		// Insert code to have servos and motors respond to joystick and button values.

		// Look in the ROBOTC sam(ples folder for programs that may be similar to what you want to perform.
		// You may be able to find "snippets" of code that are similar to the functions that you want to
		// perform.

		if(abs(joystick.joy1_y1)>20){
			motor[driveLeft] = joystick.joy1_y1;
		}
		else{
			motor[driveLeft] = 0;
		}

		if(abs(joystick.joy1_y2)>20){
			motor[driveRight] = joystick.joy1_y2;
		}
		else{
			motor[driveRight] = 0;
		}
	}
}
