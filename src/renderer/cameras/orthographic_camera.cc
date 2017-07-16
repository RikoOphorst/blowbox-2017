#include "orthographic_camera.h"

#include "core/get.h"
#include "win32/window.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    OrthographicCamera::OrthographicCamera() :
        orthographic_size_(1.0f)
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    OrthographicCamera::~OrthographicCamera()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void OrthographicCamera::SetOrthographicSize(float orthographic_size)
    {
        view_dirty_ = true;
        orthographic_size_ = eastl::clamp(orthographic_size, 0.01f, 100.0f);
    }

    //------------------------------------------------------------------------------------------------------
    float OrthographicCamera::GetOrthographicSize() const
    {
        return orthographic_size_;
    }
    
    //------------------------------------------------------------------------------------------------------
    void OrthographicCamera::UpdateProjectionMatrix()
    {
        projection_ = DirectX::XMMatrixOrthographicLH(
            Get::MainWindow()->GetWindowResolution().width * orthographic_size_,
            Get::MainWindow()->GetWindowResolution().height * orthographic_size_,
            near_plane_,
            far_plane_
        );
    }
}