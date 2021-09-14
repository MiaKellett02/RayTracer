#include "Primitive.h"

Primitive::Primitive() : m_Transform(Matrix4::IDENTITY), m_Scale()
{

}

Primitive::~Primitive()
{

}

//Get and set the primitive matrix.
Matrix4 Primitive::GetTransform() const
{
	return m_Transform;
}

void Primitive::SetTransform(const Matrix4& a_m4)
{
	m_Transform = a_m4;
}

//Get and set the position of the primitive.
Vector3 Primitive::GetPosition() const
{
	Vector4 positionColumn = m_Transform.GetColumn(3);
	return Vector3(positionColumn.x, positionColumn.y, positionColumn.z);
}

void Primitive::SetPosition(const Vector3& a_v3)
{
	m_Transform.SetColumn(3, a_v3);
}

//Get and Set the scale of the primitive.
Vector3 Primitive::GetScale() const
{
	return m_Scale;
}

void Primitive::SetScale(const Vector3& a_v3)
{
	m_Scale = a_v3;
	Matrix4 scale;
	scale.Scale(Vector4(a_v3.x, a_v3.y, a_v3.z, 1));
	m_Transform = m_Transform * scale;
}

void Primitive::SetShear(float xy, float xz, float yx, float yz, float zx, float zy)
{
	m_Transform.Shear(xy, xz, yx, yz, zx, zy);
}

void Primitive::SetMaterial(Material* a_material)
{
	m_material = a_material;
}