#include "hzpch.h"
#include "Particle.h"

#include "Hazel/Core/Engine.h"
#include "Renderer2D.h"

#include <random>

namespace Hazel {
	
	void Particle::Update(Timestep ts)
	{
		Age += ts.Seconds();
		Position += Velocity * ts.Seconds();

		if (Age > Lifetime) Alive = false;
	}

	void ParticleEmitter::Render()
	{
		for (int i = 0; i < min(m_PoolIndex, m_PoolCapacity); i++)
		{
			Particle& particle = m_Pool[i];
			if (particle.Alive)
			{
				Renderer2DRenderable renderable;
				renderable.Position = vec3(particle.Position, 0.0f);
				renderable.Size = particle.Size;
				renderable.ApplyAnimation(particle.Animation);
				//renderable.BlendSettings = BlendMode::ADDATIVE;
			}
		}
	}

	void ParticleEmitter::Update(Timestep ts)
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


	std::random_device rd;
	std::default_random_engine rng(rd());

	Particle& ParticleEmitter::CreateParticle()
	{
		int index = m_PoolIndex++ % m_PoolCapacity;
		Particle& particle = m_Pool[index];
		particle.Alive = true;
		particle.Animation = m_Def->Animation;
		std::normal_distribution<float> lifetime(m_Def->LifetimeMean, m_Def->LifetimeStdDev);

		particle.Lifetime = lifetime(rng);
		glm::vec2 error = { 0.0f, 0.0f };
		if (m_Def->IVErrorEnabled)
		{
			std::normal_distribution<float> angleError(0.0f, m_Def->IVAngleErrorStdDev);
			std::normal_distribution<float> angleLength(m_Def->IVErrorLengthMean, m_Def->IvErrorLengthStdDev);

			error = glm::normalize(m_InitalVelocity);
			error = glm::rotate(error, glm::radians(angleError(rng)));
			error *= max(angleLength(rng), 0.0f);//No negitive lengths
		}
		glm::vec2 initalVelocity = m_InitalVelocity + error;

		particle.Velocity = initalVelocity;
		particle.Position = m_StartingPosition;

		return particle;
	}

	ParticleEmitter::ParticleEmitter(const Ref<ParticleEmitterDef>& def, int maxParticles, float pps)
		: m_Def(def), m_PoolCapacity(maxParticles), m_ParticleDelay(1.0f / pps)
	{
		m_Pool.reset(new Particle[maxParticles]);
	}

}



