#pragma once
#include <memory>
#include <vector>
#include "Transform.h"
#include "BaseComponent.h"


namespace dae
{
	class Texture2D;

	// todo: this should become final.
	class GameObject final
	{
	public:
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Update(const float deltaTime);
		virtual void FixedUpdate(const float fixedDeltaTime);
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);
		
		template <typename T, typename... Args> T* CreateComponent(Args&&... args)
		{
			 static_assert(std::is_base_of<BaseComponent, T>::value, "T must derive from BaseComponent");

			m_Components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
			return static_cast<T*>(m_Components.back().get());
		};

		void AddComponent(std::unique_ptr<BaseComponent>&& NewComponent);


		// Unregister the component so next frame can be deleted after it updates
		bool UnregisterComponent(const std::unique_ptr<BaseComponent>& Component);
		void UnregisterComponentAtIndex(unsigned int idx);
		
		BaseComponent* GetComponentAtIndex(unsigned int idx);
		
		// Get all components of one singular type
		template <typename T>
		std::vector<T*> GetAllComponentsOfType()
		{
			std::vector<T*> ComponentVector;
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



	private:
		bool DeleteUnregisteredComponents();

		Transform m_transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::shared_ptr<Texture2D> m_texture{};

		std::vector<std::unique_ptr<BaseComponent>> m_Components{};
		std::vector<std::unique_ptr<BaseComponent>> m_UnregisteredComponents{};
	};
}
