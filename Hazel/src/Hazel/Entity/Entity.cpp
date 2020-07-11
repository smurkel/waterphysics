#include "hzpch.h"
#include "Entity.h"

namespace Hazel
{
	void Entity::Render()
	{
		Renderer::Submit(m_Shader, m_Model->GetVA(), m_Model->GetTexture());
	}

	void Entity::Render(glm::mat4& transform)
	{
		Renderer::Submit(m_Shader, m_Model->GetVA(), m_Model->GetTexture(), transform);
	}

	EntityStack::EntityStack()
	{
	}

	EntityStack::~EntityStack()
	{
		for (Entity* entity : m_Entities)
			delete entity;
	}

	void EntityStack::Add(Entity* entity)
	{
		m_Entities.emplace_back(entity);
	}

	void EntityStack::Delete(Entity* entity)
	{
		auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
		if (it != m_Entities.end())
		{
			m_Entities.erase(it);
		}
	}
}