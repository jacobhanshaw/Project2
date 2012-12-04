/*	THIS IS A MODIFIED VERSION TO PERMIT ON-THE-FLY DURING CLASS MODIFICATION TO DEMO SHADERS FOR PROJECT TWO.

*/


/*	Samples from four  different epochs in OpenGL development
	in the same program. 

	Perry Kivolowitz
	Computer Sciences Department - University of Wisconsin.

	The OpenGL "hello world" of drawing a colored triangle is
	presented using:
	o The  original  means of  immediate mode  glBegin()  and 
	  glEnd().
	o The original means plugged into a display list.
	o Vertex arrays.
	o Shaders.

	x or ESC exits the program.
	F1 advances from mode to mode - showing the same triangle.

	Why the "this" pointer is used where it is not needed: To
	assist code completion in IDE's. 

	Tip: Use of vectors  (gaining the  benefit of their auto-
	matic  destruction)  where OpenGL  is  expecting an array 
	pointer.  &a_vector[0] is equivalent to an_array for this
	purpose.
*/
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


using namespace std;

/*	This CheckGLErrors() is a stub. In your code, you ought to provide
	a means of reporting the exact value of  each OpenGL  error found. 
	The glu library  provides  a function  which converts GLenum error
	codes into strings (for most but not all OpenGL errors). 

	Notice how glGetError() needs to be called  within a loop. This is
	because OpenGL stores up a bit vector  of errors.  Each time error
	status is requested using glGetError,  one of the set bits is pro-
	cessed (and the appropriate GLenum is returned). When the error is
	returned to the caller, the bit is turned off. Continuing  to call
	glGetError()  until it  returns  GL_NO_ERROR  clears  all  current 
	error bits.
*/

bool CheckGLErrors()
{
	bool error_found = false;
	while (glGetError() != GL_NO_ERROR)
	{
		error_found = true;
	}
	return error_found;
}






void DisplayFunc()
{
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, 1, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0.5, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	shader.Use();
	glUniform2i(shader.size_handle, window_width, window_height);
	glUniform2i(shader.center_handle_red, 256, 256);
	glUniform2i(shader.center_handle_green, 100, 100);
	glUniform2i(shader.mouse_position, mousePos.x, window_height-mousePos.y);
	glUniform2i(shader.last_mouse_position, lastMouseXPosition, window_height-lastMouseYPosition);
	glUniform2i(shader.first_mouse_position, firstMousePos.x, window_height-firstMousePos.y);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ilutGLLoadImage("perry-1.jpg"));
	glUniform1i(shader.texture, 0);

	glBegin(GL_QUADS);
            glVertex3f  (-1, 1, -2); // Top Left
            glVertex3f  (-1, -1, -2); // Bottom Left
            glVertex3f  (1, -1, -2); // Bottom Right
            glVertex3f  (1, 1, -2); // Top Right
		glEnd();

	glUseProgram(0);
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
	glutInitWindowSize(window_width , window_height);
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
