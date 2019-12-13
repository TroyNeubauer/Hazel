#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Renderer/Texture.h"
#include "Animation2D.h"

namespace Hazel {

	struct ParticleEmitterDef
	{
		AnimationDef2D Animation;
		float LifetimeMean;
		float LifetimeStdDev;

		//InitalVelocity = IV:
		//These affect the error vector that is added to the inital velocity when each particle is constructed
		bool  IVErrorEnabled = true;
		float IVAngleErrorStdDev;
		float IVErrorLengthMean;
		float IvErrorLengthStdDev;
	};

	struct Particle
	{
		glm::vec2 Position, Velocity;
		float Age, Lifetime;

		inline bool IsDead() const { return Age >= Lifetime; }
		inline bool GetAgeRatio() const { if (IsDead()) { return 1.0f; } else { return Age / Lifetime; } }

		void Update(Timestep ts);
	};
	
	class ParticleEmitter
	{
	public:
		ParticleEmitter() {}

		ParticleEmitter(const ParticleEmitterDef& def, int maxParticles, float particlesPerSecond) : m_ParticleDelay(1.0f / particlesPerSecond), m_Def(def), m_Pool(maxParticles) {}

		inline const ParticleEmitterDef& GetDef() const { return m_Def; }

		//A duration of 0 means go forever
		inline void SetDuration(float duration) { m_Duration = duration; }
		inline void SetPPS(float pps) { m_ParticleDelay * 1.0f / pps; }
		inline void SetInitalVelocity(glm::vec2 InitalVelocity) { m_InitalVelocity = InitalVelocity; }
		inline void SetPosition(glm::vec2 position) { m_StartingPosition = position; }

		inline glm::vec2 GetInitalVelocity() { return m_InitalVelocity; }

		inline void Start() { m_Running = true; }
		inline void Stop () { m_Running = false; }

		void Update(Timestep ts);
	private:
		Particle& CreateParticle();

	private:
		uint64_t m_PoolIndex = 0;
		std::vector<Particle> m_Pool;

		float m_ParticleDelay;
		ParticleEmitterDef m_Def;
		glm::vec2 m_InitalVelocity = { 1.0f, 0.0f };
		glm::vec2 m_StartingPosition = { 0.0f, 0.0f };
		
		float m_Duration = 0.0f, m_Age = 0.0f;
		float m_LastParticleTime = 0.0f;

		bool m_Running = true;
	};

}
