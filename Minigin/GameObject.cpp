#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update([[maybe_unused]] const float deltaTime)
{
	for (const auto& components : m_Components)
	{
		components->Update(deltaTime);
	}
	// Delete after last update
	DeleteUnregisteredComponents();
}

void dae::GameObject::FixedUpdate([[maybe_unused]] const float fixedDeltaTime)
{
	for (const auto& components : m_Components)
	{
		components->FixedUpdate(fixedDeltaTime);
	}
}

void dae::GameObject::Render() const
{
	const auto& pos = m_transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::AddComponent(const std::shared_ptr<class BaseComponent>& NewComponent)
{
	m_Components.emplace_back(NewComponent);
}

bool dae::GameObject::UnregisterComponent(const std::shared_ptr<class BaseComponent>& Component)
{
	auto it = std::find(m_Components.begin(), m_Components.end(), Component);
	if (it != m_Components.end())
	{
		m_UnregisteredComponents.emplace_back(std::move(*it));
		size_t idx = it - m_Components.begin();

		assert(idx < m_Components.size() && "Index out of scope");

		it->reset();
		m_Components.erase(m_Components.begin() + idx);


		return true;
	}
	return false;
}

void dae::GameObject::UnregisterComponentAtIndex(unsigned int idx)
{
	assert(idx < m_Components.size() && "Index out of scope");

	m_UnregisteredComponents.emplace_back(std::move(m_Components[idx]));
	// reset the shared ptr so we don t keep the count up
	m_Components[idx].reset();
	m_Components.erase(m_Components.begin() + idx);
}

std::weak_ptr<dae::BaseComponent> dae::GameObject::GetComponentAtIndex(unsigned int idx)
{
	assert(idx < m_Components.size() && "Index out of scope");

	return m_Components[idx];
}

bool dae::GameObject::DeleteUnregisteredComponents()
{
	// avoid clearing if the vector is empty
	if (m_UnregisteredComponents.empty()) return true;

	m_UnregisteredComponents.clear();
	return m_UnregisteredComponents.empty();
}
