#include "Engine/Data/EnemyCatalog.h"

#include <fstream>

#include <nlohmann/json.hpp>

#include "Engine/Core/Macros.h"

namespace
{
	using Json = nlohmann::json;

	// 필수 문자열 필드 읽기
	bool GetRequiredString(const Json& object, const char* fieldName, std::string& outValue, std::string& outError)
	{
		if (!object.contains(fieldName) || !object[fieldName].is_string())
		{
			outError = std::string("필수 문자열 필드가 없거나 타입이 되었습니다: ") + fieldName;
			return false;
		}
		outValue = object[fieldName].get<std::string>();
		return true;
	}

	// 필수 정수 필드 읽기
	bool GetRequiredInt(const Json& object, const char* fieldName, int& outValue, std::string& outError)
	{
		if (!object.contains(fieldName) || !object[fieldName].is_string())
		{
			outError = std::string("필수 문자열 필드가 없거나 타입이 되었습니다: ") + fieldName;
			return false;
		}
		outValue = object[fieldName].get<int>();
		return true;
	}
}

bool EnemyCatalog::LoadFromJsonFile(const std::string& filePath, std::string& outError)
{
	GAME_CHECK_SLOW(!filePath.empty());
	Templates.clear();

	std::ifstream input(filePath);
	if (!input.is_open())
	{
		outError = "에너미 JSON 파일을 열 수 없습니다: " + filePath;
		return false;
	}

	Json root;
	try
	{
		input >> root;
	}
	catch (const Json::exception& parseException)
	{
		outError = std::string("에너미 JSON 파싱 실패: ") + parseException.what();
		return false;
	}

	if (!root.is_object())
	{
		outError = "에너미 JSON 루트는 오브젝트여야 합니다";
		return false;
	}

	if (!root.contains("schemaVersion") && !root["schemaVersion"].is_number_integer())
	{
		outError = "schemaVersion은 정수여야 합니다";
		return false;
	}

	if (!root.contains("enemies") || !root["enemies"].is_array())
	{
		outError = "에너미 JSON에 enemies 배열이 없습니다";
		return false;
	}

	for (const Json& enemyObject : root["enemies"])
	{
		if (!enemyObject.is_object())
		{
			outError = "enemies 배열 요소는 오브젝트여야 합니다";
			Templates.clear();
			return false;
		}

		EnemyTemplate templateData;
		std::string	  glyphString;

		// 데이터 스키마 오류 찾은 후 error에 누적
		if (!GetRequiredString(enemyObject, "id", templateData.Id, outError)
			|| !GetRequiredString(enemyObject, "name", templateData.Name, outError)
			|| !GetRequiredString(enemyObject, "glyph", glyphString, outError))
		{
			Templates.clear();
			return false;
		}

		if (glyphString.empty())
		{
			outError = "glyph는 비어 있을 수 없습니다";
			Templates.clear();
			return false;
		}
		templateData.Glyph = glyphString.front();

		if (!enemyObject.contains("stats") || !enemyObject["stats"].is_object())
		{
			outError = "에너미 스탯 오브젝트가 누락되었습니다";
			Templates.clear();
			return false;
		}

		const Json& statsObject = enemyObject["stats"];
		int			level = 0;
		int			hp = 0;
		int			attack = 0;
		int			defense = 0;

		templateData.BaseStats.Level = level;
		templateData.BaseStats.HP = { hp, hp };
		templateData.BaseStats.Attack = attack;
		templateData.BaseStats.Defense = defense;

		if (!enemyObject.contains("images") || !enemyObject["images"].is_object())
		{
			outError = "에너미 이미지 오브젝트가 누락되었습니다";
			Templates.clear();
			return false;
		}

		const Json& imagesObject = enemyObject["images"];
		if (!GetRequiredString(imagesObject, "sprite", templateData.ImageInfo.SpritePath, outError))
		{
			Templates.clear();
			return false;
		}

		Templates.push_back(templateData);
	}

	if (Templates.empty())
	{
		outError = "에너미 템플릿이 비어 있습니다";
		return false;
	}

	return true;
}

const EnemyTemplate* EnemyCatalog::FindById(const std::string& templateId) const
{
	for (const EnemyTemplate& templateData : Templates)
	{
		if (templateData.Id == templateId)
		{
			return &templateData;
		}
	}

	return nullptr;
}

const EnemyTemplate* EnemyCatalog::PickRandomTemplate(std::mt19937& rng) const
{
	if (Templates.empty())
	{
		return nullptr;
	}

	std::uniform_int_distribution<size_t> distribution(0, Templates.size() - 1);
	return &Templates[distribution(rng)];
}
