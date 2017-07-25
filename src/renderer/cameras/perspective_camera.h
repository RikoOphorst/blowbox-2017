#pragma once

#include "renderer/cameras/camera.h"

namespace blowbox
{
    /**
    * This class extends the base Camera class and offers functionality
    * for creating perspective cameras, which are useful for many things.
    *
    * @brief A camera with perspective projection, useful for 3D stuff.
    */
    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera();
        ~PerspectiveCamera();

        /**
        * @brief Sets the field of view for the camera in radians
        * @param[in] fov_in_radians The field of view in radians
        */
        void SetFovRadians(float fov_in_radians);

        /**
        * @brief Sets the field of view for the camera in degrees
        * @param[in] fov_in_degrees The field of view in degrees
        */
        void SetFovDegrees(float fov_in_degrees);

        /** @returns The field of view for the camera in radians. */
        float GetFovRadians() const;
        /** @returns The field of view for the camera in degrees. */
        float GetFovDegrees() const;

    protected:
        /** @brief (Re)-calculates the projection matrix of the camera. */
        void UpdateProjectionMatrix() override;

    protected:
        float fov_in_radians_;  //!< The field of view of the camera in radians.
    };
}