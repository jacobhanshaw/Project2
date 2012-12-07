/*	Frame buffer object demonstration - Perry Kivolowitz - University of Wisconsin Madison.

#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assert.h>
#include "globals.h"
#include "fbo.h"

using namespace std;

WindowData window;
FrameBufferObject fbo;

void Axes()
{
	GLUquadric * q = gluNewQuadric();

	glPushMatrix();
	glScalef(0.5f, 0.5f, 0.5f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(q, 0.02, 0.02, 1.0, 10, 10);
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluSphere(q, 0.02, 10, 10);
	glPopMatrix();

	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(q, 0.02, 0.02, 1.0, 10, 10);
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluSphere(q, 0.02, 10, 10);
	glPopMatrix();

	glColor3f(0.0f, 0.0f, 1.0f);
	gluCylinder(q, 0.02, 0.02, 1.0, 10, 10);
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluSphere(q, 0.02, 10, 10);
	glPopMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(q, 0.011, 10, 10);
	gluDeleteQuadric(q);
}

void RenderIntoFrameBuffer()
{
	float time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	fbo.Bind();
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib(GL_VIEWPORT_BIT | GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(20, double(fbo.size.x) / double(fbo.size.y), 1, 10);
	glViewport(0, 0, fbo.size.x, fbo.size.y);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(0, 0, 5.5, 0, 0, 0, 0, 1, 0);
	glRotatef(-time * 60.0f, 1.0f, 1.0f, 0.0f);
	Axes();
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 0.5f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(-0.5f, -0.5f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.5f, -0.5f);
	glEnd();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	fbo.Unbind();
}

void UseFramebufferToDrawSomething()
{
	float time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(15, double(window.width) / double(window.height), 1, 10);
	glViewport(0, 0, window.width, window.height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5.5, 0, 0, 0, 0, 1, 0);
	glRotatef(time * 30.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-30, 1.0f, 0.0f, 0.0f);
	Axes();
	glBindTexture(GL_TEXTURE_2D, fbo.texture_handles[0]);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(0.5f, -0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(0.5f, 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-0.5f, 0.5f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void DisplayFunc()
{
	if (window.window_handle < 1)
		return;
	glEnable(GL_DEPTH_TEST);
	RenderIntoFrameBuffer();
	UseFramebufferToDrawSomething();
	glutSwapBuffers();
}

void ReshapeFunc(int w, int h)
{
	if (h <= 0 || window.window_handle < 1)
		return;
	window.width = w;
	window.height = h;
}

void TimerFunc(int value)
{
	if (window.window_handle < 1)
		return;
	glutPostRedisplay();
	glutTimerFunc(window.timer_period, TimerFunc, 0);
}

void KeyboardFunc(unsigned char c, int x, int y)
{
	if (window.window_handle < 1)
		return;
	window.mouse_coordinates.x = x;
	window.mouse_coordinates.y = y;
	switch (c)
	{
	case 'x':
	case 27:
		glutLeaveMainLoop();
		break;
	}
}

void CloseFunc()
{
	window.window_handle = -1;
	fbo.TakeDown();
}

int main(int argc, char * argv[])
{
	window.timer_period = 1000 / 60;

	glutInit(&argc , argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0 , 0);
	glutInitWindowSize(512 , 512);

	if ((window.window_handle = glutCreateWindow("Frame Buffer Demo")) < 1)
	{
		cerr << "Window creation failed." << endl;
		return 0;
	}
	
	if (glewInit() != GLEW_OK)
	{
		cerr << "Glew failed to initialize." << endl;
		return 0;
	} 

	if (!fbo.Initialize(glm::ivec2(512, 512), 1, true))
	{
		cerr << "Frame buffer failed to initialize." << endl;
		return 0;
	}
	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutTimerFunc(window.timer_period, TimerFunc, 0);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutCloseFunc(CloseFunc);
	glutMainLoop();
	return 0;
}
*/