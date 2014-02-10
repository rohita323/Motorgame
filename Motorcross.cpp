#include <stdlib.h> 
#include <math.h> 
#include <stdio.h>
#include <GL/freeglut.h> 
#include <GL/glut.h>
#include "motor.h"
#include "functions.h"
#include "imageloader.h"
#include "vec3f.h"
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
void mouseWheel(int button, int dir, int x, int y);
Terrain* loadTerrain(const char* filename, float height);
void cleanup();
void initRendering();
void draw_Terrain();

// Camera position
float camera_x = 0.0, camera_y = 0.0; // initially 5 units south of origin
float deltaMove = 0.0; // initially camera doesn't move
int camera_view=1;
float x=0.0, y=-5.0;

// Camera direction
float lx = 1.0, ly = 0.0; // camera points initially along y-axis
float angle = 200; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts

//Motorcycle Position
float motor_x = 0.0;
float motor_y = 0.0;
float direction[2]={0.0};

Motor* motorcycle;
Terrain* _terrain;


int main(int argc, char **argv) 
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 400);
	glutCreateWindow("MotorCross");

	_terrain = loadTerrain("heightmap.bmp", 20);
	//Initialize();

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
	if (deltaMove!=0.0) { // update camera position
		x += deltaMove * lx *1.5;
		y += deltaMove * ly *1.5;
		deltaMove=0.0f;
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
	
	glClearColor(0.0, 0.7, 1.0, 1.0); // sky color is light blue
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	declare_view(camera_view, motor_x, motor_y);

	// glColor3f(0.0, 0.7, 0.0);
	// glBegin(GL_QUADS);
	// 	glVertex3f(-100.0, -100.0, 0.0);
	// 	glVertex3f(-100.0,  100.0, 0.0);
	// 	glVertex3f( 100.0,  100.0, 0.0);
	// 	glVertex3f( 100.0, -100.0, 0.0);
	// glEnd();
	
	glPushMatrix();
		glScalef(30, 30, 20);
		glRotatef(90, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0, 0.0, 1.0);
		draw_Terrain();
	glPopMatrix();

	glPushMatrix();
		motorcycle->draw();
	glPopMatrix();
	
	glFlush();
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
			x=0; lx=1.0;
			y=0; ly=0.0;
	}
	if (key == 'F' || key =='f') camera_view=5;

} 

void pressSpecialKey(int key, int xx, int yy)
{
	switch (key) {
		case GLUT_KEY_UP :  direction[0]=0.5; break;
		case GLUT_KEY_DOWN : direction[0]=-0.5; break;
		case GLUT_KEY_RIGHT: direction[1]=-0.5; break;
		case GLUT_KEY_LEFT: direction[1]=0.5; break;
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
	if(button==3)
		deltaMove=1.0;

	else if(button==4)
		deltaMove=-1.0;
	else
		deltaMove=0.0;
}

Terrain* loadTerrain(const char* filename, float height) {
	Image* image = loadBMP(filename);
	Terrain* t = new Terrain(image->width, image->height);
	for(int y = 0; y < image->height; y++) {
		for(int x = 0; x < image->width; x++) {
			unsigned char color =
				(unsigned char)image->pixels[3 * (y * image->width + x)];
			float h = height * ((color / 255.0f) - 0.5f);
			t->setHeight(x, y, h);
		}
	}
	
	delete image;
	t->computeNormals();
	return t;
}

float _angle = 60.0f;

void cleanup() {
	delete _terrain;
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
}

void draw_Terrain() {
		
	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	
	float scale = 5.0f / max(_terrain->width() - 1, _terrain->length() - 1);
	glScalef(scale, scale, scale);
	glTranslatef(-(float)(_terrain->width() - 1) / 2,
				 0.0f,
				 -(float)(_terrain->length() - 1) / 2);
	
	glColor3f(0.3f, 0.9f, 0.0f);
	for(int z = 0; z < _terrain->length() - 1; z++) {
		//Makes OpenGL draw a triangle at every three consecutive vertices
		glBegin(GL_TRIANGLE_STRIP);
		for(int x = 0; x < _terrain->width(); x++) {
			Vec3f normal = _terrain->getNormal(x, z);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, _terrain->getHeight(x, z), z);
			normal = _terrain->getNormal(x, z + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, _terrain->getHeight(x, z + 1), z + 1);
		}
		glEnd();
	}
	
	glutSwapBuffers();
}