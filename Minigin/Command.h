#pragma once
#include "GameObject.h"
#include "MovementComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"


namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;

		virtual void Exec(GameObject& GO) = 0;


	};


}

