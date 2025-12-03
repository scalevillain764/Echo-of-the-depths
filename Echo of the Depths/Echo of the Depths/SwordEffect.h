#pragma once
#include "SFML/Graphics.hpp"
#include "MainPlayer.h"

using namespace sf;

class SwordSlashEffect;

class Crit {
public:
	Sprite crit_sprite;
	int crit_alpha = 255;
	Clock crit_timer;
	bool should_remove = false;

	std::vector<Vector2f> b_pos{{-3.f, +2.f}, { +3.f, +36.f }, { +72.f,+14.f }, { +69.f, +48.f }};
	std::vector<Vector2f> f_pos{{-5.f, +2.f}, { +1.f, +36.f }, { +44.f, +12.f }, { +75.f, +33.f }};
	std::vector<Vector2f> l_pos{{-8.f, +18.f}, { -16.f, +73.f }, { +23.f, +57.f }, { +43.f, +83.f }};
	std::vector<Vector2f> r_pos{{+23.f, +5.f}, { +46.f, +38.f }, { +3.f, +62.f }, { +50.f, +71.f }};
	Vector2f cur_pos;

	Crit(const sf::Texture& texture, MainPlayer& player, SwordSlashEffect& swordEffect) {
		crit_sprite.setTexture(texture);
		crit_sprite.setColor(sf::Color(255, 255, 255, 255));
		set_position(player, swordEffect);
		crit_timer.restart();
	}

	void update() {
		if (crit_timer.getElapsedTime().asSeconds() > 0.01) {
			if (crit_alpha >= 0) {
				crit_alpha -= 12;
				crit_sprite.setColor(Color(255, 255, 255, crit_alpha));
				crit_timer.restart();
			}
			else {
				should_remove = true;
				crit_alpha = 0;
			}
		}
	}
	void set_position(MainPlayer& player, const SwordSlashEffect& swordEffect);
};

class Slowing_trail {
public :

	bool isActive = false;

	Texture slowing_trail_left, slowing_trail_right;
	Sprite sprite;

	int current_slowing_frame;
	int max_slowing_frames = 23;
	int slowing_frameHeight = 100, slowing_frameWidth = 120;

	Clock slowing_clock;
	Clock animation_clock;

	bool load() {
		if (!slowing_trail_left.loadFromFile("textures/trail_left.png") || !slowing_trail_right.loadFromFile("textures/trail_right.png")) {
			return false;
		}
		return true;
	}

};

class SwordSlashEffect {
private:

	// +dmg icon
	Sprite damage_icon_sprite;
	Texture damage_icon_texture;
	// +dmc icon

	sf::Texture texture_right;
	sf::Texture texture_left;
	sf::Texture texture_back;
	sf::Texture texture_front;
	sf::Clock lifetimeClock;// интервал атак
	float duration = 0.11f; // эффект длится 0.11 секунды
	bool active = false;
public:
	// crit
	sf::Texture crit_texture;
	std::vector<Crit> active_crits;
	// crit
	sf::Sprite sprite;
	sf::Clock attack_delay;


	void activate(MainPlayer& player) {
		if (player.is_watching_left) {
			sprite.setTexture(texture_left);
		}
		if (player.is_watching_right) {
			sprite.setTexture(texture_right);
		}
		if (player.is_watching_back) {
			sprite.setTexture(texture_front);
		}
		if (player.is_watching_front) {
			sprite.setTexture(texture_back);
		}
		active = true;
		lifetimeClock.restart();
	}

	bool loadFromItem(Sword* item) {
		if (!item) return false;

		return texture_right.loadFromFile(item->slash_texture_right_path) &&
			texture_left.loadFromFile(item->slash_texture_left_path) &&
			texture_back.loadFromFile(item->slash_texture_back_path) &&
			texture_front.loadFromFile(item->slash_texture_front_path) &&
			crit_texture.loadFromFile("textures/crit.png") && damage_icon_texture.loadFromFile("textures/buffs/damage_buff.png");
	}

	void update(MainPlayer &player) {
		if (player.is_watching_left) {
			sprite.setPosition(player.sprite.getPosition().x - 70.f, player.sprite.getPosition().y - 11.f);
		}
		else if (player.is_watching_right) {
			sprite.setPosition(player.sprite.getPosition().x + 9.f, player.sprite.getPosition().y - 4.f);
		}
		else if (player.is_watching_back) {
			sprite.setPosition(player.sprite.getPosition().x - 30.f, player.sprite.getPosition().y + 27.f);
		}
		else if (player.is_watching_front) {
			sprite.setPosition(player.sprite.getPosition().x - 20.f, player.sprite.getPosition().y - 49.f);
		}
		if (active && lifetimeClock.getElapsedTime().asSeconds() > duration)
			active = false;
	}

	void draw_crits(RenderWindow &window) {
		for (auto& crit : active_crits) {
			crit.update();
			window.draw(crit.crit_sprite);
		}
		active_crits.erase(std::remove_if(active_crits.begin(), active_crits.end(), [](const Crit &crit) { return crit.should_remove; }), active_crits.end());
	}

	/*void update_dmg_icon(RenderWindow& window, Clock dmg_clock) {

	}*/

	bool isActive() const { return active; }
};

inline void Crit::set_position(MainPlayer& player, const SwordSlashEffect& swordEffect) {
	int rand_num = rand() % 4;
	if (player.is_watching_left) {
		cur_pos = swordEffect.sprite.getPosition() + l_pos[rand_num];
	}
	if (player.is_watching_front) {
		cur_pos = swordEffect.sprite.getPosition() + f_pos[rand_num];
	}
	if (player.is_watching_back) {
		cur_pos = swordEffect.sprite.getPosition() + b_pos[rand_num];
	}
	if (player.is_watching_right) {
		cur_pos = swordEffect.sprite.getPosition() + r_pos[rand_num];
	}
	Crit::crit_sprite.setPosition(cur_pos);
}