/*	Samples from four  different epochs in OpenGL development
	in the same program. 

	Perry Kivolowitz
	Computer Sciences Department - University of Wisconsin.
*/

#version 400

layout (location = 0) out vec4 fragment_color;

uniform ivec2 framebuffer_size;
uniform ivec2 center_red;
uniform ivec2 center_green;
uniform ivec2 mouse_position;

void main(void)
{
	// How do you collapse many of these lines down into single vec manipulation?
	vec2 c_green = center_green / float(framebuffer_size);
	vec2 c_red = center_red / float(framebuffer_size);
	vec2 f = gl_FragCoord.xy / float(framebuffer_size);
	vec2 m = mouse_position.xy / float(framebuffer_size);
	float distance = length(f - m);
	if(distance <= .01) {
		fragment_color = vec4(1, 1, 0, 1);
	} else { fragment_color = vec4(0, 0, 1, 1); }
}
