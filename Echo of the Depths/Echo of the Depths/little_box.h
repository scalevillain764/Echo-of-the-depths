#pragma once
#include "SFML/Graphics.hpp"

using sf::Sprite;
using sf::Clock;
using sf::Texture;

class LittleBox {
public :
	Sprite box_sprite;
	Sprite light_sprite;

	Texture box_texture;
	std::vector<Texture> light_textures;
	Clock light_clock;
	int currentFrame = 0;

	// for appearing
	std::vector<Texture> tornado_textures;
	Sprite tornado_sprite;
	int cur_tornado_frame = 0;
	bool tornado_active = false;
	Clock tornado_clock;
	bool tornado_was_made = false;
	bool stop_drawing_tornado = false;
	// for appearing

	bool load(const string&box, const string& name_one, const string& name_two, const string& name_three, const string& name_four) {
		light_textures.resize(4);
		if (!box_texture.loadFromFile(box) || !light_textures[0].loadFromFile(name_one) ||
			!light_textures[1].loadFromFile(name_two) || !light_textures[2].loadFromFile(name_three) || 
			!light_textures[3].loadFromFile(name_four)) {
			return false;
		}
		return true;
	}

	void make_box(sf::Vector2f chest_pos) {
		box_sprite.setTexture(box_texture);
		box_sprite.setPosition(chest_pos);
		light_sprite.setTexture(light_textures[0]);
		light_sprite.setPosition(box_sprite.getPosition().x - 43.f, box_sprite.getPosition().y - 43.f);
	}

	bool load_tornado(const string &one, const string& two, const string& three, const string& four, const string& five,
		const string& six, const string& seven, const string& eight, const string& nine, const string& ten,
		const string& eleven, const string& twelve, const string& thirteen, const string& fourteen, const string& fifteen,
		const string& sixteen, const string& seventeen, const string& eighteen) {
		tornado_textures.resize(18);
		if (!tornado_textures[0].loadFromFile(one), !tornado_textures[1].loadFromFile(two), !tornado_textures[2].loadFromFile(three),
			!tornado_textures[3].loadFromFile(four), !tornado_textures[4].loadFromFile(five), !tornado_textures[5].loadFromFile(six),
			!tornado_textures[6].loadFromFile(seven), !tornado_textures[7].loadFromFile(eight), !tornado_textures[8].loadFromFile(nine),
			!tornado_textures[9].loadFromFile(ten), !tornado_textures[10].loadFromFile(eleven), !tornado_textures[11].loadFromFile(twelve),
			!tornado_textures[12].loadFromFile(thirteen), !tornado_textures[13].loadFromFile(fourteen), !tornado_textures[14].loadFromFile(fifteen),
			!tornado_textures[15].loadFromFile(sixteen), !tornado_textures[16].loadFromFile(seventeen), !tornado_textures[17].loadFromFile(eighteen)) 
		{
			return false;
		}
		return true;
	}

	void make_tornado(Vector2f pos) {
		tornado_sprite.setPosition(pos);
	}

	void update_tornado() {
		if (tornado_clock.getElapsedTime().asSeconds() > 0.15 && tornado_active) {
			tornado_sprite.setTexture(tornado_textures[cur_tornado_frame]);
			if(cur_tornado_frame < tornado_textures.size() - 1)
			cur_tornado_frame++;
			else {
				stop_drawing_tornado = true;
				cur_tornado_frame = tornado_textures.size() - 1;
			}	
			tornado_clock.restart();
		}
	}

	void update() {
		if (light_clock.getElapsedTime().asSeconds() > 0.15f) {
			light_sprite.setTexture(light_textures[currentFrame]);
			currentFrame++;

			if (currentFrame >= light_textures.size()) {
				currentFrame = 0;
			}
			light_clock.restart(); 
		}
	}

	void draw_tornado(RenderWindow& window) {
		window.draw(tornado_sprite);
	}

	void draw(sf::RenderWindow &window) {
		window.draw(light_sprite);
		window.draw(box_sprite);
	}

	void remove() {
		box_sprite.setScale(0, 0);
		light_sprite.setScale(0, 0);
	}
};