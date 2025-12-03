#pragma once
#include "SFML/Graphics.hpp"
#include <memory>
#include "IneventoryTypes.h"
#include "Ability.h"
#include <map>

using namespace sf;
using std::vector;
using std::unique_ptr;
using std::map;

const Vector2f right_scale_sword = Vector2f(0.875f, 0.875f);
const Vector2f left_front_scale_sword = Vector2f(-0.875f, 0.875f);
const Vector2f front_offset_sword = Vector2f(20.f, 48.f);
const Vector2f left_offset_sword = Vector2f(15.f, 47.f);
const Vector2f right_offset_sword = Vector2f(22.f, 47.f);

const Vector2f right_scale = Vector2f(0.5f, 0.5f);
const Vector2f left_front_scale = Vector2f(-0.5f, 0.5f);
const Vector2f left_offset = Vector2f(21.f, 68.f);
const Vector2f right_offset = Vector2f(19.f, 68.f);
const Vector2f front_offset = Vector2f(25.f, 63.f);

const Vector2f left_offset_key = Vector2f(20.f, 66.f);
const Vector2f right_offset_key = Vector2f(17.f, 66.f);
const Vector2f front_offset_key = Vector2f(23.f, 63.f);

class ItemManager {
public :
    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<Armor>> armor;

