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
	static float scaleFactorX;
	static float scaleFactorY;
	GLuint frame;
	GLuint picture;
	ILuint il_handle, width, height;
	Picture(GLuint * picTex, GLuint * frameTex);
	void render(GLuint texture);

};