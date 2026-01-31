#include "Engine/Entities/Entity.h"

#include "Engine/World/Map.h"

Entity::Entity(int x, int y, char glyph)
	: Position(x, y)
	, Glyph(glyph)
{
}

bool Entity::TryMove(int dx, int dy, const Map& map)
{
	int newX = Position.x + dx;
	int newY = Position.y + dy;

	if (map.IsWalkable(newX, newY))
	{
		Position.x = newX;
		Position.y = newY;
		return true;
	}
	
	return false;
}

void Entity::SetPosition(int x, int y)
{
	Position.x = x;
	Position.y = y;
}
