#pragma once
#include "SFML/Graphics.hpp"
using sf::Window;
using sf::Texture;
using sf::Sprite;
using sf::View;
using std::unique_ptr;

static const int SIZE = 1024;

class Chunk {
public:
	Texture* chunk_texture = nullptr;
	Sprite chunk_sprite;
	sf::Image chunk_mask;
	int X = 0, Y = 0;
	bool loaded = false;
	std::vector<sf::RectangleShape> collisions;
	std::vector<Sprite> sprites_to_draw;

	void setTexture(sf::Texture* tex) {
		chunk_texture = tex;
		chunk_sprite.setTexture(*chunk_texture);
	}
};