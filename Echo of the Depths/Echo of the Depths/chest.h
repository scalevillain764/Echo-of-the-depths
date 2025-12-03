#pragma once
#include "SFML/GRaphics.hpp"
#include "IneventoryTypes.h"

using namespace sf;
using std::string;

class Chest {
public:
	Sprite sprite, exit_button_sprite;
	Texture texture, unit_texture, exit_button_texture;

	// animation
	Clock animation_clock;
	int frame = 0;
	bool forward = true;
	// animation

	Text chest;
	RectangleShape rect;

	bool isOpened = false;

	std::vector<InventorySlot> items;
	std::vector<ArmorSlot> armor;

	bool items_initialized = false;
	bool armor_initialiezed = false;

	void initArmor() {
		if (armor_initialiezed) return;
		armor.resize(8); // создаём 8 слотов
		float x = 712.f;
		float y = 551.f;
		for (auto& slot : armor) {
			slot.hitbox.setTexture(unit_texture);
			slot.hitbox.setPosition(x, y);
			x += 68.f;
		}
		armor_initialiezed = true;		
	}

	void update_animation(int frameWidth, int frameHeight, int maxFrames) {
		if (animation_clock.getElapsedTime().asSeconds() > 0.24f) {
			if (forward) {
				frame++;
				if (frame >= maxFrames - 1) {
					frame = maxFrames - 1;
					forward = false;
				}
			}
			else {
				frame--;
				if (frame <= 0) {
					frame = 0;
					forward = true;
				}
			}
			sprite.setTextureRect(IntRect(frame * frameWidth, 0, frameWidth, frameHeight));
			animation_clock.restart();
		}
	}

	void init_items_if_needed() {
		if (items_initialized) return;
		items.resize(8); // создаём 8 слотов
		float x = 712.f;
		float y = 473.f;
		for (auto& slot : items) {
			slot.hitbox.setTexture(unit_texture);
			slot.hitbox.setPosition(x, y);
			x += 68.f;
		}
		items_initialized = true;
	}

	bool load(const string filename) {
		if (!texture.loadFromFile(filename) || !unit_texture.loadFromFile("textures/chapone/locationtwo/chests/chest_slot.png") || 
			!exit_button_texture.loadFromFile("textures/chapone/locationtwo/chests/exit.png")) {
			return false;
		}
		return true;
	}

	void update() {
		rect.setSize(sf::Vector2f(1920.f, 1080.f));
		rect.setFillColor(sf::Color(0, 0, 0, 170));
		rect.setPosition(0.f, 0.f);
		exit_button_sprite.setTexture(exit_button_texture);
		exit_button_sprite.setPosition(1256.f, 425.f);
	}

	void set_text_settings(Font& font) {
		chest.setFont(font);
		chest.setFillColor(sf::Color::White);
		chest.setCharacterSize(72);
		chest.setString("Chest");
		chest.setPosition(920.f, 60.f);
	}

	void draw_items(RenderWindow& window) {
		if (isOpened) {
			window.draw(rect);
			window.draw(chest);
			window.draw(exit_button_sprite);

			for (size_t i = 0; i < items.size(); ++i) {
				window.draw(items[i].hitbox);
				if (items[i].item != nullptr) {
					items[i].item->sprite.setPosition(items[i].hitbox.getPosition() + sf::Vector2f(15.f, 15.f));
					window.draw(items[i].item->sprite);
				}
			}

			// ARMOR
			for (size_t i = 0; i < armor.size(); ++i) {
				window.draw(armor[i].hitbox);
				if (armor[i].armor != nullptr) {
					armor[i].armor->sprite.setPosition(armor[i].hitbox.getPosition() + sf::Vector2f(10.f, 15.f));
					window.draw(armor[i].armor->sprite);
				}
			}
		}
	}

	void close_chest(bool& all_is_locked) {
		isOpened = false;
		all_is_locked = false;
	}

	void open_chest(bool& all_is_locked) {
		isOpened = true;
		all_is_locked = true;
	}

	void setTexture() {
		sprite.setTexture(texture);
	}

};