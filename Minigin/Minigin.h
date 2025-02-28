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

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;


	private:

		void ImGuiInterface();

		std::vector<float> m_IntTrashCacheData{};
		std::vector<float> m_GameObject3DTrashCacheData{};
		std::vector<float> m_GameObject3DAltTrashCacheData{};

		int m_IntSampleSize{ 10000000 };
		int m_GameObject3DSampleSize{ 10000000 };
		int m_GameObject3DAltSampleSize{ 10000000 };
		
		bool m_bIntTrashFinished{};
		bool m_bGameObjectTrashFinished{};
		bool m_bGameObjectAltTrashFinished{};

	};
}