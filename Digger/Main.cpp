#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Texture2D.h"
#include <memory>
#include <fstream>
#include <string>

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
#include <combaseapi.h>

#pragma endregion Components


using TT = dae::TileType;
#define D TT::Dirt
#define T TT::Tunnel

std::vector<std::vector<dae::TileType>> LoadTileMapFromFile(const std::string& filename)
{
	std::ifstream inFile("../Data/"+filename);
	std::vector<std::vector<dae::TileType>> tileMap;

	if (!inFile.is_open()) {
		std::cerr << "Failed to open tile map file: " << filename << "\n";
		return tileMap;
	}

	std::string line;
	while (std::getline(inFile, line)) {
		std::vector<dae::TileType> row;
		for (char c : line) {
			switch (c) {
			case 'T': row.emplace_back(dae::TileType::Tunnel); break;
			case 'D': row.emplace_back(dae::TileType::Dirt); break;
			default: std::cerr << "Invalid tile char: " << c << "\n"; break;
			}
		}
		tileMap.emplace_back(row);
	}

	return tileMap;
}



dae::GameObject* SpawnBagAtlocation(dae::Scene& scene, const glm::ivec2 Location, std::vector<dae::GameObject*>& DamagingContainer, dae::MapComponent* MapComp, std::vector<dae::GameObject*> Digger, dae::SpawnerComponent* SpawnerComp) 
{
	dae::GameObject* Money = scene.CreateNewGameObject();
	dae::FlipBookConfig BagFlipBookConfig{
		"bag.png",
		4,
		20,
		20,
		.2f,
		false,
		1
	};

	dae::FlipBookConfig BagFallingFlipBookConfig{
	"bag.png",
	4,
	20,
	20,
	.2f,
	true,
	1
	};

	dae::FlipBookConfig GoldFlipBookConfig{
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
	dae::AnimControllerComponent* AnimController = Money->CreateComponent<dae::AnimControllerComponent>();
	AnimController->CreateState<dae::IdleState>(BagFlipBookConfig);
	AnimController->CreateState<dae::FallingState>(BagFallingFlipBookConfig);
	AnimController->CreateState<dae::DestroyedState>(GoldFlipBookConfig);
	AnimController->ChangeState("Idle");


	Money->CreateComponent<dae::GravityComponent>(MapComp);
	dae::DamageComponent* MoneyDamageComp = Money->CreateComponent<dae::DamageComponent>(glm::ivec4{ 0, 0, 20, 20 });
	for (dae::GameObject* Dig : Digger)
	{
		MoneyDamageComp->AddNewDamageRecevingActor(Dig);
	}
	for (dae::GameObject* Monster : SpawnerComp->m_MonstersSpawned)
	{
		MoneyDamageComp->AddNewDamageRecevingActor(Monster);

	}
	dae::PushComponent* LeftPushComp = Money->CreateComponent<dae::PushComponent>(glm::ivec2{ 2,10 }, 1,500);
	dae::PushComponent* RightPushComp = Money->CreateComponent<dae::PushComponent>(glm::ivec2{ 18,10 }, -1,500);
	for (dae::GameObject* Dig : Digger)
	{
		LeftPushComp->AddOtherActors(Dig);
		RightPushComp->AddOtherActors(Dig);
	}


	DamagingContainer.emplace_back(Money);
	if (SpawnerComp)
	{
		Money->CreateComponent<dae::CombatInformationComponent>(SpawnerComp);
	}

	return Money;
}

void SpawnGem(dae::Scene& scene, const glm::ivec2 Location, std::vector<dae::GameObject*> Digger)
{
	dae::GameObject* Emerald = scene.CreateNewGameObject();
	Emerald->m_Name = "Emerald";

	dae::FlipBookConfig EmeraldFlipBookConfig{
		"emerald.png",
		1,
		20,
		20,
		.2f,
		false,
		1
	};
	dae::AnimControllerComponent* AnimController = Emerald->CreateComponent<dae::AnimControllerComponent>();
	AnimController->CreateState<dae::IdleState>(EmeraldFlipBookConfig);
	AnimController->ChangeState("Idle");

	Emerald->SetLocalPosition(Location.x, Location.y);

	dae::PushComponent* LeftPushComp = Emerald->CreateComponent<dae::PushComponent>(glm::ivec2{ 2,10 }, 1,25);
	dae::PushComponent* RightPushComp = Emerald->CreateComponent<dae::PushComponent>(glm::ivec2{ 18,10 }, -1,25);
	LeftPushComp->bIsInstantlyPickup = true;
	RightPushComp->bIsInstantlyPickup = true;
	for (dae::GameObject* Dig : Digger)
	{
		LeftPushComp->AddOtherActors(Dig);
		RightPushComp->AddOtherActors(Dig);
	}
}


void LoadSinglePlayer()
{
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());

	auto font = dae::ResourceManager::GetInstance().LoadFont("GameFont.ttf", 15);
	

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	dae::GameObject* TileGenerator = scene.CreateNewGameObject();
	const std::vector<std::vector<dae::TileType>> TileVector = LoadTileMapFromFile("Level1_Grid.txt");

	dae::MapComponent* MapComp = TileGenerator->CreateComponent<dae::MapComponent>(16, 50, TileVector);

	std::vector<dae::GameObject*> DamagingObjects{};
	

	dae::GameObject* ScoreGO = scene.CreateNewGameObject();
	dae::TextComponent* ScoreTextComp = ScoreGO->CreateComponent<dae::TextComponent>("00000", font, SDL_Color{0,255,0,255});
	ScoreTextComp->SetPosition(5, 0);

	dae::GameObject* Digger = scene.CreateNewGameObject();
	dae::FlipBookConfig DiggerFlipBookConfig{
		"Digger.png",
		3,
		20,
		20,
		.1f,
		true,
		1
	};

	dae::FlipBookConfig DiggerDeadFlipBookConfig{
		"diggerDeath.png",
		6,
		20,
		-1,
		.1f,
		false,
		1
	};


	

	dae::AnimControllerComponent* DiggerAnimController = Digger->CreateComponent<dae::AnimControllerComponent>();
	DiggerAnimController->CreateState<dae::IdleState>(DiggerFlipBookConfig);
	DiggerAnimController->CreateState<dae::DeathState>(DiggerDeadFlipBookConfig);
	DiggerAnimController->ChangeState("Idle");
	Digger->SetLocalPosition(5, 10);

	Digger->CreateComponent<dae::MovementComponent>(MapComp);
	Digger->CreateComponent<dae::PlayerControllerComponent>(0);

	Digger->m_Name = "Digger";

	Digger->CreateComponent<dae::DiggingComponent>(MapComp);
	Digger->CreateComponent<dae::HealthComponent>(3);

	dae::ScoreComponent* ScoreCompDigger = Digger->CreateComponent<dae::ScoreComponent>();
	ScoreCompDigger->BindTextComponent(ScoreTextComp);

	dae::GameObject* GridGO = scene.CreateNewGameObject();
	dae::AIGridComponent* AIGridComp = GridGO->CreateComponent<dae::AIGridComponent>(glm::ivec2{ 1280,960 }, glm::ivec2{10,10},MapComp);
	(void)AIGridComp;

	dae::GameObject* MonsterSpawner = scene.CreateNewGameObject();
	dae::SpawnerComponent* SpawnerComp = MonsterSpawner->CreateComponent<dae::SpawnerComponent>(&scene, AIGridComp, MapComp, Digger, 1.f, 5, glm::ivec2{270,10},DamagingObjects);

	Digger->CreateComponent<dae::CombatInformationComponent>(SpawnerComp);

	std::vector<dae::GameObject*> Diggers{};
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
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());

	auto font = dae::ResourceManager::GetInstance().LoadFont("GameFont.ttf", 15);
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	dae::GameObject* RotationPointGO = scene.CreateNewGameObject();
	RotationPointGO->SetLocalPosition(200, 200);

	dae::GameObject* BackgroundGo = scene.CreateNewGameObject();
	dae::TextureComponent* BackgroundImageTextureComp = BackgroundGo->CreateComponent<dae::TextureComponent>();
	BackgroundImageTextureComp->SetTexture("background.tga");
	BackgroundImageTextureComp->SetRenderParams({ 0, 0, 1280,960 }, { 0,0,BackgroundImageTextureComp->GetTextureResolution().x,BackgroundImageTextureComp->GetTextureResolution().y });

	dae::GameObject* LogoGO = scene.CreateNewGameObject();
	dae::TextureComponent* LogoImageTextureComp = LogoGO->CreateComponent<dae::TextureComponent>();
	LogoImageTextureComp->SetTexture("logo.tga");
	LogoGO->SetLocalPosition({ 216, 180,0.f });

	dae::GameObject* TileGenerator = scene.CreateNewGameObject();
	const std::vector<std::vector<dae::TileType>> TileVector = LoadTileMapFromFile("Level1_Grid.txt");

	dae::MapComponent* MapComp = TileGenerator->CreateComponent<dae::MapComponent>(16, 50, TileVector);

	std::vector<dae::GameObject*> DamagingObjects{};

	dae::GameObject* ScoreGO = scene.CreateNewGameObject();
	dae::TextComponent* ScoreTextComp = ScoreGO->CreateComponent<dae::TextComponent>("00000", font, SDL_Color{ 0,255,0,255 });
	ScoreTextComp->SetPosition(5, 0);

	dae::GameObject* Digger = scene.CreateNewGameObject();
	dae::FlipBookConfig DiggerFlipBookConfig{
		"Digger.png",
		3,
		20,
		20,
		.1f,
		true,
		1
	};

	dae::FlipBookConfig DiggerDeadFlipBookConfig{
		"diggerDeath.png",
		6,
		20,
		-1,
		.1f,
		false,
		1
	};


	dae::AnimControllerComponent* DiggerAnimController = Digger->CreateComponent<dae::AnimControllerComponent>();
	DiggerAnimController->CreateState<dae::IdleState>(DiggerFlipBookConfig);
	DiggerAnimController->CreateState<dae::DeathState>(DiggerDeadFlipBookConfig);
	DiggerAnimController->ChangeState("Idle");
	Digger->SetLocalPosition(5, 10);

	Digger->CreateComponent<dae::MovementComponent>(MapComp);
	Digger->CreateComponent<dae::PlayerControllerComponent>(0);

	Digger->m_Name = "Digger";

	Digger->CreateComponent<dae::DiggingComponent>(MapComp);
	Digger->CreateComponent<dae::HealthComponent>(3);

	dae::ScoreComponent* ScoreCompDigger = Digger->CreateComponent<dae::ScoreComponent>();
	ScoreCompDigger->BindTextComponent(ScoreTextComp);

	dae::GameObject* GridGO = scene.CreateNewGameObject();
	dae::AIGridComponent* AIGridComp = GridGO->CreateComponent<dae::AIGridComponent>(glm::ivec2{ 1280,960 }, glm::ivec2{ 10,10 }, MapComp);
	(void)AIGridComp;

	dae::GameObject* MonsterSpawner = scene.CreateNewGameObject();
	dae::SpawnerComponent* SpawnerComp = MonsterSpawner->CreateComponent<dae::SpawnerComponent>(&scene, AIGridComp, MapComp, Digger, 1.f, 5, glm::ivec2{ 270,10 }, DamagingObjects);


	Digger->CreateComponent<dae::CombatInformationComponent>(SpawnerComp);

	// second digger
	dae::GameObject* SecondDigger = scene.CreateNewGameObject();
	dae::AnimControllerComponent* SecondDiggerAnimController = SecondDigger->CreateComponent<dae::AnimControllerComponent>();
	SecondDiggerAnimController->CreateState<dae::IdleState>(DiggerFlipBookConfig);
	SecondDiggerAnimController->CreateState<dae::DeathState>(DiggerDeadFlipBookConfig);
	SecondDiggerAnimController->ChangeState("Idle");
	SecondDigger->SetLocalPosition(5, 30);

	SecondDigger->CreateComponent<dae::MovementComponent>(MapComp);
	SecondDigger->CreateComponent<dae::PlayerControllerComponent>(1);

	SecondDigger->m_Name = "Digger";

	SecondDigger->CreateComponent<dae::DiggingComponent>(MapComp);
	SecondDigger->CreateComponent<dae::HealthComponent>(3);

	dae::ScoreComponent* ScoreCompSecondDigger = SecondDigger->CreateComponent<dae::ScoreComponent>();
	ScoreCompSecondDigger->BindTextComponent(ScoreTextComp);
	SecondDigger->CreateComponent<dae::CombatInformationComponent>(SpawnerComp);


	std::vector<dae::GameObject*> Diggers{};
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
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());

	auto font = dae::ResourceManager::GetInstance().LoadFont("GameFont.ttf", 15);


	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	dae::GameObject* RotationPointGO = scene.CreateNewGameObject();
	RotationPointGO->SetLocalPosition(200, 200);

	dae::GameObject* BackgroundGo = scene.CreateNewGameObject();
	dae::TextureComponent* BackgroundImageTextureComp = BackgroundGo->CreateComponent<dae::TextureComponent>();
	BackgroundImageTextureComp->SetTexture("background.tga");
	BackgroundImageTextureComp->SetRenderParams({ 0, 0, 1280,960 }, { 0,0,BackgroundImageTextureComp->GetTextureResolution().x,BackgroundImageTextureComp->GetTextureResolution().y });

	dae::GameObject* LogoGO = scene.CreateNewGameObject();
	dae::TextureComponent* LogoImageTextureComp = LogoGO->CreateComponent<dae::TextureComponent>();
	LogoImageTextureComp->SetTexture("logo.tga");
	LogoGO->SetLocalPosition({ 216, 180,0.f });



	dae::GameObject* TileGenerator = scene.CreateNewGameObject();
	const std::vector<std::vector<dae::TileType>> TileVector = LoadTileMapFromFile("Level1_Grid.txt");




	dae::MapComponent* MapComp = TileGenerator->CreateComponent<dae::MapComponent>(16, 50, TileVector);

	std::vector<dae::GameObject*> DamagingObjects{};


	dae::GameObject* ScoreGO = scene.CreateNewGameObject();
	dae::TextComponent* ScoreTextComp = ScoreGO->CreateComponent<dae::TextComponent>("00000", font, SDL_Color{ 0,255,0,255 });
	ScoreTextComp->SetPosition(5, 0);

	dae::GameObject* Digger = scene.CreateNewGameObject();
	dae::FlipBookConfig DiggerFlipBookConfig{
		"Digger.png",
		3,
		20,
		20,
		.1f,
		true,
		1
	};

	dae::FlipBookConfig DiggerDeadFlipBookConfig{
		"diggerDeath.png",
		6,
		20,
		-1,
		.1f,
		false,
		1
	};

	dae::AnimControllerComponent* DiggerAnimController = Digger->CreateComponent<dae::AnimControllerComponent>();
	DiggerAnimController->CreateState<dae::IdleState>(DiggerFlipBookConfig);
	DiggerAnimController->CreateState<dae::DeathState>(DiggerDeadFlipBookConfig);
	DiggerAnimController->ChangeState("Idle");
	Digger->SetLocalPosition(5, 10);

	Digger->CreateComponent<dae::MovementComponent>(MapComp);
	Digger->CreateComponent<dae::PlayerControllerComponent>(0);

	Digger->m_Name = "Digger";

	Digger->CreateComponent<dae::DiggingComponent>(MapComp);
	Digger->CreateComponent<dae::HealthComponent>(3);

	dae::ScoreComponent* ScoreCompDigger = Digger->CreateComponent<dae::ScoreComponent>();
	ScoreCompDigger->BindTextComponent(ScoreTextComp);


	dae::GameObject* Monster = scene.CreateNewGameObject();
	Monster->m_Name = "Monster";
	Monster->SetLocalPosition(270, 10);

	Monster->CreateComponent<dae::HealthComponent>(1);
	dae::AnimControllerComponent* MonsterAnimController = Monster->CreateComponent<dae::AnimControllerComponent>();

	dae::FlipBookConfig NobbinFlipBookConfig{ "Nobbin.png", 3, 20, 20, 0.2f, true, 1 };
	dae::FlipBookConfig HobbinFlipBookConfig{ "Hobbin.png", 3, 20, 20, 0.2f, true, 1 };

	MonsterAnimController->CreateState<dae::NobbinState>(NobbinFlipBookConfig);
	MonsterAnimController->CreateState<dae::HobbinState>(HobbinFlipBookConfig);
	MonsterAnimController->ChangeState("Hobbin");

	Monster->CreateComponent<dae::MovementComponent>(MapComp);
	Monster->CreateComponent<dae::PlayerControllerComponent>(1);

	// Create Damage component
	dae::DamageComponent* dmgComp = Monster->CreateComponent<dae::DamageComponent>(glm::ivec4{ 0, 0, 20, 20 });
	dmgComp->bDoesDamageWithoutCondition = true;
	dmgComp->AddNewDamageRecevingActor(Digger);



	std::vector<dae::GameObject*> Diggers{};
	Diggers.emplace_back(Digger);
	std::vector<dae::GameObject*> MoneyBags{};

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

	for (dae::GameObject* Actor : MoneyBags)
	{
		dae::DamageComponent* OtherDmgComp = Actor->GetFirstComponentOfType<dae::DamageComponent>();
		OtherDmgComp->AddNewDamageRecevingActor(Monster);
	}

}

