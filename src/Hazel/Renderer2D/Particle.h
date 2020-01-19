#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Renderer/Texture.h"
#include "Animation2D.h"
#include "Hazel/Core/glm.h"
#include "Hazel/Core/TUtil.h"

namespace Hazel {


	class ParticleSpawn
	{
	public:
		virtual glm::vec2 Generate() = 0;

		virtual ~ParticleSpawn() {}
	};

	struct ParticleDef
	{
		TUtil::FloatCurve<glm::vec2> AlphaCurve = { {0.0f, 1.0f} };

		Ref<AnimationDef2D> Animation;
	};

	struct ParticleEmitterDef
	{
		ParticleEmitterDef(const Ref<ParticleDef>& particleDef) : Def(particleDef) {}

		Ref<ParticleDef> Def;

		float LifetimeMean = 1.0f;
		float LifetimeStdDev = 0.0f;

		//InitalVelocity = IV:
		//These affect the error vector that is added to the inital velocity when each particle is constructed
		bool  IVErrorEnabled = true;
		float IVAngleErrorStdDev = 0.0f;//Degrees

		//Determines how much error there should be based on how long IV is
		float IVErrorPercentMean = 0.1f;
		float IvErrorPercentStdDev = 0.05f;

		float SizeMean = 1.0;
		float SizeStdDev = 0.1f;

		float ParticlesPerSecond = 10.0f;
	};

	struct Particle
	{
		Particle() = default;
		Particle(const Particle& other) = default;

		glm::vec2 Position = { 0.0f, 0.0f }, Velocity = { 0.0f, 0.0f }, Size = { 0.0f, 0.0f };
		float Age = 0.0f, Lifetime = 1.0f;
		bool Alive = false;

		inline bool IsDead() const { return !Alive; }
		inline float GetAgeRatio() const { if (IsDead()) { return 1.0f; } else { return Age / Lifetime; } }

		void Update(Timestep ts);
	};
	
	class ParticleEmitter
	{
	public:

		ParticleEmitter(const Ref<ParticleEmitterDef>& def, ParticleSpawn* spawner);
		inline const ParticleEmitterDef& GetDef() const { return *m_Def.get(); }

		//A duration of 0 means go forever
		inline void SetDuration(float duration) { m_Duration = duration; }
		inline void SetPPS(float pps) { m_ParticleDelay = 1.0f / pps; }
		inline void SetInitalVelocity(glm::vec2 InitalVelocity) { m_InitalVelocity = InitalVelocity; }

		inline ParticleSpawn& GetSpawner() { return *m_Spawner.get(); }
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
		Hazel::Scope<ParticleSpawn> m_Spawner;

		float m_ParticleDelay;
		const Ref<ParticleEmitterDef> m_Def;
		glm::vec2 m_InitalVelocity = { 1.0f, 0.0f };
		
		float m_Duration = 0.0f, m_Age = 0.0f;
		float m_LastParticleTime = 0.0f;

		bool m_Running = true;
	};

	struct ParticleSpawnPoint : public ParticleSpawn
	{
		inline ParticleSpawnPoint(glm::vec2 point) : Point(point) {}
		virtual glm::vec2 Generate();

		glm::vec2 Point;
	};

	struct ParticleSpawnLine : public ParticleSpawn
	{

		inline ParticleSpawnLine(glm::vec2 a, glm::vec2 b) : A(a), B(b) {}
		virtual glm::vec2 Generate();
		glm::vec2 A, B;
	};

	struct ParticleSpawnPointCircle : public ParticleSpawn
	{
		inline ParticleSpawnPointCircle(glm::vec2 point, float radius) : Point(point), Radius(radius) {}
		virtual glm::vec2 Generate();


		glm::vec2 Point;
		float Radius;
	};

}
