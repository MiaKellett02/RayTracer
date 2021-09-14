#ifndef _INTERSECTRESPONSE_H_
#define _INTERSECTRESPONSE_H_
#include <LibMath.h>
class Material;

struct IntersectResponse
{
	Vector3 HitPos; //The location in worldspace of the intersection.
	Vector3 SurfaceNormal; //The surface normal at the intersection location.
	bool frontFace;//Is this the front face or internal face of the object (in case of refraction)
	float distance; //the distance to the hit location.
	Material* material;//The material property of the intersected object.
};

#endif // !_INTERSECTRESPONSE_H_
