#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdio.h>
#include <sstream>


const char * FramebufferCompletenessError(GLint error_code);

struct WindowData
{
	GLint window_handle;
	GLint width, height;
	int timer_period;
	glm::ivec2 mouse_coordinates;
};
