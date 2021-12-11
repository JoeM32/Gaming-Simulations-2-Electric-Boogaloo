#include "SphereVolume.h"
void NCL::SphereVolume::GetCollisionAxes(const CollisionVolume& other, std::vector<Vector3>& axes) const
{
    Vector3  direction = (other.getTransform().GetPosition() - transform.GetPosition()).Normalised();
    Vector3  p1 = transform.GetPosition ();
    Vector3  p2 = other.GetClosestPoint(p1);
    axes.push_back((p1 - p2). Normalised());
}

Vector3 NCL::SphereVolume::GetClosestPoint(Vector3 point) const
{
    Vector3  direction = (point - transform.GetPosition()).Normalised(); 
    return   transform.GetPosition() + (direction * GetRadius());
}

Vector3 NCL::SphereVolume::OBBSupport(const Vector3& axis) const
{
    return transform.GetPosition() + axis * GetRadius();
}
