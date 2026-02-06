#include "Engine/UI/Panel/PortraitPanel.h"
#include "Engine/Core/Macros.h"
#include "Engine/Core/Style.h"

PortraitPanel::PortraitPanel(float x, float y, float width, float height)
	: UIPanel(x, y, width, height)
{
}

void PortraitPanel::Update(float deltaTime)
{
}

void PortraitPanel::Render(sf::RenderWindow& window, const sf::Font& font)
{
	const Style& s = DefaultStyle;

	// 배경
	DrawBackground(window, Colors::Panel::PortraitBg, Colors::Panel::PortraitBorder);

	if (PortraitSprite.has_value() && PortraitTexture.has_value())
	{
		// 이미지를 패널 크기에 맞게 스케일링
		sf::Vector2u texSize = PortraitTexture->getSize();
		float		 scaleX = Bounds.size.x / static_cast<float>(texSize.x);
		float		 scaleY = Bounds.size.y / static_cast<float>(texSize.y);
		float		 scale = std::min(scaleX, scaleY);

		PortraitSprite->setScale({ scale, scale });

		// 중앙 정렬
		float scaleWidth = texSize.x * scale;
		float scaleHeight = texSize.y * scale;
		float offsetX = (Bounds.size.x - scaleWidth) / 2.f;
		float offsetY = (Bounds.size.y - scaleHeight) / 2.f;

		PortraitSprite->setPosition({ Bounds.position.x + offsetX, Bounds.position.y + offsetY });
		window.draw(*PortraitSprite);
	}
	else
	{
		// 플레이스 홀더 : 캐릭터 아이콘
		sf::Text placeholder(font, "[@]", s.PortraitPlaceholderFontSize);
		placeholder.setFillColor(Colors::Panel::PortraitText);

		sf::FloatRect textBounds = placeholder.getLocalBounds();
		placeholder.setPosition({ 
			Bounds.position.x + (Bounds.size.x - textBounds.size.x) / 2.f, 
			Bounds.position.y + (Bounds.size.y - textBounds.size.y) / 2.f 
			});
		window.draw(placeholder);
	}
}

bool PortraitPanel::LoadPortrait(const std::string& path)
{
	PortraitTexture.emplace();
	if (!PortraitTexture->loadFromFile(path))
	{
		PortraitSprite.reset();
		PortraitTexture.reset();
		return false;
	}

	// Texture 로드 성공 시 Sprite 생성
	PortraitSprite.emplace(*PortraitTexture);
	return true;
}
