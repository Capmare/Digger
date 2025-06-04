#pragma once
#include <string>
#include <typeinfo>

namespace dae
{
	class GameObject;
	class BaseComponent
	{
	public:
		BaseComponent(GameObject* Owner) : m_Owner{ Owner } {};
		virtual ~BaseComponent() = default;
		
		BaseComponent(const BaseComponent&) = delete;
		BaseComponent(BaseComponent&&) noexcept = delete;
		BaseComponent& operator=(const BaseComponent&) = delete;
		BaseComponent& operator=(BaseComponent&&) noexcept = delete;

		virtual void Update(const float deltaTime);
		virtual void FixedUpdate(const float fixedDeltaTime);
		virtual void Render() const;
		
		
		GameObject* GetOwner() const { return m_Owner; }

	private:
		GameObject* m_Owner{};

	};
}


