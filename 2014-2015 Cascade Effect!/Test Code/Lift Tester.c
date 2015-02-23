#pragma config(Hubs,  S1, HTMotor,  none,  none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     liftMotor,              tmotorNone, openLoop)

task main()
{
	int speed = 100;
	int lastState = -1;

	while(true){
		nxtDisplayCenteredTextLine(1,"Speed: %i",speed);

		if (nNxtButtonPressed == 1) //Forward
         {
           motor[liftMotor] = 100;
         }

      else if (nNxtButtonPressed == 2) //Backward
         {
           motor[liftMotor] = -100;
         }
         else {
           motor[liftMotor] = 0;
         }

      if (nNxtButtonPressed == 3 && lastState!=3) //Change Speed
         {
           if(speed==100)
             speed = 50;
           else if(speed == 50)
             speed = 75;
           else if(speed == 75)
             speed = 100;
         }
         lastState = nNxtButtonPressed;
      wait1Msec(1);
	}

}
