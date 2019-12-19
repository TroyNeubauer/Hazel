#include "Planet.h"

#include "world/World.h"

const int CIRCLE_POINTS = 1000;

//Calculates the density needed for a planet to have the proper accerlation at surface level
float GetDensity(float accel)
{
	return accel / (World::Constants::G * glm::pi<double>());
}


Planet::Planet(World& world, float radius, float surfaceGravity, glm::vec4 groundColor, glm::vec4 skyColor) : m_Array(Hazel::VertexArray::Create()), m_Radius(radius)
{
	b2BodyDef def;
	def.position = b2Vec2(0.0f, 0.0f);
	def.userData = this;
	def.type = b2_staticBody;
	def.active = true;
	b2Body* body = world.GetWorld()->CreateBody(&def);


	b2FixtureDef fixtureDef;
	fixtureDef.density = GetDensity(surfaceGravity);
	fixtureDef.friction = 2.0f;
	fixtureDef.restitution = 0.01f;

	b2CircleShape circle;
	circle.m_radius = radius;
	fixtureDef.shape = &circle;
	b2MassData data;
	circle.ComputeMass(&data, fixtureDef.density);
	m_Mass = data.mass;

	body->CreateFixture(&fixtureDef);
	SetPhysicsBody(body);

	std::vector<uint32_t> indices;
	std::vector<Hazel::Renderer2D::VertexData> vertices;
	vertices.emplace_back(glm::vec3(0.0f, 0.0f, -0.5f), glm::vec2(.5f, 0.5f), groundColor);

	for (int i = 1; i < CIRCLE_POINTS + 1; i++)
	{
		float angle = static_cast<float>(i) / static_cast<float>(CIRCLE_POINTS) * 2.0f * glm::pi<float>();
		glm::vec2 unitOffset = { cos(angle), sin(angle) };
		glm::vec2 offset = radius * unitOffset;
		glm::vec3 pos = { offset, -0.5f };
		glm::vec2 texCoord = (unitOffset + 1.0f) / 2.0f * radius / 100.0f;//Tile every 100m
		vertices.emplace_back(pos, texCoord, groundColor);
		indices.push_back(0);
		if (i < CIRCLE_POINTS)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
		}
	}
	indices.push_back(CIRCLE_POINTS);
	indices.push_back(1);

	Hazel::Ref<Hazel::VertexBuffer> vb = Hazel::VertexBuffer::Create(reinterpret_cast<float*>(vertices.data()), vertices.size() * sizeof(Hazel::Renderer2D::VertexData));
	Hazel::Ref<Hazel::IndexBuffer> ib = Hazel::IndexBuffer::Create(indices.data(), indices.size() * sizeof(uint32_t));
	vb->SetLayout(Hazel::Renderer2D::Layout, sizeof(Hazel::Renderer2D::VertexData));
	m_Array->SetIndexBuffer(ib);
	m_Array->AddVertexBuffer(vb);
}

void Planet::Render(const Hazel::Camera& camera, Hazel::Ref<Hazel::Shader>& planetShader)
{
	planetShader->Bind();
	planetShader->SetFloat3("u_Offset", glm::vec3(GetPosition(), 0.0f));
	planetShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	Hazel::Renderer2D::DrawMesh(m_Array);

	planetShader->UnBind();
}

void Planet::Render(const Hazel::Camera& camera)
{
	HZ_ASSERT(false, "Batch rendering not supported!");
}

void Planet::Update(Hazel::Timestep ts, World& world)
{

}

float Planet::GetMass() const
{
	return m_Mass;
}
