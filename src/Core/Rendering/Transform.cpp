#include "Transform.h"
#include <DirectXMath.h>

namespace Kraid
{

    Transform::Transform(const Transform& other)
    {
        this->translation[0] = other.translation[0];
        this->translation[1] = other.translation[1];
        this->translation[2] = other.translation[2];
        
        this->rotation[0] = other.rotation[0];
        this->rotation[1] = other.rotation[1];
        this->rotation[2] = other.rotation[2];
            
        this->scale[0] = other.scale[0];
        this->scale[1] = other.scale[1];
        this->scale[2] = other.scale[2];

        this->has_changed = true;
    }

    Transform::Transform(
            const float translation[3],
            const float scale[3],
            const float rotation[3])
    {
        this->translation[0] = translation[0];
        this->translation[1] = translation[1];
        this->translation[2] = translation[2];
            
        this->rotation[0] = rotation[0];
        this->rotation[1] = rotation[1];
        this->rotation[2] = rotation[2];
            
        this->scale[0] = scale[0];
        this->scale[1] = scale[1];
        this->scale[2] = scale[2];

        this->has_changed = true;
    }

    XMMATRIX Transform::GetModelMatrix() const
    {
        XMMATRIX ret = XMMatrixIdentity();

        FXMVECTOR x_axis = {1,0,0};
        FXMVECTOR y_axis = {0,1,0};
        FXMVECTOR z_axis = {0,0,1};
        ret *= XMMatrixRotationAxis(x_axis, this->rotation[0]); 
        ret *= XMMatrixRotationAxis(y_axis, this->rotation[1]); 
        ret *= XMMatrixRotationAxis(z_axis, this->rotation[2]);

        ret *= XMMatrixScaling(this->scale[0], this->scale[1], this->scale[2]);

        ret *= XMMatrixTranslation(this->translation[0], this->translation[1], this->translation[2]);

        return ret;
    }

    Transform Transform::operator+(const Transform& other)
    {
        Transform ret = *this;

        ret.translation[0] += other.translation[0];
        ret.translation[1] += other.translation[1];
        ret.translation[2] += other.translation[2];

        ret.scale[0] *= other.scale[0];
        ret.scale[1] *= other.scale[1];
        ret.scale[2] *= other.scale[2];

        ret.rotation[0] += other.rotation[0];
        ret.rotation[1] += other.rotation[1];
        ret.rotation[2] += other.rotation[2];

        return ret;
    }

    Transform& Transform::operator=(const Transform &other)
    {
        this->translation[0] = other.translation[0];
        this->translation[1] = other.translation[1];
        this->translation[2] = other.translation[2];
        
        this->rotation[0] = other.rotation[0];
        this->rotation[1] = other.rotation[1];
        this->rotation[2] = other.rotation[2];
            
        this->scale[0] = other.scale[0];
        this->scale[1] = other.scale[1];
        this->scale[2] = other.scale[2];

        this->has_changed = true;

        return *this;
    }

    void Transform::SetTranslation(const float x, const float y, const float z)
    {
        this->translation[0] = x;
        this->translation[1] = y;
        this->translation[2] = z;

        this->has_changed = true;
    }

    void Transform::TranslateBy(const float x, const float y, const float z)
    {
        this->translation[0] += x;
        this->translation[1] += y;
        this->translation[2] += z;

        this->has_changed = true;
    }

}
