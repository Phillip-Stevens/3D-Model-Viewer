//Created for Graphics I and II
//Author: Minsi Chen

#include "TriangleMesh.h"
#include "OBJFileReader.h"
#include "Windows.h"
#include "gl/GL.h"

TriangleMesh::TriangleMesh()
{
	m_vertices = NULL;
	m_triangles = NULL;
	m_numTriangles = 0;
	m_numVertices = 0;
}


TriangleMesh::~TriangleMesh()
{
	CleanUp();
}

void TriangleMesh::Render(const Matrix4x4* trans)
{
	//TODO: Basic Feature
	//1. Render the TriangleMesh using the vertices and triangle data stored in m_vertices and m_triangle
	//2. Transform the triangle mesh using the given transformation trans
	
	//TODO: Advanced Feature
	//Use the OpenGL fixed pipeline to render a lit mesh
	//Note: for per-face lighting you need to compute the normal for each triangle see ComputeTriangleNormals
	// for per-vertex lighting you need to compute the normal for each vertex see ComputeVertexNormals
	//glBegin(GL_LINES);
	//glColor3f(1, 0, 0);
	//for (int i = 0; i < m_numTriangles; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		Vector4D pos = m_vertices[m_triangles[i].GetVertexIndices()[j]].m_position;
	//		pos = (*trans)*pos;
	//		glVertex3f(pos[0], pos[1], pos[2]);
	//	}
	//}
	//glEnd();

	// THIS CODE RENDERS THE OBJECT IN SOLID FORM
	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 0);
	for (int i = 0; i < m_numTriangles; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Vector4D pos = m_vertices[m_triangles[i].GetVertexIndices()[j]].m_position;
			pos = (*trans)*pos;
			glVertex3f(pos[0], pos[1], pos[2]);
		}
	}
	glEnd();
}

void TriangleMesh::ComputeTriangleNormals()
{
	//TODO: Advanced feature
	//Compute the normal for each triangle stored in m_triangles
	for (int i = 0; i < m_numTriangles; i++)
	{
		Vector4D a = m_vertices[m_triangles[i].GetVertexIndices()[1]].m_position - m_vertices[m_triangles[i].GetVertexIndices()[0]].m_position;
		Vector4D b = m_vertices[m_triangles[i].GetVertexIndices()[2]].m_position - m_vertices[m_triangles[i].GetVertexIndices()[0]].m_position;
		Vector4D c = a.CrossProduct(b);
		c.Normalise();
		m_triangles[i].SetNormal(c);
	}
}

void TriangleMesh::ComputeVertexNormals()
{
	//TODO: Advanced feature
	//Compute the normal for each vertex stored in m_vertices

}

bool TriangleMesh::LoadMeshFromOBJFile(const char* filename)
{
	//First pass: reading the OBJ to gether info related to the mesh
	int numVertices = 0;
	int numNormals = 0;
	int numTexCoords = 0;
	int numTriangles = 0;

	if (OBJFileReader::firstPassOBJRead(filename, &numVertices, &numNormals, &numTexCoords, &numTriangles))
	{
		printf("Error parsing file: %s\n", filename);
		return false;
	}
	
	//allocate memory for m_vertices and m_triangles based on the number of vertices and triangles from the first pass
	if (m_vertices && m_triangles)
	{
		printf("Vertex array and triangle array have already been allocated.\n");
		return false;
	}

	m_vertices = new Vertex[numVertices];
	m_triangles = new Triangle[numTriangles];
	
	if (OBJFileReader::secondPassOBJRead(filename, m_vertices, m_triangles))
	{
		printf("Error parsing file: %s\n", filename);
		return false;
	}

	ComputeTriangleNormals();
	ComputeVertexNormals();

	m_numTriangles = numTriangles;
	m_numVertices = numVertices;
	
	return true;
}

void TriangleMesh::CleanUp()
{
	if (m_vertices)
		delete[] m_vertices;
	if (m_triangles)
		delete[] m_triangles;

	m_vertices = NULL;
	m_triangles = NULL;
	m_numTriangles = 0;
	m_numVertices = 0;
}