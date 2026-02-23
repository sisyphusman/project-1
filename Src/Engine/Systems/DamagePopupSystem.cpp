#include "DamagePopupSystem.h"

#include <algorithm>
#include <cstdint>

#include "Engine/Core/Style.h"

void DamagePopupSystem::AddAtTile(int tileX, int tileY, int damage, const sf::Color& color)
{
	DamagePopup popup;
	popup.Position = {
		static_cast<float>(tileX * UILayout::Fixed::TileSize),
		static_cast<float>(tileY * UILayout::Fixed::TileSize - UILayout::Fixed::TileSize),
	};

	popup.Damage = damage;
	popup.RemainingTime = LifeTime;
	popup.Color = color;
	Popups.push_back(popup);
}

void DamagePopupSystem::Update(float deltaTime)
{
	for (DamagePopup& popup : Popups)
	{
		popup.RemainingTime -= deltaTime;
		popup.Position.y -= 32.f * deltaTime;
	}

	Popups.erase(std::remove_if(Popups.begin(), Popups.end(), [](const DamagePopup& popup) { return popup.RemainingTime <= 0.f; }), Popups.end());
}

void DamagePopupSystem::Render(sf::RenderWindow& window, const sf::Font& font) const
{
	for (const DamagePopup& popup : Popups)
	{
		sf::Text damageText(font, std::to_string(popup.Damage), UILayout::Fixed::TileSize - 4);
		damageText.setPosition(popup.Position);

		sf::Color popupColor = popup.Color;
		popupColor.a = static_cast<std::uint8_t>(std::clamp(popup.RemainingTime / LifeTime, 0.f, 1.f) * 255.f);
		damageText.setFillColor(popupColor);
		window.draw(damageText);
	}
}
