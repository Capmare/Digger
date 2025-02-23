#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"

dae::GameObject::~GameObject() = default;

dae::GameObject::GameObject()
{
	if (m_Parent)
	{
		m_WorldTransform.m_position = m_LocalTransform.m_position + m_Parent->GetLocalTransform().m_position;
	}
	else
	{
		m_WorldTransform.m_position = m_LocalTransform.m_position;
	}
}

void dae::GameObject::Update( const float deltaTime)
{
	DeleteUnregisteredComponents();
	for (const auto& components : m_Components)
	{
		components->Update(deltaTime);
	}


}

void dae::GameObject::FixedUpdate( const float fixedDeltaTime)
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
	GetLocalTransform().m_position = {x, y, 0.0f};
}

void dae::GameObject::AddComponent(std::unique_ptr<BaseComponent>&& NewComponent)
{
	m_Components.emplace_back(std::move(NewComponent));
}

bool dae::GameObject::UnregisterComponent(BaseComponent* component)
{
	auto it = std::find_if(m_Components.begin(), m_Components.end(),
		[&](const std::unique_ptr<BaseComponent>& comp) { return comp.get() == component; }
	);

	if (it != m_Components.end())
	{
		m_UnregisteredComponents.emplace_back(std::move(*it));

		return true;
	}

	return false;
}


void dae::GameObject::UnregisterComponentAtIndex(unsigned int idx)
{
	assert(idx < m_Components.size() && "Index out of scope");

	m_UnregisteredComponents.emplace_back(std::move(m_Components[idx]));
}

dae::BaseComponent* dae::GameObject::GetComponentAtIndex(unsigned int idx)
{
	assert(idx < m_Components.size() && "Index out of scope");

	return m_Components[idx].get();
}

void dae::GameObject::RemoveChildFromParent(GameObject* Child)
{
	assert(Child != nullptr && "Child should not be nullptr");

	auto it = std::find_if(m_ChildObjects.begin(), m_ChildObjects.end(),
		[&](const std::unique_ptr<GameObject>& comp) { return comp.get() == Child; }
	);

	assert(it != m_ChildObjects.end() && "Child object does not exist in vector");

	if (it != m_ChildObjects.end())
	{
		m_ChildObjects.erase(it);
	}


}

void dae::GameObject::AddChildToParent(GameObject* Child)
{
	m_ChildObjects.emplace_back(Child);
}

void dae::GameObject::SetNewParent(GameObject* NewParent)
{
	assert(NewParent != nullptr && "NewParent can't be nullptr");
	if (NewParent != m_Parent) return; // check that parent is not the same
	if (m_Parent) // check if new parent valid
	{
		m_Parent->RemoveChildFromParent(this); // remove itself from old parent
		m_Parent = NewParent; // set new parent
		m_Parent->AddChildToParent(this);
		this->m_WorldTransform = m_Parent->GetLocalTransform();
	}
}

bool dae::GameObject::DeleteUnregisteredComponents()
{
	if (m_UnregisteredComponents.empty()) return true;

	// Remove null pointers from m_Components
	m_Components.erase(
		std::remove_if(
			m_Components.begin(), m_Components.end(),
			[](const std::unique_ptr<BaseComponent>& ptr) { return ptr == nullptr; }),
		m_Components.end()
	);

	// Clear the unregistered list
	m_UnregisteredComponents.clear();
	return m_UnregisteredComponents.empty();
}

