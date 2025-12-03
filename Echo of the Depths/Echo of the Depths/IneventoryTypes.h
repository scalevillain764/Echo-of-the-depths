#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "Ability.h"

using namespace sf;
// цвет квадрата перезарядки (полупрозрачный)

class MainPlayer;

class Item {
public:
	Item(const Item&) = delete; // копирование запрещено
	Item& operator=(const Item&) = delete;

	Item(Item&&) noexcept = default; // разрешаем перемещение
	Item& operator=(Item&&) noexcept = default;

	Item() = default;

	std::string name;
	Sprite sprite;

	// cooldown
	Color CD_RECT_COLOR = Color(0, 0, 0, 147);
	Clock cooldown_clock;
	int cooldown = 0;
	RectangleShape cd_rect;
	Text cd_text;
	bool cooldown_ready = true;
	// cooldown

	// hover
	int hover_pos_y = 0;
	float amplitude = 5.f;   
	float speed = 2.f;
	Clock hover_clock;
	// hover

	bool toDelete = false; // delete after using
	bool should_save = false;
	bool isDraggin = false;
	Vector2f offset;
	Vector2f original_pos;

	Vector2f left_offset, right_offset, front_offset;
	Vector2f right_scale, left_front_scale;

	void hover(Vector2f base_pos) {
		float time = hover_clock.getElapsedTime().asSeconds();
		float offsetY = sin(time * speed) * amplitude;
		sprite.setPosition(base_pos.x, base_pos.y + offsetY);
	}

	void toDeleteEnable() {
		if(!toDelete)
		toDelete = true;
	}

	std::unique_ptr<Ability> ability = nullptr;

	void init_current_cd(Font &font) {
		cd_rect.setFillColor(CD_RECT_COLOR);
		cd_text.setFillColor(Color::Red);
		cd_text.setFont(font);
		cd_text.setCharacterSize(40);
	}

	virtual std::unique_ptr<Item> clone() const = 0;
	virtual ~Item() = default;
};

class Sword : public Item {
public:
	float attack_cooldown = 0.f;
	int damage = 0;
	int base_damage = 0;
	float crit_multiplier = 0; // multiplier
	int crit_chance = 0;  // %

	// slowing
	int slowing_percent = 0; // %
	int slowing_duration = 0; // s
	// slowing

	// poisoning
	int poison_damage = 0;
	int poison_duration = 0;
	Clock poison_timer;
	Clock poison_tick_clock; // one tick
	// poisoning

	std::string slash_texture_right_path;
	std::string slash_texture_left_path;
	std::string slash_texture_front_path;
	std::string slash_texture_back_path;

	std::unique_ptr<Item> clone() const override {
	 	auto newSword = std::make_unique<Sword>();
		newSword->should_save = this->should_save;
		newSword->name = this->name; 
		newSword->sprite = this->sprite;
		newSword->CD_RECT_COLOR = this->CD_RECT_COLOR;
		newSword->cooldown_clock = this->cooldown_clock;
		newSword->cooldown = this->cooldown;
		newSword->cd_rect = this->cd_rect;
		newSword->cd_text = this->cd_text;
		newSword->isDraggin = this->isDraggin;
		newSword->offset = this->offset;
		newSword->original_pos = this->original_pos;
		newSword->left_offset = this->left_offset;
		newSword->right_offset = this->right_offset;
		newSword->front_offset = this->front_offset;
		newSword->right_scale = this->right_scale;
		newSword->left_front_scale = this->left_front_scale;
		if (this->ability)
			newSword->ability = this->ability->clone();
		newSword->attack_cooldown = this->attack_cooldown;
		newSword->damage = this->damage;
		newSword->base_damage = this->base_damage;
		newSword->crit_multiplier = this->crit_multiplier;
		newSword->crit_chance = this->crit_chance;
		newSword->slash_texture_right_path = this->slash_texture_right_path;
		newSword->slash_texture_left_path = this->slash_texture_left_path;
		newSword->slash_texture_front_path = this->slash_texture_front_path;
		newSword->slash_texture_back_path = this->slash_texture_back_path;
		newSword->slowing_percent = this->slowing_percent;
		newSword->slowing_duration = this->slowing_duration;
		newSword->poison_damage = this->poison_damage;
		newSword->poison_tick_clock = this->poison_tick_clock;
		newSword->poison_duration = this->poison_duration;
		newSword->poison_timer = this->poison_timer;
		newSword->amplitude = this->amplitude;
		newSword->hover_pos_y = this->hover_pos_y;
		newSword->speed = this->speed;
		newSword->hover_clock = this->hover_clock;
		newSword->cooldown_ready = this->cooldown_ready;
		newSword->toDelete = this->toDelete;
		return newSword;
	}
};

