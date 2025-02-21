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
#include "FpsComponent.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "TextureComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	std::shared_ptr<dae::GameObject> BackgroundGo = std::make_shared<dae::GameObject>();
	dae::TextureComponent* BackgroundImageTextureComp = BackgroundGo->CreateComponent<dae::TextureComponent>();
	dae::TextureComponent* LogoImageTextureComp = BackgroundGo->CreateComponent<dae::TextureComponent>();

	BackgroundImageTextureComp->SetTexture("background.tga");
	LogoImageTextureComp->SetPosition(216, 180);
	LogoImageTextureComp->SetTexture("logo.tga");

	scene.Add(BackgroundGo);

	std::shared_ptr<dae::GameObject> FPSGo = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	FPSGo->CreateComponent<dae::TextComponent>("Text",font);
	FPSGo->CreateComponent<dae::FpsComponent>();

	auto aliveComp = BackgroundGo->GetAllComponentsOfType<dae::TextureComponent>();

	// BackgroundGo->UnregisterComponentAtIndex(0); // this is used to delete components safely

	scene.Add(FPSGo);

}

int main(int, char*[]) {
	dae::Minigin engine("Data/");
	engine.Run(load);
    return 0;
}