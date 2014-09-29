#pragma config(Sensor, S3,     HTCOMPASS,           sensorI2CCustom)
#include "/hitechnic-compass.h"
task main()
{
	HTMCstartCal(HTCOMPASS);
	while(true){
		if(nNxtButtonPressed!=0)
			HTMCstopCal(HTCOMPASS);
	}



}
