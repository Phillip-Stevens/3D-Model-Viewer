//Created for Graphics I and II
//Author: Minsi Chen

#pragma once
#include "Renderable.h"
#include "Triangle.h"
#include "Vector4D.h"
#include <vector>
#include "Matrix4x4.h"

//A minimalistic Vertex structure
struct Vertex
{
	Vector4D m_position;	//the position of the vertex
	Vector4D m_normal;		//the vertex normal 
};

class TriangleMesh : public Renderable
{
	private:
		Vertex*			m_vertices;			//a list of vertices
		Triangle*		m_triangles;		//a list of triangles
		int				m_numVertices;		//the number of vertices in the mesh
		int				m_numTriangles;		//the number of triangles in the mesh
		
		//TODO: Advanced Features for lighting
		//Calculate the normal for each triangle
		//Calculate the normal for each vertex
		void ComputeTriangleNormals();
		void ComputeVertexNormals();

	public:
		TriangleMesh();
		~TriangleMesh();

		void Render(const Matrix4x4* trans = 0);

		bool LoadMeshFromOBJFile(const char* filename);

		void CleanUp();
};

