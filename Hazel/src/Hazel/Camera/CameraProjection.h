#pragma once

#include "Hazel/glm.h"

#include "Hazel/Window.h"
#include "Hazel/Application.h"

namespace Hazel {

	class CameraProjection
	{
	public:
		//Leave initalization of m_ProjectionMatrix to subclasses
		CameraProjection(float nearPlane, float farPlane) : m_NearPlane(nearPlane), m_FarPlane(farPlane), m_ProjectionMatrix() {}

		//Re-creates the projection matrix based on the new parameters
		virtual void Update() = 0;
		
		inline const mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		inline float GetNearPlane() { return m_NearPlane; }
		inline float GetFarPlane() { return m_FarPlane; }

		inline void SetNearPlane(float nearPlane) { m_NearPlane = nearPlane; }
		inline void SetFarPlane(float farPlane) { m_FarPlane = farPlane; }

	protected:
		float m_NearPlane, m_FarPlane;
		mat4 m_ProjectionMatrix;
	};

	class PerspectiveCameraProjection : public CameraProjection
	{
	public:
		PerspectiveCameraProjection(float nearPlane, float farPlane, float fov) : CameraProjection(nearPlane, farPlane), m_FOV(fov) { Update(); }

		inline virtual void Update()
		{
			Window& window = Application::Get().GetWindow();
			
			m_ProjectionMatrix = infinitePerspective(radians(m_FOV), (float) window.GetWidth() / (float) window.GetHeight(), m_NearPlane/*, m_FarPlane*/);
		}

		inline float GetFOV() { return m_FOV; }
		inline void SetFOV(float fov) { m_FOV = fov; }
	private:
		float m_FOV;
	};

}
