#include "hzpch.h"
#include "Particle.h"

#include "Hazel/Core/Engine.h"
#include "Hazel/Util/Utils.h"
#include "Renderer2D.h"

#include <random>

namespace Hazel {
	
	void Particle::Update(Timestep ts)
	{
		Age += ts.Seconds();
		Position += Velocity * ts.Seconds();

		if (Age >= Lifetime) Alive = false;
	}

	void ParticleEmitter::Render()
	{
		for (int i = 0; i < min(m_PoolIndex, m_PoolCapacity); i++)
		{
			Particle& particle = m_Pool[i];
			if (particle.Alive)
			{
				Hazel::Renderer2D::Renderable renderable;
				renderable.Position = vec3(particle.Position, -0.1f);
				renderable.Size = particle.Size;
				const ParticleDef& particleDef = *m_Def->Def.get();
				float ageRatio = particle.GetAgeRatio();
				int index = static_cast<int>(particleDef.Animation->m_Frames.size() * ageRatio);

				Frame& frame = particleDef.Animation->m_Frames[index];
				renderable.ApplyAnimation(particleDef.Animation, frame);
				renderable.BlendSettings = BlendMode::ADDATIVE;
				renderable.Color = Hazel::Utils::ToCompactColor(glm::vec4(1.0f, 1.0f, 1.0f, particleDef.AlphaCurve.Sample(ageRatio)));

				Hazel::Renderer2D::DrawQuad(renderable);
			}
		}
	}

	void ParticleEmitter::Update(Timestep ts)
	{
		for (int i = 0; i < min(m_PoolIndex, m_PoolCapacity); i++)
		{
			Particle& particle = m_Pool[i];
			particle.Update(ts);
		}

		if (m_Running)
		{
			m_LastParticleTime += ts.Seconds();
			while (m_LastParticleTime > m_ParticleDelay)
			{
				if (m_Duration > 0.0f && m_Age >= m_Duration)
				{
					Stop();//No more particles
				}
				else
				{
					Particle& particle = CreateParticle();
					particle.Update(Timestep(m_LastParticleTime));
				}
				m_LastParticleTime -= m_ParticleDelay;
				m_Age += m_ParticleDelay;
			}
			m_Age += fmodf(ts.Seconds(), m_ParticleDelay);
		}
		else
		{
			m_Age += ts.Seconds();
		}

	}


	std::random_device rd;
	std::default_random_engine rng(rd());

	Particle& ParticleEmitter::CreateParticle()
	{
		std::uint64_t index = m_PoolIndex++ % m_PoolCapacity;
		Particle& particle = m_Pool[index];
		particle.Alive = true;
		particle.Age = 0.0f;

		glm::vec2 initalVelocity = m_InitalVelocity;
		if (m_Def->IVErrorEnabled)
		{
			std::normal_distribution<float> angleError(0.0f, m_Def->IVAngleErrorStdDev);
			std::normal_distribution<float> angleLengthPercent(m_Def->IVErrorPercentMean, m_Def->IvErrorPercentStdDev);

			initalVelocity = glm::rotate(initalVelocity, glm::radians(angleError(rng)));
			initalVelocity *= max(1.0f - angleLengthPercent(rng), 0.0f);//No negitive lengths
		}

		particle.Velocity = initalVelocity;
		particle.Position = m_Spawner->Generate();

		std::normal_distribution<float> size(m_Def->SizeMean, m_Def->SizeStdDev);
		float height = size(rng);
		particle.Size = { height * m_Def->Def->Animation->AspectRatio(), height };

		std::normal_distribution<float> lifetime(m_Def->LifetimeMean, m_Def->LifetimeStdDev);
		particle.Lifetime = lifetime(rng);


		return particle;
	}

	size_t ComputeMaxParticles(const Ref<ParticleEmitterDef>& def)
	{
		return static_cast<size_t>(def->ParticlesPerSecond * def->LifetimeMean * 1.5f);
	}

	ParticleEmitter::ParticleEmitter(const Ref<ParticleEmitterDef>& def, ParticleSpawn* spawner)
		: m_Def(def), m_PoolCapacity(ComputeMaxParticles(def)), m_ParticleDelay(1.0f / def->ParticlesPerSecond), m_Spawner(spawner)
	{
		m_Pool.reset(new Particle[m_PoolCapacity]);
	}


	glm::vec2 ParticleSpawnPoint::Generate()
	{
		return Point;
	}

	glm::vec2 ParticleSpawnLine::Generate()
	{
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);
		float f = dist(rng);
		return glm::vec2(TUtil::Math::Lerp(A.x, B.x, f), TUtil::Math::Lerp(A.y, B.y, f));
	}

	glm::vec2 ParticleSpawnPointCircle::Generate()
	{
		std::uniform_real_distribution<float> angle(0.0f, TUtil::Math::PI<float>());
		std::uniform_real_distribution<float> radius(0.0f, Radius);

		glm::vec2 offset = radius(rng) * glm::rotate(glm::vec2(1.0f, 0.0f), angle(rng));
		return Point + offset;
	}

}



