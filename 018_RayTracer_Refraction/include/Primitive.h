#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_
#include <LibMath.h>
#include "IntersectResponse.h"
#include "Material.h"

class Primitive
{
public:
	Primitive();
	virtual ~Primitive();
	//Function to test for intersection with primitive and ray - pure virtual only implemented in derived classes.
	virtual bool IntersectTest(const Ray& a_ray, IntersectResponse& a_ir) const = 0;
	//Get and set primitive matrix.
	Matrix4 GetTransform() const;
	void SetTransform(const Matrix4& a_m4);
	//Get and Set the position of the primitive.
	Vector3 GetPosition() const;
	void SetPosition(const Vector3& a_v3);
	//Get and Set the scale of the primitive.
	Vector3 GetScale() const;
	void SetScale(const Vector3& a_v3);
	//Set the shear of the primitive.
	void SetShear(float xy, float xz, float yx, float yz, float zx, float zy);

	//Set and Get the material for this primitive.
	void SetMaterial(Material* a_material);
	const Material* GetMaterial() { return m_material; }
protected:
	Matrix4 m_Transform; //Position Scale and Rotation.
	Vector3 m_Scale; //Scale Vector.
	Material* m_material;//Surface material for primitive.
};

#endif // !_PRIMITIVE_H_
