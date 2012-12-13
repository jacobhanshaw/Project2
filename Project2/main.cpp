
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

GLuint pictures[8];													//array of pictures 

Shader warpShader = Shader();
int lastMouseXPosition = -1;										// checks if the mouse swipe has already started
int lastMouseYPosition = -1;
glm::vec2 firstMousePos;											//inital mouse position
glm::vec2 mousePos;													//current mouse position		
bool isWireFrame = false;
bool isFullScreen = false;
bool controlModeIsOneHanded = true;									//switches between different movement modes
int window_width = 512;											    //current window width and height
int window_height = 512;
int original_window_width = 512;									//saves original window width and height to restore it when leaving full screen mode
int original_window_height = 512;
FrameBufferObject fbo;
Museum *graphicsMuseum = new Museum();								//the object that renders the museum scene
PlanarMesh *chandelierOuter = new PlanarMesh(25, 25, true);			//outer circle around chandelier
PlanarMesh *chandelierLight = new PlanarMesh(25, 25, true);			//light within chandelier
ILContainer chandelierOuterTexture;									
ILContainer chandelierLightTexture;

glm::vec3 startCollisionPositionGiant = glm::vec3(0,-100,0);		//initial collision in 3D coord space for each picture; goes 0-1 in x and y from top left to bottom right
glm::vec3 startCollisionPositionNorth = glm::vec3(0,-100,0);
glm::vec3 startCollisionPositionEast = glm::vec3(0,-100,0);
glm::vec3 startCollisionPositionWest = glm::vec3(0,-100,0);
glm::vec3 startCollisionPositionSouth = glm::vec3(0,-100,0);


glm::vec3 lastCollisionPositionGiant = glm::vec3(0,-100,0);			//current collision in 3D coord space for each wall
glm::vec3 lastCollisionPositionNorth = glm::vec3(0,-100,0);
glm::vec3 lastCollisionPositionEast = glm::vec3(0,-100,0);
glm::vec3 lastCollisionPositionWest = glm::vec3(0,-100,0);
glm::vec3 lastCollisionPositionSouth = glm::vec3(0,-100,0);

glm::vec3 correctedLastCollisionPositionGiant = glm::vec3(0,-100,0); //current collision in 3D coord space for each picture; goes 0-1 in x and y from top left to bottom right
glm::vec3 correctedLastCollisionPositionNorth = glm::vec3(0,-100,0);
glm::vec3 correctedLastCollisionPositionEast = glm::vec3(0,-100,0);
glm::vec3 correctedLastCollisionPositionWest = glm::vec3(0,-100,0);
glm::vec3 correctedLastCollisionPositionSouth = glm::vec3(0,-100,0);

glm::vec3 endCollisionPositionGiant = glm::vec3(0,-100,0);			//end collision in 3D coord space for each picture; goes 0-1 in x and y from top left to bottom right
glm::vec3 endCollisionPositionNorth = glm::vec3(0,-100,0);
glm::vec3 endCollisionPositionEast = glm::vec3(0,-100,0);
glm::vec3 endCollisionPositionWest = glm::vec3(0,-100,0);
glm::vec3 endCollisionPositionSouth = glm::vec3(0,-100,0);

//angle of rotation and position of camera
static float xpos = 0.0, ypos = 0.0, zpos = 0.0, xrot = 0.0, yrot = 0.0, zrot = 0.0, angle = 0.0;

using namespace std;

//camera function for controlling a global camera:
// controlled with the arrow keys
void camera () {
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on teh 
									//x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the 
									//	y-axis (up and down)
	glRotatef(zrot, 0, 0, 1);
    glTranslated(-xpos,-ypos,-zpos); //translate the screen
								  // to the position of our camera
}

//function used in debugging to check for GL errors
// prints any error to the console if one is found

