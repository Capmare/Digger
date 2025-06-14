#pragma once
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Texture2D.h"
#include <memory>

#pragma region Components
#include "TextureComponent.h"
#include "RotatingComponent.h"
#include "FpsComponent.h"
#include "BaseComponent.h"
#include "MovementComponent.h"
#include "PlayerControllerComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "GameObservers.h"
#include "SoundSystem.h"
#include "ServiceLocator.h"
#include "FlipBookComponent.h"
#include "AnimControllerComponent.h"
#include "MapComponent.h"
#include "DiggingComponent.h"
#include "GravityComponent.h"
#include "DamageComponent.h"
#include "PushComponent.h"
#include "AIGridComponent.h"
#include "AIMovementComponent.h"
#include "SpawnerComponent.h"
#include "CombatInformationComponent.h"
#pragma endregion Components


static dae::Minigin engine("../Data/");


namespace dae
{
	constexpr TileType T = TileType::TunnelTile;
	constexpr TileType D = TileType::DirtTile;

	GameObject* SpawnBagAtlocation(Scene& scene, const glm::ivec2 Location, std::vector<GameObject*>& DamagingContainer, MapComponent* MapComp, std::vector<GameObject*> Digger, SpawnerComponent* SpawnerComp)
	{
		GameObject* Money = scene.CreateNewGameObject();
		FlipBookConfig BagFlipBookConfig{
			"bag.png",
			4,
			20,
			20,
			.2f,
			false,
			1
		};

		FlipBookConfig BagFallingFlipBookConfig{
		"bag.png",
		4,
		20,
		20,
		.2f,
		true,
		1
		};

		FlipBookConfig GoldFlipBookConfig{
			"gold.png",
			3,
			20,
			20,
			.2f,
			false,
			1
		};

		Money->m_Name = "MoneyBag";

		Money->SetLocalPosition(Location.x, Location.y);
		AnimControllerComponent* AnimController = Money->CreateComponent<AnimControllerComponent>();
		AnimController->CreateState<IdleState>(BagFlipBookConfig);
		AnimController->CreateState<FallingState>(BagFallingFlipBookConfig);
		AnimController->CreateState<DestroyedState>(GoldFlipBookConfig);
		AnimController->ChangeState("Idle");


		Money->CreateComponent<GravityComponent>(MapComp);
		DamageComponent* MoneyDamageComp = Money->CreateComponent<DamageComponent>(glm::ivec4{ 0, 0, 20, 20 });
		for (GameObject* Dig : Digger)
		{
			MoneyDamageComp->AddNewDamageRecevingActor(Dig);
		}
		PushComponent* LeftPushComp = Money->CreateComponent<PushComponent>(glm::ivec2{ 2,10 }, 1, 500);
		PushComponent* RightPushComp = Money->CreateComponent<PushComponent>(glm::ivec2{ 18,10 }, -1, 500);
		for (GameObject* Dig : Digger)
		{
			LeftPushComp->AddOtherActors(Dig);
			RightPushComp->AddOtherActors(Dig);
		}


		DamagingContainer.emplace_back(Money);
		if (SpawnerComp)
		{
			Money->CreateComponent<CombatInformationComponent>(SpawnerComp);
		}

		return Money;
	}

	 void SpawnGem(Scene& scene, const glm::ivec2 Location, std::vector<GameObject*> Digger)
	{
		GameObject* Emerald = scene.CreateNewGameObject();
		Emerald->m_Name = "Emerald";

		FlipBookConfig EmeraldFlipBookConfig{
			"emerald.png",
			1,
			20,
			20,
			.2f,
			false,
			1
		};
		AnimControllerComponent* AnimController = Emerald->CreateComponent<AnimControllerComponent>();
		AnimController->CreateState<IdleState>(EmeraldFlipBookConfig);
		AnimController->ChangeState("Idle");

		Emerald->SetLocalPosition(Location.x, Location.y);

		PushComponent* LeftPushComp = Emerald->CreateComponent<PushComponent>(glm::ivec2{ 2,10 }, 1, 25);
		PushComponent* RightPushComp = Emerald->CreateComponent<PushComponent>(glm::ivec2{ 18,10 }, -1, 25);
		LeftPushComp->bIsInstantlyPickup = true;
		RightPushComp->bIsInstantlyPickup = true;
		for (GameObject* Dig : Digger)
		{
			LeftPushComp->AddOtherActors(Dig);
			RightPushComp->AddOtherActors(Dig);
		}
	}


