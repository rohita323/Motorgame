#include <stdlib.h> 
#include <math.h> 
#include <stdio.h>
#include <GL/freeglut.h> 
#include <GL/glut.h>
#include "motor.h"
#include "functions.h"
#include <iostream>


#define ESC 27

void windowResize(int w, int h);
void update(int value);
void renderScene(void);
void processNormalKeys(unsigned char key, int xx, int yy);
void pressSpecialKey(int key, int xx, int yy);
void releaseSpecialKey(int key, int x, int y);
void mouseMove(int x, int y);
void mouseButton(int button, int state, int x, int y);
void arrows(int key, int x, int y);

// Camera position
float camera_x = 0.0, camera_y = 0.0; // initially 5 units south of origin
float deltaMove = 0.0; // initially camera doesn't move
int camera_view=1;
float x=0.0, y=-5.0;

// Camera direction
float lx = 1.0, ly = 0.0; // camera points initially along y-axis
float angle = 0.0; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts

//Motorcycle Position
float motor_x = 0.0;
float motor_y = 0.0;
float direction[2]={0.0};

Motor* motorcycle;


int main(int argc, char **argv) 
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 400);
	glutCreateWindow("MotorCross");

	motorcycle= new Motor(motor_x,motor_y);
	glutReshapeFunc(windowResize);
	glutDisplayFunc(renderScene); 
//	glutIdleFunc(update); 	
	glutIgnoreKeyRepeat(1); 
	glutMouseFunc(mouseButton);  
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressSpecialKey);
						
	glutSpecialUpFunc(releaseSpecialKey); 
	glEnable(GL_DEPTH_TEST);
	glutTimerFunc(10, update, 0);

	glutMainLoop();

	return 0;
}

void windowResize(int w, int h) 
{
	glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void update(int value) 
{
	if (deltaMove) { // update camera position
		camera_x += deltaMove * lx * 0.1;
		camera_y += deltaMove * ly * 0.1;
	}
	if(direction[0]!=0.0)
		motor_x=(motorcycle->motor_x+=direction[0]);
	if(direction[1]!=0.0)
		motor_y=(motorcycle->motor_y+=direction[1]);

	glutTimerFunc(10, update, 0);
	glutPostRedisplay(); // redisplay everything
}

void renderScene(void) 
{
	int i, j;

	glClearColor(0.0, 0.7, 1.0, 1.0); // sky color is light blue
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	declare_view(camera_view, motor_x, motor_y);

	glColor3f(0.0, 0.7, 0.0);
	glBegin(GL_QUADS);
		glVertex3f(-100.0, -100.0, 0.0);
		glVertex3f(-100.0,  100.0, 0.0);
		glVertex3f( 100.0,  100.0, 0.0);
		glVertex3f( 100.0, -100.0, 0.0);
	glEnd();
	
	glPushMatrix();
		motorcycle->draw();
	glPopMatrix();
	
	glutSwapBuffers(); // Make it all visible
} 


void processNormalKeys(unsigned char key, int xx, int yy)
{
	if (key == ESC || key == 'q' || key == 'Q') exit(0);
	if (key == 'D' || key =='d') camera_view=1;
	if (key == 'W' || key =='w') camera_view=2;
	if (key == 'O' || key =='o') camera_view=3;
	if (key == 'H' || key =='h') 
	{
			camera_view=4;
			x=0;
			y=0;
	}
	if (key == 'F' || key =='f') camera_view=5;

} 

void pressSpecialKey(int key, int xx, int yy)
{
	switch (key) {
		case GLUT_KEY_UP :  direction[0]=0.5; break;
		case GLUT_KEY_DOWN : direction[0]=-0.5; break;
		case GLUT_KEY_RIGHT: direction[1]=0.5; break;
		case GLUT_KEY_LEFT: direction[1]=-0.5; break;
	}
} 

void releaseSpecialKey(int key, int x, int y) 
{
	switch (key) {
		case GLUT_KEY_UP : direction[0]=0.0;; break;
		case GLUT_KEY_DOWN : direction[0]=0.0;; break;
		case GLUT_KEY_RIGHT: direction[1]=0.0; break;
		case GLUT_KEY_LEFT: direction[1]=0.0; break;
	}
} 
void mouseMove(int x, int y) 
{ 	
	if (isDragging) { // only when dragging
		// update the change in angle
		deltaAngle = (x - xDragStart) * 0.005;

		// camera's direction is set to angle + deltaAngle
		lx = -sin(angle + deltaAngle);
		ly = cos(angle + deltaAngle);

		cout<<lx<<" "<<ly<<"\n";
	}
}

void mouseButton(int button, int state, int x, int y) 
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) { // left mouse button pressed
			isDragging = 1; // start dragging
			xDragStart = x; // save x where button first pressed
		}
		else  { /* (state = GLUT_UP) */
			angle += deltaAngle; // update camera turning angle
			isDragging = 0; // no longer dragging
		}
	}
}