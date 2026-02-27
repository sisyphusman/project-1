#pragma once

#include "Engine/Entities/Entity.h"
#include "Engine/Items/InventoryComponent.h"

class Player : public Entity
{
public:
	Player(int x, int y);

	InventoryComponent&		  GetInventory() { return Inventory; }
	const InventoryComponent& GetInventory() const { return Inventory; }

private:
	InventoryComponent Inventory;
};