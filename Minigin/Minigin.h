#pragma once
#include <string>
#include <functional>




static bool AssigmentOne{ true };

namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);

		template <typename T> void TrashCacheWithData(std::vector<T>& dataset, std::vector<float>& cacheData, std::vector<float>& cacheDataAverage, int sampleSize, const char* datasetName);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;


	private:

		


		void ImGuiInterface();

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