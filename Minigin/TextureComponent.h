#pragma once
#include "BaseComponent.h"
#include <memory>
#include "Transform.h"
#include "Texture2D.h"

namespace dae  {
	class TextureComponent final: public BaseComponent
	{
	public:
		TextureComponent(GameObject* Owner) : BaseComponent(Owner) {};
		virtual ~TextureComponent() = default;
		
		TextureComponent(const TextureComponent&) = delete;
		TextureComponent(TextureComponent&&) noexcept = delete;
		TextureComponent& operator=(const TextureComponent&) = delete;
		TextureComponent& operator=(TextureComponent&&) noexcept = delete;

		void Render() const override;

		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_Texture{};
	};
}


