#ifndef _SCENE_H_
#define _SCENE_H_
#include <vector>
#include <LibMath.h>
#include "IntersectResponse.h"
#include "Primitive.h"
#include "Camera.h"
#include "Light.h"

class Scene
{
public:
	Scene();
	~Scene();

	void AddObject(const Primitive* a_object);
	void RemoveObject(const Primitive* a_object);

	void AddLight(const Light* a_light);
	void RemoveLight(const Light* a_light);

	Ray GetScreenRay(const Vector2& a_screenSpacePos) const;
	Vector3 CastRay(const const Ray& a_ray, int a_bounces) const;
	bool  IntersectTest(const Ray& a_ray, IntersectResponse& a_ir) const;

	void SetCamera(Camera* a_pCamera)
	{
		m_pCamera = a_pCamera;
	}

private:
	std::vector<const Primitive*> m_objects;
	std::vector<const Light*> m_lights;
	Camera* m_pCamera;
};

#endif // !_SCENE_H_
