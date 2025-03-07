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

#pragma region Components
#include "TextureComponent.h"
#include "RotatingComponent.h"
#include "FpsComponent.h"
#include "BaseComponent.h"
#pragma endregion Components




void load()
{
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
	SonicParent->m_Name = "SonicParent";
	SonicImage->SetTexture("Sonic.png");
	SonicParent->SetLocalPosition(10, 10);
	SonicParent->SetParent(RotationPointGO, false);
	dae::RotatingComponent* FirsRot = SonicParent->CreateComponent<dae::RotatingComponent>();
	FirsRot->m_RotationSpeed = 4.f;
	FirsRot->m_Radius = 50.f;


	dae::GameObject* RotatingSonic = scene.CreateNewGameObject();
	dae::TextureComponent* SonicSecondImage = RotatingSonic->CreateComponent<dae::TextureComponent>();
	RotatingSonic->m_Name = "RotatingSonic";
	SonicSecondImage->SetTexture("Sonic.png");
	RotatingSonic->SetLocalPosition(200, 200);
	RotatingSonic->SetParent(SonicParent, false);
	dae::RotatingComponent* SecondRot = RotatingSonic->CreateComponent<dae::RotatingComponent>();
	SecondRot->m_RotationSpeed = 10.f;
	SecondRot->m_Radius = 100.f;







	
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
	engine.Run(LoadTrashTheCacheAssigment);
    return 0;
}