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
uniform ivec2 first_mouse_position;
uniform ivec2 last_mouse_position;
uniform sampler2D texture;
varying vec2 texcoord;

void main(void)
{

	vec2 f = gl_FragCoord.xy / float(framebuffer_size);	
	vec2 m = mouse_position / float(framebuffer_size);
	vec2 first = first_mouse_position/ float(framebuffer_size);
	vec2 texLoc;
	vec2 direction = m - first;
	float sRad = .1;
	bool affected = false;
	
	for(float i = 0; i <= 1; i = i + .01) {
		vec2 point = first + direction*i;
		float distance = abs(length(point - f));
		if(distance <= sRad) {
			sRad = distance;
			affected = true;
			texLoc = texcoord + direction*(1/(-exp(1000*distance*distance))*i);			
		} 
	}
	if (affected) {
		fragment_color = texture2D(texture, texLoc);
	} else { fragment_color = texture2D(texture, texcoord); }
	
	
}

/*
float distance = length(f - m);
	float xOfSlope = (mouse_position.x - first_mouse_position.x)/float(framebuffer_size.x);
	float yOfSlope = (mouse_position.y - first_mouse_position.y)/float(framebuffer_size.y);
	float scaledX = gl_FragCoord.x/float(framebuffer_size.x);
	float scaledY = (framebuffer_size.y - gl_FragCoord.y)/float(framebuffer_size.y);
	float distance = abs(xOfSlope * scaledX + yOfSlope * scaledY)/sqrt(xOfSlope * xOfSlope + yOfSlope * yOfSlope);
//	distance /= sqrt(framebuffer_size.x * framebuffer_size.x + framebuffer_size.y * framebuffer_size.y);
	if(distance <= .1) {
		texLoc.x = texcoord.x + ((.1 - distance)/.1) * ((first_mouse_position.x - mouse_position.x)/float(framebuffer_size.x));
		texLoc.y = texcoord.y - ((.1 - distance)/.1) * ((first_mouse_position.y - mouse_position.y)/float(framebuffer_size.y));
		fragment_color = texture2D(texture, texLoc);
	} else { fragment_color = texture2D(texture, texcoord); }
*/