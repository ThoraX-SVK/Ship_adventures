#include "Point.h"

Point::Point(sf::Vector2f pos, sf::Texture& texture)
{
	this->sprite.setTexture(texture);
	this->sprite.setPosition(pos);
}

bool Point::overlaps_sprite(sf::Sprite& sprite)
{
	if (this->sprite.getGlobalBounds().contains(sprite.getPosition()))
		return true;
	else
		return false;
}