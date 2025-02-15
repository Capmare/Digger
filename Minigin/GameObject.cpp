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

	for (const auto& components : m_Components)
	{
		components->Render();
	}

}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::AddComponent(std::unique_ptr<BaseComponent>&& NewComponent)
{
	m_Components.emplace_back(std::move(NewComponent));
}

bool dae::GameObject::UnregisterComponent(const std::unique_ptr<BaseComponent>& Component)
{
	auto it = std::find_if(m_Components.begin(), m_Components.end(),
		[&](const std::unique_ptr<BaseComponent>& comp) { return comp.get() == Component.get(); }
	);

	if (it != m_Components.end())
	{
		m_UnregisteredComponents.emplace_back(std::move(*it));
		m_Components.erase(it);
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

dae::BaseComponent* dae::GameObject::GetComponentAtIndex(unsigned int idx)
{
	assert(idx < m_Components.size() && "Index out of scope");

	return m_Components[idx].get();
}

bool dae::GameObject::DeleteUnregisteredComponents()
{
	// avoid clearing if the vector is empty
	if (m_UnregisteredComponents.empty()) return true;

	m_UnregisteredComponents.clear();
	m_UnregisteredComponents.shrink_to_fit(); // this should free memory in case there is any problems
	return m_UnregisteredComponents.empty();
}
