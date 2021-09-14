#ifndef _ELLIPSOID_H_
#define _ELLIPSOID_H_
#include "Primitive.h"

class Ellipsoid : public Primitive
{
public:
	Ellipsoid();
	Ellipsoid(const Vector3& a_pos, const float& a_radius);
	virtual ~Ellipsoid();

	bool IntersectTest(const Ray& a_ray, IntersectResponse& a_ir) const;
	Vector3 m_colour;

private:
	float m_radius;
};

#endif // !_ELLIPSOID_H_