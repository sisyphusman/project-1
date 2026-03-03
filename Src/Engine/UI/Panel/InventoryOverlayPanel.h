#pragma once

#include "Engine/UI/Panel/UIPanel.h"
#include "Engine/Items/InventoryComponent.h"

class InventoryOverlayPanel : public UIPanel
{
public:
	InventoryOverlayPanel();

	void Render(sf::RenderWindow& window, const sf::Font& font) override;
	void RenderOnViewport(sf::RenderWindow& window, const sf::Font& font, const sf::FloatRect& viewport);

	void SetSource(const InventoryComponent* inventory) { Inventory = inventory; }

private:
	void UpdateBounds(const sf::View& defaultView, const sf::FloatRect& viewport);

	const InventoryComponent* Inventory = nullptr;
};