void LoadMenu()
{
	using namespace dae;
	auto font = ResourceManager::GetInstance().LoadFont("GameFont.ttf", 15);

	auto& scene = SceneManager::GetInstance().CreateScene("Menu");

	GameObject* TextGo = scene.CreateNewGameObject();
	TextGo->CreateComponent<TextComponent>("SinglePlayer", font, SDL_Color{ 255,255,255,255 })->SetPosition(120, 60);
	TextGo->CreateComponent<TextComponent>("Co-op", font, SDL_Color{ 255,255,255,255 })->SetPosition(120, 80);
	TextGo->CreateComponent<TextComponent>("Versus", font, SDL_Color{ 255,255,255,255 })->SetPosition(120, 100);
	TextureComponent* ABtn = TextGo->CreateComponent<TextureComponent>("AButton.png");
	ABtn->bOverrideOwnerPosition = true;
	ABtn->OverridenPosition = glm::vec3{ 90,60,0 };

	TextureComponent* XBtn = TextGo->CreateComponent<TextureComponent>("XButton.png");
	XBtn->bOverrideOwnerPosition = true;
	XBtn->OverridenPosition = glm::vec3{ 90,80,0 };

	TextureComponent* YBtn = TextGo->CreateComponent<TextureComponent>("YButton.png");
	YBtn->bOverrideOwnerPosition = true;
	YBtn->OverridenPosition = glm::vec3{ 90,100,0 };

	TextGo->CreateComponent<PlayerControllerComponent>(0);


}


void LoadTrashTheCacheAssigment()
{
	auto& SceneManager = dae::SceneManager::GetInstance();
	dae::Scene& scene = SceneManager.CreateScene("Demo");
	SceneManager.m_bUseImGui = true;
	scene;
}


int main(int, char*[]) {
	VLDSetOptions(VLD_OPT_REPORT_TO_DEBUGGER,100,100); 

	HRESULT COMInit = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (COMInit == S_OK )
	{
		dae::Minigin engine("../Data/");
		engine.Run(LoadSinglePlayer);

	}
	CoUninitialize();

	
    return 0;
}