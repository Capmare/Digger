#pragma once
#include "SceneManager.h"
#include <memory>
#include "GameObject.h"
#include "Observer.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		//void Add(std::shared_ptr<GameObject> object);
		//void Remove(std::shared_ptr<GameObject> object);
		GameObject* CreateNewGameObject();
		Observer* CreateObserver(std::unique_ptr<Observer> observer);
		void RemoveAll();

		void Update(const float deltaTime);
		void FixedUpdate(const float fixedDeltaTime);
		void Render() const;

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::unique_ptr<GameObject>> m_objects{};
		std::vector < std::unique_ptr<Observer>> m_observers{};

		static unsigned int m_idCounter; 
	};

}
