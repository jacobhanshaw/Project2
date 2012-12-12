#pragma once

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

using namespace std;

class Shader {
private:
	bool CheckGLErrors();

public:
	Shader();
	void TakeDown();
	void Use();
	bool Initialize(char * vertex_shader_file, char * fragment_shader_file);

	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	GLuint program_id;
	bool LoadShader(const char * file_name, GLuint shader_id);
	stringstream GetShaderLog(GLuint shader_id);
	GLuint size_handle;
	GLuint mouse_position;
	GLuint first_mouse_position;
	GLuint last_mouse_position;
	GLuint texture;
};