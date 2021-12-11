#include "OBBVolume.h"

void NCL::OBBVolume::GetCollisionAxes(const CollisionVolume& other, std::vector<Vector3>& axes) const
{
	axes.push_back(transform.GetOrientation() * Vector3(1.0f, 0.0f, 0.0f));
	axes.push_back(transform.GetOrientation() * Vector3(0.0f, 1.0f, 0.0f));
	axes.push_back(transform.GetOrientation() * Vector3(0.0f, 0.0f, 1.0f));
}

Vector3 NCL::OBBVolume::GetClosestPoint(Vector3 point) const
{
	Matrix4 inverse = (transform.GetMatrix().Inverse());
	Vector3 pointLocal = inverse * point;

	return Vector3(Maths::Clamp(pointLocal.x, transform.GetPosition().x - GetHalfDimensions().x, transform.GetPosition().x + GetHalfDimensions().x),
		Maths::Clamp(pointLocal.y, transform.GetPosition().y - GetHalfDimensions().y, transform.GetPosition().y + GetHalfDimensions().y),
		Maths::Clamp(pointLocal.z, transform.GetPosition().z - GetHalfDimensions().z, transform.GetPosition().z + GetHalfDimensions().z));
}

Vector3 NCL::OBBVolume::OBBSupport(const Vector3& axis) const
{
	Vector3 localDir = transform.GetOrientation().Conjugate() * axis;
	Vector3 vertex;
	vertex.x = localDir.x < 0 ? -0.5f : 0.5f;
	vertex.y = localDir.y < 0 ? -0.5f : 0.5f;
	vertex.z = localDir.z < 0 ? -0.5f : 0.5f;
	/*
	vertex.x = localDir.x < 0 ? -GetHalfDimensions().x : GetHalfDimensions().x;
	vertex.y = localDir.y < 0 ? -GetHalfDimensions().y : GetHalfDimensions().y;
	vertex.z = localDir.z < 0 ? -GetHalfDimensions().z : GetHalfDimensions().z;
	*/
	return transform.GetMatrix() * vertex;
}
