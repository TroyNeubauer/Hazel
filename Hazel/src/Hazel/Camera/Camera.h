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
		virtual const mat4& GetViewMatrix() const = 0;
		virtual const mat4& GetProjectionMatrix() const = 0;
		virtual const mat4& GetViewProjectionMatrix() const = 0;
	};

	class Camera3D : public Camera
	{
	public:

		virtual const mat4& GetViewMatrix() const = 0;
		virtual const mat4& GetProjectionMatrix() const = 0;
		virtual const mat4& GetViewProjectionMatrix() const = 0;

		virtual void ForceUpdate() = 0;
		virtual void Update() = 0;

		virtual const vec3 GetPosition() = 0;
		virtual const quat GetRotation() = 0;
		virtual const vec3 GetEulerAngles() = 0;

		virtual void SetRotation(const quat& rotation) = 0;
		virtual void SetPosition(const vec3& position) = 0;
		virtual void SetEulerAngles(const vec3& rotation) = 0;

		virtual void RecalculateProjectionMatrix() = 0;
		virtual void RecalculateViewMatrix() = 0;
	};

	class Camera2D : public Camera
	{
		virtual const mat4& GetViewMatrix() const = 0;
		virtual const mat4& GetProjectionMatrix() const = 0;
		virtual const mat4& GetViewProjectionMatrix() const = 0;

		virtual void ForceUpdate() = 0;

		virtual const vec2 GetPosition() = 0;
		virtual const float GetRotation() = 0;

		virtual void SetRotation(float rotation) = 0;
		virtual void SetPosition(const vec2& position) = 0;

		virtual void RecalculateProjectionMatrix() = 0;
		virtual void RecalculateViewMatrix() = 0;
	};

	class DefaultCamera : public Camera3D
	{
	public:

		DefaultCamera(CameraController3D* controller, CameraStorage3D* storage, CameraProjection3D* projection)
			: m_Controller(controller), m_Storage(storage), m_Projection(projection), m_ViewProjectionMatrix() { Update(); }
		
		inline virtual void ForceUpdate() override
		{
			m_Projection->RecalculateProjectionMatrix();
			m_Storage->RecalculateViewMatrix();
			UpdateViewProjectionMatrix();
		}

		inline virtual void Update() override
		{
			if (m_Controller->Update(*m_Storage.get())) {
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

		inline CameraController3D& GetController() const { return *m_Controller.get(); }
		inline CameraStorage3D& GetStorage() const { return *m_Storage.get(); }
		inline CameraProjection3D& GetProjection() const { return *m_Projection.get(); }

		inline virtual const mat4& GetViewMatrix() const override { return m_Storage->GetViewMatrix(); }
		inline virtual const mat4& GetProjectionMatrix() const override { return m_Projection->GetProjectionMatrix(); }
		inline virtual const mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

	private:
		void UpdateViewProjectionMatrix() {
			m_ViewProjectionMatrix = GetProjectionMatrix() * GetViewMatrix();
		}


	protected:
		mat4 m_ViewProjectionMatrix;
		std::unique_ptr<CameraController3D> m_Controller;
		std::unique_ptr<CameraStorage3D> m_Storage;
		std::unique_ptr<CameraProjection3D> m_Projection;
	};

	class FPSCamera : public DefaultCamera
	{
	public:
		FPSCamera(float fov) : DefaultCamera(new FPSCameraController(), new EulerCameraStorage(), new PerspectiveCameraProjection(0.5f, 7500.0f, fov)) {}
	};

}