#include "Picture.h"

const float PI = 3.14159265f;

vector<vec3> Picture::va_vertices;
vector<ivec3> Picture::va_indices;
vector<vec3> Picture::va_normals;

Picture::Picture() {
	//this->pointValue = 0;
}
/*
Picture::Picture(bool isMoving, vec3 position, vec3 rotation, vec3 scale, vec3 velocity, vec4 color) : 
	Object(isMoving, position, rotation, scale, velocity, color){
	this->pointValue = 0;
} 
*/
Picture::~Picture() {

}

void Picture:: render() {
	//Source: https://uwmad.courses.wisconsin.edu/d2l/lms/content/viewer/main_frame.d2l?tId=11011796&ou=1821693
	//Example vertex array code used to figure out vertex arrays

	//GL_CULL_FACE is left enabled as disabling it leads to bizarre ripple effects
	glDisable(GL_COLOR_MATERIAL); //disabled, so that the color will be determined by the material specified below
/*	GLboolean blendAlreadyEnabled; 
	glGetBooleanv(GL_BLEND, &blendAlreadyEnabled); //checks if blending is already enable, to later restore the previous state
	if(blendAlreadyEnabled) glBlendFunc(GL_ONE, GL_ONE);
	else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND); */

	//Unique color and alpha values for each component of light result in giving a unique look to the
	//model. These values were chosen to simulate the appearance of a shiny, semi-transparent balloon
	GLfloat material_ambient_gold[] = {0.25f, 0.20f, 0.08f, 0.8f};//{ 0.7f, 0.6f, 0.6f, 0.7f};
    GLfloat material_diffuse_gold[] = {0.75f, 0.61f, 0.22f, 0.8f};//{ 0.7f, 0.6f, 0.6f, 0.7f};
    GLfloat material_specular_gold[] = { 0.63f, 0.56f, 0.37f, 0.7f};
    GLfloat material_shininess_gold[] = { .4f * 128.0f };

	GLfloat material_ambient_silver[] = { 0.7f, 0.6f, 0.6f, 0.7f};
    GLfloat material_diffuse_silver[] = { 0.7f, 0.6f, 0.6f, 0.7f};
    GLfloat material_specular_silver[] = { 0.7f, 0.6f, 0.6f, 0.7f};
    GLfloat material_shininess_silver[] = { .25f * 128.0f };

	GLfloat material_ambient_red[] = {1.0f, 0.1f, 0.1f, 0.8f};//{ 0.7f, 0.6f, 0.6f, 0.7f};
    GLfloat material_diffuse_red[] = {1.0f, 0.1f, 0.1f, 0.8f};//{ 0.7f, 0.6f, 0.6f, 0.7f};
    GLfloat material_specular_red[] = { 0.7f, 0.6f, 0.6f, 0.7f};
    GLfloat material_shininess_red[] = { .25f * 128.0f };

	GLfloat material_ambient_green[] = {0.1f, 1.0f, 0.1f, 0.8f};//{ 0.7f, 0.6f, 0.6f, 0.7f};
    GLfloat material_diffuse_green[] = {0.1f, 1.0f, 0.1f, 0.8f};//{ 0.7f, 0.6f, 0.6f, 0.7f};
    GLfloat material_specular_green[] = { 0.6f, 0.7f, 0.6f, 0.7f};
    GLfloat material_shininess_green[] = { .25f * 128.0f };

	GLfloat material_ambient_blue[] = {0.1f, 0.1f, 1.0f, 0.8f};//{ 0.7f, 0.6f, 0.6f, 0.7f};
    GLfloat material_diffuse_blue[] = {0.1f, 0.1f, 1.0f, 0.8f};//{ 0.7f, 0.6f, 0.6f, 0.7f};
    GLfloat material_specular_blue[] = { 0.6f, 0.6f, 0.7f, 0.7f};
    GLfloat material_shininess_blue[] = { .25f * 128.0f };


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient_red);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse_red);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular_red);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess_red);

	if (va_vertices.size() == 0) {


		for(int y = 0; y <= yLength * yDensity; ++y) {
			for(int x = 0; x <= xLength * xDensity; ++x) {

				int row = (int)y/yDensity;
			
				this->va_vertices.push_back(glm::vec3(x/xDensity,y/yDensity,0));

			}
		}
	
	

		GLfloat a;
		GLfloat aMax = (omegaPoints - 1) * phiPoints;
		//Iterates through the loop and connects the vertices into triangular shapes that blur together into the final shape
		//the max index of the loop does not include the last row of points as each row constructs triangles with the row
		//beneath it and thus the last row does not need to construct triangles
		for(a = 0; a < aMax; a++) {
			//the last vertex in a row is unique as the point considered next to it is actually the first point in the row
			if(((int)a % (int)phiPoints) != ((int)phiPoints-1)) {
				this->va_indices.push_back(glm::ivec3(a,a+phiPoints+1,a+phiPoints));
				this->va_indices.push_back(glm::ivec3(a,a + 1,a+phiPoints+1));
			}
			else {
				this->va_indices.push_back(glm::ivec3(a,a+1,a+phiPoints));
				this->va_indices.push_back(glm::ivec3(a,a-phiPoints+1,a+1));
			}
			//the top row of normals are easily predicted due to the shape of the balloon, the rest are calculated
			if(a < phiPoints) {
				this->va_normals.push_back(glm::vec3(0,1,0));
			}
			else {

				//Source: http://glm.g-truc.net/code.html used to figure our normal calculations

				glm::vec3 one;
				glm::vec3 two;
				glm::vec3 three;
				glm::vec3 four;
				glm::vec3 five;
				glm::vec3 six;

				//get all points starting at point directly above and going clockwise
				one = va_vertices[GLuint(a - phiPoints)];
				//points at the end of the row must wrap around the array to find their neighbor points
				if(((int)a % (int)phiPoints) != ((int)phiPoints-1)) {
					two = va_vertices[GLuint(a + 1)];
					three = va_vertices[GLuint(a + phiPoints + 1)];
				}
				else {
					two = va_vertices[GLuint(a - phiPoints + 1)];
					three = va_vertices[GLuint(a + 1)];
				}
				four = va_vertices[GLuint(a + phiPoints)];
				//points at the end of the row must wrap around the array to find their neighbor points
				if(((int)a % (int)phiPoints) != 0) {
					five = va_vertices[GLuint(a - 1)];
					six = va_vertices[GLuint(a - phiPoints - 1)];
				}
				else {
					five = va_vertices[GLuint(a + phiPoints - 1)];
					six = va_vertices[GLuint(a - 1)];
				}
				
				vec3 sum = glm::cross(one - va_vertices[GLuint(a)], two - va_vertices[GLuint(a)]);
				sum += glm::cross(two - va_vertices[GLuint(a)], three - va_vertices[GLuint(a)]);
				sum += glm::cross(three - va_vertices[GLuint(a)], four - va_vertices[GLuint(a)]);
				sum += glm::cross(four - va_vertices[GLuint(a)], five - va_vertices[GLuint(a)]);
				sum += glm::cross(five - va_vertices[GLuint(a)], six - va_vertices[GLuint(a)]);
				sum += glm::cross(six - va_vertices[GLuint(a)], one - va_vertices[GLuint(a)]);
				this->va_normals.push_back(glm::normalize(sum));
			}
		} 

		//The preceding loop did not address the final row of normals as it did not need to loop through the final
	    //row to specify vertices. This loop fills in the rest of the normals as they are easily predicted due to the
		//shape of the balloon.
		for(a = 0; a < phiPoints; ++a) {
			this->va_normals.push_back(glm::vec3(0,-1,0));
		}


	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3 , GL_FLOAT , 0 , &va_vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &va_normals[0]);

	glDrawElements(GL_TRIANGLES, 3 * va_indices.size(), GL_UNSIGNED_INT, &va_indices[0]);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPushMatrix();
	glTranslated(0, -1.55, 0);
	glRotated(90, 1, 0, 0);
	glutSolidTorus((GLdouble) 0.018, (GLdouble) 0.11, (GLint) 20, (GLint) 20);
	//Source: http://www.nigels.com/glt/doc/class_glut_torus.html
	glPopMatrix();

	glEnable(GL_COLOR_MATERIAL);
	if(!blendAlreadyEnabled){
		glDisable(GL_BLEND);
	}
	else glBlendFunc(GL_ONE, GL_ONE);
}
