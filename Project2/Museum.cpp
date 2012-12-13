#include "Museum.h"

Museum::Museum() {
}

const float PI = 3.14159265f;

void Museum::render(FrameBufferObject * fbo) {
	
//	GLboolean lightingState = glIsEnabled(GL_LIGHTING);
//	glDisable(GL_LIGHTING);

//	GLboolean depthTestState = glIsEnabled(GL_DEPTH_TEST);
//	glDisable(GL_DEPTH_TEST);

	GLboolean texture2DState = glIsEnabled(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);

	glLoadIdentity();

	glScalef(4.0f, 2.0f, 2.0f);
	
	//north walls start
	glPushMatrix();
	glTranslatef(-3.0f, 0.0f, -5.0f);
	this->wall.Bind();
	glScalef(2.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);	
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.0f, 0.0f, -5.0f);
	this->wall.Bind();
	glScalef(2.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();

	//NW Wall
	glPushMatrix();
	glTranslatef(-8.0f, 0.0f, -9.0f);
	this->wall.Bind();
	glPushMatrix();
	glScalef(3.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();
	pictures[0].worldCenterPosition = glm::vec3(-32.0f, 0.0f, -18.0f);
	pictures[0].render(fbo->texture_handles[1]);
	glPopMatrix();

	//NE Wall
	glPushMatrix();
	glTranslatef(8.0f, 0.0f, -9.0f);
	this->wall.Bind();
	glPushMatrix();
	glScalef(3.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();
	pictures[0].render(fbo->texture_handles[3]);
	glPopMatrix();

	//Giant Wall
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -150.0f);
	this->wall.Bind();
	glPushMatrix();
	glScalef(50.0f, 50.0f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();
	glScalef(25.0f, 25.0f, 1.0f);
	pictures[0].render(fbo->texture_handles[2]);
	glPopMatrix();

	//north walls end

	//south walls start

	glPushMatrix();
	glRotated(-180, 0, 1, 0);
	glPushMatrix();
	glTranslatef(-4.0f, 0.0f, -5.0f);
	this->wall.Bind();
	glScalef(1.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.0f, 0.0f, -5.0f);
	this->wall.Bind();
	glScalef(1.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();

	//SW Wall
	glPushMatrix();
	glTranslatef(-8.0f, 0.0f, -9.0f);
	this->wall.Bind();
	glPushMatrix();
	glScalef(3.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();
	pictures[0].render(fbo->texture_handles[6]);
	glPopMatrix();

	//SE Wall
	glPushMatrix();
	glTranslatef(8.0f, 0.0f, -9.0f);
	this->wall.Bind();
	glPushMatrix();
	glScalef(3.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();
	pictures[0].render(fbo->texture_handles[5]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.0f, 0.0f, -7.0f);
	this->wall.Bind();
	glScalef(1.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0f, 0.0f, -7.0f);
	this->wall.Bind();
	glScalef(1.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -15.0f);
	this->wall.Bind();
	glScalef(1.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();

	glPopMatrix();
	//south walls end


	//side walls start
	glPushMatrix();

	glRotated(-90, 0, 1, 0);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -11.0f);
	this->wall.Bind();

	glPushMatrix();
	glScalef(9.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();

	glPushMatrix();
	//glScalef(0.5f,1.0f,2.0f);
	pictures[0].render(fbo->texture_handles[4]);
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.0f, 0.0f, -5.0f);
	this->wall.Bind();
	glScalef(2.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7.0f, 0.0f, -5.0f);
	this->wall.Bind();
	glScalef(2.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6.0f, 0.0f, -3.0f); //EDITED THIS ONE
	this->wall.Bind();
	glScalef(1.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(11.0f, 0.0f, -1.0f); //EDITED THIS ONE
	this->wall.Bind();
	glScalef(4.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();
	
	glPopMatrix();

	//side walls end

	//side walls start

	glPushMatrix();

	glRotated(90, 0, 1, 0);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -11.0f);
	this->wall.Bind();
	glPushMatrix();
	glScalef(9.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();
	pictures[0].render(fbo->texture_handles[0]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.0f, 0.0f, -5.0f);
	this->wall.Bind();
	glScalef(2.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7.0f, 0.0f, -5.0f);
	this->wall.Bind();
	glScalef(2.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-6.0f, 0.0f, -3.0f);
	this->wall.Bind();
	glScalef(1.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();

	
	glPushMatrix();
	glTranslatef(-11.0f, 0.0f, -1.0f);
	this->wall.Bind();
	glScalef(4.0f, 2.5f, 1.0f);
	MakeTextureQuad(1,1);
	glPopMatrix();
	

	glPopMatrix();

	//side walls end

	//floor start

	glPushMatrix();

	glRotated(90, 1, 0, 0);

	glPushMatrix();
	glTranslatef(0.0f, -30.0f, 2.5f);
	this->floor.Bind();
	glScalef(2.0f, 25.0f, 1.0f);
	MakeTextureQuad(4,6);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 5.0f, 2.5f);
	this->floor.Bind();
	glScalef(5.0f, 10.0f, 1.0f);
	MakeTextureQuad(10,2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.0f, 0.0f, 2.5f);
	this->floor.Bind();
	glScalef(3.0f, 9.0f, 1.0f);
	MakeTextureQuad(6,2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-8.0f, 0.0f, 2.5f);
	this->floor.Bind();
	glScalef(3.0f, 9.0f, 1.0f);
	MakeTextureQuad(6,2);
	glPopMatrix();

	glPopMatrix();

	//floor end
	//ceiling start
	
	glPushMatrix();

	glRotated(-90, 1, 0, 0);

	glRotated(180, 0, 0, 1);

	glPushMatrix();
	glTranslatef(0.0f, 5.0f, 2.5f);
	this->ceiling.Bind();
	glScalef(5.0f, 10.0f, 1.0f);
	MakeTextureQuad(10,10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.0f, 0.0f, 2.5f);
	this->ceiling.Bind();
	glScalef(3.0f, 9.0f, 1.0f);
	MakeTextureQuad(6,9);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-8.0f, 0.0f, 2.5f);
	this->ceiling.Bind();
	glScalef(3.0f, 9.0f, 1.0f);
	MakeTextureQuad(6,9);
	glPopMatrix();

	glPopMatrix();
	
	//ceiling end

	this->floor.Bind();

	glBegin(GL_QUADS);
	glTexCoord2d(1.0, 0.0); 
	glVertex3d(-50, 50, -150);
	glTexCoord2d(0.0, 0.0);
	glVertex3d(-1, 2.5, -5);
	glTexCoord2d(0.0, 1.0);
	glVertex3d(-1, -2.5, -5);
	glTexCoord2d(1.0, 1.0);
	glVertex3d(-50, -50, -150);
	glEnd();
	
	glBegin(GL_QUADS);
	glTexCoord2d(1.0, 0.0);
	glVertex3d(50, 50, -150);  //TOP RIGHT
	glTexCoord2d(0.0, 0.0);
	glVertex3d(1, 2.5, -5);  //TOP LEFT
	glTexCoord2d(0.0, 1.0);
	glVertex3d(1, -2.5, -5); //BOTTOM LEFT
	glTexCoord2d(1.0, 1.0);
	glVertex3d(50, -50, -150); //BOTTOM RIGHT
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2d(1.0, 0.0);
	glVertex3d(50, 50, -150);  //TOP RIGHT
	glTexCoord2d(0.0, 0.0);
	glVertex3d(-50, 50, -150);  //TOP LEFT
	glTexCoord2d(0.0, 1.0);
	glVertex3d(-1, 2.5, -5); //BOTTOM LEFT
	glTexCoord2d(1.0, 1.0);
	glVertex3d(1, 2.5, -5); //BOTTOM RIGHT
	glEnd();
	

//	if(lightingState) glEnable(GL_LIGHTING);
//	if(depthTestState) glEnable(GL_DEPTH_TEST);
	if(!texture2DState) glDisable(GL_TEXTURE_2D);
} 

void Museum::MakeTextureQuad(int xTextureDensity, int yTextureDensity)
{
	glBegin(GL_QUADS);
	glTexCoord2d(1.0 * xTextureDensity, 0.0);
	glVertex3d(1, 1, 0);
	glTexCoord2d(0.0, 0.0);
	glVertex3d(-1, 1, 0);
	glTexCoord2d(0.0, 1.0 * yTextureDensity);
	glVertex3d(-1, -1, 0);
	glTexCoord2d(1.0 * xTextureDensity, 1.0 * yTextureDensity);
	glVertex3d(1, -1, 0);
	glEnd();
}