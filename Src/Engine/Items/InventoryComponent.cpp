#include "Engine/Items/InventoryComponent.h"

#include <sstream>

InventoryComponent::InventoryComponent(int maxSlotCount)
	: MaxSlotCount(maxSlotCount)
{
	if (maxSlotCount < 1)
	{
		maxSlotCount = 1;
	}
}

bool InventoryComponent::TryAddItem(const ItemArchetype& item)
{
	// 스택이 가능한 아이템이 있을 경우
	if (item.Stackable)
	{
		auto sameStack = FindStackById(item.Id);
		if (sameStack != Slots.end())
		{
			sameStack->Quantity += 1;
			return true;
		}
	}

	// 아이템이 꽉 찬 경우
	if (IsFull())
	{
		return false;
	}

	// 첫 아이템 추가 인 경우
	ItemStack newStack;
	newStack.Archetype = item;
	newStack.Quantity = 1;
	Slots.push_back(newStack);
	return false;
}

std::optional<ItemArchetype> InventoryComponent::TryRemoveOneAt(int index)
{
	// 인덱스 검사
	if (index < 0 || index >= static_cast<int>(Slots.size()))
	{
		return std::nullopt;
	}

	// 해당 슬롯 개수 검사
	ItemStack& target = Slots[static_cast<size_t>(index)];
	if (target.Quantity <= 0)
	{
		return std::nullopt;
	}

	ItemArchetype removedItem = target.Archetype;
	target.Quantity -= 1;

	// 감소한 슬롯 아이템 개수가 0개일때 제거
	if (target.Quantity <= 0)
	{
		Slots.erase(Slots.begin() + index);
	}

	return removedItem;
}

bool InventoryComponent::IsFull() const
{
	return static_cast<int>(Slots.size()) >= MaxSlotCount;
}

std::string InventoryComponent::BuildSummaryText() const
{
	std::ostringstream oss;
	oss << "[Inv " << GetUsedSlotCount() << "/" << MaxSlotCount << "]";
	if (!Slots.empty())
	{
		oss << " ";
		for (size_t i = 0; i < Slots.size(); ++i)
		{
			const ItemStack& slot = Slots[i];
			oss << slot.Archetype.Name << "x" << slot.Quantity;
			if (i + 1 < Slots.size())
			{
				oss << ", ";
			}
		}
	}
	return oss.str();
}

std::vector<ItemStack>::iterator InventoryComponent::FindStackById(const std::string& id)
{
	for (auto it = Slots.begin(); it != Slots.end(); ++it)
	{
		if (it->Archetype.Id == id)
		{
			return it;
		}
	}

	return Slots.end();
}

std::vector<ItemStack>::const_iterator InventoryComponent::FindStackById(const std::string& id) const
{
	for (auto it = Slots.begin(); it != Slots.end(); ++it)
	{
		if (it->Archetype.Id == id)
		{
			return it;
		}
	}

	return Slots.end();
}