bool CheckForGLErrors(string location) {
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

void warpPictures(int picture) {
	//West Wall
	fbo.Bind(picture);
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushAttrib(GL_VIEWPORT_BIT | GL_TRANSFORM_BIT);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(20, fbo.size.x/fbo.size.y, 1, 10);
		glViewport(0, 0, fbo.size.x, fbo.size.y);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		warpShader.Use();
		glUniform2f(warpShader.size_handle, graphicsMuseum->pictures[picture].width * graphicsMuseum->pictures[picture].frameRelativeScaleFactorX,
			graphicsMuseum->pictures[picture].height * graphicsMuseum->pictures[picture].frameRelativeScaleFactorY);
		glUniform2f(warpShader.mouse_position, correctedLastCollisionPositionGiant.x, 1-correctedLastCollisionPositionGiant.y);
		glUniform2f(warpShader.last_mouse_position, endCollisionPositionGiant.x, 1-endCollisionPositionGiant.y);
		glUniform2f(warpShader.first_mouse_position, startCollisionPositionGiant.x, 1-startCollisionPositionGiant.y);	
		glBindTexture(GL_TEXTURE_2D, pictures[picture]);
		glUniform1i(warpShader.texture, 0);//GL_TEXTURE0 + pictures[picture]);

		glBegin(GL_QUADS);
            glVertex3f  (-1, 1, -2); // Top Left
            glVertex3f  (1, 1, -2); // Bottom Left
            glVertex3f  (1, -1, -2); // Bottom Right
            glVertex3f  (-1, -1, -2); // Top Right
		glEnd();

		glUseProgram(0);
		
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopAttrib();
	fbo.Unbind();

}

void DisplayFunc()
{

	for(int i = 0; i < 7; ++i) {
		warpPictures(i);
	}
	
	
	CheckForGLErrors("Start Display: ");
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window_width, window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, double(window_width) / double(window_height), 1, 500);
	camera();
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);
	
	graphicsMuseum->render(&fbo);
	
	glLoadIdentity();
	glTranslated(0,1.75,-5.5);
	glScalef(0.35f,0.35f,0.35f);
	chandelierOuterTexture.Bind();
	chandelierOuter->Draw((PlanarMesh::OutArray));
	glScalef(2.0f,2.0f,2.0f);
	chandelierLightTexture.Bind();
	chandelierLight->Draw(PlanarMesh::OutArray);
	
	if(lastCollisionPositionGiant.y != -100){
	glLoadIdentity();
	glTranslated(lastCollisionPositionGiant.x,lastCollisionPositionGiant.y,lastCollisionPositionGiant.z);
	glScalef(2.0f,2.0f,2.0f);
	chandelierLightTexture.Bind();
	chandelierLight->Draw(PlanarMesh::OutArray);
	}

	if(lastCollisionPositionNorth.y != -100){
	glLoadIdentity();
	glTranslated(lastCollisionPositionNorth.x,lastCollisionPositionNorth.y,lastCollisionPositionNorth.z);
	glScalef(2.0f,2.0f,2.0f);
	chandelierLightTexture.Bind();
	chandelierLight->Draw(PlanarMesh::OutArray);
	}

	if(lastCollisionPositionWest.y != -100){
	glLoadIdentity();
	glTranslated(lastCollisionPositionWest.x,lastCollisionPositionWest.y,lastCollisionPositionWest.z);
	glScalef(2.0f,2.0f,2.0f);
	chandelierLightTexture.Bind();
	chandelierLight->Draw(PlanarMesh::OutArray);
	}

	if(lastCollisionPositionEast.y != -100){
	glLoadIdentity();
	glTranslated(lastCollisionPositionEast.x,lastCollisionPositionEast.y,lastCollisionPositionEast.z);
	glScalef(2.0f,2.0f,2.0f);
	chandelierLightTexture.Bind();
	chandelierLight->Draw(PlanarMesh::OutArray);
	}

	if(lastCollisionPositionSouth.y != -100){
	glLoadIdentity();
	glTranslated(lastCollisionPositionSouth.x,lastCollisionPositionSouth.y,lastCollisionPositionSouth.z);
	glScalef(2.0f,2.0f,2.0f);
	chandelierLightTexture.Bind();
	chandelierLight->Draw(PlanarMesh::OutArray);
	}

	float time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
	
	glutSwapBuffers();
	CheckForGLErrors("End Display: ");
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
	switch (c) {
	case GLUT_KEY_LEFT:
		if (yrot == 0 || yrot == 360) yrot = 359; 
		else yrot--;
		break;

	case GLUT_KEY_RIGHT: 
		if (yrot == 0 || yrot == 360) yrot = 1;
		else yrot++;
		break;

	case GLUT_KEY_UP:
		if (xrot == 0 || xrot == 360) xrot = 359; 
		else xrot--;
		break;

	case GLUT_KEY_DOWN:
		if (yrot == 0 || yrot == 360) yrot = 1;
		else xrot++;
		break;

	default: break;
	}
}

