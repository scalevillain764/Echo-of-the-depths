#pragma once
#pragma once
#include "SFML/Graphics.hpp"
//#include "IneventoryTypes.h"

using namespace sf;

class InventorySlot;

class Item_Buff {
public:
    bool shouldRemove = false;
    Sprite icon;
    Texture texture;
    string name;
    /*Item* targetItem = nullptr;*/

    string getName() const { return name; }

    virtual void update(std::vector<InventorySlot>& inventory) = 0;
    virtual void draw(RenderWindow& window) = 0;

    void update_status_description(Vector2i& mouse_pos, RenderWindow& window) {
        if (icon.getGlobalBounds().contains(static_cast<Vector2f>(mouse_pos))) {
            status_description_sprite.setPosition(icon.getPosition() + Vector2f(-28.f, -45.f));
            window.draw(status_description_sprite);
        }
    }

    Texture status_description_texture;
    Sprite status_description_sprite;

};

class DamageBoost : public Item_Buff {
    float multiplier;
    sf::Clock duration_timer;
    float active_time;
    bool isActive = false;

public:
    DamageBoost(const std::string& filename, float mult, float active_time, const std::string &status_description)
        : multiplier(mult), active_time(active_time)
    {
        name = "Damage boost";
        texture.loadFromFile(filename);
        icon.setTexture(texture);
        status_description_texture.loadFromFile(status_description);
        status_description_sprite.setTexture(status_description_texture);
    }

    void use(std::vector<InventorySlot>& inventory) {
        if (isActive) return;

        for (auto& item : inventory) {
            if (item.item) {
                if (auto* sword = dynamic_cast<Sword*>(item.item.get())) {
                    sword->damage = static_cast<int>(sword->base_damage * (1.f + multiplier));
                    isActive = true;
                }
            }

        }
        duration_timer.restart();
    }

    void update(std::vector<InventorySlot>& inventory) override {
        if (!isActive) return;

        if (duration_timer.getElapsedTime().asSeconds() >= active_time) {
            for (auto& item : inventory) {
                if (auto* sword = dynamic_cast<Sword*>(item.item.get())) {
                    sword->damage = sword->base_damage;
                }
            }
            shouldRemove = true;
            isActive = false;
        }
    }

    void draw(RenderWindow& window) override {
        if (isActive)
            window.draw(icon);
    }
};
