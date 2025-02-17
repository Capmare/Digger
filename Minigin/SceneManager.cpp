#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update( const float deltaTime)
{
	deltaTime;
	for(auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void dae::SceneManager::FixedUpdate( const float fixedDeltaTime)
{
	for (const auto& scene : m_scenes)
	{
		scene->FixedUpdate(fixedDeltaTime);
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
