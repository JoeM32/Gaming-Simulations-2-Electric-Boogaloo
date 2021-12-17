#pragma once
#include "CollisionVolume.h"
#include "../../Common/Vector3.h"
namespace NCL {
	class OBBVolume : public  CollisionVolume
	{
	public:
		OBBVolume(const NCL::CSC8503::Transform& t) : CollisionVolume(t) {
			type		= VolumeType::OBB;
		}
		~OBBVolume() {}

		Maths::Vector3 GetHalfDimensions() const {
			return transform.GetScale()/2.0f;
		}

		void GetCollisionAxes(const CollisionVolume& other, std::vector <Vector3 >& axes) const override;

		virtual Vector3  GetClosestPoint(Vector3 point) const override;//gets closest vertex to a point

		virtual Vector3  OBBSupport(const  Vector3& axis) const override; //gets closest vertex along an axis


		
	};
}

