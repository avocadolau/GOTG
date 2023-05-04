#include <wipch.h>
#include "GameEnemyManager.h"
#include <Wiwa/utilities/Log.h>
#include "json.hpp"

namespace nlohmann {
    template <>
    struct adl_serializer<Wiwa::EnemyData> {
        static void to_json(json& j, const Wiwa::EnemyData& e)
        {
            j = json{ {"level", e.level}, {"name", e.name}, {"health", e.health}, {"damage", e.damage}, {"maxVelocity", e.maxVelocity}, 
				{"maxAcceleration", e.maxAcceleration}, {"rateOfFire", e.rateOfFire},  {"range", e.range}, {"creditsDrop", e.creditsDrop} };
        }

        static void from_json(const json& j, Wiwa::EnemyData& e)
        {
            j.at("level").get_to(e.level);
            j.at("name").get_to(e.name);
            j.at("health").get_to(e.health);
            j.at("damage").get_to(e.damage);
            j.at("maxVelocity").get_to(e.maxVelocity);
            j.at("maxAcceleration").get_to(e.maxAcceleration);
            j.at("rateOfFire").get_to(e.rateOfFire);
            j.at("range").get_to(e.range);
            j.at("creditsDrop").get_to(e.creditsDrop);
        }
    };

    template <>
    struct adl_serializer<std::map<std::pair<int, std::string>, Wiwa::EnemyData>> {
        static void to_json(json& j, const std::map<std::pair<int, std::string>, Wiwa::EnemyData>& m)
        {
            for (const auto& [key, value] : m) {
                j[std::to_string(key.first) + "_" + key.second] = value;
            }
        }

        static void from_json(const json& j, std::map<std::pair<int, std::string>, Wiwa::EnemyData>& m)
        {
            for (const auto& [key, value] : j.items()) {
                auto underscore_pos = key.find_first_of('_');
                int first = std::stoi(key.substr(0, underscore_pos));
                std::string second = key.substr(underscore_pos + 1);
                m[std::make_pair(first, second)] = value.get<Wiwa::EnemyData>();
            }
        }
    };
}

struct MapKeyComparator {
	bool operator()(const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) const {
		if (a.second != b.second) {
			return a.second < b.second; // sort by string first
		}
		return a.first < b.first; // then sort by int
	}
};

using json = nlohmann::json;

namespace Wiwa
{
	EnemyManager::EnemyManager()
	{
		m_MaxLevel = 4;
	}
	EnemyManager::~EnemyManager()
	{
	}
	void EnemyManager::CleanUp()
	{
	}
	
	bool EnemyManager::Serialize()
	{
		return false;
	}

