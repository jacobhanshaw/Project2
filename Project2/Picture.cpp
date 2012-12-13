#include "Picture.h"


Picture::Picture() {
	
	this->position = glm::vec2();
}
Picture::Picture(int x, int z) {
	
	this->position = glm::vec2(x, z);
}
Picture::Picture(glm::vec2 pos) {
	
	this->position = pos;
}

void Picture::render(GLuint texture) {
	glTranslatef(position.x, 0, position.y + .01);
	this->frame.Bind();
	glPushMatrix();
	//glLoadIdentity();
	glScalef(.62, 1, 1);


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

	glScalef(.79, .75, 1);

	glTranslatef(0, 0, .01);
	
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
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


	glPopMatrix();
}