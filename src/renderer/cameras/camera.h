#pragma once

#include <DirectXMath.h>

namespace blowbox
{
    /**
    * This is the base class for all cameras in Blowbox. It exposes the base
    * functionality that every camera should have. Pretty straightforward.
    *
    * @brief A camera through which the scene is rendered.
    */
    class Camera
    {
    public:
        Camera();
        virtual ~Camera();

        /**
        * @brief Translates the camera in a given direction relative to its current orientation.
        * @param[in] translation How much the camera should be translated.
        */
        void Translate(const DirectX::XMFLOAT3& translation);

        /**
        * @brief Rotates the camera by a certain rotation relative to its current rotation.
        * @param[in] rotation How much the camera should be rotated.
        */
        void Rotate(const DirectX::XMFLOAT3& rotation);

        /**
        * @brief Sets the position of the camera in world space.
        * @param[in] position The new position of the camera.
        */
        void SetPosition(const DirectX::XMFLOAT3& position);

        /**
        * @brief Sets the rotation of the camera in world space.
        * @param[in] rotation The new rotation of the camera.
        */
        void SetRotation(const DirectX::XMFLOAT3& rotation);

        /** @returns The world space position of the camera. */
        const DirectX::XMFLOAT3& GetPosition() const;
        /** @returns The world space rotation of the camera. */
        const DirectX::XMFLOAT3& GetRotation() const;

        /** 
        * @brief Sets the near plane of the camera.
        * @param[in] near_plane The near plane z-value.
        */
        void SetNearPlane(float near_plane);

        /**
        * @brief Sets the far plane of the camera.
        * @param[in] far_plane The far plane z-value.
        */
        void SetFarPlane(float far_plane);

        /** @returns The near plane z-value of the camera. */
        float GetNearPlane() const;
        /** @returns The far plane z-value of the camera. */
        float GetFarPlane() const;

        /** @returns The view matrix of the camera. */
        const DirectX::XMMATRIX& GetViewMatrix();

        /** @returns The projection matrix of the camera. */
        const DirectX::XMMATRIX& GetProjectionMatrix();

    protected:
        /** @brief (Re)-calculates the view matrix of the camera. */
        void UpdateViewMatrix();

        /** @brief (Re)-calculates the projection matrix of the camera. */
        virtual void UpdateProjectionMatrix() = 0;

    protected:
        DirectX::XMFLOAT3 position_;                    //!< The position of the camera.
        DirectX::XMFLOAT3 rotation_;                    //!< The rotation of the camera.

        DirectX::XMMATRIX view_;                        //!< The view matrix of the camera.
        DirectX::XMMATRIX projection_;                  //!< The projection matrix of the camera.
        bool view_dirty_;                               //!< Whether the view matrix has to be updated before it can be used again.
        bool projection_dirty_;                         //!< Whether the projection matrix has to be updated before it can be used again.

        float near_plane_;                              //!< The near plane z-value of the camera.
        float far_plane_;                               //!< The far plane z-value of the camera.

    private:
        DirectX::XMFLOAT3 translation_acculumator_;     //!< Accumulator for how much the camera should be translated relative to its current orientation.
    };
}