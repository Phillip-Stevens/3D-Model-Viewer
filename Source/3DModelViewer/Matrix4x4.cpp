#include <memory.h>
#include "Vector4D.h"
#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	SetIdentity();
}

Matrix4x4::Matrix4x4(const Matrix4x4& rhs)
{
	size_t matrix_size = sizeof(m_element);

	memcpy_s((&m_element[0][0]), matrix_size, rhs.ToPtr(), matrix_size);
	ROWS = 4;
	COLUMNS = 4;
}

Matrix4x4::~Matrix4x4()
{

}

const float* Matrix4x4::ToPtr() const
{
	return &(m_element[0][0]);
}

void Matrix4x4::SetZero()
{
	memset(&m_element, 0, sizeof(m_element));
}

void Matrix4x4::SetIdentity()
{
	SetZero();
	m_element[0][0] = m_element[1][1] = m_element[2][2] = m_element[3][3] = 1.0f;
}

Matrix4x4 Matrix4x4::operator* (const Matrix4x4& rhs) const
{
	//TODO: Implement Matrix-Matrix Multiplcation
	Matrix4x4 result;

	// For loop version that didn't work properly
	// NEED TO FIX

	//for (int i = 0; i < ROWS; i++)
	//{
	//	float resultVal = 0;
	//	for (int j = 0; j < COLUMNS; j++)
	//	{
	//		for (int k = 0; k < ROWS; k++)
	//		{
	//			resultVal += m_element[i][k] * rhs.m_element[k][j];
	//		}	
	//		result.m_element[i][j] = resultVal;
	//	}
	//}

	// For loop over each of the elements and work them out!
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m_element[i][j] = m_element[i][0] * rhs.m_element[0][j] + m_element[i][1] * rhs.m_element[1][j] + m_element[i][2] * rhs.m_element[2][j] + m_element[i][3] * rhs.m_element[3][j];
		}
	}
	return result;
}

Vector4D Matrix4x4::operator* (const Vector4D& rhs) const
{
	Vector4D result;

	result = rhs;
	
	//TODO: Implement Matrix-Vector Multiplcation
	result[0] = m_element[0][0] * rhs[0] + m_element[0][1] * rhs[1] + m_element[0][2] * rhs[2] + m_element[0][3] * rhs[3];
	result[1] = m_element[1][0] * rhs[0] + m_element[1][1] * rhs[1] + m_element[1][2] * rhs[2] + m_element[1][3] * rhs[3];
	result[2] = m_element[2][0] * rhs[0] + m_element[2][1] * rhs[1] + m_element[2][2] * rhs[2] + m_element[2][3] * rhs[3];
	result[3] = m_element[3][0] * rhs[0] + m_element[3][1] * rhs[1] + m_element[3][2] * rhs[2] + m_element[3][3] * rhs[3];

	return result;
}

void Matrix4x4::SetRotationXAxis(float degrees)
{
	// Set the Radian to pi * the degrees passed in / 180
	float pi = 3.14;
	float radian = pi*degrees / 180;

	//TODO: Set this matrix as a rotation matrix representing a rotation about the X axis by degrees
	SetIdentity();
	m_element[1][1] = cos(radian);
	m_element[2][1] = sin(radian);
	m_element[1][2] = -sin(radian);
	m_element[2][2] = cos(radian);
	
}

void Matrix4x4::SetRotationYAxis(float degrees)
{
	// Set the Radian to pi * the degrees passed in / 180
	float pi = 3.14;
	float radian = pi*degrees / 180;

	//TODO: Set this matrix as a rotation matrix representing a rotation about the Y axis by degrees
	SetIdentity();
	m_element[0][0] = cos(radian);
	m_element[2][0] = -sin(radian);
	m_element[0][2] = sin(radian);
	m_element[2][2] = cos(radian);
}

void Matrix4x4::SetRotationZAxis(float degrees)
{
	// Set the Radian to pi * the degrees passed in / 180
	float pi = 3.14;
	float radian = pi*degrees / 180;

	//TODO: Set this matrix as a rotation matrix representing a rotation about the Z axis by degrees
	SetIdentity();
	m_element[0][0] = cos(radian);
	m_element[0][1] = -sin(radian);
	m_element[1][0] = sin(radian);
	m_element[1][1] = cos(radian);
}

void Matrix4x4::SetTranslate(const Vector4D& tvec)
{
	//TODO: Set this matrix as a translation matrix given the translation vector tvec

	m_element[0][3] = tvec[0];
	m_element[1][3] = tvec[1];
	m_element[2][3] = tvec[2];
	
}

void Matrix4x4::SetScale(float sx, float sy, float sz)
{
	//TODO: Set this matrix as a scaling matrix given the scaling factors sx, sy, sz
	SetIdentity();
	m_element[0][0] = sx;
	m_element[1][1] = sy;
	m_element[2][2] = sz;
}

void Matrix4x4::SetViewMatrix(const Vector4D& camera_position, const Vector4D& view_vector, const Vector4D& up_vector)
{
	//TODO: Slightly more advanced
	//Set this matrix as a view matrix based on the given camera_position, view_vector and up_vector

	Vector4D vect(view_vector.CrossProduct(up_vector));
	vect.Normalise();

	SetIdentity();
	m_element[0][0] = vect[0];
	m_element[0][1] = vect[1];
	m_element[0][2] = vect[2];
	m_element[0][3] = camera_position[0];
	m_element[1][0] = up_vector[0];
	m_element[1][1] = up_vector[1];
	m_element[1][2] = up_vector[2];
	m_element[1][3] = camera_position[1];
	m_element[2][0] = view_vector[0];
	m_element[2][1] = view_vector[1];
	m_element[2][2] = view_vector[2];
	m_element[2][3] = camera_position[2];

}
