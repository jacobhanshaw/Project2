/*	Samples from four  different epochs in OpenGL development
	in the same program. 

	Perry Kivolowitz
	Computer Sciences Department - University of Wisconsin.
*/

#version 400

layout (location = 0) out vec4 fragment_color;

uniform sampler2D texture;
varying vec2 texcoord;

void main(void)
{

	fragment_color = texture2D(texture, texcoord);
	
	
}
