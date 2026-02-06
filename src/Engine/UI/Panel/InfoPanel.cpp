#include "Engine/UI/Panel/InfoPanel.h"

#include "Engine/Core/Style.h"

#include <algorithm>
#include <format>

InfoPanel::InfoPanel(float x, float y, float width, float height)
	: UIPanel(x, y, width, height)
{
}

void InfoPanel::Update(float deltaTime)
{
}

void InfoPanel::Render(sf::RenderWindow& window, const sf::Font& font)
{
	DrawBackground(window, Colors::Black, Colors::Panel::InfoBorder);

	const CharacterStats& stats = StatsPtr ? *StatsPtr : CachedStats;
	const Style&		  s = DefaultStyle;

	float barWidth = Bounds.size.x - s.Padding * 2.f;
	float xPos = Bounds.position.x + s.Padding;
	float yPos = Bounds.position.y + s.Padding;

	// HP 바
	DrawProgressBar(window, font, xPos, yPos, barWidth, s.BarHeight, stats.HP.Current, stats.HP.Max, Colors::Bar::HP, Colors::Bar::HPBg);
	yPos += s.BarHeight + s.BarSpacing;

	// MP 바
	DrawProgressBar(window, font, xPos, yPos, barWidth, s.BarHeight, stats.MP.Current, stats.MP.Max, Colors::Bar::MP, Colors::Bar::MPBg);
	yPos += s.BarHeight + s.SectionSpacing;

	// 레벨
	sf::Text statText(font, std::format("Lv.{}", stats.Level), s.FontSize);
	statText.setFillColor(Colors::Text);
	statText.setPosition({ xPos, yPos });
	window.draw(statText);
	yPos += s.LineSpacing + 4.f;

	// 스탯
	statText.setCharacterSize(s.SmallFontSize);

	statText.setString(std::format("STR:{}", stats.STR));
	statText.setPosition({ xPos, yPos });
	window.draw(statText);
	yPos += s.LineSpacing;

	statText.setString(std::format("DEX:{}", stats.DEX));
	statText.setPosition({ xPos, yPos });
	window.draw(statText);
	yPos += s.LineSpacing;

	statText.setString(std::format("INT:{}", stats.INT));
	statText.setPosition({ xPos, yPos });
	window.draw(statText);
	yPos += s.LineSpacing + 4.f;

	// 상태효과
	for (const auto& effect : StatusEffects)
	{
		sf::Text effectText(font, effect.Name, s.SmallFontSize);
		effectText.setFillColor(effect.Color);
		effectText.setPosition({ xPos, yPos });
		window.draw(effectText);
		yPos += s.LineSpacing;
	}
}

void InfoPanel::SetStats(const CharacterStats& stats)
{
	CachedStats = stats;
	StatsPtr = nullptr;
}

void InfoPanel::DrawProgressBar(sf::RenderWindow& window, const sf::Font& font,
	float x, float y, float width, float height, int current, int max, sf::Color barColor, sf::Color bgColor)
{
	const Style& s = DefaultStyle;

	// 배경
	sf::RectangleShape bg;
	bg.setSize({ width, height });
	bg.setPosition({ x, y });
	bg.setFillColor(bgColor);
	bg.setOutlineColor(Colors::Bar::Outline);
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
	sf::Text	valueText(font, valueStr, s.SmallFontSize);
	valueText.setFillColor(Colors::White);

	sf::FloatRect textBounds = valueText.getLocalBounds();
	valueText.setPosition({ x + (width - textBounds.size.x) / 2.f, y + (height - textBounds.size.y) / 2.f - 2.f });
	window.draw(valueText);
}

void InfoPanel::AddStatusEffect(const std::string& name, sf::Color color, int duration)
{
	StatusEffects.push_back({ name, color, duration });
}

void InfoPanel::RemoveStatusEffect(const std::string& name)
{
	std::erase_if(StatusEffects, [&](const StatusEffect& e) { return e.Name == name; });
}

void InfoPanel::ClearStatusEffects()
{
	StatusEffects.clear();
}
