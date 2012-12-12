#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <vector>
#include <assert.h>

#include "ilcontainer.h"

using namespace std;

class Picture {

private:
	
	glm::vec2 position;

public:
	ILContainer frame;
	ILContainer picture;

	Picture();
	Picture(int x, int z);
	Picture(glm::vec2 pos);
	void render();

};