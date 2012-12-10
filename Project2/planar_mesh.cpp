#include <GL/freeglut.h>
#include "planar_mesh.h"

#ifndef	BAD_GL_VALUE
#define	BAD_GL_VALUE	((GLuint) -1)
#endif

/*	Perry Kivolowitz - University of Wisconsin CS 559
	
	A class to draw a planar mesh  grid  triangulated in one
	of two ways. The mesh is drawn  over +/- 0.5 so scale to
	taste. Vector3s can be drawn  to assist in the debugging
	of displacement maps.

	Vector3 arrays are used.  The  caller is given access to
	both the original data (the planar  mesh which should be
	kept pristine) and the output array (which the user must
	fill in). When the mesh is asked to draw itself the user
	specifies which mesh (in or out) to draw from.
*/

// A constructur which sets the settings and builds the
// mesh right away.
PlanarMesh::PlanarMesh(int xDensity, int yDensity, bool open, MeshStyle meshStyle)
{
	this->open = open;
	this->xDensity = xDensity;
	this->yDensity = yDensity;

	this->ReleaseMemory();
	this->AllocateMemory();
	this->MakeMesh(meshStyle);
}

PlanarMesh::~PlanarMesh()
{
	this->ReleaseMemory();
}

// ReleaseMemory is abstracted from the destructor because it
// can then be shared with ResizeMesh().
void PlanarMesh::ReleaseMemory()
{
	this->VInArray.clear();
	this->VTInArray.clear();
	this->VTOutArray.clear();
	this->VOutArray.clear();
	this->TArray.clear();
}

// AllocateMemory is similarly abstracted from the constructor allowing
// it to be shared with ResizeMesh.
void PlanarMesh::AllocateMemory()
{
	this->VInArray.resize(xDensity * yDensity);
	this->VTInArray.resize(xDensity * yDensity);
	this->TArray.resize((xDensity - 1) * (yDensity - 1) * 2);
	this->NInArray.resize(xDensity * yDensity);
}

void PlanarMesh::ResizeMesh(int xDensity, int yDensity, MeshStyle meshStyle)
{
	ReleaseMemory();
	this->xDensity = xDensity;
	this->yDensity = yDensity;
	AllocateMemory();
	MakeMesh(meshStyle);
}

// These two inline functions help in defining the "flexible" mesh.
inline bool EVEN(int n)
{
	return ((n % 2) == 0);
}

inline bool ODD(int n)
{
	return ((n & 1) == 1);
}

void PlanarMesh::MakeMesh(MeshStyle meshStyle)
{
	int		y , x , i;
	float		HalfW;
	float		HalfH;
	glm::uvec3 * t;

	// assigns values to vertices

	HalfW = (xDensity - 1) / 2.0f;
	HalfH = (yDensity - 1) / 2.0f;

	// Define a grid of Vector3es with Vector3s that point down the Z axis.
	// The Vector3 grid ranges over +/- 0.5.

	// We also define a grid of texture coordinates. These simply range
	// from zero to one.

	for (y = 0; y < yDensity; y++)
	{
		for (x = 0; x < xDensity; x++)
		{
			i = y * xDensity + x;

			VInArray[i].x = (((float) x) - HalfW) / ((float) xDensity - 1);
			VInArray[i].y = (((float) y) - HalfH) / ((float) yDensity - 1);
			VInArray[i].z = 0.0;

			NInArray[i].x = 0.0;
			NInArray[i].y = 0.0;
			NInArray[i].z = 1.0;

			VTInArray[i].s = ((float) x) / ((float) xDensity - 1);
			VTInArray[i].t = ((float) y) / ((float) yDensity - 1);
		}
	}

	// To use vector arrays, we must define triangles by indicating which
	// Vector3es live in the same triangle. With more complicated code
	// TRIANGLE_STRIPS could have been drawn force large changes to the
	// following code.

	// This code provides two means of assigning Vector3es to triangles.
	// The two methods produce grids conforming to two schools of thought.
	// For a planar mesh, once a texture is placed on them it won't really
	// make a difference. However, if you start manipulating the components
	// of the mesh (in the VOutArray) one triangulation method might 
	// produce better results than another.

	for (y = 0; y < yDensity - 1; y++)
	{
		for (x = 0; x < xDensity - 1; x++)
		{
			t = &TArray[(y * (xDensity - 1) + x) * 2];

			if (meshStyle == Flexible)
			{
				if ((EVEN(x) && EVEN(y)) || (ODD(x) && ODD(y)))
				{
					t->x = (y + 0) * xDensity + x + 0;
					t->y = (y + 0) * xDensity + x + 1;
					t->z = (y + 1) * xDensity + x + 0;
					t++;
					t->x = (y + 0) * xDensity + x + 1;
					t->y = (y + 1) * xDensity + x + 1;
					t->z = (y + 1) * xDensity + x + 0;
				}
				else 
				{
					t->x = (y + 0) * xDensity + x + 0;
					t->y = (y + 0) * xDensity + x + 1;
					t->z = (y + 1) * xDensity + x + 1;
					t++;
					t->x = (y + 0) * xDensity + x + 0;
					t->y = (y + 1) * xDensity + x + 1;
					t->z = (y + 1) * xDensity + x + 0;
				}
			}
			else // Regular
			{
				// "top" triangle
				t->x = (y + 0) * xDensity + x + 0;
				t->y = (y + 0) * xDensity + x + 1;
				t->z = (y + 1) * xDensity + x + 0;
				t++;
				// "bottom" triangle
				t->x = (y + 0) * xDensity + x + 1;
				t->y = (y + 1) * xDensity + x + 1;
				t->z = (y + 1) * xDensity + x + 0;
			}
		}
	}

	this->NOutArray.insert(this->NOutArray.begin(), this->NInArray.begin(), this->NInArray.end());
	this->VTOutArray.insert(this->VTOutArray.begin(), this->VTInArray.begin(), this->VTInArray.end());
	this->VOutArray.insert(this->VOutArray.begin(), this->VInArray.begin(), this->VInArray.end());
}

