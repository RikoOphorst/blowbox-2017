#pragma once

#include "renderer/cameras/camera.h"

namespace blowbox
{
    /**
    * This class extends the base Camera class and offers functionality
    * for creating orthographic cameras, which are useful for many things.
    *
    * @brief A camera with orthographic projection, useful for 2D stuff.
    */
    class OrthographicCamera : public Camera
    {
    public:
        OrthographicCamera();
        ~OrthographicCamera();

        /**
        * @brief Sets the orthographic size of this camera.
        * @param[in] orthographic_size The orthographic size for the camera.
        * @remarks Orthographic size can be used to "zoom" in or out on the scene. Default is 1.
        */
        void SetOrthographicSize(float orthographic_size);

        /** @returns The orthographic size that is currently set on the scene. Default is 1. */
        float GetOrthographicSize() const;

    protected:
        /** @brief (Re)-calculates the projection matrix of the camera. */
        void UpdateProjectionMatrix() override;

    protected:
        float orthographic_size_; //!< The orthographic size that should be used with this camera.
    };
}