#pragma once
#include <string>
#include <typeinfo>

namespace dae
{
	class GameObject;
	class BaseComponent
	{
	public:
		BaseComponent(const GameObject* Owner) : m_Owner{ Owner } {};
		virtual ~BaseComponent() = default;
		
		BaseComponent(const BaseComponent&) = delete;
		BaseComponent(BaseComponent&&) noexcept = delete;
		BaseComponent& operator=(const BaseComponent&) = delete;
		BaseComponent& operator=(BaseComponent&&) noexcept = delete;

		virtual void Update(const float deltaTime);
		virtual void FixedUpdate(const float fixedDeltaTime);
		virtual void Render() const;
		

	protected:
		const class GameObject* GetOwner() const { return m_Owner; }
	private:
		const class GameObject* m_Owner{};

	};
}


