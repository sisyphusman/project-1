#include "Engine/UI/Panel/InfoPanel.h"

#include "Engine/Core/Style.h"

#include <algorithm>
#include <format>

InfoPanel::InfoPanel(float x, float y, float width, float height)
	: UIPanel(x, y, width, height)
{
}

void InfoPanel::Render(sf::RenderWindow& window, const sf::Font& font)
{
	DrawBackground(window, Colors::Panel::InfoBg, Colors::Panel::InfoBorder);

	if (StatsPtr == nullptr)
	{
		sf::Text emptyText(font, "No stats source", UILayout::Tunable::SmallFontSize);
		emptyText.setFillColor(Colors::Text);
		emptyText.setPosition({ Bounds.position.x + UILayout::Tunable::Padding, Bounds.position.y + UILayout::Tunable::Padding });
		window.draw(emptyText);
		return;
	}

	const CharacterStats& stats = *StatsPtr;

	float barWidth = Bounds.size.x - UILayout::Tunable::Padding * 2.f;
	float xPos = Bounds.position.x + UILayout::Tunable::Padding;
	float yPos = Bounds.position.y + UILayout::Tunable::Padding;

	// HP 바
	DrawProgressBar(window, font, xPos, yPos, barWidth, UILayout::Tunable::BarHeight, stats.HP.Current, stats.HP.Max, Colors::Bar::HP, Colors::Bar::HPBg);
	yPos += UILayout::Tunable::BarHeight + UILayout::Tunable::BarSpacing;

	// MP 바
	DrawProgressBar(window, font, xPos, yPos, barWidth, UILayout::Tunable::BarHeight, stats.MP.Current, stats.MP.Max, Colors::Bar::MP, Colors::Bar::MPBg);
	yPos += UILayout::Tunable::BarHeight + UILayout::Tunable::SectionSpacing;

	// 레벨
	sf::Text statText(font, std::format("Lv.{}", stats.Level), UILayout::Tunable::FontSize);
	statText.setFillColor(Colors::Text);
	statText.setPosition({ xPos, yPos });
	window.draw(statText);
	yPos += UILayout::Tunable::LineSpacing + UILayout::Tunable::InfoStatExtraSpacing;

	// 스탯
	statText.setCharacterSize(UILayout::Tunable::SmallFontSize);

	statText.setString(std::format("STR:{}", stats.STR));
	statText.setPosition({ xPos, yPos });
	window.draw(statText);
	yPos += UILayout::Tunable::LineSpacing;

	statText.setString(std::format("DEX:{}", stats.DEX));
	statText.setPosition({ xPos, yPos });
	window.draw(statText);
	yPos += UILayout::Tunable::LineSpacing;

	statText.setString(std::format("INT:{}", stats.INT));
	statText.setPosition({ xPos, yPos });
	window.draw(statText);
	yPos += UILayout::Tunable::LineSpacing + UILayout::Tunable::InfoStatExtraSpacing;

	// 상태효과
	for (const auto& effect : StatusEffects)
	{
		sf::Text effectText(font, effect.Name, UILayout::Tunable::SmallFontSize);
		effectText.setFillColor(effect.Color);
		effectText.setPosition({ xPos, yPos });
		window.draw(effectText);
		yPos += UILayout::Tunable::LineSpacing;
	}
}

void InfoPanel::SetSource(const CharacterStats* stats)
{
	StatsPtr = stats;
}

void InfoPanel::DrawProgressBar(sf::RenderWindow& window, const sf::Font& font, float x, float y, float width, float height, int current, int max,
	sf::Color barColor, sf::Color bgColor)
{
	// 배경
	sf::RectangleShape bg;
	bg.setSize({ width, height });
	bg.setPosition({ x, y });
	bg.setFillColor(bgColor);
	bg.setOutlineColor(Colors::Bar::Outline);
	bg.setOutlineThickness(UILayout::Tunable::BarOutlineThickness);
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
	sf::Text	valueText(font, valueStr, UILayout::Tunable::SmallFontSize);
	valueText.setFillColor(Colors::White);

	sf::FloatRect textBounds = valueText.getLocalBounds();
	valueText.setPosition({ x + (width - textBounds.size.x) / 2.f, y + (height - textBounds.size.y) / 2.f + UILayout::Tunable::BarTextOffsetY });
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
