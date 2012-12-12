/*	Samples from four  different epochs in OpenGL development
	in the same program. 

	Perry Kivolowitz
	Computer Sciences Department - University of Wisconsin.
*/

#version 400

layout (location = 0) in vec2 VertexPosition;
varying vec2 texcoord;

void main()
{
	gl_Position = vec4(VertexPosition, 0, 1);
	
	texcoord.x = VertexPosition.x/2 -0.5;
	texcoord.y = VertexPosition.y/2 -0.5;
}
