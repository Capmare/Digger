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
#pragma endregion Components




void load()
{
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);


	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	dae::GameObject* RotationPointGO = scene.CreateNewGameObject();
	RotationPointGO->SetLocalPosition(200, 200);

	dae::GameObject* BackgroundGo = scene.CreateNewGameObject();
	dae::TextureComponent* BackgroundImageTextureComp = BackgroundGo->CreateComponent<dae::TextureComponent>();
	BackgroundImageTextureComp->SetTexture("background.tga");

	dae::GameObject* LogoGO = scene.CreateNewGameObject();
	dae::TextureComponent* LogoImageTextureComp = LogoGO->CreateComponent<dae::TextureComponent>();
	LogoImageTextureComp->SetTexture("logo.tga");
	LogoGO->SetLocalPosition({ 216, 180,0.f });

	dae::GameObject* SonicParent = scene.CreateNewGameObject();
	dae::TextureComponent* SonicImage = SonicParent->CreateComponent<dae::TextureComponent>();
	dae::MovementComponent* MovComp = SonicParent->CreateComponent<dae::MovementComponent>();
	SonicParent->CreateComponent<dae::PlayerControllerComponent>(false);
	SonicParent->m_Name = "SonicParent";
	SonicImage->SetTexture("Sonic.png");
	SonicParent->SetLocalPosition(10, 10);
	MovComp->AddMovementInput(100,0);
	
	dae::GameObject* SonicTwo = scene.CreateNewGameObject();
	dae::TextureComponent* SonicSecondImage = SonicTwo->CreateComponent<dae::TextureComponent>();
	dae::MovementComponent* OtherMovComp = SonicTwo->CreateComponent<dae::MovementComponent>();
	SonicTwo->CreateComponent<dae::PlayerControllerComponent>(true);
	SonicTwo->m_Name = "SonicParent";
	SonicSecondImage->SetTexture("Sonic.png");
	SonicTwo->SetLocalPosition(10, 10);
	OtherMovComp->AddMovementInput(100, 100);
	dae::HealthComponent* OtherHealthComp = SonicTwo->CreateComponent<dae::HealthComponent>(5);
	dae::TextComponent* OtherHealthText = SonicParent->CreateComponent<dae::TextComponent>("Health: ", font);
	OtherHealthText->SetPosition(0, 70);
	OtherHealthComp->BindTextComponent(OtherHealthText);
	dae::GameObject* TextGO = scene.CreateNewGameObject();
	TextGO->CreateComponent<dae::TextComponent>("Use D-PAD to move Sonic 1, press A to damage sonic 1 or B to pick up pellets", font);
	dae::TextComponent* SecondText = TextGO->CreateComponent<dae::TextComponent>("Use WASD to move Sonic 2, press Z to damage sonic 2 or X to pick up pellets", font);
	SecondText->SetPosition(0, 25);

	dae::TextComponent* HealthText = SonicParent->CreateComponent<dae::TextComponent>("Health: ", font);
	HealthText->SetPosition(0,50);
	dae::HealthComponent* HealthComp = SonicParent->CreateComponent<dae::HealthComponent>(5);
	HealthComp->BindTextComponent(HealthText);


	dae::Observer* HealthObserver =  scene.CreateObserver(std::move(std::make_unique<dae::HealthObserver>()));
	HealthComp->m_Subject.AddObserver(HealthObserver);

	OtherHealthComp->m_Subject.AddObserver(HealthObserver);

	HealthObserver->OnNotify(*SonicParent, dae::ObserverEvents::OnHealthChanged);
	HealthObserver->OnNotify(*SonicTwo, dae::ObserverEvents::OnHealthChanged);

	dae::Observer* ScoreObserver = scene.CreateObserver(std::move(std::make_unique<dae::ScoreObserver>()));
	
	dae::TextComponent* ScoreParentText = SonicParent->CreateComponent<dae::TextComponent>("Score: ", font);
	ScoreParentText->SetPosition(200, 50);
	dae::ScoreComponent* SonicParentScoreComp = SonicParent->CreateComponent<dae::ScoreComponent>();
	SonicParentScoreComp->BindTextComponent(ScoreParentText);
	SonicParentScoreComp->m_Subject.AddObserver(ScoreObserver);

	dae::TextComponent* ScoreChildText = SonicTwo->CreateComponent<dae::TextComponent>("Score: ", font);
	ScoreChildText->SetPosition(200, 70);
	dae::ScoreComponent* SonicChildScoreComp = SonicTwo->CreateComponent<dae::ScoreComponent>();
	SonicChildScoreComp->BindTextComponent(ScoreChildText);
	SonicChildScoreComp->m_Subject.AddObserver(ScoreObserver);

	ScoreObserver->OnNotify(*SonicParent, dae::ObserverEvents::OnScoreChanged);
	ScoreObserver->OnNotify(*SonicTwo, dae::ObserverEvents::OnScoreChanged);

}

void LoadTrashTheCacheAssigment()
{
	auto& SceneManager = dae::SceneManager::GetInstance();
	dae::Scene& scene = SceneManager.CreateScene("Demo");
	SceneManager.m_bUseImGui = true;
	scene;
}


int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}