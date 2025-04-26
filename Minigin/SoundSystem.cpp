#include "SoundSystem.h"
#include <iostream>

#include <unordered_map>
#include "..\SDL_mixer\Include\SDL_mixer.h"

namespace dae
{
	class SDLSoundSystem::Impl
	{
	public:
		Impl()
		{

		}

		~Impl()
		{

		}

		void PlaySound(const std::string& soundFile, int volume)
		{

		}

	private:
		std::unordered_map<std::string, Mix_Chunk*> m_SoundCache;
	};

	SDLSoundSystem::SDLSoundSystem() : m_pImpl{ std::make_unique<Impl>() }
	{
	}

	SDLSoundSystem::~SDLSoundSystem() = default;

	void SDLSoundSystem::PlaySound(const std::string& soundFile, int volume)
	{
		m_pImpl->PlaySound(soundFile, volume);
	}

	SoundSystem::SoundSystem() = default;

}