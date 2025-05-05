#pragma once
#include "BaseComponent.h"
#include <glm.hpp>
#include <vector>
#include "GameObject.h"

namespace dae
{
	struct BoundingBox
	{
		glm::vec2 Minpoint; 
		glm::vec2 Maxpoint;
		float BoxHalfWidth; 
		float BoxHalfHeight; 
	};

	class CollisionComponent final : public BaseComponent
	{
	public:
		CollisionComponent(GameObject* Owner, const BoundingBox& Box) : BaseComponent(Owner), m_Box{Box} {};
		~CollisionComponent() = default;
		
		CollisionComponent(const CollisionComponent&) = delete;
		CollisionComponent(CollisionComponent&&) noexcept = delete;
		CollisionComponent& operator=(const CollisionComponent&) = delete;
		CollisionComponent& operator=(CollisionComponent&&) noexcept = delete;

		

		void AddActors(const std::vector<GameObject*>& NewActors)
		{
			m_WorldActors.resize(m_WorldActors.size() + NewActors.size());
			for (auto& actor : NewActors)
			{
				if (actor->HasComponentOfType<dae::CollisionComponent>())
				{
					m_WorldActors.emplace_back(actor);
				}
			}
		}

		void AddActor(GameObject* NewActor)
		{
			m_WorldActors.emplace_back(NewActor);
		}

	private:
		std::vector<GameObject*> m_WorldActors{};
		bool CheckBoxOverLap(BoundingBox Box1, BoundingBox Box2)
		{
			if (Box1.Maxpoint.x < Box2.Minpoint.x ||
				Box1.Minpoint.x > Box2.Maxpoint.x) {
				return false;
			}
			if (Box1.Maxpoint.y < Box2.Minpoint.y ||
				Box1.Minpoint.y > Box2.Maxpoint.y) {
				return false;
			}
			return true;
		}

		void Update(const float) override {}
		void FixedUpdate(const float) override {}
		void Render() const override {}
		

		BoundingBox m_Box{};
	};
}

