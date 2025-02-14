#pragma once
#include <string>

namespace dae
{
	class BaseComponent
	{
	public:
		BaseComponent();
		~BaseComponent() = default;
		
		BaseComponent(const BaseComponent&) = delete;
		BaseComponent(BaseComponent&&) noexcept = delete;
		BaseComponent& operator=(const BaseComponent&) = delete;
		BaseComponent& operator=(BaseComponent&&) noexcept = delete;

		virtual void Update(const float deltaTime);
		virtual void FixedUpdate(const float fixedDeltaTime);

		
		const std::string GetType() const { return TYPE; }
	protected:

	private:
		const std::string TYPE;
	};
}


