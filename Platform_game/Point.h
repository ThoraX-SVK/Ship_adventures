#ifndef POINT_H_DEFINED
#define POINT_H_DEFINED

#include <SFML\Graphics.hpp>

class Point
{
private:
	sf::Sprite sprite;

public:
	Point(sf::Vector2f pos, sf::Texture& texture);
	sf::Sprite get_sprite() { return sprite; }
	bool overlaps_sprite(sf::Sprite& sprite);

};




#endif // !POINT_H_DEFINED

