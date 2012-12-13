#include "Picture.h"

float Picture::scaleFactorX = 0.79;
float Picture::scaleFactorY = 0.75;

Picture::Picture(GLuint * picTex, GLuint * frameTex) {
	il_handle = ilGenImage();
	ilBindImage(il_handle);
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	picture = *(picTex);
	frame = *(frameTex);
	this->position = glm::vec2();
	aspect = width/height;
	scaleFactorX = 0.79;
	scaleFactorY = 0.75;
}


void Picture::render(GLuint texture) {
	glTranslatef(position.x, 0, position.y + .01);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frame);
	glPushMatrix();

	glScalef(1/aspect, aspect*2, 1);


	glBegin(GL_QUADS);
		glTexCoord2d(1.0, 0.0);
		glVertex3d(1, 1, 0);
		glTexCoord2d(0.0, 0.0);
		glVertex3d(-1, 1, 0);
		glTexCoord2d(0.0, 1.0);
		glVertex3d(-1, -1, 0);
		glTexCoord2d(1.0, 1.0);
		glVertex3d(1, -1, 0);
	glEnd();

	glScalef(scaleFactorX, scaleFactorY, 1);

	glTranslatef(0, 0, .01);
	
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
		glTexCoord2d(1.0, 0.0);
		glVertex3d(1, -1, 0);
		glTexCoord2d(0.0, 0.0);
		glVertex3d(-1, -1, 0);
		glTexCoord2d(0.0, 1.0);
		glVertex3d(-1, 1, 0);
		glTexCoord2d(1.0, 1.0);
		glVertex3d(1, 1, 0);
	glEnd();


	glPopMatrix();
}