#include "Engine/UI/Panel/InventoryOverlayPanel.h"

#include "Engine/Core/Style.h"
#include "Engine/Data/TextCatalog.h"
#include "Engine/Core/Macros.h"

InventoryOverlayPanel::InventoryOverlayPanel()
	: UIPanel(0.f, 0.f, 0.f, 0.f)
{
}

void InventoryOverlayPanel::UpdateBounds(const sf::View& defaultView, const sf::FloatRect& viewport)
{
	const sf::Vector2f viewSize = defaultView.getSize();
	const sf::Vector2f viewCenter = defaultView.getCenter();

	const float viewportCenterX = viewCenter.x + ((viewport.position.x + (viewport.size.x * 0.5f)) - 0.5f) * viewSize.x;
	const float viewportCenterY = viewCenter.y + ((viewport.position.y + (viewport.size.y * 0.5f)) - 0.5f) * viewSize.y;

	const float overlayWidth = viewSize.x * 0.45f;
	const float overlayHeight = viewSize.y * 0.45f;

	Bounds = sf::FloatRect({ viewportCenterX - (overlayWidth * 0.5f), viewportCenterY - (overlayHeight * 0.5f) }, { overlayWidth, overlayHeight });
}

void InventoryOverlayPanel::Render(sf::RenderWindow& window, const sf::Font& font)
{
	RenderOnViewport(window, font, sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
}

void InventoryOverlayPanel::RenderOnViewport(sf::RenderWindow& window, const sf::Font& font, const sf::FloatRect& viewport)
{
	if (Inventory == nullptr)
	{
		return;
	}

	GAME_CHECK(OutTextCatalog != nullptr);

	const sf::View defaultView = window.getDefaultView();
	UpdateBounds(defaultView, viewport);

	window.setView(defaultView);

	DrawBackground(window, sf::Color(12, 12, 18, 232), Colors::Panel::LogBorder);

	const unsigned int titleFontSize = 34;
	const unsigned int bodyFontSize = 24;

	const std::string inventoryTitle = OutTextCatalog->Get("ui_inventory_title");
	sf::Text		  titleText(font, sf::String::fromUtf8(inventoryTitle.begin(), inventoryTitle.end()), titleFontSize);
	titleText.setFillColor(Colors::White);
	titleText.setStyle(sf::Text::Bold);
	titleText.setPosition({ Bounds.position.x + 24.f, Bounds.position.y + 20.f });
	window.draw(titleText);

	const std::vector<ItemStack>& slots = Inventory->GetSlots();
	float						  y = Bounds.position.y + 78.f;

	if (slots.empty())
	{
		const std::string emptyInventoryText = OutTextCatalog->Get("ui_inventory_footer_empty");
		sf::Text		  emptyText(font, sf::String::fromUtf8(emptyInventoryText.begin(), emptyInventoryText.end()), bodyFontSize);
		emptyText.setFillColor(Colors::Grey);
		emptyText.setPosition({ Bounds.position.x + 24.f, y });
		window.draw(emptyText);
	}
	else
	{
		for (size_t index = 0; index < slots.size(); ++index)
		{
			const ItemStack&  slot = slots[index];
			const std::string line = std::to_string(index + 1) + ". " + slot.Archetype.Name + " x" + std::to_string(slot.Quantity);
			sf::Text		  itemText(font, sf::String::fromUtf8(line.begin(), line.end()), bodyFontSize);
			itemText.setFillColor(Colors::Text);
			itemText.setPosition({ Bounds.position.x + 24.f, y });
			window.draw(itemText);
			y += 34.f;
		}
	}

	const std::string footer = OutTextCatalog->Get("ui_inventory_close");
	sf::Text		  footerText(font, sf::String::fromUtf8(footer.begin(), footer.end()), 20);
	footerText.setFillColor(Colors::LightGrey);
	footerText.setPosition({ Bounds.position.x + 24.f, Bounds.position.y + Bounds.size.y - 38.f });
	window.draw(footerText);
}
