#include "RotatingComponent.h"
void dae::RotatingComponent::Update(const float deltaTime)
{
	m_CurrentAngle += m_RotationSpeed * deltaTime;
	m_MidPoint = GetOwner()->GetParent()->GetWorldPosition();

	glm::vec3 newPos = { 
        m_MidPoint.x + cosf(m_CurrentAngle) * m_Radius,
        m_MidPoint.y - sinf(m_CurrentAngle) * m_Radius,
        m_MidPoint.z 
    };

    GetOwner()->SetLocalPosition(newPos - GetOwner()->GetParent()->GetWorldPosition());
	
}

glm::vec3 dae::RotatingComponent::RotateAroundPoint(const glm::vec3& RotatingPoint, const glm::vec3& Center, float RotationAngle)
{
	float angleRad = RotationAngle * (3.1415f / 180.0f);

	float translatedX = RotatingPoint.x - Center.x;
	float translatedY = RotatingPoint.y - Center.y;

	float rotatedX = translatedX * cos(angleRad) - translatedY * sin(angleRad);
	float rotatedY = translatedX * sin(angleRad) + translatedY * cos(angleRad);

	return {rotatedX + Center.x, rotatedY + Center.y,RotatingPoint.z};
}

