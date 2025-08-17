#include "ProjectileComponent.h"


void dae::FireballShooterComponent::Update(float dt)
{
	if (m_Remaining > 0.0f) m_Remaining -= dt;
}

void dae::FireballShooterComponent::TryFire(const glm::vec2& aimDir)
{
	if (!GetOwner() || !HasCharge()) return;

	glm::vec2 dir = aimDir;
	Normalize(dir);

	const auto p = GetOwner()->GetLocalTransform().m_position;
	const glm::vec3 spawnPos{
		p.x + dir.x * m_SpawnOffset,
		p.y + dir.y * m_SpawnOffset,
		p.z
	};

	GameObject* proj = ProjectileComponent::Spawn(spawnPos, dir, m_Speed, m_Life);
	if (!proj) return;

	if (auto* pc = proj->GetFirstComponentOfType<ProjectileComponent>())
		pc->SetVisualRadius(m_VisRadiusPx);

	auto* dmg = proj->CreateComponent<dae::DamageComponent>(m_DamageRect);
	dmg->bDoesDamageWithoutCondition = true;

	if (m_Targets)
	{
		for (GameObject* target : *m_Targets)
		{
			if (target && target != GetOwner())
				dmg->AddNewDamageRecevingActor(target);
		}
	}

	m_Remaining = m_Cooldown;
}

bool dae::FireballShooterComponent::HasCharge() const
{
	return m_Remaining <= 0.0f;
}

float dae::FireballShooterComponent::GetCooldownRemaining() const
{
	return (std::max)(0.0f, m_Remaining);
}

void dae::FireballShooterComponent::SetCooldown(float s)
{
	m_Cooldown = s; if (m_Remaining > m_Cooldown) m_Remaining = m_Cooldown;
}

void dae::FireballShooterComponent::Normalize(glm::vec2& v)
{
	const float lsq = v.x * v.x + v.y * v.y;
	if (lsq < 1e-6f) v = { 1.0f, 0.0f };
	else v *= (1.0f / std::sqrt(lsq));
}

void dae::ProjectileComponent::Update(float dt)
{
	auto* owner = GetOwner();
	if (!owner ) return;  

	const auto cur = owner->GetLocalTransform().m_position;
	const glm::vec3 next{ cur.x + m_Dir.x * m_Speed * dt,
						  cur.y + m_Dir.y * m_Speed * dt,
						  cur.z };
	owner->SetLocalPosition(next);

	if (m_Life > 0.0f) m_Life -= dt;
	if (m_Life <= 0.0f && owner) owner->Destroy();    
}

void dae::ProjectileComponent::Render() const
{
	auto* owner = GetOwner();
	if (!owner) return;

	const auto wp = owner->GetWorldPosition();
	const int cx = static_cast<int>(std::lround(wp.x));
	const int cy = static_cast<int>(std::lround(wp.y));

	SDL_Renderer* sdl = Renderer::GetInstance().GetSDLRenderer();
	if (!sdl) return;

	Uint8 alpha = 255;
	if (m_LifeFade > 0.0f)
	{
		const float t = std::clamp(m_Life / m_LifeFade, 0.0f, 1.0f);
		alpha = static_cast<Uint8>(255.0f * (0.5f + 0.5f * t));
	}

	SDL_SetRenderDrawBlendMode(sdl, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(sdl, 255, 160, 64, alpha);
	DrawFilledCircle(sdl, cx, cy, static_cast<int>(m_VisualRadius));
}

dae::GameObject* dae::ProjectileComponent::Spawn(const glm::vec3& pos, const glm::vec2& dir, float speed, float lifeSec)
{
	auto& scene = dae::SceneManager::GetInstance().GetLastScene();
	auto* go = scene.CreateNewGameObject();
	if (!go) return nullptr;

	go->m_Name = "Fireball";
	go->SetLocalPosition(pos);
	go->CreateComponent<ProjectileComponent>(dir, speed, lifeSec);
	return go;
}

void dae::DrawFilledCircle(SDL_Renderer* r, int cx, int cy, int radius)
{
	int ox = 0;
	int oy = radius;
	int d = radius - 1;

	while (oy >= ox)
	{
		SDL_RenderDrawLine(r, cx - oy, cy + ox, cx + oy, cy + ox);
		SDL_RenderDrawLine(r, cx - ox, cy + oy, cx + ox, cy + oy);
		SDL_RenderDrawLine(r, cx - ox, cy - oy, cx + ox, cy - oy);
		SDL_RenderDrawLine(r, cx - oy, cy - ox, cx + oy, cy - ox);

		if (d >= 2 * ox) { d -= 2 * ox + 1; ++ox; }
		else if (d < 2 * (radius - oy)) { d += 2 * oy - 1; --oy; }
		else { d += 2 * (oy - ox - 1); --oy; ++ox; }
	}
}
