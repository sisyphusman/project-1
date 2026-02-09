#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

class DamagePopupSystem
{
public:
	void AddAtTile(int tileX, int tileY, int damage, const sf::Color& color); // 월드 타일 좌표 -> 화면 좌표 변환 후 팝업 시작점 생성
	void Update(float deltaTime);											  // 남은 시간 감소
	void Render(sf::RenderWindow& window, const sf::Font& font) const;		  // 숫자의 투명도를 낮춤

private:
	struct DamagePopup
	{
		sf::Vector2f Position;
		int			 Damage = 0;
		float		 RemainingTime = 0.f;
		sf::Color	 Color = sf::Color::White;
	};

	static constexpr float	 LifeTime = 0.6f;
	std::vector<DamagePopup> Popups;
};