#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Engine/Items/ItemTypes.h"

class InventoryComponent
{
public:
	explicit InventoryComponent(int maxSlotCount = 20);

	bool TryAddItem(const ItemArchetype& item); // 바닥 아이템을 인벤토리에 추가, stackable이면 같은 Id 슬롯에 누적

	std::optional<ItemArchetype> TryRemoveOneAt(int index);

	bool IsFull() const;
	bool IsEmpty() const { return Slots.empty(); }

	int GetMaxSlotCount() const { return MaxSlotCount; }
	int GetUsedSlotCount() const { return static_cast<int>(Slots.size()); }

	const std::vector<ItemStack>& GetSlots() const { return Slots; }

	std::string BuildSummaryText() const;

private:
	std::vector<ItemStack>::iterator	   FindStackById(const std::string& id);
	std::vector<ItemStack>::const_iterator FindStackById(const std::string& id) const;

	int					   MaxSlotCount;
	std::vector<ItemStack> Slots;
};