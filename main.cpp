#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#pragma warning(disable:4996)
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "glslprogram.h"
#include "FFT.h"

#define ESCAPE		0x1b
#define BUFF_SIZE 400

const char *WINDOWTITLE = { "Minna Barry - Shaders Final Project" };
const int LEFT = { 4 };
const int MIDDLE = { 2 };
const int RIGHT = { 1 };

void Update();
void Render();
void Increment();
void Menu(int);
void InitGraphics();
void InitMenu();
void InitFFT();
void InitShaders();
void MouseButton(int, int, int, int);
void MouseMotion(int, int);
void Reset();
void Resize(int, int);
void Visibility(int);

GLSLProgram	*Spectrum;
GLSLProgram *Bars;
FFT *fft;
Vector2f samples[796];
int WindowSize = 800;
int	ActiveButton;
int	MainWindow;	
int	Xmouse, Ymouse;			
float Xrot, Yrot;				
bool Switch;
int counter;
string path = "Ressources/01IDontWannaLoveYou.wav";
int buffer = 4096;
//FFT fft("Ressources/wind.wav", 4096);
//FFT fft("Ressources/01IDontWannaLoveYou.wav", 4096);
//FFT fft("Ressources/money.wav", 4096);
//FFT fft("Ressources/ff6.wav", 4096);
//FFT fft("Ressources/ff9.wav", 4096);

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	InitGraphics();
	Reset();
	InitMenu();
	glutSetWindow(MainWindow);
	glutMainLoop();
	return 0;
}

void Increment() {
	counter++;
	if(counter >=BUFF_SIZE)
		counter = 0;
}

void Update(){
	fft->positions(samples);
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void Render(){
	glColor3f(1, 1, 1);
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glViewport(0, 0, WindowSize, WindowSize);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90., 1., 0.1, 1000.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0., 10., 20., 0., 0., 0., 0., 1., 0.);
	if (Switch) {
		glPushMatrix();
		glScalef(2., 2., 2.);
		Spectrum->Use();
		glPointSize(3.);
		counter = 0;
		glBegin(GL_LINE_LOOP);
		for (float angle = 0.; angle < 360; angle += 360. / BUFF_SIZE)
		{
			float t = (M_PI*angle) / 180.;
			float x = 5 * cos(t);
			float y = 5 * sin(t);
			glVertex4f(x, y, 0, samples[counter].y / 1000000.0);
			glNormal3f(x, y, 0);
			Increment();
			if (counter > 360)
				counter = 0;
		}
		glVertex4f(5, 0, 0, samples[0].y / 1000000.0);
		glNormal3f(1, 0, 0);
		glEnd();
		Spectrum->Use(0);
		glPopMatrix();
	}

	glRotatef((GLfloat)Yrot, 0., 1., 0.);
	glRotatef((GLfloat)Xrot, 1., 0., 0.);
	glEnable(GL_NORMALIZE);

	if (!Switch) {
		glPushMatrix();
		Bars->Use();
		glRotatef(-45, 0., 1., 0.);
		glTranslatef(-10, 0, -10);

		glBegin(GL_TRIANGLES);
		glNormal3f(0., 1., 0.);
		int size = 1;
		for (float i = 0, k = 0; i < 20; i += 1.5) {
			for (float j = 0, l = 0; j < 20; j += 1.5) {
				glVertex4f(i, 0, j, samples[counter].y);
				glVertex4f(i + size, 0, j, samples[counter].y);
				glVertex4f(i, 0, j + size, samples[counter].y);

				glVertex4f(i + size, 0, j + size, samples[counter].y);
				glVertex4f(i + size, 0, j, samples[counter].y);
				glVertex4f(i, 0, j + size, samples[counter].y);

				Increment();

			}
		}
		glEnd();
		glPopMatrix();
	}
	counter = 0;
	glutSwapBuffers();
	glFlush();
}

void Menu(int option){
	switch (option){
	case 0:
		Reset();
		break;
	case 1:
		glutSetWindow(MainWindow);
		glFinish();
		glutDestroyWindow(MainWindow);
		exit(0);
		break;
	case 2:
		Switch = !Switch;
	}
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void InitMenu(){
	glutSetWindow(MainWindow);
	int mainmenu = glutCreateMenu(Menu);
	glutAddMenuEntry("Switch", 2);
	glutAddMenuEntry("Reset", 0);
	glutAddMenuEntry("Quit", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void InitGraphics(){
	InitFFT();
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WindowSize, WindowSize);
	MainWindow = glutCreateWindow(WINDOWTITLE);
	glutSetWindowTitle(WINDOWTITLE);
	glClearColor(0.,0.,0.,1.);
	glutIdleFunc(Update);
	glutSetWindow(MainWindow);
	glutDisplayFunc(Render);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutVisibilityFunc(Visibility);
	glutSetCursor(GLUT_CURSOR_NONE);
	GLenum err = glewInit();
	InitShaders();
}
void InitFFT() {
	cout << "Enter a valid .wav file path: ";
	cin >> path;
	cout << "Loading samples.. Ignore the unexpected message if there was one..\n";
	fft = new FFT(path, buffer);
	
}

void InitShaders() {
	Spectrum = new GLSLProgram();
	Bars = new GLSLProgram();
	Spectrum->Create("spectrum.vert", "spectrum.frag");
	Bars->Create("bars.vert","bars.geom" ,"bars.frag");
	Spectrum->SetVerbose(false);
	Bars->SetVerbose(false);
}

void MouseButton(int button, int state, int x, int y){
	int b = 0;
	switch (button){
	case GLUT_LEFT_BUTTON:
		b = LEFT;		break;

	case GLUT_MIDDLE_BUTTON:
		b = MIDDLE;		break;

	case GLUT_RIGHT_BUTTON:
		b = RIGHT;		break;

	}
	if (state == GLUT_DOWN){
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;
	}
	else{
		ActiveButton &= ~b;
	}
}
void MouseMotion(int x, int y){
	int dx = x - Xmouse;
	int dy = y - Ymouse;
	if ((ActiveButton & LEFT) != 0)
	{
		Xrot += dy;
		Yrot += dx;
	}
	Xmouse = x;	
	Ymouse = y;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void Reset(){
	ActiveButton = 0;
	Xrot = Yrot = 0.;
	Switch = 1;
	counter = 0;
}
void Resize(int width, int height){
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void Visibility(int state){
	if (state == GLUT_VISIBLE){
		glutSetWindow(MainWindow);
		glutPostRedisplay();
	}
}