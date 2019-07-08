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
		virtual const mat4& GetViewProjectionMatrix() const = 0;

		virtual void ForceUpdate() = 0;
		virtual void Update(bool paused) = 0;

		virtual const vec3 GetPosition() = 0;
		virtual const quat GetRotation() = 0;
		virtual const vec3 GetEulerAngles() = 0;

		virtual void SetRotation(const quat& rotation) = 0;
		virtual void SetPosition(const vec3& position) = 0;
		virtual void SetEulerAngles(const vec3& rotation) = 0;

		virtual void RecalculateProjectionMatrix() = 0;
		virtual void RecalculateViewMatrix() = 0;
	};

	class DefaultCamera : public Camera
	{
	public:

		DefaultCamera(CameraController* controller, CameraStorage* storage, CameraProjection* projection)
			: m_Controller(controller), m_Storage(storage), m_Projection(projection), m_ViewProjectionMatrix() { Update(false); }
		
		inline virtual void ForceUpdate() override
		{
			m_Projection->RecalculateProjectionMatrix();
			m_Storage->RecalculateViewMatrix();
			UpdateViewProjectionMatrix();
		}

		inline virtual void Update(bool paused) override
		{
			if (m_Controller->Update(*m_Storage.get(), paused)) {
				m_Storage->RecalculateViewMatrix();
			}
			UpdateViewProjectionMatrix();
		}

		virtual void RecalculateProjectionMatrix() { m_Projection->RecalculateProjectionMatrix(); UpdateViewProjectionMatrix(); }
		virtual void RecalculateViewMatrix() { m_Storage->RecalculateViewMatrix(); UpdateViewProjectionMatrix(); }

		virtual const vec3 GetPosition() { return m_Storage->GetPosition(); }
		virtual const quat GetRotation() { return m_Storage->GetRotation(); }
		virtual const vec3 GetEulerAngles() { return m_Storage->GetEulerAngles(); }

		virtual void SetRotation(const quat& rotation) { m_Storage->SetRotation(rotation); }
		virtual void SetPosition(const vec3& position) { m_Storage->SetPosition(position); }
		virtual void SetEulerAngles(const vec3& rotation) { m_Storage->SetEulerAngles(rotation); }

		inline CameraController& GetController() const { return *m_Controller.get(); }
		inline CameraStorage& GetStorage() const { return *m_Storage.get(); }
		inline CameraProjection& GetProjection() const { return *m_Projection.get(); }

		inline virtual const mat4& GetViewMatrix() const override { return m_Storage->GetViewMatrix(); }
		inline virtual const mat4& GetProjectionMatrix() const override { return m_Projection->GetProjectionMatrix(); }
		inline virtual const mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

	private:
		void UpdateViewProjectionMatrix() {
			m_ViewProjectionMatrix = GetProjectionMatrix() * GetViewMatrix();
		}


	protected:
		mat4 m_ViewProjectionMatrix;
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