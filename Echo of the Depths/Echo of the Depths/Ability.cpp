#include "Ability.h"
#include "MainPlayer.h"
#include <iostream>

// Reflect damage
Reflect_damage::Reflect_damage(float reflection_percentage, int duration, const string& filename, const string &status_filename, const string &description) : 
    reflection_percentage(reflection_percentage),  duration(duration)  {
    texture_list.loadFromFile(filename);
    status_texture.loadFromFile(status_filename);
    status_sprite.setTexture(status_texture);
    sprite.setTexture(texture_list);
    sprite.setTextureRect(IntRect(current_frame, 0, FRAME_WIDTH, FRAME_HEIGHT));
    name = "Damage reflection";
    status_description_texture.loadFromFile(description);
    status_description_sprite.setTexture(status_description_texture);
}

void Reflect_damage::use(MainPlayer& player, bool& all_is_locked) {
    isActive = true;
}

void Reflect_damage::update(MainPlayer &player) {
    if (isActive) {
        sprite.setPosition(player.sprite.getPosition().x - 36.f, player.sprite.getPosition().y - 42.f);
        if (animation_clock.getElapsedTime().asSeconds() > 1) {
            sprite.setTextureRect(IntRect(current_frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
            if (forward) {
                current_frame++;
            }
            else {
                current_frame--;
            }         
        }
        if (current_frame >= 2) {
            current_frame = 2;
            forward = false;
        }
        else if (current_frame <= 0) {
            current_frame = 0;
            forward = true;
        }
        if (life_time_clock.getElapsedTime().asSeconds() >= 1.f) {
            life_time_clock.restart();
            duration--;
        }
        if (duration < 0) {
            isActive = false;
            shouldRemove = true;
        }
    }
}

void Reflect_damage::draw(RenderWindow& window, MainPlayer& player) {
    if (isActive) {
        window.draw(sprite);
    }
}

std::unique_ptr<Ability> Reflect_damage::clone()
{
    return std::make_unique<Reflect_damage>(*this);
}

// Reflect damage

// Read Book
ReadBook::ReadBook(int amount_of_pages, const std::string& folder, Font &font) {
    pages.reserve(amount_of_pages);
    for (int i = 1; i <= amount_of_pages; ++i) {
        sf::Texture texture;
        if (!texture.loadFromFile("textures/books/" + folder + "/P" + std::to_string(i) + ".png")) {        
            continue;
        }
        pages.push_back(std::move(texture));
    }
    rect.setSize(sf::Vector2f(1920.f, 1080.f));
    rect.setFillColor(sf::Color(0, 0, 0, 170));
    rect.setPosition(0.f, 0.f);
    if (!pages.empty()) {
        sprite.setTexture(pages[0]);
    }
    sprite.setPosition(378.f, 103.f);
    
    keys.setCharacterSize(24);
    keys.setFillColor(Color::White);
    keys.setFont(font);
    keys.setPosition(8.f, 58.f);
    keys.setString("[->] - Next page\n[<-] - Previous page\n[ESC] - Close");
}

void ReadBook::use(MainPlayer& player, bool& all_is_locked) {
    current_page = 0;
    sprite.setTexture(*pages.begin());
    isActive = true;
    all_is_locked = true;
}

void ReadBook::update_event(const Event& event, bool& all_is_locked) {
    if (isActive) {
        if (event.type == event.KeyPressed) {
            if (event.key.code == Keyboard::Escape) {
                isActive = false;
                all_is_locked = false;
            }
            if (event.key.code == Keyboard::Right) {
                if (current_page + 1 < pages.size()) {
                    current_page++;
                    sprite.setTexture(pages[current_page]);
                }
            }
            if (event.key.code == Keyboard::Left) {
                if (current_page > 0) {
                    current_page--;
                    sprite.setTexture(pages[current_page]);
                }
            }
        }
    }
}

void ReadBook::add_page(const string& page_name) {
    Texture new_page;
    new_page.loadFromFile(page_name);
    pages.push_back(std::move(new_page));
}

void ReadBook::draw(RenderWindow& window, MainPlayer &player) {
    if (isActive) {
        window.draw(rect);
        window.draw(sprite);
        window.draw(keys);
    }
}

std::unique_ptr<Ability> ReadBook::clone()
{
    return std::make_unique<ReadBook>(*this);
}
 // Read Book

// TP
Teleport::Teleport(float dist) : distance(dist) {
    for (int i = 0; i < 8; i++) {
        textures[i].loadFromFile("textures/chapone/locationtwo/Items/fog_mask/puff" + std::to_string(i + 1) + ".png");
    }
}

void Teleport::use(MainPlayer& player, bool &all_is_locked) {
    Vector2f direction(0.f, 0.f);
    Vector2f distance_vector(distance, distance);
    if (player.is_watching_back) {
        direction = Vector2f(0.f, 1.f);
    }
    if (player.is_watching_front) {
        direction = Vector2f(0.f, -1.f);
    }
    if (player.is_watching_left) {
        direction = Vector2f(-1.f, 0.f);
    }
    if (player.is_watching_right) {
        direction = Vector2f(1.f, 0.f);
    }
    Vector2f curent_result_distance(distance_vector.x * direction.x, distance_vector.y * direction.y);
    result_distance = Vector2f(player.sprite.getPosition() + curent_result_distance);

    draw_effect = true;
    current_frame = 0;
}

void Teleport::draw(RenderWindow& window, MainPlayer& player) {
    if (draw_effect) {
        if (clock.getElapsedTime().asMilliseconds() > 50) {
            current_frame++;
            if (current_frame >= 8) {
                current_frame = 0;
                draw_effect = false;
                shouldRemove = true;
            }
            sprite.setTexture(textures[current_frame]);
            clock.restart();
        }
        sprite.setPosition(player.sprite.getPosition().x - 61.f, player.sprite.getPosition().y - 26.f);
        window.draw(sprite);
    }
}

std::unique_ptr<Ability> Teleport::clone()
{
    return std::make_unique<Teleport>(*this);
}

Vector2f Teleport::get_result_distance() {
    return result_distance;
}
// TP

// HEAL
Heal::Heal(int dur, int heal_amount_second, const string& filename, const string& description) : duration(dur), heal_amount_second(heal_amount_second) {
    for (int i = 0; i < 8; i++) {
        heal_effect[i].loadFromFile("textures/Potions/heal/heal_effect/heal" + std::to_string(i + 1) + ".png");
    }
    heal_effect_sprite.setTexture(heal_effect[0]);
    heal_effect_sprite.setScale(0.4f, 0.4f);

    status_texture.loadFromFile(filename);
    status_sprite.setTexture(status_texture);
    name = "Healing";
    status_description_texture.loadFromFile(description);
    status_description_sprite.setTexture(status_description_texture);
}

void Heal::use(MainPlayer& player, bool& all_is_locked) {
    isActive = true;
    heal_timer.restart();
    heal_effect_clock.restart();
}

std::unique_ptr<Ability> Heal::clone()
{
    return std::make_unique<Heal>(*this);
}

void Heal::update(MainPlayer& player) {
    if (!isActive) return;

    if (heal_timer.getElapsedTime().asSeconds() >= 1) {
        player.current_health = std::min(player.maxHP, player.current_health + heal_amount_second); // выбираем меньшее
        heal_timer.restart();
        duration--;
    }

    if (heal_effect_clock.getElapsedTime().asMilliseconds() > 100) {
        current_heal_frame = (current_heal_frame + 1) % 8;
        heal_effect_sprite.setTexture(heal_effect[current_heal_frame]);
        heal_effect_clock.restart();
    }

    if (duration <= 0) {
        isActive = false;
        shouldRemove = true;
    }

    heal_effect_sprite.setPosition(player.sprite.getPosition().x - 76.f, player.sprite.getPosition().y + 5.f);
}

void Heal::draw(RenderWindow& window, MainPlayer& player) {
    if (isActive) {
        window.draw(heal_effect_sprite);
    }
}
// HEAL

// open scroll
Open_Scroll::Open_Scroll(Texture &texture1, Texture &texture2, Font& font) { // texture1 - сам скрол,  texture2 - текст сверху
    sprite.setTexture(texture1);
    sprite.setPosition(719.f, 411.f);

    up_text.setPosition(570.f, 10.f);
    up_text.setTexture(texture2);

    background.setSize({ 1920.f, 1080.f });
    background.setFillColor(sf::Color(0, 0, 0, 170));

    esc.setFont(font);
    esc.setString("[ESC] - close");
    esc.setCharacterSize(24);
    esc.setFillColor(sf::Color::White);
    esc.setPosition(8.f, 60.f);
}

void Open_Scroll::use(MainPlayer& player, bool &all_is_locked)
{
    isActive = true;
}

void Open_Scroll::update(const Event& event, bool& isSeen, bool &all_is_locked)
{
    if (!isActive) return;

    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
        isActive = false;
        shouldRemove = true;
        isSeen = true;
        all_is_locked = false;
    }
}

std::unique_ptr<Ability> Open_Scroll::clone()
{
    return std::make_unique<Open_Scroll>(*this);
}

void Open_Scroll::draw(RenderWindow& window, MainPlayer& player) {
    if (isActive) {
        window.draw(background);
        window.draw(up_text);
        window.draw(sprite);
        window.draw(esc);
    }
}
// open scroll

// shield constructor
Shield::Shield(int dmg_reduction, int duration, const string &filename)
    : dmg_reduction(dmg_reduction), duration(duration) {
    status_texture.loadFromFile(filename);
    status_sprite.setTexture(status_texture);
}
// shield destructor

// ice shield
Ice_Shield::Ice_Shield(int dmg_reduction, int duration, const string &filename, const string& description) : Shield(dmg_reduction, duration, filename) {
    texture.loadFromFile("textures/chapone/locationtwo/Items/ice_amulet/ice_amulet_shield2.png");
    sprite.setTexture(texture);
    name = "Ice shield";
    status_description_texture.loadFromFile(description);
    status_description_sprite.setTexture(status_description_texture);
}

void Ice_Shield::use(MainPlayer& player, bool &all_is_locked)
{
    isActive = true;
}

void Ice_Shield::update(MainPlayer& player)
{
    if (!isActive) return;

    sprite.setPosition(player.sprite.getPosition().x - 50.f, player.sprite.getPosition().y - 22.f);
    
    if (LifeTimeClock.getElapsedTime().asSeconds() >= 1.f) {
        LifeTimeClock.restart();
        duration--;
    }

    if (duration < 0) {
        isActive = false;
        shouldRemove = true;
    }
}

void Ice_Shield::draw(RenderWindow& window, MainPlayer& player)
{
    if (isActive) {
        window.draw(sprite);
        window.draw(status_sprite);
    }
}

std::unique_ptr<Ability> Ice_Shield::clone() {
    return std::make_unique<Ice_Shield>(*this);
}
// ice shield


