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

#include "shader.h"



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

// Details of shader use is not commented upon in this program.




class Window
{
public:
	Window(GLsizei w, GLsizei h)
	{
		this->handle = (GLuint) -1;
		this->width = w;
		this->height = h;
		this->mode = MODE_MODERN;
		this->display_list_handle = (GLuint) -1;
		this->vertex_array_handle = (GLuint) -1;
		this->vertex_coordinate_handle = (GLuint) - 1;
	}

	enum Mode
	{
		MODE_ORIGINAL,
		MODE_DISPLAY_LIST,
		MODE_VERTEX_ARRAY,
		MODE_MODERN,
		MODE_END
	};

	GLuint handle;
	GLsizei width;
	GLsizei height;
	Mode mode;

	// Used with display lists.
	GLuint display_list_handle;

	// Used with vertex arrays.
	vector<GLdouble> va_vertices;
	vector<GLdouble> va_colors;
	vector<GLuint> va_indices;

	// Used with shaders.
	vector<glm::vec2> sh_vertices;
	GLuint vertex_coordinate_handle;
	GLuint vertex_array_handle;

	void DisplayMode(char * s, bool clear = false);

	void OriginalMode();
	void DisplayListMode();
	void VertexArrayMode();
	void ModernMode();
};

/*	DisplayMode() is used to (optionally clear the buffer and then)
	render text indicating which mode the program is in  on-screen.
	The font engine in  GLUT stroke  fonts.   The function works by 
	creating an orthographic projection  using screen  units.  This 
	means,  for  example,  the  call  to  glTranslatef()  specifies 
	actual screen coordinates.

	The  parameter determining if  the framebuffer is cleared is an
	optional paramter - if not provided by the caller, the value is
	false (no screen clearing is done).
*/

void Window::DisplayMode(char * s, bool clear)
{
	if (clear)
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, this->width, 0, this->height, 1, 10);
	glViewport(0, 0, this->width, this->height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(10, 10, -5.5f);
	glScalef(0.25f, 0.25f, 1.0f);
	glColor3f(1, 1, 1);
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *) s);
}

/*	OriginalMode() - draws a triangle using the original method of
	specifying colors  and vertices within a glBegin() and glEnd()
	pair.
*/
void Window::OriginalMode()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, ((double) this->width) / ((double) this->height), 1, 10);
	glViewport(0, 0, this->width, this->height);

	glClearColor(0, 0, 0.5, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, -5.5, 0, 0, 0, 0, 1, 0);

	glBegin(GL_TRIANGLES);
	glColor3d(1, 0, 0);		glVertex2d( 0,  1);
	glColor3d(0, 1, 0);		glVertex2d(-1, -1);
	glColor3d(0, 0, 1);		glVertex2d( 1, -1);
	glEnd();

	this->DisplayMode("Original OpenGL");
}


/*	ModernMode() - This shows how OpenGL no longer uses  the
	projection and modelview stacks and has no glBegin() and
	glEnd().

	The gist of  this code is very similar to the vertex ar-
	ray version.  Here  glDrawArrays  is  being  used  (not 
	glDrawElements). This means no buffer containing  indic-
	es is being  specified -  and  if  there were repeating
	vertices, they would indeed have to be fully repeated.

	A shader  program  is  responsible for  filling in what
	used to be fixed parts  of  the  graphics pipeline. See
	the files stub.vert and stub.frag.

*/

void Window::ModernMode()
{
	if (this->sh_vertices.size() == 0)
	{
		this->sh_vertices.push_back(glm::vec2(-1.0f, -1.0f));
		this->sh_vertices.push_back(glm::vec2( 1.0f, -1.0f));
		this->sh_vertices.push_back(glm::vec2( 1.0f,  1.0f));
		this->sh_vertices.push_back(glm::vec2(-1.0f,  1.0f));

		glGenBuffers(1, &this->vertex_coordinate_handle);
		assert(this->vertex_coordinate_handle != (GLuint) -1);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_coordinate_handle);
		glBufferData(GL_ARRAY_BUFFER, this->sh_vertices.size() * sizeof(glm::vec2), &this->sh_vertices[0], GL_STATIC_DRAW);

		glGenVertexArrays(1, &this->vertex_array_handle);
		glBindVertexArray(this->vertex_array_handle);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_coordinate_handle);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *) NULL);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	shader.Use();
	glUniform2i(shader.size_handle, this->width, this->height);
	glUniform2i(shader.center_handle_red, 256, 256);
	glUniform2i(shader.center_handle_green, 100, 100);

	glViewport(0, 0, this->width, this->height);

	glClearColor(0, 0, 0.5, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	assert(this->vertex_array_handle != (GLuint) -1);
	glBindVertexArray(this->vertex_array_handle);
	glDrawArrays(GL_QUADS, 0, (GLsizei) this->sh_vertices.size());
	// Note - the following  could  also be
	// used in this case:
	// glDrawElements(GL_TRIANGLES , 3 , GL_UNSIGNED_INT , &this->va_indices[0]);
	// The benefit to using glDrawElements() 
	// is  that  repeated vertices  (and at-
	// tributes)  would  be  indicated only
	// by repeated indices.
	glBindVertexArray(0);
	glUseProgram(0);

	this->DisplayMode("Shaders");
}

Window window(512, 512);

void DisplayFunc()
{
	window.ModernMode();
	glutSwapBuffers();
}

void TimerFunc(int period)
{
	glutTimerFunc((unsigned int) period, TimerFunc, period);

	// Test the window handle before
	// calling   glutPostRedisplay()
	// because  the  window may have
	// closed while  we were asleep.
	// Without  this  test, we might
	// be posting  a message  for a
	// window that isn't there.
	if (window.handle != (GLuint) -1)
		glutPostRedisplay();
}

void ReshapeFunc(int w, int h)
{
	if (h == 0)
		return;
	window.width = w;
	window.height = h;
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

	if (window.display_list_handle != (GLuint) -1)
		glDeleteLists(window.display_list_handle, 1);

	// This  is  necessary to   prevent one
	// last execution of the timer function
	// from calling glutPostRedisplay after
	// the window has been closed.
	window.handle = (GLuint) -1;
}

int main(int argc, char * argv[])
{
	glutInit(&argc , argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(0 , 0);
	glutInitWindowSize(window.width , window.height);
	window.handle = glutCreateWindow("OpenGL Drawing Methods Through The Ages - Perry Kivolowitz");
	glutDisplayFunc(DisplayFunc);
	glutTimerFunc(1000 / 60, TimerFunc, 1000 / 60);
	glutReshapeFunc(ReshapeFunc);
	glutSpecialFunc(SpecialFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutCloseFunc(CloseFunc);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW failed to initialize." << endl;
		return 0;
	}

	if (!shader.Initialize("stub.vert", "stub.frag"))
	{
		return 0;
	}

	glutMainLoop();
	return 0;
}
