#pragma once
#include "Transform.h"
#include "GameObject.h"
#include "../../Common/Maths.h"
#include <vector>

namespace NCL {
	enum class VolumeType {
		AABB	= 1,
		OBB		= 2,
		Sphere	= 4, 
		Mesh	= 8,
		Capsule = 16,
		Compound= 32,
		Invalid = 256
	};

	class CollisionVolume
	{
	public:
		CollisionVolume(const NCL::CSC8503::Transform& t) : transform(t) {
			type = VolumeType::Invalid;
		}
		~CollisionVolume() {}

		VolumeType type;

		virtual void GetCollisionAxes(const CollisionVolume& other, std::vector <Vector3 >& axes) const;

		virtual Vector3  GetClosestPoint(Vector3 point) const;//gets closest vertex to a point
		
		virtual Vector3  OBBSupport(const  Vector3& axis) const; //gets closest vertex along an axis

		NCL::CSC8503::Transform getTransform() const { return transform; } ;

		std::string GetTypename()
		{
			switch (type)
			{
			case VolumeType::AABB:
				return "AABB";
				break;
			case VolumeType::OBB:
				return "OBB";
				break;
			case VolumeType::Sphere:
				return "Sphere";
				break;
			case VolumeType::Mesh:
				return "Mesh";
				break;
			case VolumeType::Compound:
				return "Compound";
				break;
			case VolumeType::Invalid:
				return "Invalid";
				break;
			case VolumeType::Capsule:
				return "Capsule";
				break;
			default:
				return "Unknown";
			}
		}


	protected:
		const NCL::CSC8503::Transform& transform;
	};
}