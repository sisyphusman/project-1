#include "StatusPanel.h"

StatusPanel::StatusPanel(float x, float y, float width, float height)
	: UIPanel(x, y, width, height)
{
}

void StatusPanel::Update(float deltaTime)
{
}

void StatusPanel::Render(sf::RenderWindow& window, const sf::Font& font)
{
	DrawBackground(window, sf::Color(30, 40, 30), sf::Color(60, 80, 60));

	const CharacterStats& stats = StatsPtr ? *StatsPtr : CachedStats;

	float padding = 10.f;
	float barHeight = 24.f;
	float barWidth = Bounds.size.x - padding * 2.f;
	float yOffset = Bounds.position.y + padding;

	// HP 바
	DrawProgressBar(window, font, Bounds.position.x + padding, yOffset, barWidth, barHeight,
		stats.HP.Current, stats.HP.Max, sf::Color(180, 50, 50), sf::Color(60, 20, 20), "HP");
	yOffset += barHeight + 8.f;

	DrawProgressBar(window, font, Bounds.position.x + padding, yOffset, barWidth, barHeight,
		stats.MP.Current, stats.MP.Max, sf::Color(50, 80, 180), sf::Color(23, 30, 60), "MP");
	yOffset += barHeight + 8.f;

	// 레벨 & 스텟
	sf::Text statText(font, "", 16);
	statText.setFillColor(sf::Color(200, 200, 200));

	statText.setString(std::format("LV.{}", stats.Level));
	statText.setPosition({ Bounds.position.x + padding, yOffset });
	window.draw(statText);
	yOffset += 24.f;

	statText.setString(std::format("STR:{} DEX:{} INT:{}", stats.STR, stats.DEX, stats.INT));
	statText.setCharacterSize(14);
	statText.setPosition({ Bounds.position.x + padding, yOffset });
	window.draw(statText);
	yOffset += 28.f;

	// 상태효과
	if (!StatusEffects.empty())
	{
		sf::Text effectText(font, "", 14);
		for (const auto& effect : StatusEffects)
		{
			effectText.setString(sf::String::fromUtf8(effect.Name.begin(), effect.Name.end()));
			effectText.setFillColor(effect.Color);
			effectText.setPosition({ Bounds.position.x + padding, yOffset });
			window.draw(effectText);
			yOffset += 18.f;
		}
	}
}

void StatusPanel::SetStats(const CharacterStats& stats)
{
	CachedStats = stats;
	StatsPtr = nullptr;
}

void StatusPanel::DrawProgressBar(sf::RenderWindow& window, const sf::Font& font, float x, float y, float width, float height, int current, int max, sf::Color barColor, sf::Color bgColor, const std::string& label)
{
	// 배경
	sf::RectangleShape bg;
	bg.setSize({ width, height });
	bg.setPosition({ x, y });
	bg.setFillColor(bgColor);
	bg.setOutlineColor(sf::Color(80, 80, 80));
	bg.setOutlineThickness(1.f);
	window.draw(bg);

	// 프로그래스
	float			   ratio = (max > 0) ? static_cast<float>(current) / max : 0.f;
	sf::RectangleShape bar;
	bar.setSize({ width * ratio, height });
	bar.setPosition({ x, y });
	bar.setFillColor(barColor);
	window.draw(bar);

	// 텍스트
	std::string valueStr = std::format("{}/{}", current, max);
	sf::Text	valueText(font, valueStr, 14);
	valueText.setFillColor(sf::Color::White);

	sf::FloatRect textBounds = valueText.getLocalBounds();
	valueText.setPosition({ x + (width - textBounds.size.x) / 2.f, y + (height - textBounds.size.y) / 2.f - 2.f });
	window.draw(valueText);
}

void StatusPanel::AddStatusEffect(const std::string& name, sf::Color color, int duration)
{
	StatusEffects.push_back({ name, color, duration });
}

void StatusPanel::RemoveStatusEffect(const std::string& name)
{
	// 조건에 맞는 모든 요소를 컨테이너에서 제거
	std::erase_if(StatusEffects, [&](const StatusEffect& e) {
		return e.Name == name;
	});
}

void StatusPanel::ClearStatusEffects()
{
	StatusEffects.clear();
}
