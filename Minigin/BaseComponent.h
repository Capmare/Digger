#pragma once

namespace dae
{
	class BaseComponent
	{
	public:
		BaseComponent() = default;
		~BaseComponent() = default;
		
		BaseComponent(const BaseComponent&) = delete;
		BaseComponent(BaseComponent&&) noexcept = delete;
		BaseComponent& operator=(const BaseComponent&) = delete;
		BaseComponent& operator=(BaseComponent&&) noexcept = delete;

		virtual void Update(const float deltaTime);
		virtual void FixedUpdate(const float fixedDeltaTime);

		
	protected:

	private:
		
	};
}


