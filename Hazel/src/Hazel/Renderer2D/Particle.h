#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Renderer/Texture.h"
#include "Animation2D.h"
#include "Hazel/Core/glm.h"

namespace Hazel {

	struct ParticleEmitterDef
	{
		ParticleEmitterDef(const Ref<AnimationDef2D>& animation) : Animation(animation) {}

		Ref<AnimationDef2D> Animation;
		float LifetimeMean = 1.0f;
		float LifetimeStdDev = 0.0f;

		//InitalVelocity = IV:
		//These affect the error vector that is added to the inital velocity when each particle is constructed
		bool  IVErrorEnabled = true;
		float IVAngleErrorStdDev = 0.0f;
		float IVErrorLengthMean = 0.1f;
		float IvErrorLengthStdDev = 0.0f;
	};

	struct Particle
	{
		Particle() : Animation(nullptr) {}
		Particle(const Particle& other) = default;

		glm::vec2 Position = { 0.0f, 0.0f }, Velocity = { 0.0f, 0.0f }, Size = { 0.0f, 0.0f };
		float Age = 0.0f, Lifetime = 1.0f;
		bool Alive = false;
		Animation2D Animation;

		inline bool IsDead() const { return !Alive; }
		inline bool GetAgeRatio() const { if (IsDead()) { return 1.0f; } else { return Age / Lifetime; } }

		void Update(Timestep ts);
	};
	
	class ParticleEmitter
	{
	public:

		ParticleEmitter(const Ref<ParticleEmitterDef>& def, int maxParticles = 100, float particlesPerSecond = 10.0f);
		inline const ParticleEmitterDef& GetDef() const { return *m_Def.get(); }

		//A duration of 0 means go forever
		inline void SetDuration(float duration) { m_Duration = duration; }
		inline void SetPPS(float pps) { m_ParticleDelay = 1.0f / pps; }
		inline void SetInitalVelocity(glm::vec2 InitalVelocity) { m_InitalVelocity = InitalVelocity; }
		inline void SetPosition(glm::vec2 position) { m_StartingPosition = position; }

		inline glm::vec2 GetInitalVelocity() { return m_InitalVelocity; }

		inline void Start() { m_Running = true; }
		inline void Stop () { m_Running = false; }

		void Update(Timestep ts);
		void Render();
	private:
		Particle& CreateParticle();

	private:
		uint64_t m_PoolIndex = 0, m_PoolCapacity;
		Hazel::Scope<Particle[]> m_Pool;

		float m_ParticleDelay;
		const Ref<ParticleEmitterDef> m_Def;
		glm::vec2 m_InitalVelocity = { 1.0f, 0.0f };
		glm::vec2 m_StartingPosition = { 0.0f, 0.0f };
		
		float m_Duration = 0.0f, m_Age = 0.0f;
		float m_LastParticleTime = 0.0f;

		bool m_Running = true;
	};

}