	 void LoadSinglePlayer()
	{
		ServiceLocator::RegisterSoundSystem(std::make_unique<SDLSoundSystem>());

		auto font = ResourceManager::GetInstance().LoadFont("GameFont.ttf", 15);


		auto& scene = SceneManager::GetInstance().CreateScene("sp");

		GameObject* RotationPointGO = scene.CreateNewGameObject();
		RotationPointGO->SetLocalPosition(200, 200);

		GameObject* BackgroundGo = scene.CreateNewGameObject();
		TextureComponent* BackgroundImageTextureComp = BackgroundGo->CreateComponent<TextureComponent>();
		BackgroundImageTextureComp->SetTexture("background.tga");
		BackgroundImageTextureComp->SetRenderParams({ 0, 0, 1280,960 }, { 0,0,BackgroundImageTextureComp->GetTextureResolution().x,BackgroundImageTextureComp->GetTextureResolution().y });

		GameObject* LogoGO = scene.CreateNewGameObject();
		TextureComponent* LogoImageTextureComp = LogoGO->CreateComponent<TextureComponent>();
		LogoImageTextureComp->SetTexture("logo.tga");
		LogoGO->SetLocalPosition({ 216, 180,0.f });



		GameObject* TileGenerator = scene.CreateNewGameObject();
		const std::vector<std::vector<TileType>> TileVector
		{
			{ D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D },
		{ D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, T, T, T, T },
		{ T, D, D, D, D, D, D, D, D, D, D, T, T, T, T, T },
		{ T, D, D, D, D, D, D, D, D, D, D, T, T, T, T, T },
		{ T, D, D, D, D, D, D, D, D, D, D, T, T, T, T, T },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, T, T, T, T, T, T, D, D, D, D },
		{ D, D, D, D, D, T, T, T, T, T, T, T, D, D, D, D },
		{ D, D, D, D, D, T, T, T, T, T, T, T, D, D, D, D },
		{ D, D, D, D, D, T, T, T, T, T, T, T, D, D, D, D }

		};



		MapComponent* MapComp = TileGenerator->CreateComponent<MapComponent>(16, 50, TileVector);

		std::vector<GameObject*> DamagingObjects{};


		GameObject* ScoreGO = scene.CreateNewGameObject();
		TextComponent* ScoreTextComp = ScoreGO->CreateComponent<TextComponent>("00000", font, SDL_Color{ 0,255,0,255 });
		ScoreTextComp->SetPosition(5, 0);

		GameObject* Digger = scene.CreateNewGameObject();
		FlipBookConfig DiggerFlipBookConfig{
			"Digger.png",
			3,
			20,
			20,
			.1f,
			true,
			1
		};

		FlipBookConfig DiggerDeadFlipBookConfig{
			"diggerDeath.png",
			6,
			20,
			-1,
			.1f,
			false,
			1
		};




		AnimControllerComponent* DiggerAnimController = Digger->CreateComponent<AnimControllerComponent>();
		DiggerAnimController->CreateState<IdleState>(DiggerFlipBookConfig);
		DiggerAnimController->CreateState<DeathState>(DiggerDeadFlipBookConfig);
		DiggerAnimController->ChangeState("Idle");
		Digger->SetLocalPosition(5, 10);

		Digger->CreateComponent<MovementComponent>(MapComp);
		Digger->CreateComponent<PlayerControllerComponent>(0,engine);

		Digger->m_Name = "Digger";

		Digger->CreateComponent<DiggingComponent>(MapComp);
		Digger->CreateComponent<HealthComponent>(3);

		ScoreComponent* ScoreCompDigger = Digger->CreateComponent<ScoreComponent>();
		ScoreCompDigger->BindTextComponent(ScoreTextComp);

		GameObject* GridGO = scene.CreateNewGameObject();
		AIGridComponent* AIGridComp = GridGO->CreateComponent<AIGridComponent>(glm::ivec2{ 1280,960 }, glm::ivec2{ 10,10 }, MapComp);
		(void)AIGridComp;

