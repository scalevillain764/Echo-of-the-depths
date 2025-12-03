#pragma once
#include "SFML/Graphics.hpp"

using namespace sf;

class Button {
private:
	Texture button_texture;
	Texture button_clicked_texture;
	bool button_clicked;
public:
	sf::Sprite button_sprite;

	Button() {
		button_clicked = false;
	}

	void setTexture() {
		button_sprite.setTexture(button_texture);
	}

	void setClickedTexture() {
		button_sprite.setTexture(button_clicked_texture);
	}

	void to_true() {
		button_clicked = true;
	}

	void to_false() {
		button_clicked = false;
	}

	bool get_clicked() {
		return button_clicked;
	}

	bool load_texture(const std::string& button_name, const std::string& button_clicked_name) {
		if (!button_texture.loadFromFile(button_name) || !button_clicked_texture.loadFromFile(button_clicked_name)) {
			return false;
		}
		return true;
	}
};