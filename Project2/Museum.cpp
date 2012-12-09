#include "Museum.h"

Museum::Museum() { }

const float PI = 3.14159265f;

void Museum::render() {
	
	glLoadIdentity();
	glScalef(2.0f, 2.0f, 2.0f);
	glTranslatef(0.0f, 0.0f, -2.75f);
	DrawWall();

} 

void Museum::DrawWall()
{
	GLboolean lightingState = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);

	GLboolean depthTestState = glIsEnabled(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);

	
	this->wallB.Bind();
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2d(1.0, 0.0);
	glVertex3d(1, 1, -1);
	glTexCoord2d(0.0, 0.0);
	glVertex3d(-1, 1, -1);
	glTexCoord2d(0.0, 1.0);
	glVertex3d(-1, -1, -1);
	glTexCoord2d(1.0, 1.0);
	glVertex3d(1, -1, -1);
	glEnd();

	if(lightingState) glEnable(GL_LIGHTING);
	if(depthTestState) glEnable(GL_DEPTH_TEST);
}