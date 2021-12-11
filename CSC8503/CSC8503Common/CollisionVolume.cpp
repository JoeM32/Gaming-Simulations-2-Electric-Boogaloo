#include "CollisionVolume.h"

using namespace NCL;

void NCL::CollisionVolume::GetCollisionAxes(const CollisionVolume& other, std::vector<Vector3>& axes) const
{
}

Vector3 NCL::CollisionVolume::GetClosestPoint(Vector3 point) const
{
	return Vector3();
}

Vector3 NCL::CollisionVolume::OBBSupport(const Vector3& axis) const
{
	return Vector3();
}
