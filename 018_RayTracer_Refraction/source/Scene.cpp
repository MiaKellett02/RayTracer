#include "Scene.h"

Scene::Scene()
{
	m_objects.clear();
	m_lights.clear();
}

Scene::~Scene()
{
	m_pCamera = nullptr;
	m_objects.clear();
	m_lights.clear();
}

void Scene::AddObject(const Primitive* a_object)
{
	m_objects.push_back(a_object);
}

void Scene::RemoveObject(const Primitive* a_object)
{
	for (auto iter = m_objects.begin(); iter != m_objects.end(); iter++)
	{
		if (*iter == a_object) //We have located the object..
		{
			iter = m_objects.erase(iter); //Delete the object from the vector.
		}
	}
}

void Scene::AddLight(const Light* a_light)
{
	m_lights.push_back(a_light);
}

void Scene::RemoveLight(const Light* a_light)
{
	for (auto iter = m_lights.begin(); iter != m_lights.end(); iter++)
	{
		if (*iter == a_light) //We have located the light.
		{
			iter = m_lights.erase(iter);
		}
	}
}

Ray Scene::GetScreenRay(const Vector2& a_screenSpacePos) const
{
	//Create a Ray with origin at the camera and direction into the near plane offset.
	return m_pCamera->CastRay(a_screenSpacePos);
}

Vector3 Scene::CastRay(const const Ray& a_ray, int a_bounces) const
{
	if (a_bounces <= 0)//number of bounces remaining for ray (prevents calling function recursively forever).
	{
		return ColourRGB(0.0f, 0.0f, 0.0f);
	}

	//Convert ray direction into colour space 0 => 1.
	IntersectResponse ir;
	if (IntersectTest(a_ray, ir)) //if true an intersection has occured.
	{
		//Calculate lighting.
		Vector3 rayColour = Vector3(0.0f, 0.0f, 0.0f);
		//For all lights in the scene sum the effects the lights have on the object.
		for (auto lightIter = m_lights.begin(); lightIter != m_lights.end(); lightIter++)
		{
			//Test to see if in shadow -- cast ray from intersection toward light.
			Ray shadowRay = Ray(ir.HitPos, -(*lightIter)->GetDirectionToLight(ir.HitPos), 0.001f);
			IntersectResponse sr;
			float shadowValue = (!IntersectTest(shadowRay, sr)); //Call hittest on the shadow ray to see if we have an occuluder.
			rayColour += (*lightIter)->CalculateLighting(ir, m_pCamera->GetPosition(), shadowValue);
			//If the material that we have hit is transparent and refractive we need to calculate the refraction vector.
			//and create a new ray to project into the scene.
			Ray refractRay;
			ColourRGB refractionColour = ColourRGB(0.0f, 0.0f, 0.0f);
			if (ir.material->CalculateRefraction(a_ray, ir, refractRay))
			{
				refractionColour = CastRay(refractRay, a_bounces - 1) * ir.material->GetTransparency();
			}
			//If the material that we have hit is reflective we need to calculate the reflection vector
			//and create a new ray to project into the scene.
			ColourRGB reflectionColour = ColourRGB(0.0f, 0.0f, 0.0f);
			Ray bounceRay;
			if (ir.material->CalculateReflection(a_ray, ir, bounceRay))
			{
				//Call intersect test function to accumulate colour of pixel with bounce ray.
				reflectionColour = CastRay(bounceRay, a_bounces - 1) * ir.material->GetReflective();
			}
			//If the material is reflective and transparent we need to calculate the ration of each reflective and refractive colours.
			//Use schlicks approximation to calculate fresnel term for object.
			if (ir.material->GetReflective() > 0.0f && ir.material->GetTransparency() > 0.0f)
			{
				float reflectance = ir.material->Schlick(a_ray, ir);
				rayColour += reflectionColour * reflectance + refractionColour * (1.0f - reflectance);
			}
			else
			{
				rayColour += reflectionColour + refractionColour;
			}
		}
		return rayColour;
	}
	else
	{
		Vector3 rayToColour = RayToColour(a_ray);
		//Use lerp to get a colour between white and blue based on the vertical value of the rayColour.
		rayToColour = Lerp(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.4f, 0.7f, 1.0f), rayToColour.y);
		return rayToColour;
	}
}

bool Scene::IntersectTest(const Ray& a_ray, IntersectResponse& a_ir) const
{
	//Set the current hit distance to be very far away.
	float intersectDist = a_ray.MaxDistance(); //FLT_MAX is the maximum positive value a float can store.
	bool intersectionOccured = false;
	IntersectResponse objectIntersection;
	//For each object in the world test to see if the ray intersects the object.
	for (auto iter = m_objects.begin(); iter != m_objects.end(); iter++)
	{
		const Primitive* object = (*iter); //Get a pointer to the primitive object.
		if (object->IntersectTest(a_ray, objectIntersection)) //perform intersection test on each object.
		{
			//intersection occured.
			if (objectIntersection.distance > a_ray.MinLength())
			{
				intersectionOccured = true;
				if (objectIntersection.distance < intersectDist) //Is the intersection closer than the previous intersection.
				{
					intersectDist = objectIntersection.distance; //Store the new distance to the intersection.
					a_ir = objectIntersection;
				}
			}
		}
	}
	return intersectionOccured;
}