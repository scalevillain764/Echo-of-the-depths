#pragma once
#include "SFML/Graphics.hpp"
#include "buttons.h"

using sf::Sprite;
using sf::RectangleShape;
using sf::Color;
using sf::Texture;

const Color color(0, 0, 8);

class TryAgainMenu {
public:
	RectangleShape rectangle;
	Texture menu_screen_texture;
	Sprite menu_screen_sprite;

	bool isActive;

	Button retry;
	Button menu;
	sf::Clock button_clock;

	bool load() {
		if (!menu_screen_texture.loadFromFile("textures/Youdied/youdied.png") || !retry.load_texture("textures/Youdied/retry.png", "textures/Youdied/retry_clicked.png") || 
			!menu.load_texture("textures/Youdied/menu.png", "textures/Youdied/menu_clicked.png")) {
			return false;
		}
		rectangle.setSize(sf::Vector2f(1920.f, 1080.f));
		rectangle.setPosition(0.f, 0.f);
		rectangle.setFillColor(color);

		menu_screen_sprite.setTexture(menu_screen_texture);
		menu_screen_sprite.setPosition(450.f, 28.f);

		retry.setTexture();
		retry.button_sprite.setPosition(696.f, 600.f);

		menu.setTexture();
		menu.button_sprite.setPosition(696.f, 776.f);
		return true;
	}

};