#include "Planet.h"

#include "world/World.h"

const int CIRCLE_POINTS = 500;

Planet::Planet(World& world, float radius, glm::vec4 groundColor, glm::vec4 skyColor) : m_Array(Hazel::VertexArray::Create())
{
	b2BodyDef def;
	def.position = b2Vec2(0.0f, 0.0f);
	def.userData = this;
	def.type = b2_dynamicBody;
	def.active = true;
	b2Body* body = world.GetWorld()->CreateBody(&def);


	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.1f;

	b2CircleShape circle;
	circle.m_radius = radius;
	fixtureDef.shape = &circle;

	body->CreateFixture(&fixtureDef);

	std::vector<uint32_t> indices;
	std::vector<Hazel::Renderer2D::VertexData> vertices(CIRCLE_POINTS + 1);
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
	vb->SetLayout(Hazel::Renderer2D::Layout);
	m_Array->SetIndexBuffer(ib);
	m_Array->AddVertexBuffer(vb);
}

void Planet::Render(World& world)
{
	Hazel::Renderer2D::DrawMesh(m_Array);
}

void Planet::Update(Hazel::Timestep ts, World& world)
{

}
