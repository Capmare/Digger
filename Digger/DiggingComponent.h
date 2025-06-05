#pragma once
#include <BaseComponent.h>
#include "AnimControllerComponent.h"
#include "GameObject.h"

namespace dae
{
	class DiggingComponent :
		public BaseComponent
	{
	public:
		DiggingComponent(GameObject* Owner, class GridComponent* GridComp) : BaseComponent(Owner), m_Grid(GridComp) {

			m_AnimControllerComp = GetOwner()->GetFirstComponentOfType<AnimControllerComponent>();
			const AnimationState* CurrentStateAnim = m_AnimControllerComp->GetCurrentState();
			m_TextureResolution = CurrentStateAnim->GetFlipBook()->GetUsedTexture()->GetTextureResolution();

		};
		virtual ~DiggingComponent() = default;
		
		DiggingComponent(const DiggingComponent&) = delete;
		DiggingComponent(DiggingComponent&&) noexcept = delete;
		DiggingComponent& operator=(const DiggingComponent&) = delete;
		DiggingComponent& operator=(DiggingComponent&&) noexcept = delete;


		void DigRight(const bool bInvert);
		void DigUp(const bool bInvert);

	private:
		class GridComponent* m_Grid{};
		glm::ivec2 m_TextureResolution{};
		AnimControllerComponent* m_AnimControllerComp{};
	};

}

