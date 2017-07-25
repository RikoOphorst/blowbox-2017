#include "perspective_camera.h"

#include "core/get.h"
#include "win32/window.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    PerspectiveCamera::PerspectiveCamera() :
        fov_in_radians_(DirectX::XM_PIDIV2)
    {

    }

    //------------------------------------------------------------------------------------------------------
    PerspectiveCamera::~PerspectiveCamera()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void PerspectiveCamera::SetFovRadians(float fov_in_radians)
    {
        projection_dirty_ = true;
        fov_in_radians_ = fov_in_radians;
    }

    //------------------------------------------------------------------------------------------------------
    void PerspectiveCamera::SetFovDegrees(float fov_in_degrees)
    {
        projection_dirty_ = true;
        fov_in_radians_ = DirectX::XMConvertToRadians(fov_in_degrees);
    }

    //------------------------------------------------------------------------------------------------------
    float PerspectiveCamera::GetFovRadians() const
    {
        return fov_in_radians_;
    }

    //------------------------------------------------------------------------------------------------------
    float PerspectiveCamera::GetFovDegrees() const
    {
        return DirectX::XMConvertToDegrees(fov_in_radians_);
    }

    //------------------------------------------------------------------------------------------------------
    void PerspectiveCamera::UpdateProjectionMatrix()
    {
        projection_ = DirectX::XMMatrixPerspectiveFovLH(
            fov_in_radians_, 
            Get::MainWindow()->GetWindowResolution().GetAspectRatio(), 
            near_plane_, 
            far_plane_
        );
    }
}