
#undef _UNICODE
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <assert.h>
#include "globals.h"
#include "fbo.h"

#include "shader.h"

struct point {
	int x;
	int y;
};

Shader shader = Shader();
int lastMouseXPosition = -1;
int lastMouseYPosition = -1;
glm::vec2 firstMousePos;
glm::vec2 mousePos;
point positions[2000];
int index = 0;
int window_width = 512;
int window_height = 512;
FrameBufferObject fbo;

using namespace std;



bool CheckGLErrors()
{
	bool error_found = false;
	while (glGetError() != GL_NO_ERROR)
	{
		error_found = true;
	}
	return error_found;
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


void DisplayFunc()
{
	glEnable(GL_DEPTH_TEST);
	RenderIntoFrameBuffer();
	float time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(15, double(window_width) / double(window_height), 1, 10);
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5.5, 0, 0, 0, 0, 1, 0);
	glRotatef(time * 30.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-30, 1.0f, 0.0f, 0.0f);
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
	/*
	glClearColor(0, 0, 0.5, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, 1, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
	shader.Use();
	glUniform2i(shader.size_handle, window_width, window_height);
	glUniform2i(shader.center_handle_red, 256, 256);
	glUniform2i(shader.center_handle_green, 100, 100);
	glUniform2i(shader.mouse_position, mousePos.x, window_height-mousePos.y);
	glUniform2i(shader.last_mouse_position, lastMouseXPosition, window_height-lastMouseYPosition);
	glUniform2i(shader.first_mouse_position, firstMousePos.x, window_height-firstMousePos.y);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo.texture_handles[0]);
	glUniform1i(shader.texture, 0);

	glBegin(GL_QUADS);
            glVertex3f  (-1, 1, -2); // Top Left
            glVertex3f  (-1, -1, -2); // Bottom Left
            glVertex3f  (1, -1, -2); // Bottom Right
            glVertex3f  (1, 1, -2); // Top Right
	glEnd();



	glUseProgram(0);

	*/
	glutSwapBuffers();
}

void TimerFunc(int period)
{
	glutTimerFunc((unsigned int) period, TimerFunc, period);

	glutPostRedisplay();
}

void ReshapeFunc(int w, int h)
{
	if (h == 0)
		return;
	window_width = w;
	window_height = h;
}

void SpecialFunc(int c, int x, int y)
{

}

void KeyboardFunc(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 27:
	case 'x':
		glutLeaveMainLoop();
		return;
	}
}

void CloseFunc()
{
	shader.TakeDown();

}


void MouseFunc(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		firstMousePos.x = x;
		firstMousePos.y = y;
		lastMouseXPosition = x;
		lastMouseYPosition = y;
		cout << "\nReceived Left Button Down with XPos: ";
		cout << x;
		cout << " and YPos: ";
		cout << y;
	}
	else if(button !=GLUT_RIGHT_BUTTON && button != GLUT_MIDDLE_BUTTON) {
		lastMouseXPosition = -1;
		lastMouseYPosition = -1;
		//firstMousePos.x = -1;
		//firstMousePos.y = -1;
		cout << "\nReset Mouse Position";
	}
	mousePos.x = x;
	mousePos.y = y;
}

void MotionFunc(int x, int y) {
	if(x > window_width || x < 0 || y > window_height || y < 0){
		lastMouseXPosition = -1;
		lastMouseYPosition = -1;
	//	firstMousePos.x = -1;
	//	firstMousePos.y = -1;
	}
	if(lastMouseXPosition != -1 && lastMouseYPosition != -1){
		int differenceX = x - lastMouseXPosition;
		int differenceY = y - lastMouseYPosition;
		cout << "\nReceived Motion Update with XPos: ";
		cout << x;
		cout << " and YPos: ";
		cout << y;
		cout << "\nDifference in Motion Update from lastXPos: ";
		cout << differenceX;
		cout << " and lastYPos: ";
		cout << differenceY;
		lastMouseXPosition = x;
		lastMouseYPosition = y;
	}
	mousePos.x = x;
	mousePos.y = y;
}

int main(int argc, char * argv[])
{
	
	glutInit(&argc , argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(0 , 0);
	glutInitWindowSize(512 , 512);
	glutCreateWindow("Project 2 - Adam Hart and Jacob Hanshaw");
	
	glutDisplayFunc(DisplayFunc);
	glutTimerFunc(1000 / 60, TimerFunc, 1000 / 60);
	glutReshapeFunc(ReshapeFunc);
	glutSpecialFunc(SpecialFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutCloseFunc(CloseFunc);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);

	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW failed to initialize." << endl;
		return 0;
	}
	if (!fbo.Initialize(glm::ivec2(512, 512), 1, true))
	{
		cerr << "Frame buffer failed to initialize." << endl;
		return 0;
	}
	if (!shader.Initialize("stub.vert", "stub.frag"))
	{
		return 0;
	}
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	glutMainLoop();
	return 0;
}
