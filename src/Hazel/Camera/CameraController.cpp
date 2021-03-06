#include "hzpch.h"
#include "CameraController.h"

#include "Camera.h"
#include <glm/gtx/norm.hpp>
#include <math.h>
#include "../Core/Engine.h"
#include "../Core/Input.h"
#include "../Core/KeyCodes.h"

namespace Hazel {

	const float ROT_AMOUNT = glm::radians(1.0f), MOVE_AMOUNT = 10, PI_2 = glm::pi<float>() / 2.0f;

	bool AdvancedCameraController::Update(Hazel::Timestep ts, CameraStorage3D& camera)
	{
		bool update = false;
		glm::vec3 pos = camera.GetPosition();
		glm::quat rot = camera.GetRotation();

		glm::mat3 matRot = toMat3(rot);
		glm::vec3 forward = rot * glm::vec3(0.0f, 0.0f, -1.0f), up = rot * glm::vec3(0.0f, 1.0f, 0.0f), right = rot * glm::vec3(1.0f, 0.0f, 0.0f), move(0, 0, 0);

		//rot.x += mouseMove.y * ROT_AMOUNT / 10.0f;
		//rot.y += mouseMove.x * ROT_AMOUNT / 10.0f;
		glm::vec2 mouseMove = Hazel::Input::GetMouseDelta();
		if (mouseMove.x || mouseMove.y)
			update = true;

		rot = rotate(rot, mouseMove.y * ROT_AMOUNT / 10.0f, right);
		rot = rotate(rot, mouseMove.x * ROT_AMOUNT / 10.0f, up);

		if (Hazel::Input::IsKeyPressed(HZ_KEY_Q)) {
			
			update = true;
		} if (Hazel::Input::IsKeyPressed(HZ_KEY_E)) {
			
			update = true;
		}
		
		camera.SetRotation(rot);
		

		if (Hazel::Input::IsKeyPressed(HZ_KEY_W)) {
			move += forward;
		} if (Hazel::Input::IsKeyPressed(HZ_KEY_S)) {
			move -= forward;
		} if (Hazel::Input::IsKeyPressed(HZ_KEY_D)) {
			move += right;
		} if (Hazel::Input::IsKeyPressed(HZ_KEY_A)) {
			move -= right;
		} if (Hazel::Input::IsKeyPressed(HZ_KEY_SPACE)) {
			move += up;
		} if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT)) {
			move -= up;
		}
		if (length2(move) != 0.0f) {
			update = true;
			move = normalize(move);
			move *= MOVE_AMOUNT;
			pos += move * ts.Seconds();
			camera.SetPosition(pos);
		}

		return update;
	}

	bool FPSCameraController::Update(Hazel::Timestep ts, CameraStorage3D& camera)
	{
		bool update = false;
		glm::vec3 pos = camera.GetPosition();
		glm::vec3 rot = camera.GetEulerAngles();
		float pitch = rot.x, yaw = rot.y;

		glm::vec3 forward = glm::vec3(sin(yaw), 0.0f, -cos(yaw)),
			up = glm::vec3(0.0f, 1.0f, 0.0f), move(0, 0, 0),
			right = glm::vec3(sin(yaw + PI_2), 0.0f, -cos(yaw + PI_2));

		glm::vec2 mouseMove = Hazel::Input::GetMouseDelta();
		if (mouseMove.x || mouseMove.y)
			update = true;

		pitch -= mouseMove.y * ROT_AMOUNT / 10.0f;
		yaw += mouseMove.x * ROT_AMOUNT / 10.0f;

		pitch = glm::clamp(pitch, glm::radians<float>(-110.0f), glm::radians<float>(90.0f));
		yaw = glm::mod(yaw, glm::radians<float>(360));

		camera.SetEulerAngles(glm::vec3(pitch, yaw, rot.z));


		if (Hazel::Input::IsKeyPressed(HZ_KEY_W)) {
			move += forward;
		} if (Hazel::Input::IsKeyPressed(HZ_KEY_S)) {
			move -= forward;
		} if (Hazel::Input::IsKeyPressed(HZ_KEY_D)) {
			move += right;
		} if (Hazel::Input::IsKeyPressed(HZ_KEY_A)) {
			move -= right;
		} if (Hazel::Input::IsKeyPressed(HZ_KEY_SPACE)) {
			move += up;
		} if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT)) {
			move -= up;
		}
		if (length2(move) != 0.0f) {
			update = true;
			move = normalize(move);
			move *= MOVE_AMOUNT;
			pos += move * ts.Seconds();
			camera.SetPosition(pos);
		}
		return update;
	}

}

