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

		virtual void PlaySound(const std::string& SoundFile, int Volume = 30) = 0;
		virtual void StopAllSound() = 0;
	};

	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem() override;

		void PlaySound(const std::string& SoundFile, int Volume = 30) override;
		void StopAllSound() override;
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}
