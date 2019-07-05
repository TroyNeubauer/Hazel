#pragma once

#include <memory>
#include "Hazel/glm.h"

#include "CameraStorage.h"
#include "CameraController.h"
#include "CameraProjection.h"

#include "Hazel/Application.h"
#include "Hazel/Window.h"

using namespace glm;
namespace Hazel {

	class Camera
	{
	public:

		virtual const mat4& GetViewMatrix() const = 0;
		virtual const mat4& GetProjectionMatrix() const = 0;
		virtual const mat4& GetVPMatrix() const = 0;

		virtual void ForceUpdate() = 0;
		virtual void Update(bool paused) = 0;

	};

	class DefaultCamera : public Camera
	{
	public:

		DefaultCamera(CameraController* controller, CameraStorage* storage, CameraProjection* projection)
			: m_Controller(controller), m_Storage(storage), m_Projection(projection), m_VPMatrix() { Update(false); }
		
		inline virtual void ForceUpdate() override
		{
			m_Projection->Update();
			m_Storage->Update();
			m_VPMatrix = GetProjectionMatrix() * GetViewMatrix();
		}

		inline virtual void Update(bool paused) override
		{
			if (m_Controller->Update(*m_Storage.get(), paused)) {
				m_Storage->Update();
			}
			m_VPMatrix = GetProjectionMatrix() * GetViewMatrix();
		}

		inline CameraController& GetController() const { return *m_Controller.get(); }
		inline CameraStorage& GetStorage() const { return *m_Storage.get(); }
		inline CameraProjection& GetProjection() const { return *m_Projection.get(); }

		inline virtual const mat4& GetViewMatrix() const override { return m_Storage->GetViewMatrix(); }
		inline virtual const mat4& GetProjectionMatrix() const override { return m_Projection->GetProjectionMatrix(); }
		inline virtual const mat4& GetVPMatrix() const override { return m_VPMatrix; }

	protected:
		mat4 m_VPMatrix;
		std::unique_ptr<CameraController> m_Controller;
		std::unique_ptr<CameraStorage> m_Storage;
		std::unique_ptr<CameraProjection> m_Projection;
	};

	class FPSCamera : public DefaultCamera
	{
	public:
		FPSCamera(float fov) : DefaultCamera(new FPSCameraController(), new EulerCameraStorage(), new PerspectiveCameraProjection(0.5f, 7500.0f, fov)) {}
	};

}