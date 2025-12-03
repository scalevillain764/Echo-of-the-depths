#pragma once
#include "SFML/Graphics.hpp"
#include "IneventoryTypes.h"
#include "Item_buffs.h"
#include "chest.h"
#include <unordered_map>

using namespace sf;
const sf::Color health_color(218, 24, 24); // для полоски здоровья
const sf::Color back_color(37, 40, 89); // для задней полоски здоровья

class Hub_assets;
class Chunk;

class MainPlayer {
protected :
	float startX = 1212.f; // правая граница
	float startY = 970.f;
	float offset = 30.f;   // расстояние между иконками

	int StartMenuX = 824;
	int StartMenuY = 275;

	// textures
	Texture front_right0, front_left0, back_right0, back_left0, left_left_leg0, left_right_leg0, right_left_leg0, right_right_leg0;
	Texture front_right1, front_left1, back_right1, back_left1, left_left_leg1, left_right_leg1, right_left_leg1, right_right_leg1;
	std::map<string, std::vector<Texture>> back;
	std::map<string, std::vector<Texture>> left;
	std::map<string, std::vector<Texture>> right;
	std::map<string, std::vector<Texture>> front;
	// textures
public:
	Sprite sprite;

	// inventory
	Item* activeItem = nullptr;
	Sprite activeSlot, invslots_nums_sprite;
	Texture unitTex, armor_slot_texture, invslots_nums_texture, activeSlotTex;	

	std::vector<std::unique_ptr<Item_Buff>> active_item_buffs; // активные бафы предметов
	std::vector<std::unique_ptr<Ability>> active_effects; // активные эффекты

	Text space_use_item; // text "[space] - use item"
	Text tab_inventory;  // text "[TAB] - inventory"

	bool inv_saved_for_new_level;
	bool inv_saved_for_this_level;
	ArmorSlot armor_slot;
	ArmorSlot armor_slot_copy;
	std::vector<InventorySlot> inventory;
	std::vector<InventorySlot> inventory_copy_for_this;

	int selectedSlot = 0;

		// menu
		bool menu_is_active = false;
		RectangleShape menu_black_rect;
		Sprite menu_sprite;
		Texture menu_texture;
		std::vector<std::vector<InventorySlot>> menu_inventory;
		std::vector<std::vector<InventorySlot>> menu_inventory_copy;

		// player icon in menu
		Sprite player_icon_sprite;
		Texture player_collvl0;
		Texture player_collvl1;
		std::vector<Texture> menu_inventory_player_texture;
		// player icon in menu

		Text damage_text; // menu_damage_text
		Text armor_text; // menu_armor_text
		Text name_text; // menu_name_text
		
		// menu
	// inventory


	// indicators 
		// health
		int maxHP = 0;
		bool hasMaxHP = false; 
		int current_health;
		int cur_health_copy;
		sf::RectangleShape health_bar;
		sf::RectangleShape health_back_bar;
		sf::Text health_text;
		bool isDead = false;
		// health
		
		// speed
		float speed = 200.f;
		// speed

		string name = "Eli"; // name

		// chains of light
		string chains_of_light_name = "Chains of light";
		int col_lvl = 0;
		float col_resistance_multiplier = 0.f;
		string col_description; /*= " - Resistnace to slowing "*/
		Text colTEXT;
		// chains of light

	// indicators

	// moving
	bool right_leg = true;
	bool is_watching_front = false; // стоит к нам спиной
	bool is_watching_back = true; // стоит к нам лицом
	bool is_watching_right = false;
	bool is_watching_left = true;
	bool moving = false;

	sf::Clock animClock; // switching legs timer
	float switchTime = 0.15f;
	// moving

	void upd_item_ptr();
	void fill_textures_map();
	bool loadTextures();
	void setUpUI();

	// saving
	void copy_items_ifdead();
	void copy_items_for_newlvl();
	void restore_items_from_copy();
	// saving
	
	// drawing inventory & cooldowns
	void draw_cooldowns(RenderWindow& window);
	void draw_inventory(RenderWindow& window);
	// drawing inventory & cooldowns

	// activate & disable menu
	void activateMenu(bool& all_is_locked);
	void disableMenu(bool& all_is_locked);
	// activate & disable menu

	// settings
	void setTextureFrontLeft();
	void setTextureFrontRight();
	void setTextureBackLeft();
	void setTextureBackRight();
	void setTextureLeftLeft();
	void setTextureLeftRight();
	void setTextureRightLeft();
	void setTextureRightRight();
	void setTextsettings(Font& font);
	void make_inventory();
	// settings

	// drawing other
	void draw_status_bar(Vector2i& mouse_pos, RenderWindow& window);
	void drawEffectsVisuals(RenderWindow& window, MainPlayer& player);
	void draw_active_item(RenderWindow& window);
	void draw_health(RenderWindow& window);
	// drawing other

	// update 
	void updateAllEffects(sf::Event* event = nullptr, bool* isSeen = nullptr, bool* all_is_locked = nullptr);
	void updateAllBuffs(std::vector<InventorySlot>& inventory);	
	void update_all_(Font& font);
	// update

	// items
	void removeDeletedItems();

	void useActiveItem(const sf::Event& event, bool& all_is_locked, sf::Font& font, std::vector<std::vector<Chunk>>& chunks, Hub_assets& hub);
	void useActiveItem(const sf::Event& event, bool& all_is_locked, Font& font, vector<RectangleShape>& walls);
	// items

	// moving
	void handleMovementInput(bool& all_is_locked, Vector2f& move, float& dt);
	void TryToMove(Vector2f& result_position, std::vector<RectangleShape>& walls, vector<sf::RectangleShape>& chest_zones, RectangleShape& boiler_zone);
	void TryToMove(Vector2f& result_position, vector<vector<Chunk>>& chunks, Hub_assets& hub);
	void TryToMove(Vector2f& result_position, std::vector<RectangleShape>& walls);
	// moving
	
	// drag & drop
	void drag_item(RenderWindow& window, Vector2i& mouse_pos, bool& inventoryClicked, bool& all_is_locked, vector<std::reference_wrapper<Chest>>& chests, unique_ptr<Armor>& temp_armor,
		unique_ptr<Item>& temp_holder, Vector2f& back_pos, InventorySlot*& oldSlot, ArmorSlot*& oldArmor);
	void drag_item(RenderWindow& window, Vector2f& mouse_pos_UI, bool& inventoryClicked, bool& all_is_locked, vector<std::reference_wrapper<Chest>>& chests, unique_ptr<Armor>& temp_armor,
		unique_ptr<Item>& temp_holder, Vector2f& back_pos, InventorySlot*& oldSlot, ArmorSlot*& oldArmor);
	void drop_item(const Event& event, vector<std::reference_wrapper<Chest>>& chests, unique_ptr<Armor>& temp_armor, unique_ptr<Item>& temp_holder, Vector2f& back_pos, InventorySlot*& oldSlot, ArmorSlot*& oldArmor);
	// drag & drop

};