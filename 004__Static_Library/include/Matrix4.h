#ifndef _MATRIX4_H_
#define _MATRIX4_H_
//================================================
//INCLUDES.
//================================================
#include "Vector4.h"
#include "Vector3.h"

//================================================
//Matrix4 Class.
// Class that implements a 4x4 homogeneous matrix.
// Column-major Order.
//================================================
class Matrix4
{
private:
	//=========================================================
	//Member variables held in unnamed union for accessibility.
	// Items in a union share the same memory.
	// E.G. m[0][0] == m_11 == m_xAxis.x .
	//      m[2][2] == m_33 == m_zAxis.z .
	//=========================================================
	union
	{
		float m[4][4];
		struct
		{
			float m_11, m_21, m_31, m_41;
			float m_12, m_22, m_32, m_42;
			float m_13, m_23, m_33, m_43;
			float m_14, m_24, m_34, m_44;
		};
		struct
		{
			Vector4 m_xAxis;
			Vector4 m_yAxis;
			Vector4 m_zAxis;
			Vector4 m_wAxis;
		};
	};

public:
	//=========
	//Constants
	//=========
	static const Matrix4 IDENTITY;
	//============
	//Constructors
	//============
	Matrix4();
	Matrix4(const float* a_mat);
	Matrix4(float a_m11, float a_m21, float a_m31, float a_m41,
		    float a_m12, float a_m22, float a_m32, float a_m42,
		    float a_m13, float a_m23, float a_m33, float a_m43,
		    float a_m14, float a_m24, float a_m34, float a_m44);
	Matrix4(const Vector4& a_xAxis, const Vector4& a_yAxis, const Vector4& a_zAxis, const Vector4& a_wAxis);
	Matrix4(const Matrix4& a_m4);
	//==========
	//Destructor
	//==========
	~Matrix4();
	//==========================
	//Component Access Operators
	//==========================
	float& operator() (int a_iColumn, int a_iRow);
	float  operator() (int a_iColumn, int a_iRow) const;
	//=====================
	//Column and Row Access
	//=====================
	void    SetColumn(int a_iCol, const Vector4& a_vCol);
	void    SetColumn(int a_iCol, const Vector3& a_vCol);
	Vector4 GetColumn(int a_iCol) const;
	Vector3 GetColumnV3(int a_iCol) const;
	//=====================
	//Equivalence Operators
	//=====================
	bool operator == (const Matrix4& a_m3) const;
	bool operator != (const Matrix4& a_m3) const;
	//================================
	//Overload operators for addition.
	//================================
	Matrix4 operator + (const Matrix4& a_m3) const;
	const Matrix4& operator += (const Matrix4& a_m3);
	//====================================
	//Overload operators for subtractions.
	//====================================
	Matrix4 operator - (const Matrix4& a_m3) const;
	const Matrix4& operator -= (const Matrix4& a_m3);
	//======================================
	//Overload operators for multiplication.
	//======================================
	Matrix4 operator * (const float a_scalar) const;
	const Matrix4& operator *= (const float a_scalar);

	Vector4 operator * (const Vector4& a_v4) const;

	Matrix4 operator * (const Matrix4& a_m3) const;
	const Matrix4& operator *= (const Matrix4& a_m3);
	//=========
	//Transpose
	//=========
	void Transpose();
	Matrix4 GetTranspose() const;
	//=====
	//Scale
	//=====
	void Scale(const float a_scalar);
	void Scale(const Vector4& a_v4);
	//========
	//Identity
	//========
	void Identity();
	//=======
	//Inverse
	//=======
	float Determinant()const;
	Matrix4 Inverse()const;
	//============================
	//Camera Projection Functions.
	//============================
	bool Perspective(float fRadFov, float fApsectRatio, float fZNear, float fZFar);
	bool Orthographic(float fLeft, float fRight, float fTop, float fBottom, float fNear, float fFar);
	Matrix4 LookAt(const Vector3& a_v3EyePos, const Vector3& a_v3Target, const Vector3& a_v3Up);
	//=============
	//Shear Matrix.
	//=============
	void Shear(float xy, float xz, float yx, float yz, float zx, float zy);
	//===============
	//OrthoNormalise.
	//===============
	void Orthonormalise();
};
#endif // !_MATRIX4_H_
