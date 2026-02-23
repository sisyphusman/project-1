#include "Engine/UI/Panel/MessageLogPanel.h"

#include "Engine/Core/Style.h"

MessageLogPanel::MessageLogPanel(float x, float y, float width, float height)
	: UIPanel(x, y, width, height)
{
	int padding = UILayout::Tunable::LogPaddingTop;
	int lineHeight = UILayout::Derived::LogLineHeight();
	MaxVisibleLines = static_cast<int>(height - padding * 2) / lineHeight;
}

void MessageLogPanel::Render(sf::RenderWindow& window, const sf::Font& font)
{
	// 패널 배경과 테두리 색
	DrawBackground(window, Colors::Panel::LogBg, Colors::Panel::LogBorder);

	auto entries = Log.GetRecentMessages(MaxVisibleLines);

	sf::Text logText(font, "", UILayout::Tunable::LogFontSize);

	float xPos = Bounds.position.x + static_cast<float>(UILayout::Tunable::LogPaddingLeft);
	float yPos = Bounds.position.y + static_cast<float>(UILayout::Tunable::LogPaddingTop);

	// 가져온 메시지들을 위에서 아래로 순차 출력
	for (const Message& entry : entries)
	{
		logText.setString(sf::String::fromUtf8(entry.Text.begin(), entry.Text.end()));
		logText.setPosition({ xPos, yPos });

		logText.setFillColor(sf::Color(
			static_cast<uint8_t>((entry.Color >> 24) & 0xFF),
			static_cast<uint8_t>((entry.Color >> 16) & 0xFF),
			static_cast<uint8_t>((entry.Color >> 8) & 0xFF),
			static_cast<uint8_t>(entry.Color & 0xFF)));

		window.draw(logText);
		yPos += UILayout::Derived::LogLineHeight();
	}
}