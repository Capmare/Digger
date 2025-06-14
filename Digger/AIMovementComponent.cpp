#include "AIMovementComponent.h"
#include "AnimControllerComponent.h"
#include "AIGridComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "MapComponent.h"

void dae::AIMovementComponent::FixedUpdate(const float fixedDeltaTime)
{

	m_StateChangeTimer += fixedDeltaTime;

	// Handle state switching
	if (m_IsInAlternateState)
	{
		m_AlternateStateTimer += fixedDeltaTime;
		if (m_AlternateStateTimer >= 5.0f)
		{
			m_AnimControllerComp->ChangeState(m_DefaultState);
			RecreatePath();
			m_PassedTime = 0.0f;

			m_IsInAlternateState = false;
			m_StateChangeTimer = 0.0f;
			m_NextStateChangeTime = 10.0f + static_cast<float>(rand() % 11); // 10–20s
		}
	}
	else if (m_StateChangeTimer >= m_NextStateChangeTime)
	{
		m_AnimControllerComp->ChangeState(m_AlternateState);
		RecreatePath();
		m_PassedTime = 0.0f;

		m_IsInAlternateState = true;
		m_AlternateStateTimer = 0.0f;
	}

	// Path refresh timer
	m_PassedTime += fixedDeltaTime;
	if (m_PassedTime > m_MaxTime)
	{
		RecreatePath();
		m_PassedTime = 0.0f;
	}

	if (m_Destination.empty()) return;

	if (!m_MovingToTarget)
	{
		if (m_Destination.empty()) return;

		m_StartPos = glm::vec2(GetOwner()->GetWorldTransform().m_position);
		m_TargetPos = m_Destination.front();
		glm::vec2 segment = m_TargetPos - m_StartPos;
		m_SegmentDuration = glm::length(segment) / speed;

		m_MoveLerpTime = 0.0f;
		m_MovingToTarget = true;
	}

	m_MoveLerpTime += fixedDeltaTime;
	float t = glm::clamp(m_MoveLerpTime / m_SegmentDuration, 0.0f, 1.0f);
	glm::vec2 newPos2D = glm::mix(m_StartPos, m_TargetPos, t);

	GetOwner()->SetLocalPosition(glm::vec3(newPos2D, 0.0f));

	if (m_IsInAlternateState)
	{
		m_MapComponent->ClearTunnelArea(newPos2D + glm::vec2{ 10.f, 10.f }, 10);
	}

	if (t >= 1.0f)
	{
		if (!m_Destination.empty())
			m_Destination.erase(m_Destination.begin());

		m_MovingToTarget = false;

		if (m_Destination.empty())
		{
			m_MovingToTarget = false;
		}
	}
}

void dae::AIMovementComponent::Render() const
{
	// auto renderer = Renderer::GetInstance().GetSDLRenderer();
	// if (m_Destination.empty() || !renderer)
	// 	return;
	// 
	// constexpr int halfLength = 6;
	// 
	// SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
	// 
	// for (const auto& point : m_Destination)
	// {
	// 	int x = static_cast<int>(point.x);
	// 	int y = static_cast<int>(point.y);
	// 
	// 	SDL_RenderDrawLine(renderer, x - halfLength, y, x + halfLength, y);
	// 
	// 	SDL_RenderDrawLine(renderer, x, y - halfLength, x, y + halfLength);
	// }
}

void dae::AIMovementComponent::RecreatePath()
{
	if (!m_Player || !GetOwner() || !m_GridComponent || !m_AnimControllerComp)
		return;

	m_Destination.clear();

	glm::vec2 aiPos = glm::vec2(GetOwner()->GetWorldPosition());
	glm::vec2 playerPos = m_Player->GetWorldPosition();

	// Slight offset to avoid corners
	playerPos += glm::vec2{ 10.0f, 10.0f };

	const std::string& stateName = m_AnimControllerComp->GetCurrentState()->GetStateName();

	if (stateName == "Nobbin")
	{
		m_Destination = m_GridComponent->FindPathFreeTilesFromPixels(aiPos, playerPos);
	}
	else if (stateName == "Hobbin")
	{
		m_Destination = m_GridComponent->FindPathAllMapFromPixels(aiPos, playerPos);
	}

	if (m_Destination.empty())
	{
		std::cout << "cannot find path";
		m_Destination = m_GridComponent->FindPathToRandomFreeTile(aiPos, 1);
	}

}
