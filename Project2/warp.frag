/*	Samples from four  different epochs in OpenGL development
	in the same program. 

	Perry Kivolowitz
	Computer Sciences Department - University of Wisconsin.
*/

#version 400

layout (location = 0) out vec4 fragment_color;

uniform vec2 framebuffer_size;
uniform vec2 mouse_position;
uniform vec2 first_mouse_position;
uniform vec2 last_mouse_position;
uniform sampler2D texture;
varying vec2 texcoord;

void main(void)
{

	vec2 f = gl_FragCoord.xy / float(framebuffer_size);	
	vec2 m = mouse_position; // float(framebuffer_size);
	vec2 first = first_mouse_position; // float(framebuffer_size);
	vec2 texLoc;
	vec2 direction = m - first;
	float sRad = .5;
	bool affected = false;
	
	for(float i = 0; i <= 1; i = i + .005) {
		vec2 point = first + direction*i;
		float distance = length(point - f);

		if(distance <= sRad) {
			sRad = distance;
			affected = true;
			texLoc = texcoord + direction*(1/(-exp(100*distance*distance))*i);			
		} 
	}

	if (affected) {
		fragment_color = texture2D(texture, texLoc);
	} else { fragment_color = texture2D(texture, texcoord); }
	
	
}
