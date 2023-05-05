#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <Wiwa/ecs/components/game/enemy/EnemyData.h>

namespace Wiwa
{
	/*struct WI_API EnemyData
	{
		std::string name = "NONE";
		int level = 0;
		int health = 0;
		int damage = 0;
		float maxVelocity = 0;
		float maxAcceleration = 0;
		float rateOfFire = 0;
		float range = 0;
		int creditsDrop = 0;
	};*/

	class WI_API EnemyManager
	{
	public:
		EnemyManager();
		~EnemyManager();

		void CleanUp();
		bool Serialize();
		bool DeSerialize();
		bool CreateEmptyFile();
		void Reset();

		bool CheckIfHasLevelCreate(int level, const std::string& str);
		bool CheckIfHasLevelDelete(int level, const std::string& str);

		//void PushBackArrayJson(JSONValue& json_value, const EnemyData& data);
		inline std::map<std::pair<int, std::string>, EnemyData>& GetData() { return m_EnemyData; };
		std::map<std::pair<int, std::string>, EnemyData> m_EnemyData;

		const int m_BaseLevel = 0;
		int m_MaxLevel = 4;
		int m_CurrentRunLevel = 0;
		int m_IncreaseDiffEvery = 1;
	};
}