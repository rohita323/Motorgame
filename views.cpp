#include "functions.h"


void declare_view(int v, float motor_x, float motor_y, float lx, float ly)
{
	
	switch(v)
	{

		case 1:						//Driver View
			gluLookAt(
			motor_x-0.5,      motor_y,      2.0,
			motor_x+1.5, motor_y, 1.5,
			0.0,    0.0,    1.0);
			break;

		case 2:						//Wheel View
			gluLookAt(
			motor_x-0.5,      motor_y,      1.5,
			motor_x+1.5, motor_y, 1.0,
			0.0,    0.0,    1.0);
			break;

		case 3:						//Overhead View
			gluLookAt(
			motor_x-7.0,      motor_y,      3.5,
			motor_x, motor_y, 1.0,
			0.0,    0.0,    1.0);

		break;

		case 4:						//Helicopter Cam

		break;

		case 5:						//Follow Cam

		break;

	}

	//cout<<motor_x<<motor_y<<lx<<ly<<"\n";
}