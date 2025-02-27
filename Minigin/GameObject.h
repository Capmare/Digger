#pragma once
#include <memory>
#include <vector>
#include "Transform.h"
#include "BaseComponent.h"


namespace dae
{
	class Texture2D;

	enum class TransformUpdate
	{
		KeepWorldPosition,
		InheritParentWorldPosition
	};


	// todo: this should become final.
	class GameObject final
	{
	public:
		GameObject();
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update(const float deltaTime);
		void FixedUpdate(const float fixedDeltaTime);
		void Render() const;

		
		template <typename T, typename... Args> T* CreateComponent(Args&&... args)
		{
			static_assert(std::is_base_of<BaseComponent, T>::value, "T must derive from BaseComponent");
			GameObject* self = this;
			std::unique_ptr<BaseComponent> NewComp = std::make_unique<T>(self,std::forward<Args>(args)...);
			m_Components.emplace_back(std::move(NewComp));
			return static_cast<T*>(m_Components.back().get());
		};

		void AddComponent(std::unique_ptr<BaseComponent>&& NewComponent);


		// Unregister the component so next frame can be deleted after it updates
		bool UnregisterComponent(BaseComponent* component);
		void UnregisterComponentAtIndex(unsigned int idx);
		
		BaseComponent* GetComponentAtIndex(unsigned int idx);
		
		// Get all components of one singular type
		template <typename T>
		std::vector<T*> GetAllComponentsOfType() const
		{
			std::vector<T*> ComponentVector;
			if (m_Components.empty()) return ComponentVector;

			ComponentVector.reserve(m_Components.size());
			for (const auto& Comp : m_Components)
			{
				if (typeid(*Comp) == typeid(T)) // There is no way to avoid using typeid since i am using templates without constructing the object.
				{
					ComponentVector.push_back(static_cast<T*>(Comp.get()));
				}
			}
			return ComponentVector;
		}

		void SetParent(GameObject* NewParent, bool bKeepWorldPosition);
		GameObject* GetParent() const { return m_Parent; }

		void SetLocalPosition(const glm::vec3& pos);
		void SetLocalPosition(int x, int y);
		dae::Transform GetWorldTransform() const { return m_WorldTransform; }
		dae::Transform GetLocalTransform() const { return m_LocalTransform; }
		const glm::vec3& GetWorldPosition();

		std::string m_Name{};
		void RemoveSelfFromParent();

	private:
		bool DeleteUnregisteredComponents();

		void RemoveChildFromParent(GameObject* Child);
		void AddChildToParent(GameObject* Child);
		bool IsChild(GameObject* Child);

		void UpdateWorldPosition();

		void SetPositionDirty() { m_bPositionDirty = true; }


		Transform m_LocalTransform{};
		Transform m_WorldTransform{};

		GameObject* m_Parent{};

		std::vector<GameObject*> m_ChildObjects{};
		std::vector<std::unique_ptr<BaseComponent>> m_Components{};
		std::vector<std::unique_ptr<BaseComponent>> m_UnregisteredComponents{};

		bool m_bPositionDirty{ false };
	};
}
