#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

class DamagePopupSystem
{
public:
	void AddAtTile(int tileX, int tileY, int damage, const sf::Color& color);
	void Update(float deltaTime);
	void Render(sf::RenderWindow& window, const sf::Font& font) const;

	private:
		struct DamagePopup
		{
			sf::Vector2f Position;
			int			 Damage = 0;
			float		 RemainingTime = 0.f;
			sf::Color	 Color = sf::Color::White;
		};
		
		static constexpr float LifeTime = 0.6f;
		std::vector<DamagePopup> Popups;
};