class Book : public Item {
	std::unique_ptr<Item> clone() const override {
		auto newBook = std::make_unique<Book>();
		newBook->should_save = this->should_save;
		newBook->name = this->name;
		newBook->sprite = this->sprite;
		newBook->CD_RECT_COLOR = this->CD_RECT_COLOR;
		newBook->cooldown_clock = this->cooldown_clock;
		newBook->cooldown = this->cooldown;
		newBook->cd_rect = this->cd_rect;
		newBook->cd_text = this->cd_text;
		newBook->isDraggin = this->isDraggin;
		newBook->offset = this->offset;
		newBook->original_pos = this->original_pos;
		newBook->left_offset = this->left_offset;
		newBook->right_offset = this->right_offset;
		newBook->front_offset = this->front_offset;
		newBook->right_scale = this->right_scale;
		newBook->left_front_scale = this->left_front_scale;
		if (this->ability)
			newBook->ability = this->ability->clone();
		newBook->cooldown_ready = this->cooldown_ready;
		newBook->toDelete = this->toDelete;
		return newBook;
	}
};

class Key : public Item {
	std::unique_ptr<Item> clone() const override {
		auto newKey = std::make_unique<Key>();
		newKey->should_save = this->should_save;
		newKey->name = this->name;
		newKey->sprite = this->sprite;
		newKey->CD_RECT_COLOR = this->CD_RECT_COLOR;
		newKey->cooldown_clock = this->cooldown_clock;
		newKey->cooldown = this->cooldown;
		newKey->cd_rect = this->cd_rect;
		newKey->cd_text = this->cd_text;
		newKey->isDraggin = this->isDraggin;
		newKey->offset = this->offset;
		newKey->original_pos = this->original_pos;
		newKey->left_offset = this->left_offset;
		newKey->right_offset = this->right_offset;
		newKey->front_offset = this->front_offset;
		newKey->right_scale = this->right_scale;
		newKey->left_front_scale = this->left_front_scale;
		if (this->ability)
			newKey->ability = this->ability->clone();
		newKey->cooldown_ready = this->cooldown_ready;
		newKey->toDelete = this->toDelete;
		return newKey;
	}
};

class Heal_Potion : public Item {
	std::unique_ptr<Item> clone() const override {
		auto newHeal = std::make_unique<Heal_Potion>();
		newHeal->should_save = this->should_save;
		newHeal->name = this->name;
		newHeal->sprite = this->sprite;
		newHeal->CD_RECT_COLOR = this->CD_RECT_COLOR;
		newHeal->cooldown_clock = this->cooldown_clock;
		newHeal->cooldown = this->cooldown;
		newHeal->cd_rect = this->cd_rect;
		newHeal->cd_text = this->cd_text;
		newHeal->isDraggin = this->isDraggin;
		newHeal->offset = this->offset;
		newHeal->original_pos = this->original_pos;
		newHeal->left_offset = this->left_offset;
		newHeal->right_offset = this->right_offset;
		newHeal->front_offset = this->front_offset;
		newHeal->right_scale = this->right_scale;
		newHeal->left_front_scale = this->left_front_scale;
		if (this->ability) {
			newHeal->ability = this->ability->clone();
		}
		newHeal->cooldown_ready = this->cooldown_ready;
		newHeal->toDelete = this->toDelete;
		return newHeal;
	}
};

