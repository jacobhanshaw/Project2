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

class Shader {
	
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
	GLuint center_handle_red;
	GLuint center_handle_green;

} 