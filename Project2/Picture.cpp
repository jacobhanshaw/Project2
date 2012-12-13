#include "Picture.h"

Picture::Picture(GLuint * picTex, GLuint * frameTex, FrameBufferObject * fboIn, int colorAttachmentIndexIn, Shader *lightShaderIn, Shader *distortionShaderIn) {
	il_handle = ilGenImage();
	ilBindImage(il_handle);
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	frame = *(frameTex);
	this->position = glm::vec2();
	aspect = width/height;
	fbo = fboIn;
	colorAttachmentIndex = colorAttachmentIndexIn;
	lightShader = lightShaderIn;
	distortionShader = distortionShaderIn;
	scaleFactorX = 1.0;
	scaleFactorY = 1.0;
	frameRelativeScaleFactorX= 0.79;
	frameRelativeScaleFactorY= 0.75;
}


void Picture::render() {
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

	glScalef(frameRelativeScaleFactorX, frameRelativeScaleFactorY, 1);

	glTranslatef(0, 0, .01);
	
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo->texture_handles[colorAttachmentIndex]);
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

void Picture::setWorldCenterPosition(glm::vec3 pos){
	worldCenterPosition = pos;
	topLeftCoord.x = worldCenterPosition.x - scaleFactorX * (4 * width * frameRelativeScaleFactorX);
	topLeftCoord.y = worldCenterPosition.y + scaleFactorY * (4 * height * frameRelativeScaleFactorY);

	bottomRightCoord.x = worldCenterPosition.x + scaleFactorX * (4 * width * frameRelativeScaleFactorX);
	bottomRightCoord.y = worldCenterPosition.y - scaleFactorY * (4 * height * frameRelativeScaleFactorY);

	bottomRightMinusTopLeft.x = bottomRightCoord.x -topLeftCoord.x;
	bottomRightMinusTopLeft.y = bottomRightCoord.y -topLeftCoord.y;
}