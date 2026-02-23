#pragma once

#include <optional>
#include <random>
#include <string>
#include <vector>

#include "Engine/Entities/CharacterStats.h"

struct EnemyTemplate
{
	std::string	   Id;
	std::string	   Name;
	std::string	   SpritePath;
	char		   Glyph = 'e';
	CharacterStats BaseStats;
};

// 적 데이터 템플릿 모음
class EnemyCatalog
{
public:
	bool LoadFromJsonFile(const std::string& filePath, std::string& outError);

	const EnemyTemplate*			  FindById(const std::string& templateId) const;
	const EnemyTemplate*			  PickRandomTemplate(std::mt19937& rng) const;
	const std::vector<EnemyTemplate>& GetTemplates() const { return Templates; }

private:
	std::vector<EnemyTemplate> Templates;
};