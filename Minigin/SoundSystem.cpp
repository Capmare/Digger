#include "SoundSystem.h"
#include <iostream>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "..\SDL_mixer\Include\SDL_mixer.h"

namespace dae
{
	class SDLSoundSystem::Impl
	{
	public:
		Impl()
		{
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			{
				std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
			}

			m_WorkerThread = std::thread(&Impl::ProcessQueue, this);
		}

		~Impl()
		{
			{
				std::lock_guard<std::mutex> lock(m_QueueMutex);
				m_Running = false;
			}
			m_Condition.notify_all();

			if (m_WorkerThread.joinable())
				m_WorkerThread.join();

			for (auto& [str, chunk] : m_SoundCache)
			{
				Mix_FreeChunk(chunk);
			}
			Mix_CloseAudio();
		}

		void PlaySound(const std::string& soundFile, int volume)
		{
			Mix_Chunk* chunk = nullptr;
			{
				std::lock_guard<std::mutex> lock(m_QueueMutex);

				auto it = m_SoundCache.find(soundFile);
				if (it != m_SoundCache.end())
				{
					chunk = it->second;
				}
				else
				{
					chunk = Mix_LoadWAV(soundFile.c_str());
					if (!chunk)
					{
						std::cerr << "Failed to load sound: " << soundFile << ", Error: " << Mix_GetError() << std::endl;
						return;
					}
					m_SoundCache[soundFile] = chunk;
				}

				m_SoundQueue.emplace(SoundJob{ chunk, volume });
			}

			m_Condition.notify_one();
		}

	private:
		struct SoundJob
		{
			Mix_Chunk* chunk;
			int volume;
		};

		void ProcessQueue()
		{
			// use while true here so that it always waits for the next sound, otherwise thread is dead
			while (true)
			{
				SoundJob job{};

				{
					std::unique_lock<std::mutex> lock(m_QueueMutex);
					m_Condition.wait(lock, [this]() { return !m_SoundQueue.empty() || !m_Running; });

					if (!m_Running && m_SoundQueue.empty())
						break;

					if (!m_SoundQueue.empty())
					{
						job = m_SoundQueue.front();
						m_SoundQueue.pop();
					}
					else
					{
						continue;
					}
				}


				if (job.chunk != nullptr)
				{
					Mix_VolumeChunk(job.chunk, job.volume);
					Mix_PlayChannel(-1, job.chunk, 0);
				}
			}
		}

		std::unordered_map<std::string, Mix_Chunk*> m_SoundCache;

		std::queue<SoundJob> m_SoundQueue;
		std::mutex m_QueueMutex;
		std::condition_variable m_Condition;
		std::thread m_WorkerThread;
		bool m_Running{ true };
	};

	SDLSoundSystem::SDLSoundSystem() : m_pImpl{ std::make_unique<Impl>() }
	{
	}

	SDLSoundSystem::~SDLSoundSystem() = default;

	void SDLSoundSystem::PlaySound(const std::string& soundFile, int volume)
	{
		m_pImpl->PlaySound(soundFile, volume);
	}

	void SDLSoundSystem::StopAllSound()
	{
		Mix_HaltChannel(-1);
	}

	SoundSystem::SoundSystem() = default;

}
