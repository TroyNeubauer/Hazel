#pragma once

#include "Hazel/glm.h"

namespace Hazel {
	class CameraStorage
	{
	public:
		//Updates the view matrix
		virtual void RecalculateViewMatrix() = 0;

		virtual const vec3 GetPosition() = 0;
		virtual const quat GetRotation() = 0;
		virtual const vec3 GetEulerAngles() = 0;

		virtual void SetRotation(const quat& rotation) = 0;
		virtual void SetPosition(const vec3& position) = 0;
		virtual void SetEulerAngles(const vec3& rotation) = 0;

		virtual const mat4& GetViewMatrix() const = 0;

		virtual ~CameraStorage();

	};

	class DefaultCameraStorage : public CameraStorage
	{
	public:
		DefaultCameraStorage() { RecalculateViewMatrix(); }
		virtual void RecalculateViewMatrix() { m_ViewMatrix = toMat4(m_Rotation) * translate(-m_Posirion); }

		inline virtual const vec3 GetPosition() override { return m_Posirion; }
		inline virtual const quat GetRotation() override { return m_Rotation; }
		inline virtual const vec3 GetEulerAngles() override { return eulerAngles(m_Rotation); }

		inline virtual void SetPosition(const vec3& position) override { m_Posirion = position; }
		inline virtual void SetRotation(const quat& rotation) override { m_Rotation = rotation; }
		inline virtual void SetEulerAngles(const vec3& rotation) override { m_Rotation = quat(rotation); }

		inline virtual const mat4& GetViewMatrix() const override { return m_ViewMatrix; }

		virtual ~DefaultCameraStorage() override {}
	private:
		vec3 m_Posirion;
		quat m_Rotation;
		mat4 m_ViewMatrix;
	};

	class EulerCameraStorage : public CameraStorage
	{
	public:
		EulerCameraStorage() 
			: m_Posirion(0.0f), m_Pitch(0.0f), m_Yaw(0.0f), m_Roll(0.0f) { RecalculateViewMatrix(); }
		virtual void RecalculateViewMatrix()
		{
			m_ViewMatrix = identity<mat4>();
			m_ViewMatrix = rotate(m_ViewMatrix, -m_Pitch, vec3(1.0f, 0.0f, 0.0f));
			m_ViewMatrix = rotate(m_ViewMatrix, m_Yaw   , vec3(0.0f, 1.0f, 0.0f));
			m_ViewMatrix = rotate(m_ViewMatrix, m_Roll  , vec3(0.0f, 0.0f, 1.0f));
			m_ViewMatrix = translate(m_ViewMatrix, -m_Posirion);
		}

		inline virtual const vec3 GetPosition() override { return m_Posirion; }
		inline virtual const quat GetRotation() override { return quat(GetEulerAngles()); }
		inline virtual const vec3 GetEulerAngles() override { return vec3(m_Pitch, m_Yaw, m_Roll); }

		inline virtual void SetPosition(const vec3& position) override { m_Posirion = position; }
		inline virtual void SetRotation(const quat& rotation) override { SetEulerAngles(eulerAngles(rotation)); }
		inline virtual void SetEulerAngles(const vec3& rotation) override { m_Pitch = rotation.x; m_Yaw = rotation.y; m_Roll = rotation.z; }

		inline virtual const mat4& GetViewMatrix() const override { return m_ViewMatrix; }

		virtual ~EulerCameraStorage() override{}
	private:
		vec3 m_Posirion;
		float m_Pitch, m_Yaw, m_Roll;
		mat4 m_ViewMatrix;
	};

}
