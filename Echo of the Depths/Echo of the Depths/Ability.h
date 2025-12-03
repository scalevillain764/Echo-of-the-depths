#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class MainPlayer;  // forward declaration

class Ability {
public:
	Ability() = default;

	string name;

    virtual void use(MainPlayer& player, bool &all_is_locked) = 0;
	virtual void draw(RenderWindow &window, MainPlayer& player) = 0;
	virtual std::unique_ptr<Ability> clone() = 0; // клон для эффектов по типу тп
	bool shouldRemove = false;
	bool isActive = false;

	string getName() const { return name; }

	Texture status_texture;
	Sprite status_sprite;

	void update_status_description(Vector2i &mouse_pos, RenderWindow &window) {
		if (status_sprite.getGlobalBounds().contains(static_cast<Vector2f>(mouse_pos))) {
			status_description_sprite.setPosition(status_sprite.getPosition() + Vector2f(-28.f, -45.f));
			window.draw(status_description_sprite);
		}
	}

	Texture status_description_texture;
	Sprite status_description_sprite;

    virtual ~Ability() {}
};

class Reflect_damage : public Ability {
public:
	Sprite sprite;
	Texture texture_list;

	bool forward = true;
	int current_frame = 0;
	const int FRAME_WIDTH = 141, FRAME_HEIGHT = 169;
	Clock life_time_clock;
	Clock animation_clock;

	float reflection_percentage;
	int duration;

	Reflect_damage(float reflection_percentage, int duration, const string& filename, const string& status_filename, const string& description);
	void use(MainPlayer& player, bool& all_is_locked) override;
	void draw(RenderWindow& window, MainPlayer& player) override;
	void update(MainPlayer& player);
	unique_ptr<Ability> clone() override;
};

class ReadBook: public Ability {
public:
	vector<Texture> pages;
	Sprite sprite;
	RectangleShape rect;
	int current_page = 0;
	Text keys;

	// methods
	ReadBook(int amount_of_pages, const string& folder, Font &font);
	void use(MainPlayer& player, bool &all_is_locked) override;
	void draw(RenderWindow& window, MainPlayer& player) override;
	void update_event(/*Event *event,*/ const Event& event, bool &all_is_locked);
	void add_page(const string& page_name);
	unique_ptr<Ability> clone() override;
};

class Teleport : public Ability {
    float distance;
public:
	Sprite sprite;
	Texture textures[9];
	Clock clock;
	bool draw_effect = false;
	int current_frame = 0;
	Vector2f result_distance;

	// methods
	Teleport(float dist);
	void use(MainPlayer& player, bool &all_is_locked) override;
	Vector2f get_result_distance();
	void draw(RenderWindow& window, MainPlayer& player) override;
	unique_ptr<Ability> clone() override;
};

//class Heal : public Ability {
//public :
//	Texture heal_effect_texture;
//	Sprite heal_effect_sprite;
//
//	int current_heal_frame = 0;
//	int frameWidth = 500, frameHeight = 200;
//
//	std::string filename;
//
//	Clock heal_effect_clock;
//	Clock heal_timer;
//	int duration = 0;
//	int heal_amount_second = 0;
//
//	// methods
//	Heal(int dur, int heal_amount_second, const string &filename);
//	void use(MainPlayer& player, bool &all_is_locked) override;
//	void update(MainPlayer& player);
//	void draw(RenderWindow& window, MainPlayer& player) override;
//	unique_ptr<Ability> clone() override;
//};
class Heal : public Ability {
public:
	Texture heal_effect[8];
	Sprite heal_effect_sprite;

	int current_heal_frame;

	Clock heal_effect_clock;
	Clock heal_timer;
	int duration;
	int heal_amount_second;

	// methods
	Heal(int dur, int heal_amount_second, const string& filename, const string& description);
	void use(MainPlayer& player, bool& all_is_locked) override;
	void update(MainPlayer& player);
	void draw(RenderWindow& window, MainPlayer& player) override;
	unique_ptr<Ability> clone() override;
};

class Open_Scroll : public Ability {
public:
    Sprite sprite;
	Sprite up_text;
	RectangleShape background;
    Text esc;

    Open_Scroll(Texture& texture1, Texture& texture2, Font& font);

	// methods
    void use(MainPlayer& player, bool &all_is_locked) override;
	void update(const Event& event, bool& isSeen, bool &all_is_locked);
	void draw(RenderWindow& window, MainPlayer& player) override;
	unique_ptr<Ability> clone() override;
};

class Shield : public Ability {
public :
	Sprite sprite;
	Texture texture;
	int dmg_reduction;
	int duration;

	Clock get_damage_clock;
	Clock LifeTimeClock;
	
	// methods
	Shield(int dmg_reduction, int duration, const string& filename);
	virtual void use(MainPlayer& player, bool &all_is_locked) = 0;
	virtual void draw(RenderWindow& window, MainPlayer& player) = 0;
	virtual unique_ptr<Ability> clone() = 0;
};

class Ice_Shield : public Shield {
public :
	Ice_Shield(int hp, int dur, const string& filename, const string& description);
	void update(MainPlayer& player);
	void use(MainPlayer& player, bool &all_is_locked) override;
	void draw(RenderWindow& window, MainPlayer& player) override;
	unique_ptr<Ability> clone() override;
};

