#include "AABBVolume.h"

void NCL::AABBVolume::GetCollisionAxes(const CollisionVolume& other, std::vector<Vector3>& axes) const
{
	axes.push_back(Vector3(1.0f, 0.0f, 0.0f));
	axes.push_back(Vector3(0.0f, 1.0f, 0.0f));
	axes.push_back(Vector3(0.0f, 0.0f, 1.0f));
}

Vector3 NCL::AABBVolume::GetClosestPoint(Vector3 point) const
{
	return Vector3(Maths::Clamp(point.x, transform.GetPosition().x - GetHalfDimensions().x, transform.GetPosition().x + GetHalfDimensions().x),
		Maths::Clamp(point.y, transform.GetPosition().y - GetHalfDimensions().y, transform.GetPosition().y + GetHalfDimensions().y),
		Maths::Clamp(point.z, transform.GetPosition().z - GetHalfDimensions().z, transform.GetPosition().z + GetHalfDimensions().z));
}

Vector3 NCL::AABBVolume::OBBSupport(const Vector3& axis) const//treated like an obb
{
	Vector3 localDir = axis;
	Vector3 vertex;
	vertex.x = localDir.x < 0 ? -GetHalfDimensions().x : GetHalfDimensions().x;
	vertex.y = localDir.y < 0 ? -GetHalfDimensions().y : GetHalfDimensions().y;
	vertex.z = localDir.z < 0 ? -GetHalfDimensions().z : GetHalfDimensions().z;
	return vertex + transform.GetPosition();
}