    bool loadAllItems(std::map<std::string, Texture>& allItemTextures, Font& font) {
         // usual sword 0
        std::unique_ptr<Sword> sword = std::make_unique<Sword>();
        sword->name = "sword";
        sword->should_save = true;
        sword->sprite.setTexture(allItemTextures.at("sword"));
        sword->base_damage = 13;
        sword->damage = 13;
        sword->right_scale = right_scale_sword;
        sword->left_front_scale = left_front_scale_sword;
        sword->front_offset = front_offset_sword;
        sword->left_offset = left_offset_sword;
        sword->right_offset = right_offset_sword;
        sword->slash_texture_right_path = "textures/chapone/weapons/hit_right.png";
        sword->slash_texture_left_path = "textures/chapone/weapons/hit_left.png";
        sword->slash_texture_back_path = "textures/chapone/weapons/hit_back.png";
        sword->slash_texture_front_path = "textures/chapone/weapons/hit_front.png";
        sword->attack_cooldown = 0.85f;
        items.push_back(std::move(sword));
        
        // bloody crystal 1
        std::unique_ptr<SimpleItem> bloody_crystal = std::make_unique<SimpleItem>();
        bloody_crystal->name = "bloody_crystal";
        bloody_crystal->sprite.setTexture(allItemTextures.at("bloody_crystal"));
        bloody_crystal->right_offset = right_offset;
        bloody_crystal->left_offset = left_offset;
        bloody_crystal->front_offset = front_offset;
        bloody_crystal->right_scale = right_scale;
        bloody_crystal->left_front_scale = left_front_scale;
        items.push_back(std::move(bloody_crystal));

        // edge of darkness 2
        std::unique_ptr<Sword> edge_of_darkness = std::make_unique<Sword>();
        edge_of_darkness->name = "edge of darkness sword";
        edge_of_darkness->should_save = true;
        edge_of_darkness->sprite.setTexture(allItemTextures.at("edge_of_dark"));
        edge_of_darkness->base_damage = 28;
        edge_of_darkness->damage = 28;
        edge_of_darkness->right_scale = right_scale_sword;
        edge_of_darkness->left_front_scale = left_front_scale_sword;
        edge_of_darkness->front_offset = front_offset_sword;
        edge_of_darkness->left_offset = left_offset_sword;
        edge_of_darkness->right_offset = right_offset_sword;
        edge_of_darkness->slash_texture_right_path = "textures/chapone/locationtwo/weapons/edge_of_darkness/hit_right.png"; 
        edge_of_darkness->slash_texture_left_path = "textures/chapone/locationtwo/weapons/edge_of_darkness/hit_left.png";
        edge_of_darkness->slash_texture_back_path = "textures/chapone/locationtwo/weapons/edge_of_darkness/hit_back.png";
        edge_of_darkness->slash_texture_front_path = "textures/chapone/locationtwo/weapons/edge_of_darkness/hit_front.png";
        edge_of_darkness->attack_cooldown = 0.65f;
        edge_of_darkness->crit_chance = 25;
        edge_of_darkness->crit_multiplier = 1.3f;
        items.push_back(std::move(edge_of_darkness));

        // heal potion 3
        std::unique_ptr <Heal_Potion> heal_potion = std::make_unique<Heal_Potion>();
        heal_potion->name = "heal_potion";
        heal_potion->sprite.setTexture(allItemTextures.at("heal_potion"));
        heal_potion->right_offset = right_offset;
        heal_potion->left_offset = left_offset;
        heal_potion->front_offset = front_offset;
        heal_potion->right_scale = right_scale;
        heal_potion->left_front_scale = left_front_scale;
        heal_potion->ability = std::make_unique<Heal>(5, 5, "textures/Potions/heal/heal_icon.png", "textures/buffs/buffs_description/heal_buff_description.png");
        items.push_back(std::move(heal_potion));

        // stone memory 4
        std::unique_ptr<SimpleItem> memory_stone = std::make_unique<SimpleItem>();
        memory_stone->name = "memory_stone";
        memory_stone->sprite.setTexture(allItemTextures.at("memory_stone"));
        memory_stone->right_offset = right_offset;
        memory_stone->left_offset = left_offset;
        memory_stone->front_offset = front_offset;
        memory_stone->right_scale = right_scale;
        memory_stone->left_front_scale = left_front_scale;
        items.push_back(std::move(memory_stone));

        // fog mask 5
        std::unique_ptr<Amulet> fog_mask = std::make_unique<Amulet>();
        fog_mask->name = "fog mask";
        fog_mask->should_save = true;
        fog_mask->sprite.setTexture(allItemTextures.at("fog mask"));
        fog_mask->right_offset = right_offset;
        fog_mask->left_offset = left_offset;
        fog_mask->front_offset = front_offset;
        fog_mask->right_scale = right_scale;
        fog_mask->left_front_scale = left_front_scale;
        fog_mask->ability = std::make_unique<Teleport>(100.f);
        fog_mask->cooldown = 6;
        items.push_back(std::move(fog_mask));

        // ice amulet 6
        std::unique_ptr<Amulet> ice_amulet = std::make_unique<Amulet>();
        ice_amulet->name = "ice amulet";
        ice_amulet->should_save = true;
        ice_amulet->sprite.setTexture(allItemTextures.at("ice amulet"));
        ice_amulet->right_offset = right_offset;
        ice_amulet->left_offset = left_offset;
        ice_amulet->front_offset = front_offset;
        ice_amulet->right_scale = right_scale;
        ice_amulet->left_front_scale = left_front_scale;
        ice_amulet->ability = std::make_unique<Ice_Shield>(30, 6, "textures/chapone/locationtwo/Items/ice_amulet/shield_status.png", "textures/buffs/buffs_description/ice_shield_description.png");
        ice_amulet->cooldown = 11;
        items.push_back(std::move(ice_amulet));

        // torch scroll 7
        std::unique_ptr<Scroll> torch_scroll = std::make_unique<Scroll>();
        torch_scroll->name = "torch_scroll";
        torch_scroll->should_save = true;
        torch_scroll->sprite.setTexture(allItemTextures.at("torch_scroll"));
        torch_scroll->right_offset = right_offset;
        torch_scroll->left_offset = left_offset;
        torch_scroll->front_offset = front_offset;
        torch_scroll->right_scale = right_scale;
        torch_scroll->left_front_scale = left_front_scale;
        torch_scroll->ability = std::make_unique<Open_Scroll>(allItemTextures.at("torch_scroll_activated"), allItemTextures.at("lim_scroll_alert"), font);
        items.push_back(std::move(torch_scroll));

        // fern leaf 8
        std::unique_ptr<SimpleItem> fern_leaf = std::make_unique<SimpleItem>();
        fern_leaf->name = "fern_leaf";
        fern_leaf->sprite.setTexture(allItemTextures.at("fern_leaf"));
        fern_leaf->right_offset = right_offset;
        fern_leaf->left_offset = left_offset;
        fern_leaf->front_offset = front_offset;
        fern_leaf->right_scale = right_scale;
        fern_leaf->left_front_scale = left_front_scale;
        items.push_back(std::move(fern_leaf));

        // skull 9
        std::unique_ptr<SimpleItem> skull = std::make_unique <SimpleItem>();
        skull->name = "skull";
        skull->sprite.setTexture(allItemTextures.at("skull"));
        skull->right_offset = right_offset;
        skull->left_offset = left_offset;
        skull->front_offset = front_offset;
        skull->right_scale = right_scale;
        skull->left_front_scale = left_front_scale;
        items.push_back(std::move(skull));

        // vein of earth 10
        std::unique_ptr<Sword> vein_of_earth = std::make_unique<Sword>();
        vein_of_earth->name = "vein_of_earth";
        vein_of_earth->should_save = true;
        vein_of_earth->sprite.setTexture(allItemTextures.at("vein_of_earth"));
        vein_of_earth->base_damage = 41;
        vein_of_earth->damage = 41;
        vein_of_earth->right_scale = right_scale_sword;
        vein_of_earth->left_front_scale = left_front_scale_sword;
        vein_of_earth->front_offset = front_offset_sword;
        vein_of_earth->left_offset = left_offset_sword;
        vein_of_earth->right_offset = right_offset_sword;
        vein_of_earth->slash_texture_right_path = "textures/chapone/locationthree/weapons/vein_of_earth/hit_right.png";
        vein_of_earth->slash_texture_left_path = "textures/chapone/locationthree/weapons/vein_of_earth/hit_left.png";
        vein_of_earth->slash_texture_back_path = "textures/chapone/locationthree/weapons/vein_of_earth/hit_back.png";
        vein_of_earth->slash_texture_front_path = "textures/chapone/locationthree/weapons/vein_of_earth/hit_front.png";
        vein_of_earth->attack_cooldown = 0.55f;
        vein_of_earth->poison_duration = 3;
        vein_of_earth->poison_damage = 2;
        vein_of_earth->slowing_duration = 3;
        vein_of_earth->slowing_percent = 30;

        items.push_back(std::move(vein_of_earth));

        // ancient seal1 11
        std::unique_ptr<SimpleItem> ancient_seal1 = std::make_unique<SimpleItem>();
        ancient_seal1->should_save = true;
        ancient_seal1->name = "ancient_seal1";
        ancient_seal1->sprite.setTexture(allItemTextures.at("ancient_seal1"));
        ancient_seal1->right_offset = right_offset;
        ancient_seal1->left_offset = left_offset;
        ancient_seal1->front_offset = front_offset;
        ancient_seal1->right_scale = right_scale;
        ancient_seal1->left_front_scale = left_front_scale;
        items.push_back(std::move(ancient_seal1));

        // cursed diary (book third location) 12
        std::unique_ptr<Book> cursed_diary = std::make_unique<Book>();
        cursed_diary->should_save = true;
        cursed_diary->name = "cursed_diary";
        cursed_diary->sprite.setTexture(allItemTextures.at("cursed_diary"));
        cursed_diary->right_offset = right_offset;
        cursed_diary->left_offset = left_offset;
        cursed_diary->front_offset = front_offset;
        cursed_diary->right_scale = right_scale;
        cursed_diary->left_front_scale = left_front_scale;
        cursed_diary->ability = std::make_unique<ReadBook>(2, "cursed_diary(third_loc)", font);
        items.push_back(std::move(cursed_diary));
        
        // wardrope key 13
        std::unique_ptr<Key> wardrope_key = std::make_unique<Key>();
        wardrope_key->should_save = true;
        wardrope_key->name = "third_loc_wardrope_key";
        wardrope_key->sprite.setTexture(allItemTextures.at("third_loc_wardrope_key"));
        wardrope_key->right_offset = right_offset_key;
        wardrope_key->left_offset = left_offset_key;
        wardrope_key->front_offset = front_offset_key;
        wardrope_key->right_scale = right_scale;
        wardrope_key->left_front_scale = left_front_scale;
        items.push_back(std::move(wardrope_key));

        // cursed mirror 14
        std::unique_ptr<SimpleItem> cursed_mirror = std::make_unique<SimpleItem>();
        cursed_mirror->should_save = true;
        cursed_mirror->name = "cursed_mirror";
        cursed_mirror->sprite.setTexture(allItemTextures.at("cursed_mirror"));
        cursed_mirror->right_offset = right_offset;
        cursed_mirror->left_offset = left_offset;
        cursed_mirror->front_offset = front_offset;
        cursed_mirror->right_scale = right_scale;
        cursed_mirror->left_front_scale = left_front_scale;
        cursed_mirror->cooldown = 14;
        cursed_mirror->ability = std::make_unique <Reflect_damage>(0.3f, 8, "textures/chapone/locationthree/Items/cursed_mirror/cursed_mirror_effect.png",
            "textures/chapone/locationthree/Items/cursed_mirror/icon.png", "textures/buffs/buffs_description/dmg_reflection_description.png");
        items.push_back(std::move(cursed_mirror));
        
        // portal book 15 (third location)
        std::unique_ptr<Book> portal_book = std::make_unique<Book>();
        portal_book->should_save = true;
        portal_book->name = "portal_book";
        portal_book->sprite.setTexture(allItemTextures.at("portal_book"));
        portal_book->right_offset = right_offset;
        portal_book->left_offset = left_offset;
        portal_book->front_offset = front_offset;
        portal_book->right_scale = right_scale;
        portal_book->left_front_scale = left_front_scale;
        portal_book->ability = std::make_unique<ReadBook>(1, "portal_book(third_loc)", font);
        items.push_back(std::move(portal_book));

        // apple 16
        std::unique_ptr<Book> mysterious_apple = std::make_unique<Book>();
        mysterious_apple->should_save = true;
        mysterious_apple->name = "mysterious_apple";
        mysterious_apple->sprite.setTexture(allItemTextures.at("apple"));
        mysterious_apple->right_offset = right_offset;
        mysterious_apple->left_offset = left_offset;
        mysterious_apple->front_offset = front_offset;
        mysterious_apple->right_scale = right_scale;
        mysterious_apple->left_front_scale = left_front_scale;
        items.push_back(std::move(mysterious_apple));

       return true;
    }

    bool loadArmor(std::map<std::string, Texture>& allItemTextures) {
        // shadow armor 0
        std::unique_ptr<Armor> shadow_armor = std::make_unique<Armor>();
        shadow_armor->name = "shadow_armor";
        shadow_armor->sprite.setTexture(allItemTextures.at("shadow_armor"));
        shadow_armor->evasion = 15;
        shadow_armor->armor = 12;
        shadow_armor->base_armor = 12;
        armor.push_back(std::move(shadow_armor));
        return true;
    }

    Item* get(size_t index) {
        if (index < items.size())
            return items[index].get();
        return nullptr;
    }

    const std::vector<std::unique_ptr<Item>>& getAll() const {
        return items;
    }
};
