#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include <GL/freeglut.h>
using namespace std;

void drawWheel();

class Motor {

public:
	float motor_x, motor_y, handle_theta, rotate_theta;

	Motor(float motor_x, float motor_y) {
		this->motor_x= motor_x;			
		this->motor_y = motor_y;
	}

	void draw() {
		glTranslatef(motor_x, motor_y, 0.0);
		drawWheel();

		glPushMatrix();
			glTranslatef(-2.5, 0.0, 0.0);
			drawWheel();
		glPopMatrix();
	}
};



void drawWheel()
{
	glColor3f(0.0, 0.0, 0.0); 
	glPushMatrix();
	 	glRotatef(90, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0, 0.85, 0.75);
		glutSolidTorus(0.1, 0.75, 5, 20);
 	glPopMatrix();

	glColor3f(0.0, 0.5, 0.0);
	glPushMatrix();
		glTranslatef(0.2, 0.2, 0.001);	// translate to just above ground
		glScalef(1.0, 1.0, 0.0); // scale sphere into a flat pancake
		glutSolidSphere(0.75, 20, 20); // shadow same size as body
	glPopMatrix();
}