	bool EnemyManager::DeSerialize()
	{
		std::filesystem::path path;
		path = "config/enemy_data.json";
		bool exists = std::filesystem::exists(path);

		if (!exists)
			CreateEmptyFile();

		std::ifstream file(path);
		json serializedData;
		if (file.is_open())
		{
			std::string data((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());

			serializedData = json::parse(data);

			m_EnemyData = serializedData.get<std::map<std::pair<int, std::string>, EnemyData>>();

			file.close();
			WI_INFO("Enemy stats saved to enemy_stats.json");
			return true;
		}

		return false;
	}

	bool EnemyManager::CreateEmptyFile()
	{
		// Load base stats melee phalanx generic
		EnemyData meleePhalanxGeneric;
		meleePhalanxGeneric.name = "MELEE_PHALANX_GENERIC";
		meleePhalanxGeneric.health = 30;
		meleePhalanxGeneric.damage = 10;
		meleePhalanxGeneric.maxVelocity = 10;
		meleePhalanxGeneric.maxAcceleration = 10;
		meleePhalanxGeneric.rateOfFire = 1;
		meleePhalanxGeneric.range = 0;
		meleePhalanxGeneric.creditsDrop = 15;
		meleePhalanxGeneric.level = 0;
		m_EnemyData[{meleePhalanxGeneric.level, meleePhalanxGeneric.name.c_str()}] = meleePhalanxGeneric;

		// Load base stats melee phalanx RedVariant
		EnemyData meleePhalanxRedVariant;
		meleePhalanxRedVariant.name = "MELEE_PHALANX_REDVARIANT";
		meleePhalanxRedVariant.health = 70;
		meleePhalanxRedVariant.damage = 20;
		meleePhalanxRedVariant.maxVelocity = 10;
		meleePhalanxRedVariant.maxAcceleration = 10;
		meleePhalanxRedVariant.rateOfFire = 1;
		meleePhalanxRedVariant.range = 0;
		meleePhalanxRedVariant.creditsDrop = 20;
		meleePhalanxRedVariant.level = 0;
		m_EnemyData[{meleePhalanxRedVariant.level, meleePhalanxRedVariant.name.c_str()}] = meleePhalanxRedVariant;
		m_EnemyData[{meleePhalanxRedVariant.level++, meleePhalanxRedVariant.name.c_str()}] = meleePhalanxRedVariant;
		m_EnemyData[{meleePhalanxRedVariant.level++, meleePhalanxRedVariant.name.c_str()}] = meleePhalanxRedVariant;
		m_EnemyData[{meleePhalanxRedVariant.level++, meleePhalanxRedVariant.name.c_str()}] = meleePhalanxRedVariant;

		// Load base stats melee phalanx BlueVariant
		EnemyData meleePhalanxBlueVariant;
		meleePhalanxBlueVariant.name = "MELEE_PHALANX_BLUEVARIANT";
		meleePhalanxBlueVariant.health = 20;
		meleePhalanxBlueVariant.damage = 20;
		meleePhalanxBlueVariant.maxVelocity = 10;
		meleePhalanxBlueVariant.maxAcceleration = 10;
		meleePhalanxBlueVariant.rateOfFire = 3;
		meleePhalanxBlueVariant.range = 0;
		meleePhalanxBlueVariant.creditsDrop = 20;
		meleePhalanxBlueVariant.level = 0;
		m_EnemyData[{meleePhalanxBlueVariant.level, meleePhalanxBlueVariant.name.c_str()}] = meleePhalanxBlueVariant;

		// Load base stats ranged phalanx generic
		EnemyData rangedPhalanxGeneric;
		rangedPhalanxGeneric.name = "RANGED_PHALANX_GENERIC";
		rangedPhalanxGeneric.health = 20;
		rangedPhalanxGeneric.damage = 5;
		rangedPhalanxGeneric.maxVelocity = 10;
		rangedPhalanxGeneric.maxAcceleration = 10;
		rangedPhalanxGeneric.range = 5;
		rangedPhalanxGeneric.rateOfFire = 1;
		rangedPhalanxGeneric.creditsDrop = 15;
		rangedPhalanxGeneric.level = 0;
		m_EnemyData[{rangedPhalanxGeneric.level, rangedPhalanxGeneric.name.c_str()}] = rangedPhalanxGeneric;

		// Load base stats ranged phalanx red variant
		EnemyData rangedPhalanxRedVariant;
		rangedPhalanxRedVariant.name = "RANGED_PHALANX_REDVARIANT";
		rangedPhalanxRedVariant.health = 20;
		rangedPhalanxRedVariant.damage = 25;
		rangedPhalanxRedVariant.maxVelocity = 10;
		rangedPhalanxRedVariant.maxAcceleration = 10;
		rangedPhalanxRedVariant.range = 8;
		rangedPhalanxRedVariant.rateOfFire = 1.5;
		rangedPhalanxRedVariant.creditsDrop = 20;
		rangedPhalanxRedVariant.level = 0;
		m_EnemyData[{rangedPhalanxRedVariant.level, rangedPhalanxRedVariant.name.c_str()}] = rangedPhalanxRedVariant;

		// Load base stats ranged phalanx blue variant
		EnemyData rangedPhalanxblueVariant;
		rangedPhalanxblueVariant.name = "RANGED_PHALANX_BLUEVARIANT";
		rangedPhalanxblueVariant.health = 20;
		rangedPhalanxblueVariant.damage = 20;
		rangedPhalanxblueVariant.maxVelocity = 10;
		rangedPhalanxblueVariant.maxAcceleration = 10;
		rangedPhalanxblueVariant.range = 5;
		rangedPhalanxblueVariant.rateOfFire = 3;
		rangedPhalanxblueVariant.creditsDrop = 25;
		rangedPhalanxblueVariant.level = 0;
		m_EnemyData[{rangedPhalanxblueVariant.level, rangedPhalanxblueVariant.name.c_str()}] = rangedPhalanxblueVariant;

		// Load base stats sentinel
		EnemyData sentinel;
		sentinel.name = "SENTINEL";
		sentinel.health = 20;
		sentinel.damage = 25;
		sentinel.maxVelocity = 10;
		sentinel.maxAcceleration = 10;
		sentinel.range = 2;
		sentinel.rateOfFire = 0;
		sentinel.creditsDrop = 15;
		sentinel.level = 0;
		m_EnemyData[{sentinel.level, sentinel.name.c_str()}] = sentinel;

		// Load base stats subjugator
		EnemyData subjugator;
		subjugator.name = "SUBJUGATOR";
		subjugator.health = 40;
		subjugator.damage = 15;
		subjugator.maxVelocity = 10;
		subjugator.maxAcceleration = 10;
		subjugator.range = 8;
		subjugator.rateOfFire = 2;
		subjugator.creditsDrop = 30;
		subjugator.level = 0;
		m_EnemyData[{subjugator.level, subjugator.name.c_str()}] = subjugator;

		// Load base stats subjugatorChief
		EnemyData subjugatorChief;
		subjugatorChief.name = "SUBJUGATOR_CHIEF";
		subjugatorChief.health = 50;
		subjugatorChief.damage = 25;
		subjugatorChief.maxVelocity = 10;
		subjugatorChief.maxAcceleration = 10;
		subjugatorChief.range = 10;
		subjugatorChief.rateOfFire = 2;
		subjugatorChief.creditsDrop = 35;
		subjugatorChief.level = 0;
		m_EnemyData[{subjugatorChief.level, subjugatorChief.name.c_str()}] = subjugatorChief;

		// Load base stats ultron
		EnemyData ultron;
		ultron.name = "ULTRON";
		ultron.health = 500;
		ultron.damage = 20;
		ultron.maxVelocity = 10;
		ultron.maxAcceleration = 10;
		ultron.range = 15;
		ultron.rateOfFire = 2;
		ultron.creditsDrop = 35;
		ultron.level = 0;
		m_EnemyData[{ultron.level, ultron.name.c_str()}] = ultron;

		json doc = m_EnemyData;
		std::ofstream file("config/enemy_data.json");
		if (file.is_open())
		{
			file << doc;
			file.close();
			WI_INFO("Enemy stats saved to enemy_stats.json");
			return true;
		}
		
		WI_INFO("Error: Could not open config/enemy_data.json for writing");
		return false;
	}

	void EnemyManager::Reset()
	{
		std::filesystem::path path;
		path = "config/enemy_data.json";
		std::filesystem::remove(path);

		CreateEmptyFile();
	}

	bool EnemyManager::CheckIfHasLevel(int level, const std::string& str)
	{	
		std::pair<int, std::string> key = std::make_pair(level, str);
		EnemyData enemyData;

		// Check if the key exists in the map
		auto it = m_EnemyData.find(key);
		if (it == m_EnemyData.end()) {
			m_EnemyData.insert({ key, enemyData });
			return true;
		}
		return false;
	}

	//void EnemyManager::PushBackArrayJson(JSONValue& json_value_array, const EnemyData& data)
	//{
	//	JSONValue dataJson = json_value_array.PushBackObject();
	//	dataJson.AddMember("level", data.level);
	//	dataJson.AddMember("name", data.name.c_str());
	//	dataJson.AddMember("health", data.health);
	//	dataJson.AddMember("damage", data.damage);
	//	dataJson.AddMember("maxVelocity", data.maxVelocity);
	//	dataJson.AddMember("maxAcceleration", data.maxAcceleration);
	//	dataJson.AddMember("rateOfFire", data.rateOfFire);
	//	dataJson.AddMember("creditsDrop", data.creditsDrop);
	//}



}