bool checkForWallCollisions(float newX, float newY, float newZ) {
	if(!(newY >= 5 || newY <= -5)){
		if(newZ >= 28) return false;
		if(newZ <= -100) return false;
		if(newX <= -42) return false;
		if(newX >= 42) return false;

		if(newZ >= 8) {
			if((newX <= 22 && newX >=2) || (newX >= -22 && newX <=-2)) return false;
			else if (newX > 22 || newX < -22) if(newZ < -16 || newZ > 16) return false;
		}
		if(newZ <= -8) {
			if((newX <= 22 && newX >=2) || (newX >= -22 && newX <=-2)) return false;
			else if (newX > 22 || newX < -22) if(newZ < -16 || newZ > 16) return false;
		}
		
		return true;
	}
	else return true;
}

void KeyboardFunc(unsigned char c, int x, int y)
{
	float newX = xpos;
	float newY = ypos;
	float newZ = zpos;
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
		
	case 'Q':
	case 'q':
		if (!isWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		isWireFrame = !isWireFrame;
		break;
	
	case 'E':
	case 'e':
		controlModeIsOneHanded = !controlModeIsOneHanded;
		break;

	case 'W':
	case 'w':
		if(yrot >= 0 && yrot <= 90){
			newX += yrot/90;
			newZ -= 1 - yrot/90;
			if(checkForWallCollisions(newX,newY,newZ)){
				xpos = newX;
				zpos = newZ;
			}
		}
		else if(yrot <= 180){
			newX += 1 - (yrot -90)/90;
			newZ += (yrot -90)/90;
			if(checkForWallCollisions(newX,newY,newZ)){
				xpos = newX;
				zpos = newZ;
			}
		}
		else if(yrot <= 270){
			newX -= (yrot - 180)/90;
			newZ += 1 - (yrot - 180)/90;
			if(checkForWallCollisions(newX,newY,newZ)){
				xpos = newX;
				zpos = newZ;
			}
		}
		else if(yrot <= 360){
			newX -= 1 - (yrot - 270)/90;
			newZ -= (yrot - 270)/90;
			if(checkForWallCollisions(newX,newY,newZ)){
				xpos = newX;
				zpos = newZ;
			}
		}
		else cout << "yRotation reached what should be an unreachable state";
		break;

	case 'S':
	case 's':
		if(yrot >= 0 && yrot <= 90){
			newX -= yrot/90;
			newZ += 1 - yrot/90;
			if(checkForWallCollisions(newX,newY,newZ)){
				xpos = newX;
				zpos = newZ;
			}
		}
		else if(yrot <= 180){
			newX -= 1 - (yrot -90)/90;
			newZ -= (yrot -90)/90;
			if(checkForWallCollisions(newX,newY,newZ)){
				xpos = newX;
				zpos = newZ;
			}
		}
		else if(yrot <= 270){
			newX += (yrot - 180)/90;
			newZ -= 1 - (yrot - 180)/90;
			if(checkForWallCollisions(newX,newY,newZ)){
				xpos = newX;
				zpos = newZ;
			}
		}
		else if(yrot <= 360){
			newX += 1 - (yrot - 270)/90;
			newZ += (yrot - 270)/90;
			if(checkForWallCollisions(newX,newY,newZ)){
				xpos = newX;
				zpos = newZ;
			}
		}
		else cout << "yRotation reached what should be an unreachable state";
		break;

	case 'A':
	case 'a':
		if(controlModeIsOneHanded){
			if (yrot == 0 || yrot == 360) yrot = 359;
			else yrot--;
		}
		else{
			float yrotOriginal = yrot;
			yrot -= 90;
			if(yrot < 0) yrot += 360;
			if(yrot >= 0 && yrot <= 90){
			newX += yrot/90;
			newZ -= 1 - yrot/90;
			if(checkForWallCollisions(newX,newY,newZ)){
				xpos = newX;
				zpos = newZ;
			}
		}
		else if(yrot <= 180){
			newX += 1 - (yrot -90)/90;
			newZ += (yrot -90)/90;
			if(checkForWallCollisions(newX,newY,newZ)){
				xpos = newX;
				zpos = newZ;
			}
		}
		else if(yrot <= 270){
			newX -= (yrot - 180)/90;
			newZ += 1 - (yrot - 180)/90;
			if(checkForWallCollisions(newX,newY,newZ)){
				xpos = newX;
				zpos = newZ;
			}
		}
		else if(yrot <= 360){
			newX -= 1 - (yrot - 270)/90;
			newZ -= (yrot - 270)/90;
			if(checkForWallCollisions(newX,newY,newZ)){
				xpos = newX;
				zpos = newZ;
			}
		}
		else cout << "yRotation reached what should be an unreachable state";
			yrot = yrotOriginal;
		}
		break;

	case 'D':
	case 'd':
		if(controlModeIsOneHanded){
			if (yrot == 0 || yrot == 360) yrot = 1;
			else yrot++;
		}
		else{
			float yrotOriginal = yrot;
			yrot += 90;
			if(yrot > 360) yrot -= 360;
			if(yrot >= 0 && yrot <= 90){
			newX += yrot/90;
			newZ -= 1 - yrot/90;
			if(checkForWallCollisions(newX,newY,newZ)){
				xpos = newX;
				zpos = newZ;
			}
		}
		else if(yrot <= 180){
			newX += 1 - (yrot -90)/90;
			newZ += (yrot -90)/90;
		}
		else if(yrot <= 270){
			newX -= (yrot - 180)/90;
			newZ += 1 - (yrot - 180)/90;
		}
		else if(yrot <= 360){
			newX -= 1 - (yrot - 270)/90;
			newZ -= (yrot - 270)/90;
		}
		else cout << "yRotation reached what should be an unreachable state";
		yrot = yrotOriginal;
		}
		break;

	case 'C':
	case 'c':
		ypos++;
		break;

	case 'Z':
	case 'z':
		ypos--;
		break;

	}
}

