#pragma once
#include <SDL.h>
#include "Singleton.h"

namespace dae
{
	class Texture2D;

	struct LockedTexture {
		void* pixels = nullptr;
		int pitch = 0;
		int width = 0;
		int height = 0;
		SDL_Texture* texture = nullptr;
	};

	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};


	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, const SDL_Rect& DstRect, const SDL_Rect& SrcRect) const;
		void RenderMaskedTexture(SDL_Texture* source, SDL_Texture* mask, const SDL_Rect& dstRect);

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

		void DrawPoint(int x, int y, int size);

		bool LockTexture(SDL_Texture* texture, LockedTexture& outLocked);
		void UnlockTexture(const LockedTexture& locked);
		SDL_Color ReadPixelColor(SDL_Texture* texture, int x, int y);
		SDL_Color FastReadPixel(const LockedTexture& locked, int x, int y);

	};
}
