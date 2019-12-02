#pragma once

#include <memory>

#include "../Core/Application.h"
#include "../Core/glm.h"
#include "../Core/Window.h"
#include "CameraStorage.h"
#include "CameraController.h"
#include "CameraProjection.h"


using namespace glm;
namespace Hazel {

	class Camera
	{
	public:
		virtual const mat4& GetViewMatrix() const = 0;
		virtual const mat4& GetProjectionMatrix() const = 0;
		virtual const mat4& GetViewProjectionMatrix() const = 0;

		virtual ~Camera() {}
	};

	class Camera3D : public Camera
	{
	public:
		virtual const mat4& GetViewMatrix() const override = 0;
		virtual const mat4& GetProjectionMatrix() const override = 0;
		virtual const mat4& GetViewProjectionMatrix() const override = 0;

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

		virtual ~Camera3D() {}
	};

	class Camera2D : public Camera
	{
	public:
		Camera2D(CameraController2D* controller) : m_Controller(controller) {}

		inline void Update () { if (m_Controller) { m_Controller->Update(*this); ForceUpdate(); } }

		inline const mat4& GetViewMatrix() const override { return m_ViewMatrix; }
		inline const mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
		inline const mat4& GetViewProjectionMatrix() const override { return m_VPMatrix; }

		inline void ForceUpdate()
		{
			RecalculateProjectionMatrix();
			RecalculateViewMatrix();
			m_VPMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}

		inline const vec2 GetPosition() { return m_Pos; }
		inline float GetRotation() { return m_Rot; }
		inline float GetZoom() {return m_Zoom; }

		inline void SetRotation(float rotation) { m_Rot = rotation; }
		inline void SetPosition(const vec2& position) { m_Pos = position; }
		inline void SetZoom(float zoom) { m_Zoom = zoom; }
		inline CameraController2D& GetController() { return *m_Controller; }

		inline void RecalculateProjectionMatrix()
		{
			float aspect = static_cast<float>(Application::Get().GetWindow().GetWidth()) / static_cast<float>(Application::Get().GetWindow().GetHeight());
			m_ProjectionMatrix = ortho(m_Zoom * -aspect, m_Zoom * aspect, -m_Zoom, m_Zoom, -1.0f, 1.0f);
		}
		inline void RecalculateViewMatrix()
		{
			mat4 transform =
				translate(mat4(1.0f), vec3(m_Pos, 0.0f)) *
				rotate(mat4(1.0f), radians(m_Rot), vec3(0, 0, 1));

			m_ViewMatrix = inverse(transform);
		}

	public:
		mat4 m_ProjectionMatrix;
		mat4 m_ViewMatrix;
		mat4 m_VPMatrix;
		float m_Rot = 0.0f, m_Zoom = 1.0f, m_ZoomVel = 0.0f;
		vec2 m_Pos = {0, 0}, m_Vel = {0, 0};

		std::unique_ptr<CameraController2D> m_Controller;

		friend class CameraController2D;
	};

	class DefaultCamera3D : public Camera3D
	{
	public:

		DefaultCamera3D(CameraController3D* controller, CameraStorage3D* storage, CameraProjection3D* projection)
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

		virtual void RecalculateProjectionMatrix() override { m_Projection->RecalculateProjectionMatrix(); UpdateViewProjectionMatrix(); }
		virtual void RecalculateViewMatrix() override { m_Storage->RecalculateViewMatrix(); UpdateViewProjectionMatrix(); }

		virtual const vec3 GetPosition() override { return m_Storage->GetPosition(); }
		virtual const quat GetRotation() override { return m_Storage->GetRotation(); }
		virtual const vec3 GetEulerAngles() override { return m_Storage->GetEulerAngles(); }

		virtual void SetRotation(const quat& rotation) override { m_Storage->SetRotation(rotation); }
		virtual void SetPosition(const vec3& position) override { m_Storage->SetPosition(position); }
		virtual void SetEulerAngles(const vec3& rotation) override { m_Storage->SetEulerAngles(rotation); }

		inline CameraController3D& GetController() const { return *m_Controller.get(); }
		inline CameraStorage3D& GetStorage() const { return *m_Storage.get(); }
		inline CameraProjection3D& GetProjection() const { return *m_Projection.get(); }

		inline virtual const mat4& GetViewMatrix() const override { return m_Storage->GetViewMatrix(); }
		inline virtual const mat4& GetProjectionMatrix() const override { return m_Projection->GetProjectionMatrix(); }
		inline virtual const mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

		virtual ~DefaultCamera3D() {}

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

	class FPSCamera : public DefaultCamera3D
	{
	public:
		FPSCamera(float fov) : DefaultCamera3D(new FPSCameraController(), new EulerCameraStorage(), new PerspectiveCameraProjection(0.5f, 7500.0f, fov)) {}

		virtual ~FPSCamera() {}
	};

}
