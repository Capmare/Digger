#pragma once
#include "BaseComponent.h"
#include "glm.hpp"
#include "GameObject.h"

namespace dae
{
	class RotatingComponent final: public BaseComponent

	{
	public:
		RotatingComponent(GameObject* Owner) : BaseComponent(Owner) {};
		virtual ~RotatingComponent() = default;

		RotatingComponent(const RotatingComponent&) = delete;
		RotatingComponent(RotatingComponent&&) noexcept = delete;
		RotatingComponent& operator=(const RotatingComponent&) = delete;
		RotatingComponent& operator=(RotatingComponent&&) noexcept = delete;

		void Update(const float deltaTime) override;

		float m_RotationSpeed{500.f};
		float m_Radius{ 20.f };
	protected:
	private:
		glm::vec3 RotateAroundPoint(const glm::vec3& RotatingPoint, const glm::vec3& Center, float RotationAngle);
		float m_CurrentAngle{};
		glm::vec3 m_MidPoint{};
		
	};
}


