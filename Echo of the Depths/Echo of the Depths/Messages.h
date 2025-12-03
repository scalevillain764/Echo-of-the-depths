#pragma once
#include "SFML/Graphics.hpp"

using sf::Text;
using sf::Clock;
using sf::Color;
using sf::RenderWindow;
using sf::Vector2f;

class Error {
private:
	std::string error_name;
	std::string error_string;
	Color error_color;
	int error_alpha = 255;
	Clock error_timer;
	Vector2f position;
	int character_size = 24;
	bool isActive = false;
public:
	Text error_text;
	Error(const std::string& error_name, const std::string& error_string, const sf::Font& font, int character_size, const Color& error_color, const Vector2f& position, int error_alpha) :
		error_name(error_name), error_string(error_string), character_size(character_size), error_color(error_color), position(position), error_alpha(error_alpha) {
		error_text.setString(error_string);
		error_text.setFont(font);
		error_text.setPosition(position);
		error_text.setCharacterSize(character_size);
		error_text.setFillColor(error_color);
	}

	void emitError() {
		if (!isActive) {
			isActive = true;
			error_alpha = 255;
		}
	}

	void tickError() {
		if (!isActive) return;

		if (error_timer.getElapsedTime().asSeconds() > 0.015) {

			Color c = error_color;
			c.a = error_alpha;
			error_text.setFillColor(c);

			error_alpha -= 2;
			error_timer.restart();
		}
		if (error_alpha <= 0) {
			error_alpha = 255;
			isActive = false;
		}		
	}

	void disableError() {
		isActive = false;
	}

	bool isEnabled() {
		return isActive;
	}

	void resetAlpha() {
		error_alpha = 255;
	}

	void setErrorText(const std::string& error_string) {
		error_text.setString(error_string);
	}

	void renderError(RenderWindow& window) {
		if (!isActive) return;
		window.draw(error_text);
	}
};

class Tip {
private:
	std::string tip_name;
	Color tip_color;
	Clock tip_timer;
	std::string tip_string;
	Vector2f position;
	int character_size = 24;
	bool isActive = false;
public:
	Text tip_text;
	int tip_alpha = 255;
	Tip(const std::string& tip_name, const std::string& tip_string, const sf::Font& font, int character_size, const Color& tip_color, const Vector2f& position, int tip_alpha) :
		tip_name(tip_name), tip_string(tip_string), character_size(character_size), tip_color(tip_color), position(position), tip_alpha(tip_alpha) {
		tip_text.setString(tip_string);
		tip_text.setFont(font);
		tip_text.setPosition(position);
		tip_text.setCharacterSize(character_size);
		tip_text.setFillColor(tip_color);
	}

	void emitTip() {
		if(!isActive)
		isActive = true;
	}

	void setTipText(const std::string& tip_string) {
		tip_text.setString(tip_string);
	}

	void tickTip() {
		if (!isActive) return;

		if (tip_timer.getElapsedTime().asSeconds() > 0.015) {

			Color c = tip_color;
			c.a = tip_alpha;
			tip_text.setFillColor(c);

			tip_alpha -= 2;
			tip_timer.restart();
		}
		if (tip_alpha <= 0) {
			tip_alpha = 0;
			isActive = false;
		}
	}

	void disableTip() {
		isActive = false;
	}

	bool isEnabled() {
		return isActive;
	}

	void resetAlpha() {
		tip_alpha = 255;
	}

	void renderTip(RenderWindow& window) {
		if (!isActive) return;
		window.draw(tip_text);
	}
};