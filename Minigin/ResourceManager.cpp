#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

namespace fs = std::filesystem;

void dae::ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture(const std::string& file)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	if(m_loadedTextures.find(filename) == m_loadedTextures.end())
		m_loadedTextures.insert(std::pair(filename,std::make_shared<Texture2D>(fullPath.string())));
	return m_loadedTextures.at(filename);
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& file, uint8_t size)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	const auto key = std::pair<std::string, uint8_t>(filename, size);
	if(m_loadedFonts.find(key) == m_loadedFonts.end())
		m_loadedFonts.insert(std::pair(key,std::make_shared<Font>(fullPath.string(), size)));
	return m_loadedFonts.at(key);
}

void dae::ResourceManager::UnloadUnusedResources()
{
	for (auto it = m_loadedTextures.begin(); it != m_loadedTextures.end();)
	{
		if (it->second.use_count() == 1)
			it = m_loadedTextures.erase(it);
		else
			++it;
	}

	for (auto it = m_loadedFonts.begin(); it != m_loadedFonts.end();)
	{
		if (it->second.use_count() == 1)
			it = m_loadedFonts.erase(it);
		else
			++it;
	}
}

SDL_Texture* dae::ResourceManager::MergeTextures(const std::vector<std::unique_ptr<TextureComponent>>& Textures, size_t Width)
{
	if (Textures.empty()) return nullptr;

	size_t height = Textures.size() / Width;

	SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();

	// size of one tile
	int tileW, tileH;
	SDL_QueryTexture(Textures[0]->GetTexture(), nullptr, nullptr, &tileW, &tileH);

	int mergedW = static_cast<int>(Width * tileW);
	int mergedH = static_cast<int>(height * tileH);

	// final texture
	SDL_Texture* mergedTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		mergedW,
		mergedH
	);

	if (!mergedTexture) return nullptr;

	// render target to merged texture
	SDL_SetRenderTarget(renderer, mergedTexture);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	for (size_t idx = 0; idx < Textures.size(); ++idx)
	{
		int row = static_cast<int>(idx / Width);
		int col = static_cast<int>(idx % Width);

		SDL_Texture* tex = Textures[idx]->GetTexture();

		SDL_Rect dstRect{
			col * tileW,
			row * tileH,
			tileW,
			tileH
		};

		SDL_RenderCopy(renderer, tex, nullptr, &dstRect);

	}

	// Reset render target to default
	SDL_SetRenderTarget(renderer, nullptr);

	return mergedTexture;
}


