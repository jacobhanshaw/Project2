#pragma once

#include <gl/glew.h>
#include <stdio.h>
#include <vector>
#include <assert.h>

#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "ilcontainer.h"
#include "planar_mesh.h"
#include "shader.h"
#include "Picture.h"
#include "fbo.h"

using namespace std;
using namespace glm;


class Museum {

public:

	//PlanarMesh *wallA;

	ILContainer ceiling;
	ILContainer wall;
	ILContainer door;
	ILContainer floor;

	vector<Picture> pictures;

	Museum();

	void render(FrameBufferObject * fbo);
	void MakeTextureQuad(int xTextureDensity, int yTextureDensity);


private:

	
	vector<vec3> va_vertices; //array of vertices of the Museum
	vector<vec4> va_colors;   //array of colors indicating the color of the vertices
	vector<ivec3> va_indices; //array of indices indicating how the vertices should join together to form a shape
	vector<vec3> va_normals;  //array of normals indicating how light should reflect off the shape

};