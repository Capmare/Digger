#pragma once
#include "Observer.h"

namespace dae
{
	// Observer classes start

	class HealthObserver final : public Observer
	{
	public:
		virtual ~HealthObserver() = default;

		void OnNotify(GameObject& GO, const ObserverEvents& Event) override;

	};

	class ScoreObserver final : public Observer
	{
	public:
		virtual ~ScoreObserver() = default;

		void OnNotify(GameObject& GO, const ObserverEvents& Event) override;

	};

	// Observer classes end



}