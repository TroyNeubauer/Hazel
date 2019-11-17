#pragma once

#include "Hazel/glm.h"

#include "Hazel/Window.h"
#include "Hazel/Application.h"

namespace Hazel {

	class CameraProjection3D
	{
	public:
		//Leave initalization of m_ProjectionMatrix to subclasses
		CameraProjection3D(float nearPlane, float farPlane) : m_NearPlane(nearPlane), m_FarPlane(farPlane), m_ProjectionMatrix() {}

		//Re-creates the projection matrix based on the new parameters
		virtual void RecalculateProjectionMatrix() = 0;
		
		inline const mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		inline float GetNearPlane() { return m_NearPlane; }
		inline float GetFarPlane() { return m_FarPlane; }

		inline void SetNearPlane(float nearPlane) { m_NearPlane = nearPlane; }
		inline void SetFarPlane(float farPlane) { m_FarPlane = farPlane; }
		virtual ~CameraProjection3D() {}

	protected:
		float m_NearPlane, m_FarPlane;
		mat4 m_ProjectionMatrix;
	};

	class PerspectiveCameraProjection : public CameraProjection3D
	{
	public:
		PerspectiveCameraProjection(float nearPlane, float farPlane, float fov) 
			: CameraProjection3D(nearPlane, farPlane), m_FOV(fov) { RecalculateProjectionMatrix(); }

		inline virtual void RecalculateProjectionMatrix()
		{
			Window& window = Application::Get().GetWindow();
			
			m_ProjectionMatrix = infinitePerspective(radians(m_FOV), (float) window.GetWidth() / (float) window.GetHeight(), m_NearPlane/*, m_FarPlane*/);
		}

		inline float GetFOV() { return m_FOV; }
		inline void SetFOV(float fov) { m_FOV = fov; }
	private:
		float m_FOV;
	};


	class OrthographicCameraProjection : public CameraProjection3D
	{
	public:
		OrthographicCameraProjection(float left, float right, float top, float bottom, float nearPlane = -1.0f, float farPlane = 1.0f) 
			: CameraProjection3D(nearPlane, farPlane) { RecalculateProjectionMatrix(); }

		inline virtual void RecalculateProjectionMatrix()
		{
			m_ProjectionMatrix = ortho(m_Left, m_Right, m_Bottom, m_Top, m_NearPlane, m_FarPlane);
		}

	private:
		float m_Left, m_Right, m_Bottom, m_Top;
	};

}
