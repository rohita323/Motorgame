#include "functions.h"


void declare_view(int v, float motor_x, float motor_y)
{
	
	switch(v)
	{

		case 1:						//Driver View
			gluLookAt(
			motor_x-1.0,      motor_y-.75,      2.0,
			motor_x, motor_y-.75, 1.75,
			0.0,    0.0,    1.0);
			break;

		case 2:						//Wheel View
			gluLookAt(
			motor_x-0.5,      motor_y-0.5,      1.35,
			motor_x, motor_y-0.5, 1.25,
			0.0,    0.0,    1.0);
			break;

		case 3:						//Overhead View
			gluLookAt(
			motor_x-7.0,    motor_y,    3.5,
			motor_x, 		motor_y, 	1.0,
			0.0,    		0.0,    	1.0);
			break;


		case 4:						//Helicopter Cam
			gluLookAt(
			x-20.0+ motor_x,      	y+motor_y,      	10.0,
			x + lx -20.0+motor_x, 	y + motor_y+ ly, 	9.75,
			0.0,    0.0,    1.0);
			break;

		case 5:						//Follow Cam
			gluLookAt(
			motor_x-8.0,      motor_y-.75,     2.75,
			motor_x, motor_y-.75, 1.75,
			0.0,    0.0,    1.0);
			break;
		
	}

	//cout<<motor_x<<motor_y<<lx<<ly<<"\n";
}

