#pragma once
#include <BaseComponent.h>

namespace dae
{
	class DiggingComponent :
		public BaseComponent
	{
	public:
		DiggingComponent(GameObject* Owner, class GridComponent* GridComp) : BaseComponent(Owner), m_Grid(GridComp) {};
		virtual ~DiggingComponent() = default;
		
		DiggingComponent(const DiggingComponent&) = delete;
		DiggingComponent(DiggingComponent&&) noexcept = delete;
		DiggingComponent& operator=(const DiggingComponent&) = delete;
		DiggingComponent& operator=(DiggingComponent&&) noexcept = delete;


		void DigRight(const bool bInvert);
		void DigUp(const bool bInvert);

	private:
		class GridComponent* m_Grid{};

	};

}