void CloseFunc()
{
	warpShader.TakeDown();
	fbo.TakeDown();

}

void checkForMouseCollisions(int startLastEnd){
	float wallPlaneWestDistance = (float) (-44 + 0.02); //picture is 0.02 away from wall
	float wallPlaneNorthDistance = (float) (-18 + 0.02);
	float wallPlaneGiantDistance = (float) (-300 + 0.02);

	glLoadIdentity();
	GLint viewport[4];                  // Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);    
	GLdouble modelview[16];                 // Where The 16 Doubles Of The Modelview Matrix Are To Be Stored
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);  
	GLdouble projection[16];                // Where The 16 Doubles Of The Projection Matrix Are To Be Stored
	glGetDoublev(GL_PROJECTION_MATRIX, projection);  

	GLfloat winX, winY, winZ;               // Holds Our X, Y and Z Coordinates

	winX = (float) mousePos.x;
	winY = (float) mousePos.y;
		
	winY = (float)viewport[3] - winY;           // Subtract The Current Mouse Y Coordinate From The Screen Height.
	glReadPixels((GLint)winX, (GLint)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	GLdouble posX, posY, posZ;              // Hold The Final Values

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	vec3 line =  glm::vec3(posX, posY, posZ);

	//cout << "\nUnprojected " << posX << " " << posY << " " << posZ;

	//east and west walls
	float bottom = (float) (posX-xpos);
	float xPositionDifference = (float) (posX - xpos);
	float yPositionDifference = (float) (posY - ypos);
	float zPositionDifference = (float) (posZ - zpos);

	float t;
	if((bottom !=0)){
	t = ((wallPlaneWestDistance-xpos)/bottom);
	if(t > 0){

	if(startLastEnd == 0){
	startCollisionPositionWest.x = xpos + t * xPositionDifference;
	startCollisionPositionWest.y = ypos + t * yPositionDifference;
	startCollisionPositionWest.z = zpos + t * zPositionDifference;
	}
	else if(startLastEnd == 1){
	lastCollisionPositionWest.x = xpos + t * xPositionDifference;
	lastCollisionPositionWest.y = ypos + t * yPositionDifference;
	lastCollisionPositionWest.z = zpos + t * zPositionDifference;

	}
	else if(startLastEnd == 2){
	endCollisionPositionWest.x = xpos + t * xPositionDifference;
	endCollisionPositionWest.y = ypos + t * yPositionDifference;
	endCollisionPositionWest.z = zpos + t * zPositionDifference;
	}
	else cout << "Bad ENUM to collision checking";

	}
	else{
		t= ((-wallPlaneWestDistance-xpos)/bottom);

	if(startLastEnd == 0){
	startCollisionPositionEast.x =  xpos + t * xPositionDifference;
	startCollisionPositionEast.y =  ypos + t * yPositionDifference;
	startCollisionPositionEast.z =  zpos + t * zPositionDifference;
	}
	else if(startLastEnd == 1){
	lastCollisionPositionEast.x = xpos + t * xPositionDifference;
	lastCollisionPositionEast.y = ypos + t * yPositionDifference;
	lastCollisionPositionEast.z = zpos + t * zPositionDifference;
	}
	else if(startLastEnd == 2){
	endCollisionPositionEast.x = xpos + t * xPositionDifference;
	endCollisionPositionEast.y = ypos + t * yPositionDifference;
	endCollisionPositionEast.z = zpos + t * zPositionDifference;
	}
	else cout << "Bad ENUM to collision checking";

	}
	}

	//north and south walls
	bottom = (float) (posZ -zpos);
	if((bottom !=0)){
	t = ((wallPlaneNorthDistance-zpos)/bottom);
	if(t > 0){

	if(startLastEnd == 0){
	startCollisionPositionNorth.x = xpos + t * xPositionDifference;
	startCollisionPositionNorth.y = ypos + t * yPositionDifference;
	startCollisionPositionNorth.z = zpos + t * zPositionDifference;
	}
	else if(startLastEnd == 1){
	lastCollisionPositionNorth.x = xpos + t * xPositionDifference;
	lastCollisionPositionNorth.y = ypos + t * yPositionDifference;
	lastCollisionPositionNorth.z = zpos + t * zPositionDifference;
	}
	else if(startLastEnd == 2){
	endCollisionPositionNorth.x = xpos + t * xPositionDifference;
	endCollisionPositionNorth.y = ypos + t * yPositionDifference;
	endCollisionPositionNorth.z = zpos + t * zPositionDifference;
	}
	else cout << "Bad ENUM to collision checking";

	}
	else{
		t= ((-wallPlaneNorthDistance-zpos)/bottom);

	if(startLastEnd == 0){
	startCollisionPositionSouth.x = xpos + t * xPositionDifference;
	startCollisionPositionSouth.y = ypos + t * yPositionDifference;
	startCollisionPositionSouth.z = zpos + t * zPositionDifference;
	}
	else if(startLastEnd == 1){
	lastCollisionPositionSouth.x = xpos + t * xPositionDifference;
	lastCollisionPositionSouth.y = ypos + t * yPositionDifference;
	lastCollisionPositionSouth.z = zpos + t * zPositionDifference;
	}
	else if(startLastEnd == 2){
	endCollisionPositionSouth.x = xpos + t * xPositionDifference;
	endCollisionPositionSouth.y = ypos + t * yPositionDifference;
	endCollisionPositionSouth.z = zpos + t * zPositionDifference;
	}
	else cout << "Bad ENUM to collision checking";

	}
	}

	//far wall
	if((bottom !=0)){
	t = ((wallPlaneGiantDistance-zpos)/bottom);
	if(t > 0){

	if(startLastEnd == 0){
	startCollisionPositionGiant.x = xpos + t * xPositionDifference;
	startCollisionPositionGiant.y = ypos + t * yPositionDifference;
	startCollisionPositionGiant.z = zpos + t * zPositionDifference;

	cout << "\nTop Left coord is " << graphicsMuseum->pictures[2].topLeftCoord.x << " " << graphicsMuseum->pictures[2].topLeftCoord.y;
	cout << "\nBottom Right coord is " << graphicsMuseum->pictures[2].bottomRightCoord.x << " " << graphicsMuseum->pictures[2].bottomRightCoord.y;
	cout << "\nDiff is " << graphicsMuseum->pictures[2].bottomRightMinusTopLeft.x << " " << graphicsMuseum->pictures[2].bottomRightMinusTopLeft.y;

	startCollisionPositionGiant.x = (startCollisionPositionGiant.x - graphicsMuseum->pictures[2].topLeftCoord.x)/graphicsMuseum->pictures[2].bottomRightMinusTopLeft.x;
	startCollisionPositionGiant.y = (startCollisionPositionGiant.y - graphicsMuseum->pictures[2].topLeftCoord.y)/graphicsMuseum->pictures[2].bottomRightMinusTopLeft.y;

	cout << "\nStart collision is " << startCollisionPositionGiant.x << " " << startCollisionPositionGiant.y;

	}
	else if(startLastEnd == 1){
	lastCollisionPositionGiant.x = xpos + t * xPositionDifference;
	lastCollisionPositionGiant.y = ypos + t * yPositionDifference;
	lastCollisionPositionGiant.z = zpos + t * zPositionDifference;

	correctedLastCollisionPositionGiant.x = (lastCollisionPositionGiant.x - graphicsMuseum->pictures[2].topLeftCoord.x)/graphicsMuseum->pictures[2].bottomRightMinusTopLeft.x;
	correctedLastCollisionPositionGiant.y = (lastCollisionPositionGiant.y - graphicsMuseum->pictures[2].topLeftCoord.y)/graphicsMuseum->pictures[2].bottomRightMinusTopLeft.y;

	cout << "\nLast collision is " << correctedLastCollisionPositionGiant.x << " " << correctedLastCollisionPositionGiant.y;

	}
	else if(startLastEnd == 2){
	endCollisionPositionGiant.x = xpos + t * xPositionDifference;
	endCollisionPositionGiant.y = ypos + t * yPositionDifference;
	endCollisionPositionGiant.z = zpos + t * zPositionDifference;

	endCollisionPositionGiant.x = (endCollisionPositionGiant.x - graphicsMuseum->pictures[2].topLeftCoord.x)/graphicsMuseum->pictures[2].bottomRightMinusTopLeft.x;
	endCollisionPositionGiant.y = (endCollisionPositionGiant.y - graphicsMuseum->pictures[2].topLeftCoord.y)/graphicsMuseum->pictures[2].bottomRightMinusTopLeft.y;

	cout << "\nEnd collision is " << endCollisionPositionGiant.x << " " << endCollisionPositionGiant.y;

	}
	else cout << "Bad ENUM to collision checking";

	}
	}

}