// When you ask the mesh to draw itself, you must specify which array
// to draw. There are two arrays. The InArray should always be kept
// pristine. The OutArray might contain new vertices after some type
// of displacement map operation is performed by the calling code.
void PlanarMesh::Draw(WhichArray whichArray, bool drawNormals)
{
	// We will specify array pointers for Vector3es, Vector3s and texture
	// coordinates. Therefore, we must enable OpenGL to process these
	// arrays.
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// These functions specify the array pointers of the various types.
	// Notice we can point to either the IN arrays or the OUT arrays.
	// This permits the use of the OUT array after processing using
	// a displacement map.
	glVertexPointer(3 , GL_FLOAT , sizeof(glm::vec3) , (whichArray == InArray) ? &VInArray[0] : &VOutArray[0]);
	glNormalPointer(GL_FLOAT, sizeof(glm::vec3), (whichArray == InArray) ? &NInArray[0] : &NOutArray[0]);
	glTexCoordPointer(2 , GL_FLOAT , sizeof(glm::vec2) , (whichArray == InArray) ? &VTInArray[0].x : &VTOutArray[0].x);

	// Here is the actual drawing is made.
	glDrawElements(GL_TRIANGLES , (xDensity - 1) * (yDensity - 1) * 2 * 3 , GL_UNSIGNED_INT , &TArray[0]);


	// For debugging purposes you might wish to draw the Vector3s at each Vector3.

	if (drawNormals)
	{
		float line_width;
		glGetFloatv(GL_LINE_WIDTH, &line_width);
		glLineWidth(1.0f);

		GLboolean lightingState = glIsEnabled(GL_LIGHTING);
		glDisable(GL_LIGHTING);

		GLboolean textureState = glIsEnabled(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);

		glm::vec3 * v = (whichArray == InArray) ? &VInArray[0] : &VOutArray[0];
		glm::vec3 * n = (whichArray == InArray) ? &NInArray[0] : &NOutArray[0];

		int maxDensity = (xDensity > yDensity) ? xDensity : yDensity;
		maxDensity = maxDensity / 4;

		glBegin(GL_LINES);
		for (int i = 0; i < xDensity * yDensity; i++)
		{
			glColor3d(1,1,1);

			if ((i % xDensity) == 1)
				glColor3d(1,0,0);
			else if ((i % xDensity) == xDensity - 1)
			{
				v++;
				n++;
				continue;
			}

			if (i < xDensity)
				glColor3d(0,1,0);

			glVertex3d(v->x + n->x / maxDensity, v->y + n->y / maxDensity, v->z + n->z / maxDensity);
			glVertex3d(v->x + n->x / 400.0, v->y + n->y / 400.0, v->z + n->z / 400.0); 
			
		//	glVertex3d(v->x + 2 * n->x, v->y + 2 * n->y, v->z + 2 * n->z);
		//	glVertex3d(4 * n->x, 4 * n->y, 4 * n->z);

			// By scaling the length of the Vector3s by the maximum axis density, the Vector3s
			// will get short when the mesh gets dense. This way, if you are doing displacement
			// mapping, the Vector3 vectors will tend to get in each other's way less.
			v++;
			n++;
		}
		glEnd();

		glLineWidth(line_width);

		if (lightingState)
			glEnable(GL_LIGHTING);

		if (textureState)
			glEnable(GL_TEXTURE_2D);
	}
}

