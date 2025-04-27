#pragma once
#include <memory>
#include <string>


namespace dae
{
	struct SoundJob
	{
		std::string soundFile;
		int volume;
	};

	class SoundSystem
	{
	public:
		SoundSystem();
		virtual ~SoundSystem() = default;

		virtual void PlaySound(const std::string& SoundFile, int Volume = 128) = 0;
	};

	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem() override;

		void PlaySound(const std::string& SoundFile, int Volume = 128) override;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}