		GameObject* MonsterSpawner = scene.CreateNewGameObject();
		SpawnerComponent* SpawnerComp = MonsterSpawner->CreateComponent<SpawnerComponent>(&scene, AIGridComp, MapComp, Digger, 1.f, 5, glm::ivec2{ 270,10 }, DamagingObjects);

		Digger->CreateComponent<CombatInformationComponent>(SpawnerComp);

		std::vector<GameObject*> Diggers{};
		Diggers.emplace_back(Digger);

		// Load items
		SpawnBagAtlocation(scene, glm::ivec2{ 100,13 }, DamagingObjects, MapComp, Diggers, SpawnerComp);
		SpawnBagAtlocation(scene, glm::ivec2{ 40,50 }, DamagingObjects, MapComp, Diggers, SpawnerComp);
		SpawnBagAtlocation(scene, glm::ivec2{ 120,70 }, DamagingObjects, MapComp, Diggers, SpawnerComp);
		SpawnBagAtlocation(scene, glm::ivec2{ 140,130 }, DamagingObjects, MapComp, Diggers, SpawnerComp);
		SpawnBagAtlocation(scene, glm::ivec2{ 180,120 }, DamagingObjects, MapComp, Diggers, SpawnerComp);
		SpawnBagAtlocation(scene, glm::ivec2{ 180,60 }, DamagingObjects, MapComp, Diggers, SpawnerComp);
		SpawnBagAtlocation(scene, glm::ivec2{ 264,31 }, DamagingObjects, MapComp, Diggers, SpawnerComp);

		SpawnGem(scene, glm::ivec2{ 80,30 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 80,50 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 80,70 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 80,90 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 80,110 }, Diggers);

		SpawnGem(scene, glm::ivec2{ 100,30 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 100,50 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 100,70 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 100,90 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 100,110 }, Diggers);


		SpawnGem(scene, glm::ivec2{ 5,150 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 5,170 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 25,170 }, Diggers);


		SpawnGem(scene, glm::ivec2{ 260,70 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 280,70 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 300,70 }, Diggers);

		SpawnGem(scene, glm::ivec2{ 260,90 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 280,90 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 300,90 }, Diggers);

