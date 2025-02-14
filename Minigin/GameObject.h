#pragma once
#include <memory>
#include <vector>
#include "Transform.h"
#include "BaseComponent.h"


namespace dae
{
	class Texture2D;

	// todo: this should become final.
	class GameObject 
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
		
		template <typename T> std::unique_ptr<T> CreateComponent()
		{
			m_Components.emplace_back(std::make_unique<T>());
			return m_Components.back();
		};

		void AddComponent(std::unique_ptr<BaseComponent>&& NewComponent);


		// Unregister the component so next frame can be deleted after it updates
		bool UnregisterComponent(const std::unique_ptr<BaseComponent>& Component);
		void UnregisterComponentAtIndex(unsigned int idx);
		
		BaseComponent* GetComponentAtIndex(unsigned int idx);
		// implement the template in the header so i don't need to explicitly instantiate the template for each derived class
		template <typename T>
		std::vector<std::unique_ptr<T>> GetAllComponentsOfType()
		{
			std::vector<std::unique_ptr<T>> ComponentVector;
		
			for (const std::unique_ptr<T>& Comp : m_Components)
			{
				//if (typeid(*Comp.lock()) == typeid(T)) // using typeid to avoid using dynamic casting, still RTTI
				//{
				//	ComponentVector.push_back(Comp);
				//}
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
