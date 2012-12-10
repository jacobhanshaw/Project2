
#undef _UNICODE
#include <gl/glew.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>

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

#include "planar_mesh.h"
#include "ilcontainer.h"
#include "Museum.h"

#include "shader.h"

#define PI 3.14159265

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
bool isWireFrame = false;
bool isFullScreen = false;
int index = 0;
int window_width = 512;
int window_height = 512;
int original_window_width = 512;
int original_window_height = 512;
FrameBufferObject fbo;
Museum *graphicsMuseum = new Museum();
PlanarMesh *chandelierOuter = new PlanarMesh(100, 100, true);
PlanarMesh *chandelierLight = new PlanarMesh(100, 100, true);
ILContainer chandelierOuterTexture;
ILContainer chandelierLightTexture;

using namespace std;


//function used in debugging to check for GL errors
// prints any error to the console if one is found
bool CheckGLErrors(string location) {
	bool error_found = false;
	GLenum error;
	const GLubyte *errorString;
	while ((error = glGetError()) != GL_NO_ERROR) {
		cout <<"\n";
		cout <<location;
		error_found = true;
		errorString = gluErrorString(error);
		cout << errorString;
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

float chandelier(float input) {
	double result;
	//if(input < .25) result = sin((input/.25 + .25) * 180);
//	else if (input < .5) result = 1 - sin((input/.5) * 180);
	//if(input < .75) result = 0.5 - sin((input/.75) * 180);
//	else 
	result = 4 * sin(input + .25 * 180);
	return (float)result;
}

float sphere(float input) {
	input = input - 0.5f;
	return sqrt(0.25f - input * input);
}

void DisplayFunc()
{
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window_width, window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, double(window_width) / double(window_height), 1, 50);
	glMatrixMode(GL_MODELVIEW);

	graphicsMuseum->render();

	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
	glTranslated(0,2,-3);
	glScalef(0.35f,0.35f,0.35f);
//	glBindVertexArray(0);
	chandelierOuterTexture.Bind();
	chandelierOuter->Draw(PlanarMesh::WhichArray::OutArray);
	glScalef(2.0f,2.0f,2.0f);
	chandelierLightTexture.Bind();
	chandelierLight->Draw(PlanarMesh::WhichArray::OutArray);

//	glEnable(GL_DEPTH_TEST);
//	RenderIntoFrameBuffer();
	float time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(15, double(window_width) / double(window_height), 1, 10);
	
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
	*/
	//glDrawElements(GL_TRIANGLES , graphicsMuseum->wallA->GetNumberOfElements(), GL_UNSIGNED_INT , graphicsMuseum->wallA->GetTriangleIndexArray());
//	glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_TEXTURE_2D);
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
	if (h == 0 || w == 0)
		return;
	window_width = w;
	window_height = h;
	if(!isFullScreen){
		original_window_width = w;
		original_window_height = h;
	}
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
	case 'X':
		glutLeaveMainLoop();
		return;

	case 'F':
	case 'f':
		if (!isFullScreen) glutFullScreen();
		else glutReshapeWindow(original_window_width, original_window_height);
		isFullScreen = !isFullScreen;
		break;
		
	case 'W':
	case 'w':
		if (!isWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		isWireFrame = !isWireFrame;
		break;

	}
}

void CloseFunc()
{
	shader.TakeDown();

}


void MouseFunc(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		firstMousePos.x = (float)x;
		firstMousePos.y = (float)y;
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
	mousePos.x = (float)x;
	mousePos.y = (float)y;
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
	mousePos.x = (float)x;
	mousePos.y = (float)y;
}

int main(int argc, char * argv[])
{
	
	glutInit(&argc , argv);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE , 1.0);

	//////set up light position
	GLfloat light_position[] = { 0, 2, -3, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

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

	if (graphicsMuseum->ceiling.Initialize("Art_Museum_Ceiling.jpg") == false)
	{
		cerr << "Failed to load Art Museum Ceiling texture." << endl;
	}

	if (graphicsMuseum->wall.Initialize("Art_Museum_Wall.jpg") == false)
	{
		cerr << "Failed to load Art Museum Wall texture." << endl;
	}

	if (graphicsMuseum->floor.Initialize("Art_Museum_Floor.jpg") == false)
	{
		cerr << "Failed to load Art Museum Floor texture." << endl;
	}

	if (chandelierOuterTexture.Initialize("shark_white.jpg") == false)
	{
		cerr << "Failed to load Chandelier Outer texture." << endl;
	}

	if (chandelierLightTexture.Initialize("blue_glass_texture.jpg") == false)
	{
		cerr << "Failed to load Chandelier Light texture." << endl;
	}

	chandelierOuter->ApplyCustomization(chandelier);
	chandelierLight->ApplyCustomization(sphere);
	glutMainLoop();
	return 0;
}
