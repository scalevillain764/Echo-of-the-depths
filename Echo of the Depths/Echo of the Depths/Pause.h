#pragma once
#include "SFML/Graphics.hpp"
#include "buttons.h"

using namespace sf;

class Pause {
public :
	// bools
	bool isPaused = false;
	// bools
	Clock button_clock;
	Texture pause_screen_texture, pause_button_texture;
	Sprite pause_screen_sprite, pause_button_sprite;

	Button resume;
	Button settings;
	Button exit;

	bool load() {
		if (!pause_button_texture.loadFromFile("textures/pause/Pause.png") || !pause_screen_texture.loadFromFile("textures/pause/pause_screen.png") || !resume.load_texture("textures/pause/button_resume.png", "textures/pause/button_resume_clicked.png") ||
			!settings.load_texture("textures/pause/button_settings.png", "textures/pause/button_settings_clicked.png") || !exit.load_texture("textures/pause/button_exit.png", "textures/pause/button_exit_clicked.png")) {
			return false;
		}
		resume.setTexture();
		settings.setTexture();
		exit.setTexture();
		pause_button_sprite.setTexture(pause_button_texture);
		pause_screen_sprite.setTexture(pause_screen_texture);
		pause_screen_sprite.setPosition(0.f, 0.f);

		resume.button_sprite.setPosition(693.f, 403.f);
		settings.button_sprite.setPosition(693.f, 585.f);
		exit.button_sprite.setPosition(693.f, 768.f);
		pause_button_sprite.setPosition(7.f, 7.f);
		return true;
	}
};