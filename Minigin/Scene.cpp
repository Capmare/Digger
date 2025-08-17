#include "Scene.h"
#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

//void Scene::Add(std::shared_ptr<GameObject> object)
//{
//	m_objects.emplace_back(std::move(object));
//}
//
//void Scene::Remove(std::shared_ptr<GameObject> object)
//{
//	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
//}

void Scene::RemoveAll()
{

	for (auto& obj : m_objects) {
		if (!obj->IsMarkedForDestroy())     
			obj->Destroy();
	}

	m_objects.clear();
}

dae::GameObject* dae::Scene::CreateNewGameObject()
{
	m_objects.emplace_back(std::make_unique<GameObject>());
	return m_objects.back().get();
}

dae::Observer* dae::Scene::CreateObserver(std::unique_ptr<Observer> observer)
{
	m_observers.emplace_back(std::move(observer));
	return m_observers.back().get();
}

void dae::Scene::Update(float dt)
{
	std::vector<std::unique_ptr<GameObject>> old = std::move(m_objects);
	m_objects.clear();
	m_objects.reserve(old.size());

	for (auto& obj : old)
	{
		if (!obj) continue; 

		if (obj->IsMarkedForDestroy())
		{
			std::cout << "Checking " << obj->m_Name << " -> Destroyed\n";
			continue;
		}

		obj->Update(dt);
		m_objects.emplace_back(std::move(obj)); 
	}

}


void Scene::FixedUpdate( const float fixedDeltaTime)
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate(fixedDeltaTime);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

