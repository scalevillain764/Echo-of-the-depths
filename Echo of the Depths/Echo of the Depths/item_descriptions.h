#pragma once
#include "SFML/Graphics.hpp"
#include <unordered_map>
using namespace sf;

class ItemDescription {
private :
	Texture usual_sword_description;
	Texture skull_description;
	Texture edge_of_darkness_description;
	Texture bloody_crystal_description;
	Texture heal_potion_description;
	Texture ice_amulet_description;
	Texture shadow_armor_description;
	Texture emtpy_armor_slot;
	Texture fog_mask_description;
	Texture memory_stone_description;
	Texture ancient_seal_description;
	Texture vein_of_earth_description;
	Texture wkey_description; // third location
	Texture cursed_mirror_description;
public :
	std::unordered_map<std::string, Texture> item_descriptions; // ключ - название предмета, значение - текстура с описанием
	Sprite sprite;

	bool load() {
		if (!usual_sword_description.loadFromFile("textures/item_descr/sword_description.png") ||
			!skull_description.loadFromFile("textures/item_descr/skull_description.png") ||
			!edge_of_darkness_description.loadFromFile("textures/item_descr/edge_of_darkness_description.png") ||
			!bloody_crystal_description.loadFromFile("textures/item_descr/bloody_crystal_description.png") ||
			!heal_potion_description.loadFromFile("textures/item_descr/heal_potion_description.png") ||
			!ice_amulet_description.loadFromFile("textures/item_descr/ice_amulet_description.png") ||
			!shadow_armor_description.loadFromFile("textures/item_descr/shadow_armor_description.png") ||
			!emtpy_armor_slot.loadFromFile("textures/item_descr/empty_armor_slot_description.png") ||
			!fog_mask_description.loadFromFile("textures/item_descr/fog_mask_description.png") ||
			!memory_stone_description.loadFromFile("textures/item_descr/memory_stone_description.png") ||
			!ancient_seal_description.loadFromFile("textures/item_descr/ancient_seal_description.png") ||
			!vein_of_earth_description.loadFromFile("textures/item_descr/vein_of_earth_description.png") || 
			!wkey_description.loadFromFile("textures/item_descr/wkey_description.png") || 
			!cursed_mirror_description.loadFromFile("textures/item_descr/cursed_mirror_description.png"))
		{
			return false;		
		}
		item_descriptions.emplace("sword", usual_sword_description);
		item_descriptions.emplace("", skull_description);
		item_descriptions.emplace("edge of darkness sword", edge_of_darkness_description);
		item_descriptions.emplace("bloody_crystal", bloody_crystal_description);
		item_descriptions.emplace("heal_potion", heal_potion_description);
		item_descriptions.emplace("shadow_armor", shadow_armor_description);
		item_descriptions.emplace("ice amulet", ice_amulet_description);
		item_descriptions.emplace("fog mask", fog_mask_description);
		item_descriptions.emplace("memory_stone", memory_stone_description);
		item_descriptions.emplace("ancient_seal1", ancient_seal_description);
		item_descriptions.emplace("vein_of_earth", vein_of_earth_description);
		item_descriptions.emplace("third_loc_wardrope_key", wkey_description);
		item_descriptions.emplace("cursed_mirror", cursed_mirror_description);
		return true;
	}

	void find_name(Item* showingItem) {
		if (!showingItem) {
			std::cout << "find_name: nullptr received\n";
			return;
		}
		auto it = item_descriptions.find(showingItem->name);
		if (it != item_descriptions.end()) {
			sprite.setTexture(it->second);
		}
	}

	void find_armor_name(Armor* showingItem) {
		if (!showingItem) {
			std::cout << "find_name: nullptr received\n";
			return;
		}
		auto it = item_descriptions.find(showingItem->name);
		if (it != item_descriptions.end()) {
			sprite.setTexture(it->second);
		} 
	}

	void check_if_slot_empty(ArmorSlot& armor, RenderWindow &window) {
		if (armor.armor == nullptr) {
			sprite.setTexture(emtpy_armor_slot);
			sprite.setPosition(armor.hitbox.getPosition().x - 50.f,
				armor.hitbox.getPosition().y - 170.f);
		}
		window.draw(sprite);
	}

	void draw_descriptions(std::vector<InventorySlot>& inventory, RenderWindow& window, Vector2i &mouse_pos, Item *showingItem) {
		for (auto& slot : inventory) { // player inventory
			if (slot.item && slot.item->sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos))) {
				showingItem = slot.item.get();
				find_name(showingItem);
				sprite.setPosition(showingItem->sprite.getPosition().x - 60.f,
					showingItem->sprite.getPosition().y - 170.f);
				window.draw(sprite);
			}
		}
	}

	void draw_armor_player(ArmorSlot &armor_slot, RenderWindow &window, Vector2i& mouse_pos, Armor *showingArmor) {
		if (armor_slot.armor && armor_slot.armor->sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos))) {
			showingArmor = armor_slot.armor.get();
			find_armor_name(showingArmor);
			sprite.setPosition(showingArmor->sprite.getPosition().x - 50.f,
				showingArmor->sprite.getPosition().y - 170.f);
			window.draw(sprite);
		}
	}

	void draw_armor_chest(vector<ArmorSlot>& armor, RenderWindow& window, Vector2i& mouse_pos, Armor* showingArmor) {
		for (auto& slot : armor) { // player inventory
			if (slot.armor && slot.armor->sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos))) {
				showingArmor = slot.armor.get();
				find_armor_name(showingArmor);
				sprite.setPosition(showingArmor->sprite.getPosition().x - 50.f,
					showingArmor->sprite.getPosition().y - 170.f);
				window.draw(sprite);
			}
		}
	}

};