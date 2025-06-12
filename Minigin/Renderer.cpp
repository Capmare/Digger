#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl2.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	if (ImGui::GetDrawData())
	{
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	}
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect& DstRect, const SDL_Rect& SrcRect) const
{
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &SrcRect, &DstRect);
}

void dae::Renderer::RenderMaskedTexture(SDL_Texture* source, SDL_Texture* mask, const SDL_Rect& dstRect)
{
	int w = dstRect.w;
	int h = dstRect.h;

	SDL_Texture* target = SDL_CreateTexture(
		m_renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		w,
		h
	);

	if (!target)
	{
		SDL_Log("Failed to create render target: %s", SDL_GetError());
		return;
	}

	SDL_Texture* previousTarget = SDL_GetRenderTarget(m_renderer);

	SDL_SetRenderTarget(m_renderer, target);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	SDL_RenderClear(m_renderer);

	SDL_SetTextureBlendMode(mask, SDL_BLENDMODE_MOD);
	SDL_RenderCopy(m_renderer, mask, nullptr, nullptr);

	SDL_SetTextureBlendMode(source, SDL_BLENDMODE_MOD);
	SDL_RenderCopy(m_renderer, source, nullptr, nullptr);

	SDL_SetRenderTarget(m_renderer, previousTarget);
	SDL_RenderCopy(m_renderer, target, nullptr, &dstRect);

	SDL_DestroyTexture(target);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }

void dae::Renderer::DrawPoint(int x, int y, int size)
{
	SDL_Rect rect;
	rect.x = x - size / 2;
	rect.y = y - size / 2;
	rect.w = size;
	rect.h = size;

	SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 1);
	SDL_RenderFillRect(m_renderer, &rect);
}

SDL_Color dae::Renderer::ReadPixelColor(SDL_Texture* texture, int x, int y)
{
	if (!texture) return SDL_Color{ 0, 0, 0, 0 };

	void* pixels = nullptr;
	int pitch = 0;

	if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
		SDL_Log("Failed to lock texture: %s", SDL_GetError());
		return SDL_Color{ 0, 0, 0, 0 };
	}

	int texW = 0, texH = 0;
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
	if (x < 0 || y < 0 || x >= texW || y >= texH) {
		SDL_UnlockTexture(texture);
		return SDL_Color{ 0, 0, 0, 0 };
	}

	Uint32* pixelData = static_cast<Uint32*>(pixels);
	int index = y * (pitch / sizeof(Uint32)) + x;
	Uint32 pixel = pixelData[index];

	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	SDL_Color color;
	SDL_GetRGBA(pixel, format, &color.r, &color.g, &color.b, &color.a);
	SDL_FreeFormat(format);

	SDL_UnlockTexture(texture);
	return color;
}
