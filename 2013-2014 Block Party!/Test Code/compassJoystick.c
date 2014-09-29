
#include "JoystickDriver.c"

	int heading=0;
task main()
{
	while(true){
		if(abs(joystick.joy1_y1)>70 || abs(joystick.joy1_x1)>70){
			heading=atan((float)joystick.joy1_y1/(float)joystick.joy1_x1)*180/PI;
			if(joystick.joy1_x1>=0){
					heading*=-1;
				if(joystick.joy1_y1>0)
					heading+=360;
			}
			if(joystick.joy1_x1<0){
				heading*=-1;
				heading+=180;
			}
		}
		nxtDisplayCenteredTextLine(2,"%4d",heading);
}



}