		SpawnGem(scene, glm::ivec2{ 260,110 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 280,110 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 300,110 }, Diggers);



	}


	 void LoadCoOp()
	{
		ServiceLocator::RegisterSoundSystem(std::make_unique<SDLSoundSystem>());

		auto font = ResourceManager::GetInstance().LoadFont("GameFont.ttf", 15);


		auto& scene = SceneManager::GetInstance().CreateScene("Coop");

		GameObject* RotationPointGO = scene.CreateNewGameObject();
		RotationPointGO->SetLocalPosition(200, 200);

		GameObject* BackgroundGo = scene.CreateNewGameObject();
		TextureComponent* BackgroundImageTextureComp = BackgroundGo->CreateComponent<TextureComponent>();
		BackgroundImageTextureComp->SetTexture("background.tga");
		BackgroundImageTextureComp->SetRenderParams({ 0, 0, 1280,960 }, { 0,0,BackgroundImageTextureComp->GetTextureResolution().x,BackgroundImageTextureComp->GetTextureResolution().y });

		GameObject* LogoGO = scene.CreateNewGameObject();
		TextureComponent* LogoImageTextureComp = LogoGO->CreateComponent<TextureComponent>();
		LogoImageTextureComp->SetTexture("logo.tga");
		LogoGO->SetLocalPosition({ 216, 180,0.f });



		GameObject* TileGenerator = scene.CreateNewGameObject();
		const std::vector<std::vector<TileType>> TileVector
		{
			{ D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D },
		{ D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, T, T, T, T },
		{ T, D, D, D, D, D, D, D, D, D, D, T, T, T, T, T },
		{ T, D, D, D, D, D, D, D, D, D, D, T, T, T, T, T },
		{ T, D, D, D, D, D, D, D, D, D, D, T, T, T, T, T },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, T, T, T, T, T, T, D, D, D, D },
		{ D, D, D, D, D, T, T, T, T, T, T, T, D, D, D, D },
		{ D, D, D, D, D, T, T, T, T, T, T, T, D, D, D, D },
		{ D, D, D, D, D, T, T, T, T, T, T, T, D, D, D, D }

		};



		MapComponent* MapComp = TileGenerator->CreateComponent<MapComponent>(16, 50, TileVector);

		std::vector<GameObject*> DamagingObjects{};


		GameObject* ScoreGO = scene.CreateNewGameObject();
		TextComponent* ScoreTextComp = ScoreGO->CreateComponent<TextComponent>("00000", font, SDL_Color{ 0,255,0,255 });
		ScoreTextComp->SetPosition(5, 0);

		GameObject* Digger = scene.CreateNewGameObject();
		FlipBookConfig DiggerFlipBookConfig{
			"Digger.png",
			3,
			20,
			20,
			.1f,
			true,
			1
		};

		FlipBookConfig DiggerDeadFlipBookConfig{
			"diggerDeath.png",
			6,
			20,
			-1,
			.1f,
			false,
			1
		};




		AnimControllerComponent* DiggerAnimController = Digger->CreateComponent<AnimControllerComponent>();
		DiggerAnimController->CreateState<IdleState>(DiggerFlipBookConfig);
		DiggerAnimController->CreateState<DeathState>(DiggerDeadFlipBookConfig);
		DiggerAnimController->ChangeState("Idle");
		Digger->SetLocalPosition(5, 10);

		Digger->CreateComponent<MovementComponent>(MapComp);
		Digger->CreateComponent<PlayerControllerComponent>(0,engine);

		Digger->m_Name = "Digger";

		Digger->CreateComponent<DiggingComponent>(MapComp);
		Digger->CreateComponent<HealthComponent>(3);

		ScoreComponent* ScoreCompDigger = Digger->CreateComponent<ScoreComponent>();
		ScoreCompDigger->BindTextComponent(ScoreTextComp);

		GameObject* GridGO = scene.CreateNewGameObject();
		AIGridComponent* AIGridComp = GridGO->CreateComponent<AIGridComponent>(glm::ivec2{ 1280,960 }, glm::ivec2{ 10,10 }, MapComp);
		(void)AIGridComp;

		GameObject* MonsterSpawner = scene.CreateNewGameObject();
		SpawnerComponent* SpawnerComp = MonsterSpawner->CreateComponent<SpawnerComponent>(&scene, AIGridComp, MapComp, Digger, 1.f, 5, glm::ivec2{ 270,10 }, DamagingObjects);


		Digger->CreateComponent<CombatInformationComponent>(SpawnerComp);

		// second digger
		GameObject* SecondDigger = scene.CreateNewGameObject();
		AnimControllerComponent* SecondDiggerAnimController = SecondDigger->CreateComponent<AnimControllerComponent>();
		SecondDiggerAnimController->CreateState<IdleState>(DiggerFlipBookConfig);
		SecondDiggerAnimController->CreateState<DeathState>(DiggerDeadFlipBookConfig);
		SecondDiggerAnimController->ChangeState("Idle");
		SecondDigger->SetLocalPosition(5, 30);

		SecondDigger->CreateComponent<MovementComponent>(MapComp);
		SecondDigger->CreateComponent<PlayerControllerComponent>(1,engine);

		SecondDigger->m_Name = "Digger";

		SecondDigger->CreateComponent<DiggingComponent>(MapComp);
		SecondDigger->CreateComponent<HealthComponent>(3);

		ScoreComponent* ScoreCompSecondDigger = SecondDigger->CreateComponent<ScoreComponent>();
		ScoreCompSecondDigger->BindTextComponent(ScoreTextComp);
		SecondDigger->CreateComponent<CombatInformationComponent>(SpawnerComp);


		std::vector<GameObject*> Diggers{};
		Diggers.emplace_back(Digger);
		Diggers.emplace_back(SecondDigger);

		// Load items
		SpawnBagAtlocation(scene, glm::ivec2{ 100,13 }, DamagingObjects, MapComp, Diggers, SpawnerComp);
		SpawnBagAtlocation(scene, glm::ivec2{ 40,50 }, DamagingObjects, MapComp, Diggers, SpawnerComp);
		SpawnBagAtlocation(scene, glm::ivec2{ 120,70 }, DamagingObjects, MapComp, Diggers, SpawnerComp);
		SpawnBagAtlocation(scene, glm::ivec2{ 140,130 }, DamagingObjects, MapComp, Diggers, SpawnerComp);
		SpawnBagAtlocation(scene, glm::ivec2{ 180,120 }, DamagingObjects, MapComp, Diggers, SpawnerComp);
		SpawnBagAtlocation(scene, glm::ivec2{ 180,60 }, DamagingObjects, MapComp, Diggers, SpawnerComp);
		SpawnBagAtlocation(scene, glm::ivec2{ 264,31 }, DamagingObjects, MapComp, Diggers, SpawnerComp);

		SpawnGem(scene, glm::ivec2{ 80,30 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 80,50 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 80,70 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 80,90 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 80,110 }, Diggers);

		SpawnGem(scene, glm::ivec2{ 100,30 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 100,50 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 100,70 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 100,90 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 100,110 }, Diggers);


		SpawnGem(scene, glm::ivec2{ 5,150 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 5,170 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 25,170 }, Diggers);


		SpawnGem(scene, glm::ivec2{ 260,70 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 280,70 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 300,70 }, Diggers);

		SpawnGem(scene, glm::ivec2{ 260,90 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 280,90 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 300,90 }, Diggers);

		SpawnGem(scene, glm::ivec2{ 260,110 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 280,110 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 300,110 }, Diggers);



	}

	 void LoadVersus()
	{
		ServiceLocator::RegisterSoundSystem(std::make_unique<SDLSoundSystem>());

		auto font = ResourceManager::GetInstance().LoadFont("GameFont.ttf", 15);


		auto& scene = SceneManager::GetInstance().CreateScene("Versus");

		GameObject* RotationPointGO = scene.CreateNewGameObject();
		RotationPointGO->SetLocalPosition(200, 200);

		GameObject* BackgroundGo = scene.CreateNewGameObject();
		TextureComponent* BackgroundImageTextureComp = BackgroundGo->CreateComponent<TextureComponent>();
		BackgroundImageTextureComp->SetTexture("background.tga");
		BackgroundImageTextureComp->SetRenderParams({ 0, 0, 1280,960 }, { 0,0,BackgroundImageTextureComp->GetTextureResolution().x,BackgroundImageTextureComp->GetTextureResolution().y });

		GameObject* LogoGO = scene.CreateNewGameObject();
		TextureComponent* LogoImageTextureComp = LogoGO->CreateComponent<TextureComponent>();
		LogoImageTextureComp->SetTexture("logo.tga");
		LogoGO->SetLocalPosition({ 216, 180,0.f });



		GameObject* TileGenerator = scene.CreateNewGameObject();
		const std::vector<std::vector<TileType>> TileVector
		{
			{ D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D },
		{ D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, T, T, T, T },
		{ T, D, D, D, D, D, D, D, D, D, D, T, T, T, T, T },
		{ T, D, D, D, D, D, D, D, D, D, D, T, T, T, T, T },
		{ T, D, D, D, D, D, D, D, D, D, D, T, T, T, T, T },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, D, D, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ T, T, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, D, D, D, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, T, T, T, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, D, D, D, D, D, T, D, D, D, D },
		{ D, D, D, D, D, T, T, T, T, T, T, T, D, D, D, D },
		{ D, D, D, D, D, T, T, T, T, T, T, T, D, D, D, D },
		{ D, D, D, D, D, T, T, T, T, T, T, T, D, D, D, D },
		{ D, D, D, D, D, T, T, T, T, T, T, T, D, D, D, D }

		};



		MapComponent* MapComp = TileGenerator->CreateComponent<MapComponent>(16, 50, TileVector);

		std::vector<GameObject*> DamagingObjects{};


		GameObject* ScoreGO = scene.CreateNewGameObject();
		TextComponent* ScoreTextComp = ScoreGO->CreateComponent<TextComponent>("00000", font, SDL_Color{ 0,255,0,255 });
		ScoreTextComp->SetPosition(5, 0);

		GameObject* Digger = scene.CreateNewGameObject();
		FlipBookConfig DiggerFlipBookConfig{
			"Digger.png",
			3,
			20,
			20,
			.1f,
			true,
			1
		};

		FlipBookConfig DiggerDeadFlipBookConfig{
			"diggerDeath.png",
			6,
			20,
			-1,
			.1f,
			false,
			1
		};

		AnimControllerComponent* DiggerAnimController = Digger->CreateComponent<AnimControllerComponent>();
		DiggerAnimController->CreateState<IdleState>(DiggerFlipBookConfig);
		DiggerAnimController->CreateState<DeathState>(DiggerDeadFlipBookConfig);
		DiggerAnimController->ChangeState("Idle");
		Digger->SetLocalPosition(5, 10);

		Digger->CreateComponent<MovementComponent>(MapComp);
		Digger->CreateComponent<PlayerControllerComponent>(0,engine);

		Digger->m_Name = "Digger";

		Digger->CreateComponent<DiggingComponent>(MapComp);
		Digger->CreateComponent<HealthComponent>(3);

		ScoreComponent* ScoreCompDigger = Digger->CreateComponent<ScoreComponent>();
		ScoreCompDigger->BindTextComponent(ScoreTextComp);


		GameObject* Monster = scene.CreateNewGameObject();
		Monster->m_Name = "Monster";
		Monster->SetLocalPosition(270, 10);

		Monster->CreateComponent<HealthComponent>(1);
		AnimControllerComponent* MonsterAnimController = Monster->CreateComponent<AnimControllerComponent>();

		FlipBookConfig NobbinFlipBookConfig{ "Nobbin.png", 3, 20, 20, 0.2f, true, 1 };
		FlipBookConfig HobbinFlipBookConfig{ "Hobbin.png", 3, 20, 20, 0.2f, true, 1 };

		MonsterAnimController->CreateState<NobbinState>(NobbinFlipBookConfig);
		MonsterAnimController->CreateState<HobbinState>(HobbinFlipBookConfig);
		MonsterAnimController->ChangeState("Hobbin");

		Monster->CreateComponent<MovementComponent>(MapComp);
		Monster->CreateComponent<PlayerControllerComponent>(1,engine);

		// Create Damage component
		DamageComponent* dmgComp = Monster->CreateComponent<DamageComponent>(glm::ivec4{ 0, 0, 20, 20 });
		dmgComp->bDoesDamageWithoutCondition = true;
		dmgComp->AddNewDamageRecevingActor(Digger);



		std::vector<GameObject*> Diggers{};
		Diggers.emplace_back(Digger);
		std::vector<GameObject*> MoneyBags{};

		// Load items
		MoneyBags.emplace_back(SpawnBagAtlocation(scene, glm::ivec2{ 100,13 }, DamagingObjects, MapComp, Diggers, nullptr));
		MoneyBags.emplace_back(SpawnBagAtlocation(scene, glm::ivec2{ 40,50 }, DamagingObjects, MapComp, Diggers, nullptr));
		MoneyBags.emplace_back(SpawnBagAtlocation(scene, glm::ivec2{ 120,70 }, DamagingObjects, MapComp, Diggers, nullptr));
		MoneyBags.emplace_back(SpawnBagAtlocation(scene, glm::ivec2{ 140,130 }, DamagingObjects, MapComp, Diggers, nullptr));
		MoneyBags.emplace_back(SpawnBagAtlocation(scene, glm::ivec2{ 180,120 }, DamagingObjects, MapComp, Diggers, nullptr));
		MoneyBags.emplace_back(SpawnBagAtlocation(scene, glm::ivec2{ 180,60 }, DamagingObjects, MapComp, Diggers, nullptr));
		MoneyBags.emplace_back(SpawnBagAtlocation(scene, glm::ivec2{ 264,31 }, DamagingObjects, MapComp, Diggers, nullptr));

		SpawnGem(scene, glm::ivec2{ 80,30 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 80,50 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 80,70 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 80,90 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 80,110 }, Diggers);

		SpawnGem(scene, glm::ivec2{ 100,30 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 100,50 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 100,70 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 100,90 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 100,110 }, Diggers);


		SpawnGem(scene, glm::ivec2{ 5,150 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 5,170 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 25,170 }, Diggers);


		SpawnGem(scene, glm::ivec2{ 260,70 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 280,70 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 300,70 }, Diggers);

		SpawnGem(scene, glm::ivec2{ 260,90 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 280,90 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 300,90 }, Diggers);

		SpawnGem(scene, glm::ivec2{ 260,110 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 280,110 }, Diggers);
		SpawnGem(scene, glm::ivec2{ 300,110 }, Diggers);

		for (GameObject* Actor : MoneyBags)
		{
			DamageComponent* OtherDmgComp = Actor->GetFirstComponentOfType<DamageComponent>();
			OtherDmgComp->AddNewDamageRecevingActor(Monster);
		}

	}

	 
}

