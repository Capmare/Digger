// ProjectileAndFireballShooter.h
#pragma once

#if defined(_WIN32) && !defined(NOMINMAX)
#define NOMINMAX
#endif

#include <BaseComponent.h>
#include <GameObject.h>
#include <Scene.h>
#include <glm.hpp>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>

#include "DamageComponent.h"
#include "SceneManager.h"
#include "Renderer.h"
#include <SDL.h> // change to <SDL2/SDL.h> if your setup uses that

namespace dae
{

	void DrawFilledCircle(SDL_Renderer* r, int cx, int cy, int radius);


	class ProjectileComponent final : public BaseComponent
	{
	public:
		ProjectileComponent(GameObject* owner,
			const glm::vec2& dir,
			float speed,
			float lifeSec)
			: BaseComponent(owner)
			, m_Dir(dir)
			, m_Speed(speed)
			, m_Life(lifeSec)
		{
		}

		void Update(float dt) override;

		void Render() const override;

		static GameObject* Spawn(
			const glm::vec3& pos,
			const glm::vec2& dir,
			float speed,
			float lifeSec);

		void SetVisualRadius(float px) { m_VisualRadius = px; }
		void SetLifeFadeWindow(float seconds) { m_LifeFade = seconds; }

	private:
		glm::vec2 m_Dir{};
		float m_Speed{};
		float m_Life{};
		float m_VisualRadius{ 6.0f };  
		float m_LifeFade{ 0.35f };     
	};


	class FireballShooterComponent final : public BaseComponent
	{
	public:
		FireballShooterComponent(GameObject* Owner,
			float CooldownSec,
			float ProjectileSpeed,
			float ProjectileLifeSec,
			float SpawnOffsetPx,
			glm::ivec4 DamageRectPx,
			std::vector<GameObject*>* DamageTargets)
			: BaseComponent(Owner)
			, m_Cooldown(CooldownSec)
			, m_Speed(ProjectileSpeed)
			, m_Life(ProjectileLifeSec)
			, m_SpawnOffset(SpawnOffsetPx)
			, m_DamageRect(DamageRectPx)
			, m_Targets(DamageTargets)
		{
		}

		void Update(float dt) override;

		void TryFire(const glm::vec2& aimDir);

		bool HasCharge() const;
		float GetCooldownRemaining() const;

		void SetTargets(std::vector<GameObject*>* targets) { m_Targets = targets; }
		void SetCooldown(float s);
		void SetProjectileSpeed(float s) { m_Speed = s; }
		void SetProjectileLife(float s) { m_Life = s; }
		void SetSpawnOffset(float px) { m_SpawnOffset = px; }
		void SetDamageRect(glm::ivec4 r) { m_DamageRect = r; }
		void SetVisualRadiusPx(float px) { m_VisRadiusPx = px; }

	private:
		void Normalize(glm::vec2& v);

		float m_Cooldown{ 0.45f };
		float m_Remaining{ 0.0f };
		float m_Speed{ 16.0f };
		float m_Life{ 1.6f };
		float m_SpawnOffset{ 12.0f };
		glm::ivec4 m_DamageRect{ -8, -8, 16, 16 };
		std::vector<GameObject*>* m_Targets{}; 
		float m_VisRadiusPx{ 6.0f };
	};
}