class SimpleItem : public Item {
	std::unique_ptr<Item> clone() const override {
		auto newSimple = std::make_unique<SimpleItem>();
		newSimple->should_save = this->should_save;
		newSimple->name = this->name;
		newSimple->sprite = this->sprite;
		newSimple->CD_RECT_COLOR = this->CD_RECT_COLOR;
		newSimple->cooldown_clock = this->cooldown_clock;
		newSimple->cooldown = this->cooldown;
		newSimple->cd_rect = this->cd_rect;
		newSimple->cd_text = this->cd_text;
		newSimple->isDraggin = this->isDraggin;
		newSimple->offset = this->offset;
		newSimple->original_pos = this->original_pos;
		newSimple->left_offset = this->left_offset;
		newSimple->right_offset = this->right_offset;
		newSimple->front_offset = this->front_offset;
		newSimple->right_scale = this->right_scale;
		newSimple->left_front_scale = this->left_front_scale;
		if (this->ability)
			newSimple->ability = this->ability->clone();
		newSimple->cooldown_ready = this->cooldown_ready;
		newSimple->toDelete = this->toDelete;
		return newSimple;
	}
};

class Amulet : public Item {
	std::unique_ptr<Item> clone() const override {
		auto newAmulet = std::make_unique <Amulet> ();
		newAmulet->should_save = this->should_save;
		newAmulet->name = this->name;
		newAmulet->sprite = this->sprite;
		newAmulet->CD_RECT_COLOR = this->CD_RECT_COLOR;
		newAmulet->cooldown_clock = this->cooldown_clock;
		newAmulet->cooldown = this->cooldown;
		newAmulet->cd_rect = this->cd_rect;
		newAmulet->cd_text = this->cd_text;
		newAmulet->isDraggin = this->isDraggin;
		newAmulet->offset = this->offset;
		newAmulet->original_pos = this->original_pos;
		newAmulet->left_offset = this->left_offset;
		newAmulet->right_offset = this->right_offset;
		newAmulet->front_offset = this->front_offset;
		newAmulet->right_scale = this->right_scale;
		newAmulet->left_front_scale = this->left_front_scale;
		if (this->ability)
			newAmulet->ability = this->ability->clone();
		newAmulet->cooldown_ready = this->cooldown_ready;
		newAmulet->toDelete = this->toDelete;
		return newAmulet;
	}
};

class Scroll : public Item {
	std::unique_ptr<Item> clone() const override {
		auto newScroll = std::make_unique <Scroll>();
		newScroll->should_save = this->should_save;
		newScroll->name = this->name;
		newScroll->sprite = this->sprite;
		newScroll->CD_RECT_COLOR = this->CD_RECT_COLOR;
		newScroll->cooldown_clock = this->cooldown_clock;
		newScroll->cooldown = this->cooldown;
		newScroll->cd_rect = this->cd_rect;
		newScroll->cd_text = this->cd_text;
		newScroll->isDraggin = this->isDraggin;
		newScroll->offset = this->offset;
		newScroll->original_pos = this->original_pos;
		newScroll->left_offset = this->left_offset;
		newScroll->right_offset = this->right_offset;
		newScroll->front_offset = this->front_offset;
		newScroll->right_scale = this->right_scale;
		newScroll->left_front_scale = this->left_front_scale;
		if (this->ability)
			newScroll->ability = this->ability->clone();
		newScroll->cooldown_ready = this->cooldown_ready;
		newScroll->toDelete = this->toDelete;
		return newScroll;
	}
};

struct InventorySlot {
	Sprite hitbox;
	unique_ptr<Item> item = nullptr;
};

struct Armor {
	std::string name;
	Sprite sprite;
	int evasion; // (percent)
	int armor; // final_damage = base_damage * (50 / (50 + armor))
	int base_armor;
	bool isDraggin;
	Vector2f offset;
	Vector2f original_pos;

	std::unique_ptr<Armor> clone() const {
		auto newArmor = std::make_unique<Armor>();
		newArmor->name = this->name;
		newArmor->sprite = this->sprite;
		newArmor->evasion = this->evasion;
		newArmor->armor = this->armor;
		newArmor->base_armor = this->armor;
		newArmor->isDraggin = this->isDraggin;
		newArmor->offset = this->offset;
		newArmor->original_pos = this->original_pos;
		return newArmor;
	}
};

struct ArmorSlot {
	Sprite hitbox;
	unique_ptr<Armor> armor = nullptr;
};

