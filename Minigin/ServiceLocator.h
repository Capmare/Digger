#pragma once

#include <memory>
#include "SoundSystem.h"



namespace dae
{
	class ServiceLocator
	{
	public:
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
		{
			m_SoundSystem = std::move(soundSystem);
		}

		static SoundSystem& GetSoundSystem()
		{
			if (!m_SoundSystem)
			{
				static NullSoundSystem nullSoundSystem;
				return nullSoundSystem;
			}
			return *m_SoundSystem;
		}

	private:
		class NullSoundSystem : public SoundSystem
		{
		public:
			void PlaySound(const std::string&, int) override
			{
				// invalid sound system
			}
		};

		static inline std::unique_ptr<SoundSystem> m_SoundSystem{ nullptr };
	};
}
