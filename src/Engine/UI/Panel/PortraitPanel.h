#pragma once

#include "Engine/UI/Panel/UIPanel.h"

#include <optional>

// 캐릭터 초상화 패널
class PortraitPanel : public UIPanel
{
public:
	PortraitPanel(float x, float y, float width, float height);

	void Render(sf::RenderWindow& window, const sf::Font& font) override;

	// 초상화 이미지 설정
	bool LoadPortrait(const std::string& path);

private:
	std::optional<sf::Texture> PortraitTexture;
	std::optional<sf::Sprite>  PortraitSprite;
};