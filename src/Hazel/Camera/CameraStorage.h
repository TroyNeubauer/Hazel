#pragma once

#include "Hazel/Core/glm.h"

namespace Hazel {
	class CameraStorage3D
	{
	public:
		//Updates the view matrix
		virtual void RecalculateViewMatrix() = 0;

		virtual const glm::vec3 GetPosition() = 0;
		virtual const glm::quat GetRotation() = 0;
		virtual const glm::vec3 GetEulerAngles() = 0;

		virtual void SetRotation(const glm::quat& rotation) = 0;
		virtual void SetPosition(const glm::vec3& position) = 0;
		virtual void SetEulerAngles(const glm::vec3& rotation) = 0;

		virtual const glm::mat4& GetViewMatrix() const = 0;

		virtual ~CameraStorage3D() {}

	};

	class DefaultCameraStorage : public CameraStorage3D
	{
	public:
		DefaultCameraStorage() { RecalculateViewMatrix(); }
		virtual void RecalculateViewMatrix() override { m_ViewMatrix = glm::toMat4(m_Rotation) * glm::translate(-m_Posirion); }

		inline virtual const glm::vec3 GetPosition() override { return m_Posirion; }
		inline virtual const glm::quat GetRotation() override { return m_Rotation; }
		inline virtual const glm::vec3 GetEulerAngles() override { return glm::eulerAngles(m_Rotation); }

		inline virtual void SetPosition(const glm::vec3& position) override { m_Posirion = position; }
		inline virtual void SetRotation(const glm::quat& rotation) override { m_Rotation = rotation; }
		inline virtual void SetEulerAngles(const glm::vec3& rotation) override { m_Rotation = glm::quat(rotation); }

		inline virtual const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }

		virtual ~DefaultCameraStorage() override {}
	private:
		glm::vec3 m_Posirion;
		glm::quat m_Rotation;
		glm::mat4 m_ViewMatrix;
	};

	class EulerCameraStorage : public CameraStorage3D
	{
	public:
		EulerCameraStorage() 
			: m_Posirion(0.0f), m_Pitch(0.0f), m_Yaw(0.0f), m_Roll(0.0f) { RecalculateViewMatrix(); }
		virtual void RecalculateViewMatrix() override
		{
			m_ViewMatrix = glm::identity<glm::mat4>();
			m_ViewMatrix = glm::rotate(m_ViewMatrix, -m_Pitch, glm::vec3(1.0f, 0.0f, 0.0f));
			m_ViewMatrix = glm::rotate(m_ViewMatrix, m_Yaw   , glm::vec3(0.0f, 1.0f, 0.0f));
			m_ViewMatrix = glm::rotate(m_ViewMatrix, m_Roll  , glm::vec3(0.0f, 0.0f, 1.0f));
			m_ViewMatrix = glm::translate(m_ViewMatrix, -m_Posirion);
		}

		inline virtual const glm::vec3 GetPosition() override { return m_Posirion; }
		inline virtual const glm::quat GetRotation() override { return glm::quat(GetEulerAngles()); }
		inline virtual const glm::vec3 GetEulerAngles() override { return glm::vec3(m_Pitch, m_Yaw, m_Roll); }

		inline virtual void SetPosition(const glm::vec3& position) override { m_Posirion = position; }
		inline virtual void SetRotation(const glm::quat& rotation) override { SetEulerAngles(glm::eulerAngles(rotation)); }
		inline virtual void SetEulerAngles(const glm::vec3& rotation) override { m_Pitch = rotation.x; m_Yaw = rotation.y; m_Roll = rotation.z; }

		inline virtual const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }

		virtual ~EulerCameraStorage() override{}
	private:
		glm::vec3 m_Posirion;
		float m_Pitch, m_Yaw, m_Roll;
		glm::mat4 m_ViewMatrix;
	};

}