void MouseFunc(int button, int state, int x, int y){
	mousePos.x = (float)x;
	mousePos.y = (float)y;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		firstMousePos.x = (float)x;
		firstMousePos.y = (float)y;
		lastMouseXPosition = x;
		lastMouseYPosition = y;
//		cout << "\nReceived Left Button Down with XPos: " << x << " and YPos: " << y;
		checkForMouseCollisions(0);
	}
	else if(button !=GLUT_RIGHT_BUTTON && button != GLUT_MIDDLE_BUTTON) {
		lastMouseXPosition = -1;
		lastMouseYPosition = -1;
	//	firstMousePos.x = -1;
		//firstMousePos.y = -1;
		checkForMouseCollisions(2);
	//	cout << "\nReset Mouse Position";
	}
}

void MotionFunc(int x, int y) {
	mousePos.x = (float)x;
	mousePos.y = (float)y;

	if(x > window_width || x < 0 || y > window_height || y < 0){
		lastMouseXPosition = -1;
		lastMouseYPosition = -1;
	//	firstMousePos.x = -1;
	//	firstMousePos.y = -1;
	}
	if(lastMouseXPosition != -1 && lastMouseYPosition != -1){
		int differenceX = x - lastMouseXPosition;
		int differenceY = y - lastMouseYPosition;
	//	cout << "\nReceived Motion Update with XPos: ";
	//	cout << x;
	//	cout << " and YPos: ";
	//	cout << y;
	//	cout << "\nDifference in Motion Update from lastXPos: ";
	//	cout << differenceX;
	//	cout << " and lastYPos: ";
	//	cout << differenceY;
		lastMouseXPosition = x;
		lastMouseYPosition = y;
		checkForMouseCollisions(1);
	}
}