// All these Get function assist in the creation of displacement
// map code. I acknowledge that returning pointers to the actual
// data breaks the object oriented model and permits destructive
// operations. However, they are liste as "protected".

int PlanarMesh::GetNumberOfElements()
{
	return (xDensity - 1) * (yDensity - 1) * 2 * 3;
}

void PlanarMesh::GetDensity(int & x, int & y)
{
	x = xDensity;
	y = yDensity;
}

glm::vec3 * PlanarMesh::GetInArray()
{
	return &VInArray[0];
}

glm::vec3 * PlanarMesh::GetOutArray()
{
	return &VOutArray[0];
}

glm::vec3 * PlanarMesh::GetInNormals()
{
	return &NInArray[0];
}

glm::vec3 * PlanarMesh::GetOutNormals()
{
	return &NOutArray[0];
}

glm::vec2 * PlanarMesh::GetInTextureCoordinates()
{
	return &VTInArray[0];
}

glm::vec2 * PlanarMesh::GetOutTextureCoordinates()
{
	return &VTOutArray[0];
}

glm::uvec3 * PlanarMesh::GetTriangleIndexArray()
{
	return &TArray[0];
}

void PlanarMesh::ApplyCustomization(float (* function)(float))
{
	float tr =  360.0f / ((float) this->xDensity - 1);

	glm::vec4 p;
	glm::vec4 q;

	int y_start = 0;
	int y_end = this->yDensity;

	for (int y = y_start; y < y_end; ++y)
	{
		glm::mat4 m(1.0f);
		glm::vec3 * outPointer = (this->GetOutArray() + this->xDensity * y);
		p = glm::vec4((*function)(((float) y - y_start) / ((float) y_end - 1)), ((float) y - y_start) / ((float) y_end - 1) - 0.5f, 0.0f, 0.0f);
		for (int x = 0; x < this->xDensity; ++x)
		{
			q = m * p;
			*(outPointer++) = glm::vec3(q.x, q.y, q.z);
			m = glm::rotate(m, tr, glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	// Make normals
	int left = -1;
	int right;
	int up = this->xDensity;
	int down = -this->xDensity;

	float top_radius = (*function)(0.0f);
	float bot_radius = (*function)(1.0f);

	for (int y = 0; y < this->yDensity; y++)
	{
		glm::vec3 * outVertexPointer = (this->GetOutArray() + this->xDensity * y + 1);
		glm::vec3 * outNormalPointer = (this->GetOutNormals() + this->xDensity * y + 1);
		glm::vec3 * firstNormalPointer = (this->GetOutNormals() + this->xDensity * y);

		for (int x = 1; x < this->xDensity; x++)
		{
			right = (x < this->xDensity - 1) ? 1 : -(this->xDensity - 2);

			glm::vec3 normal(0.0f);

			if (y == 0 && top_radius == 0.0f)
				normal = glm::vec3(0.0f, 1.0f, 0.0f);
			else if (y == this->yDensity - 1 && bot_radius == 0.0f)
				normal = glm::vec3(0.0f, -1.0f, 0.0f);
			else
			{
				// Row above
				if (y < this->yDensity - 1)
				{
					if (y > 0)
						normal += glm::cross(glm::normalize(*(outVertexPointer + left) - *outVertexPointer) , glm::normalize(*(outVertexPointer + left + up) - *outVertexPointer));
					normal += glm::cross(glm::normalize(*(outVertexPointer + left + up) - *outVertexPointer) , glm::normalize(*(outVertexPointer + up) - *outVertexPointer));
					normal += glm::cross(glm::normalize(*(outVertexPointer + up) - *outVertexPointer) , glm::normalize(*(outVertexPointer + right) - *outVertexPointer));
				}
				if (y > 0)
				{
					normal += glm::cross(glm::normalize(*(outVertexPointer + down) - *outVertexPointer) , glm::normalize(*(outVertexPointer + left) - *outVertexPointer));
					normal += glm::cross(glm::normalize(*(outVertexPointer + down + right) - *outVertexPointer) , glm::normalize(*(outVertexPointer + down) - *outVertexPointer));
					if (y < this->yDensity - 1)
						normal += glm::cross(glm::normalize(*(outVertexPointer + right) - *outVertexPointer) , glm::normalize(*(outVertexPointer + down + right) - *outVertexPointer));
				}
			}

			normal = glm::normalize(normal);

			if (x == this->xDensity - 1)
			{
				*firstNormalPointer = -normal;
			}

			*outNormalPointer = -normal;
			outNormalPointer++;
			outVertexPointer++;
		}
	}
}

