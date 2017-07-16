#pragma once

#include <DirectXMath.h>

namespace DirectX
{
    DirectX::XMFLOAT3 XMQuaternionToEuler(DirectX::XMVECTOR quaternion_vec)
    {
        DirectX::XMFLOAT3 output;
        float sqw;
        float sqx;
        float sqy;
        float sqz;

        DirectX::XMFLOAT4 quaternion;
        DirectX::XMStoreFloat4(&quaternion, quaternion_vec);

        sqw = quaternion.w * quaternion.w;
        sqx = quaternion.x * quaternion.x;
        sqy = quaternion.y * quaternion.y;
        sqz = quaternion.z * quaternion.z;

        float unit = sqw + sqx + sqy + sqz;
        float test = quaternion.x * quaternion.w - quaternion.y * quaternion.z;

        if (test > 0.4995f * unit)
        {
            output.y = (2.0f * atan2(quaternion.y, quaternion.x));
            output.x = (DirectX::XM_PIDIV2);

            return output;
        }
        if (test < -0.4995f * unit)
        {
            output.y = (-2.0f * atan2(quaternion.y, quaternion.x));
            output.x = (-DirectX::XM_PIDIV2);

            return output;
        }

        output.x = (asin(2.0f * (quaternion.w * quaternion.x - quaternion.z * quaternion.y)));
        output.y = (atan2(2.0f * quaternion.w * quaternion.y + 2.0f * quaternion.z * quaternion.x, 1 - 2.0f * (sqx + sqy)));
        output.z = (atan2(2.0f * quaternion.w * quaternion.z + 2.0f * quaternion.x * quaternion.y, 1 - 2.0f * (sqz + sqx)));

        return output;
    }
}