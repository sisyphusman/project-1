#pragma once

#include "Engine/UI/Panel/UIPanel.h"
#include "Engine/Items/InventoryComponent.h"
#include "Engine/Data/TextCatalog.h"

class InventoryOverlayPanel : public UIPanel
{
public:
	InventoryOverlayPanel();

	void Render(sf::RenderWindow& window, const sf::Font& font) override;
	void RenderOnViewport(sf::RenderWindow& window, const sf::Font& font, const sf::FloatRect& viewport);

	void SetTextCatalog(const TextCatalog* catalog) { OutTextCatalog = catalog; }

	void SetSource(const InventoryComponent* inventory) { Inventory = inventory; }

private:
	void UpdateBounds(const sf::View& defaultView, const sf::FloatRect& viewport);

	const InventoryComponent* Inventory = nullptr;

	const TextCatalog* OutTextCatalog = nullptr;
};