#include "Museum.h"

Museum::Museum() { }

const float PI = 3.14159265f;

void Museum::render() {
	
//	GLboolean lightingState = glIsEnabled(GL_LIGHTING);
//	glDisable(GL_LIGHTING);

//	GLboolean depthTestState = glIsEnabled(GL_DEPTH_TEST);
//	glDisable(GL_DEPTH_TEST);

	GLboolean texture2DState = glIsEnabled(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);

	glLoadIdentity();
	glScalef(2.0f, 2.0f, 2.0f);
	glTranslatef(0.0f, 0.0f, -2.75f);

	glPushMatrix();
	this->wall.Bind();
	MakeTextureQuad();
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 0 , 1, 0);
	this->wall.Bind();
	MakeTextureQuad();
	glPopMatrix();
		
	glPushMatrix();
	glRotated(-90, 0 , 1, 0);
	this->wall.Bind();
	MakeTextureQuad();
	glPopMatrix();

	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	this->floor.Bind();
	MakeTextureQuad();
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	this->ceiling.Bind();
	MakeTextureQuad();
	glPopMatrix();

//	if(lightingState) glEnable(GL_LIGHTING);
//	if(depthTestState) glEnable(GL_DEPTH_TEST);
	if(!texture2DState) glDisable(GL_TEXTURE_2D);
} 

void Museum::MakeTextureQuad()
{
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
}