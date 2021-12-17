#pragma once
#include "CollisionVolume.h"
#include "Transform.h"

namespace NCL {
	class SphereVolume : public CollisionVolume
	{
	public:
		SphereVolume(const NCL::CSC8503::Transform& t) : CollisionVolume(t) {
			type	= VolumeType::Sphere;
		}
		~SphereVolume() {}

		float GetRadius() const {
			return (transform.GetScale().x + transform.GetScale().y + transform.GetScale().z) / 3.0f;
		}

		void GetCollisionAxes(const CollisionVolume& other, std::vector <Vector3 >& axes) const override;

		virtual Vector3  GetClosestPoint(Vector3 point) const override;//gets closest vertex to a point

		virtual Vector3  OBBSupport(const  Vector3& axis) const override; //gets closest vertex along an axis


	};
}