int main(int argc, char * argv[])
{
	
	glutInit(&argc , argv);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
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
	if (!fbo.Initialize(glm::ivec2(512, 512), 7, true))
	{
		cerr << "Frame buffer failed to initialize." << endl;
		return 0;
	}

	if (!warpShader.Initialize("warp.vert", "warp.frag"))
	{
		return 0;
	}

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	if (graphicsMuseum->ceiling.Initialize("celing1.jpg") == false)
	{
		cerr << "Failed to load Art Museum Ceiling texture." << endl;
	}

	if (graphicsMuseum->wall.Initialize("wall4.jpg") == false)
	{
		cerr << "Failed to load Art Museum Wall texture." << endl;
	}

	if (graphicsMuseum->floor.Initialize("floor3.jpg") == false)
	{
		cerr << "Failed to load Art Museum Floor texture." << endl;
	}

	if (chandelierOuterTexture.Initialize("chandelier0.jpg") == false)
	{
		cerr << "Failed to load Chandelier Outer texture." << endl;
	}

	if (chandelierLightTexture.Initialize("sky.jpg") == false)
	{
		cerr << "Failed to load Chandelier Light texture." << endl;
	}

	pictures[0] = ilutGLLoadImage("computer.jpg");
	pictures[1] = ilutGLLoadImage("dragon.jpg");
	pictures[2] = ilutGLLoadImage("perry-1.jpg");
	pictures[3] = ilutGLLoadImage("blueWaterfall.jpg");
	pictures[4] = ilutGLLoadImage("crumpled.jpg");
	pictures[5] = ilutGLLoadImage("blue_glass_texture.jpg");
	pictures[6] = ilutGLLoadImage("floor0.jpg");
	pictures[7] = ilutGLLoadImage("frame1.png");

	for(int i = 0; i < 7; ++i) {
		graphicsMuseum->pictures.push_back(Picture(&pictures[i], &pictures[7], &fbo, i, &warpShader, &warpShader));
	}

	chandelierOuter->ApplyCustomization(chandelier);
	chandelierLight->ApplyCustomization(sphere);

	

	glutMainLoop();
	return 0;
}
