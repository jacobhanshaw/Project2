#pragma once

#include <stdio.h>
#include <vector>
#include <assert.h>

#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "GlobalObject.h"

using namespace std;
using namespace glm;


class Picture : public GlobalObject {

public:

	Picture();
	Picture(bool isMoving, vec3 position, vec3 rotation, vec3 scale, vec3 velocity, vec4 color);

	void render();

private:

	vector<vec3> va_vertices; //array of vertices of the Picture
	vector<vec4> va_colors;   //array of colors indicating the color of the vertices
	vector<ivec3> va_indices; //array of indices indicating how the vertices should join together to form a shape
	vector<vec3> va_normals;  //array of normals indicating how light should reflect off the shape

};