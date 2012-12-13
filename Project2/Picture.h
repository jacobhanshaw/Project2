#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <vector>
#include <assert.h>

#include "ilcontainer.h"
#include "shader.h"
#include "fbo.h"

using namespace std;

class Picture {

private:
	
	glm::vec2 position;

public:
	double aspect;
	glm::vec3 worldCenterPosition;
	float frameRelativeScaleFactorX;
	float frameRelativeScaleFactorY;
	float scaleFactorX;
	float scaleFactorY;

	glm::vec3 topLeftCoord;
	glm::vec3 bottomRightCoord;
	glm::vec3 bottomRightMinusTopLeft;

	GLuint frame;
	ILuint il_handle, width, height;
	FrameBufferObject * fbo;
	int colorAttachmentIndex;
	Shader *lightShader;
	Shader *distortionShader;

	Picture(GLuint * picTex, GLuint * frameTex, FrameBufferObject * fbo, int colorAttachmentIndex, Shader *lightShader, Shader *distortionShader);
	void render();

	void setWorldCenterPosition(glm::vec3 pos);

};