#include "Engine/UI/Panel/InventoryOverlayPanel.h"

#include "Engine/Core/Style.h"

InventoryOverlayPanel::InventoryOverlayPanel()
	: UIPanel(0.f, 0.f, 0.f, 0.f)
{
}

void InventoryOverlayPanel::UpdateBounds(const sf::View& defaultView)
{
	const sf::Vector2f viewSize = defaultView.getSize();
	const sf::Vector2f viewCenter = defaultView.getSize();

	const float overlayWidth = viewSize.x * 0.45f;
	const float overlayHeight = viewSize.y * 0.45f;

	Bounds = sf::FloatRect({ viewCenter.x - (overlayWidth * 0.5f), viewCenter.y - (overlayHeight * 0.5f) }, { overlayWidth, overlayHeight });
}

void InventoryOverlayPanel::Render(sf::RenderWindow& window, const sf::Font& font)
{
	if (Inventory == nullptr)
	{
		return;
	}

	const sf::View defaultView = window.getDefaultView();
	UpdateBounds(defaultView);

	window.setView(defaultView);

	DrawBackground(window, sf::Color(12, 12, 18, 232), Colors::Panel::LogBorder);

	const unsigned int titleFontSize = 34;
	const unsigned int bodyFontSize = 24;

	const std::string inventoryTitle = "가방";
	sf::Text		  titleText(font, sf::String::fromUtf8(inventoryTitle.begin(), inventoryTitle.end()), titleFontSize);
	titleText.setFillColor(Colors::White);
	titleText.setStyle(sf::Text::Bold);
	titleText.setPosition({ Bounds.position.x + 24.f, Bounds.position.y + 20.f });
	window.draw(titleText);

	const std::vector<ItemStack>& slots = Inventory->GetSlots();
	float						  y = Bounds.position.y + 78.f;

	if (slots.empty())
	{
		const std::string emptyInventoryText = "(비어 있음)";
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

	const std::string footer = "I: 닫기";
	sf::Text		  footerText(font, sf::String::fromUtf8(footer.begin(), footer.end()), 20);
	footerText.setFillColor(Colors::LightGrey);
	footerText.setPosition({ Bounds.position.x + 24.f, Bounds.position.y + Bounds.size.y - 38.f });
	window.draw(footerText);
}
