#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"


dae::GameObject::GameObject()
{
	m_UnregisteredComponents.reserve(10);
};


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
	if (Child == nullptr) return;

	std::erase_if(m_ChildObjects, [&](const auto& comp) { return comp == Child; });
	
}

void dae::GameObject::AddChildToParent(GameObject* Child)
{
	m_ChildObjects.emplace_back(Child);
}

bool dae::GameObject::IsChild(GameObject* Child)
{
	auto it = std::find_if(m_ChildObjects.begin(), m_ChildObjects.end(),
		[&](const auto& ChildComp) { return ChildComp == Child; }
	);

	if (it != m_ChildObjects.end()) return true;
	return false;
}

void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
	m_LocalTransform.m_position = pos;

	SetPositionDirty();
	for (const auto& children : m_ChildObjects)
	{
		children->SetPositionDirty();
	}

}

void dae::GameObject::SetLocalPosition(int x, int y)
{
	SetLocalPosition({ x,y,0.f });
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_bPositionDirty)
	{
		if (m_Parent == nullptr)
		{
			m_WorldTransform.m_position = m_LocalTransform.m_position;
		}
		else
		{
			m_WorldTransform.m_position = m_Parent->GetWorldPosition() + m_LocalTransform.m_position;
		}
	}
	m_bPositionDirty = false;
}

void dae::GameObject::SetParent(GameObject* NewParent, bool bKeepWorldPosition)
{
	if (IsChild(NewParent) || NewParent == this || m_Parent == NewParent) return;
	
	if (NewParent == nullptr)
	{
		SetLocalPosition(GetWorldPosition());
	}
	else
	{
		if (bKeepWorldPosition)
		{
			SetLocalPosition(GetWorldPosition() - NewParent->GetWorldPosition());
		}
		SetPositionDirty();
	}

	if (m_Parent) m_Parent->RemoveChildFromParent(this);
	m_Parent = NewParent;
	if (m_Parent) m_Parent->AddChildToParent(this);

}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_bPositionDirty)
	{
		UpdateWorldPosition();
	}
	return m_WorldTransform.m_position;
}

void dae::GameObject::RemoveSelfFromParent()
{
	SetParent(nullptr, true);
}

bool dae::GameObject::DeleteUnregisteredComponents()
{
	
	if (m_UnregisteredComponents.empty()) return true;

	std::erase_if(m_Components, [](const auto& Component) { return Component.get() == nullptr; });

	m_UnregisteredComponents.clear();
	return m_UnregisteredComponents.empty();
}

