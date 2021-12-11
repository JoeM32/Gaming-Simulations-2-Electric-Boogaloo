#pragma once
#include "CollisionVolume.h"
#include "Transform.h"
namespace NCL {
    class CapsuleVolume : public CollisionVolume
    {
    public:
        CapsuleVolume(float halfHeight, float radius, const NCL::CSC8503::Transform& t) : CollisionVolume(t) {
            this->halfHeight    = halfHeight;
            this->radius        = radius;
            this->type          = VolumeType::Capsule;
        };
        ~CapsuleVolume() {

        }
        float GetRadius() const {
            return radius;
        }

        float GetHalfHeight() const {
            return halfHeight;
        }

    protected:
        float radius;
        float halfHeight;
    };
}

