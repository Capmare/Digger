#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		Scene& GetLastScene() {return *m_scenes.back(); }



		void Update(const float deltaTime);
		void Render();
		void FixedUpdate(const float fixedDeltaTime);
		void RenderUI();

		bool m_bUseImGui{ false };
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;

		void ImGuiInterface();
		template <typename T> void TrashCacheWithData(std::vector<T>& dataset, std::vector<float>& cacheData, std::vector<float>& cacheDataAverage, int sampleSize, const char* datasetName);

		std::vector<float> m_IntTrashCacheData{};
		std::vector<float> m_IntTrashCacheDataAverage{};

		std::vector<float> m_GameObject3DTrashCacheData{};
		std::vector<float> m_GameObject3DTrashCacheDataAverage{};

		std::vector<float> m_GameObject3DAltTrashCacheData{};
		std::vector<float> m_GameObject3DAltTrashCacheDataAverage{};


		int m_IntSampleSize{ 100 };
		int m_GameObject3DSampleSize{ 100 };
		int m_GameObject3DAltSampleSize{ 100 };

		bool m_bIntTrashFinished{};
		bool m_bGameObjectTrashFinished{};
		bool m_bGameObjectAltTrashFinished{};

	};




}
