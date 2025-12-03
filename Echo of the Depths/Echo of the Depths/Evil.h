#pragma once
#include "SFML/Graphics.hpp"

#define M_PI 3.14159265358979323846

using namespace sf;

class MainPlayer;

float distance(sf::Vector2f a, sf::Vector2f b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return std::sqrt(dx * dx + dy * dy);
}

class Mush_monster {
private :
	// POISONING FROM PLAYER
	int poison_damage_FROMPLAYER = 0;
	int poison_duration_FROMPLAYER = 0;
	Clock poison_timer_FROMPLAYER;
	Clock poison_tick_clock_FROMPLAYER; // one tick
	// POISONING FROM PLAYER

	// SLOWING FROM PLAYER
	int slowing_duration_FROMPLAYER = 0; // s
	bool isPoisoned = false;
	// SLOWING FROM PLAYER
public :
	enum states {IDLE, walking, attacking}; // state
	states current_state = IDLE;
	
	Color cur_color;
	Clock ch_timer;
	bool isHitted = false; // ударил ли его?

	// zones
	RectangleShape vision_zone;
	RectangleShape attack_zone;
	// zones

	// health
	sf::RectangleShape health_bar;
	sf::RectangleShape health_back_bar;
	sf::Text health_text;
	float maxWidth = 100.f;
	int health;
	// health

	Sprite sprite;
	Texture front_right, front_left, back_right, back_left, left_left_leg, left_right_leg, right_left_leg, right_right_leg;

	// attack
	Texture attack_left, attack_right, attack_front, attack_back;
	int attack_frame = 0;
	int attack_maxFrame = 2;
	int attack_frameWidth = 148, attack_frameHeight = 223;
	Clock attack_clock;

	sf::Clock attackCooldownClock;
	float attackDelay;
	// attack

	// idle
	Texture idle_back, idle_front, idle_left, idle_right;
	int idle_frame = 0;
	int idle_maxFrame = 2;
	int frameWidth = 148, frameHeight = 223;
	Clock idle_clock;
	bool idle_forward = true;
	// idle

	// animation walking
	bool right_leg = false;
	sf::Clock animClock;
	float switchTime = 0.4f;
	sf::Vector2f position;
	float speed; // скорость движения
	float normal_speed;
	// animation walking

	Sprite slash_sprite;
	int damage;

	Slowing_trail trail; // slowing

	bool is_watching_front = true;
	bool is_watching_back = false;
	bool is_watching_right = false;
	bool is_watching_left = false;
	bool isAttacking = false;
	bool isKilled = false;

	Mush_monster(int dmg, int health, float speed, float attackDelay, const Vector2f& pos) {
		damage = dmg;
		this->health = health;
		this->speed = speed;
		normal_speed = speed;
		this->attackDelay = attackDelay;
		position = pos;
		sprite.setPosition(position);
	}

	bool load(Font &font) {
		if (!front_right.loadFromFile("textures/chapone/evil/mush_monster/front_right.png") || !front_left.loadFromFile("textures/chapone/evil/mush_monster/front_left.png") ||
			!back_right.loadFromFile("textures/chapone/evil/mush_monster/back_right.png") || !back_left.loadFromFile("textures/chapone/evil/mush_monster/back_left.png") ||
			!left_left_leg.loadFromFile("textures/chapone/evil/mush_monster/left_left.png") || !left_right_leg.loadFromFile("textures/chapone/evil/mush_monster/left_right.png") ||
			!right_left_leg.loadFromFile("textures/chapone/evil/mush_monster/right_left.png") || !right_right_leg.loadFromFile("textures/chapone/evil/mush_monster/right_right.png") ||
			!idle_front.loadFromFile("textures/chapone/evil/mush_monster/idle/idle_front.png") || !idle_back.loadFromFile("textures/chapone/evil/mush_monster/idle/idle_back.png") ||
			!idle_left.loadFromFile("textures/chapone/evil/mush_monster/idle/idle_left.png") || !idle_right.loadFromFile("textures/chapone/evil/mush_monster/idle/idle_right.png") || 
			!attack_left.loadFromFile("textures/chapone/evil/mush_monster/attack/attack_left.png") || !attack_front.loadFromFile("textures/chapone/evil/mush_monster/attack/attack_front.png") ||
			!attack_right.loadFromFile("textures/chapone/evil/mush_monster/attack/attack_right.png") || !attack_back.loadFromFile("textures/chapone/evil/mush_monster/attack/attack_back.png") || !trail.load()) {
			return false;
		}
		vision_zone.setSize(Vector2f(331.f, 412.f));
		health_bar.setFillColor(sf::Color(218, 24, 24));
		health_back_bar.setFillColor(sf::Color(37, 40, 89));
		health_back_bar.setSize({ 200.f, 12.f });
		health_text.setFont(font);
		health_text.setFillColor(sf::Color::White);
		health_text.setCharacterSize(14);
		sprite.setTexture(idle_front);
		sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
		trail.sprite.setTextureRect(IntRect(0, 0, trail.slowing_frameWidth, trail.slowing_frameHeight));
		return true;
	}

	void update(float dt, const sf::Vector2f& playerPos, MainPlayer& player, const std::vector<sf::RectangleShape>& walls, Sword *sword, float maxHealth) {
		health_bar.setPosition(sprite.getPosition().x - 24.f, sprite.getPosition().y - 17.f);
		health_back_bar.setPosition(sprite.getPosition().x - 24.f, sprite.getPosition().y - 17.f);
		health_text.setPosition(health_back_bar.getPosition().x + 93.f, health_back_bar.getPosition().y - 3.f);
		health_text.setString(std::to_string(this->health));

		float maxWidthBar = 200.f; // ширина при полном здоровье
		float currentHP = health;
		float widthBar = float(currentHP / maxHealth) * maxWidthBar;
		health_bar.setSize(sf::Vector2f(widthBar, 12.f));

		update_idle();          
		update_animation(player);
		update_zones();
		update_state(player);
		update_attack(player);   // вызываем тут
		updateDamageVisuals();
		update_slowing();
		update_poison();
		if(current_state == walking)
		update_moving(dt, playerPos, player, walls);
	}

	void update_moving(float dt, const sf::Vector2f& playerPos, MainPlayer& player, const std::vector<sf::RectangleShape>& walls) {
		if (current_state != walking) return;

		float dx = playerPos.x - position.x;
		float dy = playerPos.y - position.y;

		sf::Vector2f bestMove = position;
		float minDist = FLT_MAX;

		for (dx = -1; dx <= 1; ++dx) {
			for (dy = -1; dy <= 1; ++dy) {
				if (dx == 0 && dy == 0) continue;

				sf::Vector2f testPos = position + sf::Vector2f(dx, dy) * speed * dt;
				sprite.setPosition(testPos);

				bool collide = false;
				for (const auto& wall : walls) {
					if (sprite.getGlobalBounds().intersects(wall.getGlobalBounds())) {
						collide = true;
						break;
					}
				}

				if (!collide) {
					float dist = distance(testPos, playerPos);
					if (dist < minDist) {
						minDist = dist;
						bestMove = testPos;
					}
				}
			}
		}
		position = bestMove;
		sprite.setPosition(bestMove);
	}

	void update_slowing() {
		if (!trail.isActive) return;

		float frame_duration = float(slowing_duration_FROMPLAYER) / 24;

		if (is_watching_back || is_watching_right) {
			trail.sprite.setTexture(trail.slowing_trail_left);
			if (current_state == walking) {
				if (is_watching_back) {
					trail.sprite.setPosition(sprite.getPosition().x - 99.f, sprite.getPosition().y + 121.f);
				}
				else {
					trail.sprite.setPosition(sprite.getPosition().x - 94.f, sprite.getPosition().y + 119.f);
				}
			}
			else {
				if (is_watching_back) {
					trail.sprite.setPosition(sprite.getPosition().x - 95.f, sprite.getPosition().y + 121.f);
				}
				else {
					trail.sprite.setPosition(sprite.getPosition().x - 80.f, sprite.getPosition().y + 119.f);
				}
			}
			
		}
		else if (is_watching_front || is_watching_left) {
			trail.sprite.setTexture(trail.slowing_trail_right);
			if (current_state == walking) {
				if (is_watching_front) {
					trail.sprite.setPosition(sprite.getPosition().x + 120.f, sprite.getPosition().y + 125.f);
				}
				else {
					trail.sprite.setPosition(sprite.getPosition().x + 128.f, sprite.getPosition().y + 123.f);
				}
			}
			else {
				if (is_watching_front) {
					trail.sprite.setPosition(sprite.getPosition().x + 120.f, sprite.getPosition().y + 125.f);
				}
				else {
					trail.sprite.setPosition(sprite.getPosition().x + 110.f, sprite.getPosition().y + 123.f);
				}
			}
		}

		if (trail.slowing_clock.getElapsedTime().asSeconds() < slowing_duration_FROMPLAYER) {
			if (trail.animation_clock.getElapsedTime().asSeconds() > frame_duration) {
				trail.sprite.setTextureRect(IntRect(trail.current_slowing_frame * trail.slowing_frameWidth, 0, trail.slowing_frameWidth, trail.slowing_frameHeight));
				trail.current_slowing_frame++;
				if (trail.current_slowing_frame >= trail.max_slowing_frames) {
					trail.current_slowing_frame = 0;
					trail.isActive = false;
					speed = normal_speed;
				}
				trail.animation_clock.restart();
			}
		}
		else {
			trail.isActive = false; 
			speed = normal_speed;
		}
	}

	void draw_health(RenderWindow& window) {
		window.draw(health_back_bar);
		window.draw(health_bar);
		window.draw(health_text); 
	}

	void update_poison() {
		if (isPoisoned) {
			if (poison_timer_FROMPLAYER.getElapsedTime().asSeconds() < poison_duration_FROMPLAYER) {
				if (poison_tick_clock_FROMPLAYER.getElapsedTime().asSeconds() >= 1) {
					isHitted = true;
					health -= poison_damage_FROMPLAYER;
					sprite.setColor(Color::Red);
					ch_timer.restart();
					poison_tick_clock_FROMPLAYER.restart();
				}
			}
		}
	}

	void get_damage(SwordSlashEffect& swordEffect, Sword* sword, MainPlayer& player) {
		if (swordEffect.sprite.getGlobalBounds().intersects(sprite.getGlobalBounds())) {
			isHitted = true;
			sprite.setColor(Color::Red);
			int final_damage = sword->damage;
			if (sword->crit_chance > 0 && sword->crit_multiplier > 1) {
				int roll = rand() % 100;
				if (roll < sword->crit_chance) {
					final_damage = sword->damage * sword->crit_multiplier;
					swordEffect.active_crits.emplace_back(swordEffect.crit_texture, player, swordEffect);
				}
			}
			if (sword->slowing_percent > 0) {
				float buf_slowing_percent = speed * sword->slowing_percent / 100;
				speed -= buf_slowing_percent;
				slowing_duration_FROMPLAYER = sword->slowing_duration;
				trail.isActive = true;
				trail.slowing_clock.restart();
				trail.animation_clock.restart();
			}
			if (sword->poison_damage > 0) {			
				poison_damage_FROMPLAYER = sword->poison_damage;
				poison_duration_FROMPLAYER = sword->poison_duration;
				poison_timer_FROMPLAYER = sword->poison_timer;
				poison_tick_clock_FROMPLAYER = sword->poison_tick_clock;
				if (!isPoisoned) {
					isPoisoned = true;
					poison_timer_FROMPLAYER.restart();
				}
				else {
					poison_timer_FROMPLAYER.restart();
				}			
			}

			if (final_damage < 0) {
				final_damage = 0;
			}

			health -= final_damage;
			ch_timer.restart();
		}
	}

	void updateDamageVisuals() {
		if (isHitted && ch_timer.getElapsedTime().asSeconds() > 0.2f) {
			sprite.setColor(Color::White);
			isHitted = false;
		}
	}

	void update_animation(MainPlayer& player) {
		if (current_state == walking) {
			if (animClock.getElapsedTime().asSeconds() > switchTime) {
				right_leg = !right_leg;
				animClock.restart();

				sf::Vector2f direction = player.sprite.getPosition() - sprite.getPosition();
				float angle = std::atan2(direction.y, direction.x);

				// Углы направления в радианах
				if (angle >= -0.75f * M_PI && angle <= -0.25f * M_PI) {
					// вверх
					is_watching_front = true;
					is_watching_back = is_watching_left = is_watching_right = false;
					sprite.setTextureRect(IntRect(0, 0, 149, 221));
					if (right_leg) setBackRightTexture();
					else setBackLeftTexture();
				}
				else if (angle >= 0.25f * M_PI && angle <= 0.75f * M_PI) {
					// вниз
					is_watching_back = true;
					is_watching_front = is_watching_left = is_watching_right = false;
					sprite.setTextureRect(IntRect(0, 0, 149, 221));
					if (right_leg) setFrontRightTexture();
					else setFrontLeftTexture();
				}
				else if (angle <= -0.75f * M_PI || angle >= 0.75f * M_PI) {
					// влево
					is_watching_left = true;
					is_watching_back = is_watching_front = is_watching_right = false;
					sprite.setTextureRect(IntRect(0, 0, 150, 221));
					if (right_leg) setLeft_LeftTexture();
					else setLeft_RightTexture();
				}
				else {
					// вправо
					is_watching_right = true;
				    is_watching_back = is_watching_front = is_watching_left = false;
					sprite.setTextureRect(IntRect(0, 0, 150, 221));
					if (right_leg) setRight_RightTexture();
					else setRight_LeftTexture();
				}
			}
		}
	}

	void update_zones() {
		vision_zone.setPosition(sprite.getPosition().x - 91.f, sprite.getPosition().y - 96.f);
		if (is_watching_front) {
			attack_zone.setPosition(sprite.getPosition().x - 8.f, sprite.getPosition().y - 3.f);
			attack_zone.setSize(Vector2f(163.f, 80.f));
		}
		else if (is_watching_back) {
			attack_zone.setPosition(sprite.getPosition().x - 8.f, sprite.getPosition().y + 155.f);
			attack_zone.setSize(Vector2f(163.f, 80.f));
		}
		else if (is_watching_left) {
			attack_zone.setPosition(sprite.getPosition().x - 43.f, sprite.getPosition().y + 28.f);
			attack_zone.setSize(Vector2f(43.f, 163.f));
		}
		else {
			attack_zone.setPosition(sprite.getPosition().x + 150.f, sprite.getPosition().y + 28.f);
			attack_zone.setSize(Vector2f(43.f, 163.f));
		}
	}

	void update_idle() {
		if (current_state == IDLE) {
			if (is_watching_back) {
				sprite.setTexture(idle_back);
			}
			else if (is_watching_front) {
				sprite.setTexture(idle_front);
			}
			else if (is_watching_left) {
				sprite.setTexture(idle_left);
			}
			else if (is_watching_right) {
				sprite.setTexture(idle_right);
			}
			
			if (idle_clock.getElapsedTime().asSeconds() > 0.75) {
				if (idle_forward) {
					idle_frame++;
					if (idle_frame >= idle_maxFrame) {
						idle_frame = idle_maxFrame;
						idle_forward = false;
					}
				}
				else if (!idle_forward) {
					idle_frame--;
					if (idle_frame <= 0) {
						idle_frame = 0;
						idle_forward = true;
					}
				}
				sprite.setTextureRect(sf::IntRect(idle_frame * frameWidth, 0, frameWidth, frameHeight));
				idle_clock.restart();
			}
		}
	}

	void update_state(MainPlayer& player) {
		if (isAttacking) return;

		if (!isAttacking && attack_zone.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
			isAttacking = true;
			attack_frame = 0;
			current_state = attacking;
			attack_clock.restart();
			attackCooldownClock.restart(); // начинаем кд СРАЗУ
		}
		else {
			if (vision_zone.getGlobalBounds().intersects(player.sprite.getGlobalBounds()))
				current_state = walking;
			else
				current_state = IDLE;
		}
	}

	void attack(MainPlayer& player) {
		int final_damage = damage;
		if (player.armor_slot.armor != nullptr) {
			if (player.armor_slot.armor->armor > 0) {
				final_damage = damage * (50 / (50 + player.armor_slot.armor->armor));
			}
			if (player.armor_slot.armor->evasion > 0) {
				int random_chance = rand() % 100;
				if (random_chance < player.armor_slot.armor->evasion) {
					final_damage = 0;
				}
			}
		}
		for (auto& effect : player.active_effects) {
			if (auto* shield = dynamic_cast<Shield*>(effect.get())) {
				final_damage -= (final_damage * shield->dmg_reduction / 100.f);
			}
		}
		for (auto& effect : player.active_effects) {
			if (auto* reflect = dynamic_cast<Reflect_damage*>(effect.get())) {
				isHitted = true;
				int own_damage = damage * reflect->reflection_percentage;
				health -= own_damage;
				sprite.setColor(sf::Color::Red);
				ch_timer.restart();
			}
		}
		if (final_damage < 0) final_damage = 0;
		player.current_health -= final_damage;
	}

	void update_attack(MainPlayer& player) {
	if (current_state != attacking) return;

	if (is_watching_back) sprite.setTexture(attack_back);
	else if (is_watching_front) sprite.setTexture(attack_front);
	else if (is_watching_left) sprite.setTexture(attack_left);
	else if (is_watching_right) sprite.setTexture(attack_right);

	if (attackCooldownClock.getElapsedTime().asSeconds() > attackDelay) {
		if (attack_clock.getElapsedTime().asSeconds() > 0.35f) {
			sprite.setTextureRect(sf::IntRect(attack_frame * attack_frameWidth, 0, attack_frameWidth, attack_frameHeight));
				attack_frame++;
				if (attack_frame >= attack_maxFrame) {
					if (attack_zone.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
						attack(player);
					}
					attack_frame = 0;
					isAttacking = false;
					current_state = IDLE;
					attackCooldownClock.restart();
				}
				attack_clock.restart();
			}
		}	
	}

	void debug(RenderWindow &window) {
		attack_zone.setFillColor(Color::White);
		window.draw(attack_zone);
	}

	void SetHealth(int health) {
		this->health = health;
	}

	void setBackLeftTexture() {
		sprite.setTexture(back_left);
	}

	void setBackRightTexture() {
		sprite.setTexture(back_right);
	}

	void setFrontLeftTexture() {
		sprite.setTexture(front_left);
	}

	void setFrontRightTexture() {
		sprite.setTexture(front_right);
	}

	void setLeft_LeftTexture() {
		sprite.setTexture(left_left_leg);
	}

	void setLeft_RightTexture() {
		sprite.setTexture(left_right_leg);
	}

	void setRight_RightTexture() {
		sprite.setTexture(right_right_leg);
	}

	void setRight_LeftTexture() {
		sprite.setTexture(right_left_leg);
	}
};

class Wizard {
private:
	// POISONING FROM PLAYER
	int poison_damage_FROMPLAYER = 0;
	int poison_duration_FROMPLAYER = 0;
	Clock poison_timer_FROMPLAYER;
	Clock poison_tick_clock_FROMPLAYER; // one tick
	// POISONING FROM PLAYER

	// SLOWING FROM PLAYER
	int slowing_duration_FROMPLAYER = 0; // s
	bool isPoisoned = false;
	// SLOWING FROM PLAYER
public: 
	Sprite sprite;
	Texture front_right, front_left, back_right, back_left, left_left_leg, left_right_leg, right_left_leg, right_right_leg;
	Clock ch_timer;

	bool is_watching_front = false;
	bool is_watching_back = false;
	bool is_watching_right = false;
	bool is_watching_left = false;

	class fire_ball {
	public :
		Sprite sprite;
		Texture texture;
		/*CircleShape ball;*/
		bool isActive = false;
		Vector2f cur_enemypos;
		float speed = 0;
		int damage;

		fire_ball(int& damage, float& speed, Wizard& wizard, const string& texture_name) : damage(damage), speed(speed) {
			texture.loadFromFile(texture_name);
			sprite.setTexture(texture);
			if (wizard.is_watching_left) {
				sprite.setPosition(wizard.sprite.getPosition() + Vector2f(-25.f, -14.f));
			}
			else if (wizard.is_watching_right) {
				sprite.setPosition(wizard.sprite.getPosition() + Vector2f(70.f, -20.f));
			}
			else if (wizard.is_watching_front) {
				sprite.setPosition(wizard.sprite.getPosition() + Vector2f(-9.f, 0.f));
			}
			else {
				sprite.setPosition(wizard.sprite.getPosition() + Vector2f(122.f, -5.f));
			}
		}

		void activate_fireball(const Vector2f enemy_pos) {
			isActive = true;
			cur_enemypos = enemy_pos;
		}

		void update(float &dt, float speed, Wizard &wizard, MainPlayer &player) {
			if (isActive) {
				sf::Vector2f currentPos = sprite.getPosition();
				sf::Vector2f direction = cur_enemypos - currentPos;

				// нормализация направления
				float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
				if (length != 0)
					direction /= length;

				sprite.move(direction * speed * dt);

				float cur_dist = distance(sprite.getPosition(), cur_enemypos);

				if (sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
					int final_damage = damage;
					if (player.armor_slot.armor != nullptr) {
						if (player.armor_slot.armor->armor > 0) {
							final_damage = damage * (50 / (50 + player.armor_slot.armor->armor));
						}
						if (player.armor_slot.armor->evasion > 0) {
							int random_chance = rand() % 100;
							if (random_chance < player.armor_slot.armor->evasion) {
								final_damage = 0;
							}
						}
					}

					for (auto& effect : player.active_effects) {
						if (auto* shield = dynamic_cast<Shield*>(effect.get())) {
							final_damage -= (final_damage * shield->dmg_reduction / 100.f);
						}
					}

					for (auto& effect : player.active_effects) {
						if (auto* reflect = dynamic_cast<Reflect_damage*>(effect.get())) {
							wizard.isHitted = true;
							int own_damage = damage * reflect->reflection_percentage;
							wizard.health -= own_damage;
							wizard.sprite.setColor(sf::Color::Red);
							wizard.ch_timer.restart();
						}
					}

					if (final_damage < 0) {
						final_damage = 0;
					}

					player.current_health -= final_damage;
					isActive = false;
				} 
				else if (cur_dist > wizard.attack_range) {
					isActive = false;
				}
				else if (sprite.getPosition() == cur_enemypos) {
					isActive = false;
				}
			}
		}

		void draw_fireball(RenderWindow& window) {
			if (isActive) {
				window.draw(sprite);
			}
		}

	};

	unique_ptr<fire_ball> fireball = nullptr; 

	// death animation
	std::vector<Texture> tornado_textures;
	Sprite tornado_sprite;
	int cur_tornado_frame = 0;
	bool tornado_active = false;
	Clock tornado_clock;
	bool tornado_was_made = false;
	bool stop_drawing_tornado = false;
	// death animation

	Clock attack_cooldown;
	// attack 2
	float fireball_speed;
	int fireball_damage;
	string fireball_texture_name;
	Texture front_attack2, back_attack2, left_attack2, right_attack2;
	int current_attack2_frame = 0, frontnback_framewidth = 163, leftnright_framewidth = 120, attack2_frameheight = 172;
	Clock attack2_clock;
	bool attack2_isActive = false;
	// attack 2

	enum state {APPEARING, WALKING, ATTACK_1, ATTACK_2, ATTACK_3, DYING}; // attack 1 - lightning strike, .. ,
	state current_state = APPEARING;
	float attack_range;

	bool right_leg = true;
	Clock animClock;
	float switchTime = 0.25f;

	// appearing animation
	Sprite aa_sprite;
	Texture aa_texture;
	const int aa_frame_width = 186, aa_frame_height = 200;
	int  current_aa_frame = 0;
	bool aaIsActive = false;
	Clock aa_clock;
	bool spawned = false;
	// appearing animation

	// lightning strike
	Clock lightning_cooldown;
	bool hitting_right_now = false;
	int cur_lightning_value = 0; // if > 75 - strike
	bool lightning_strike_damaged = false;
	Clock two_sec_clock; 
	Texture wizard_striking_left, wizard_striking_right, wizard_striking_front, wizard_striking_back;

	int lightning_damage = 13;
	Sprite ls_sprite;
	Texture ls_texture_list;
	const int ls_frame_width = 284, ls_frame_height = 274;
	int  current_ls_frame = 0;
	bool light_strikeActive = false;
	Clock ls_clock;
	// lightning strike

	bool isHitted = false;
	bool isKilled = false;

	// health
	sf::RectangleShape health_bar;
	sf::RectangleShape health_back_bar;
	sf::Text health_text;
	float maxWidth = 165.f;
	int health;
	// health

	sf::Vector2f position;
	float speed; // скорость движения
	float normal_speed;

	// slowing
	Slowing_trail trail;
	// slowing trail

	Wizard(int lightning_damage, int health, float speed, float attack_range, int fireball_damage, float fireball_speed, const string fireball_texture_name, const Vector2f &pos) : 
		fireball_texture_name(fireball_texture_name), fireball_speed(fireball_speed), lightning_damage(lightning_damage), health(health), speed(speed), attack_range(attack_range),
		fireball_damage(fireball_damage) {
		position = pos;
		sprite.setPosition(position);
		aa_sprite.setPosition(position);
		normal_speed = speed;
	};

	bool load(Font &font) {
		if (!front_right.loadFromFile("textures/chapone/evil/wizard/walking/front_right.png") || !front_left.loadFromFile("textures/chapone/evil/wizard/walking/front_left.png") ||
			!right_right_leg.loadFromFile("textures/chapone/evil/wizard/walking/right_right.png") || !right_left_leg.loadFromFile("textures/chapone/evil/wizard/walking/right_left.png") ||
			!left_right_leg.loadFromFile("textures/chapone/evil/wizard/walking/left_right.png") || !left_left_leg.loadFromFile("textures/chapone/evil/wizard/walking/left_left.png") ||
			!back_right.loadFromFile("textures/chapone/evil/wizard/walking/back_right.png") || !back_left.loadFromFile("textures/chapone/evil/wizard/walking/back_left.png") ||
			!aa_texture.loadFromFile("textures/chapone/evil/wizard/appearing/app_animation.png") || !ls_texture_list.loadFromFile("textures/effects/lightning_strike/lightning_strike.png") || 
			!wizard_striking_left.loadFromFile("textures/chapone/evil/wizard/lightning_strike/left.png") || !wizard_striking_front.loadFromFile("textures/chapone/evil/wizard/lightning_strike/front.png") || 
		    !wizard_striking_right.loadFromFile("textures/chapone/evil/wizard/lightning_strike/right.png") || !wizard_striking_back.loadFromFile("textures/chapone/evil/wizard/lightning_strike/back.png") ||
			!front_attack2.loadFromFile("textures/chapone/evil/wizard/attack_2/front.png") || !left_attack2.loadFromFile("textures/chapone/evil/wizard/attack_2/left.png") || 
			!back_attack2.loadFromFile("textures/chapone/evil/wizard/attack_2/back.png") || !right_attack2.loadFromFile("textures/chapone/evil/wizard/attack_2/right.png") || !trail.load()) {
			return false;
		}
		sprite.setColor(Color::White);
		health_bar.setFillColor(sf::Color(218, 24, 24));
		health_back_bar.setFillColor(sf::Color(37, 40, 89));
		health_back_bar.setSize(Vector2f(maxWidth, 12.f));
		health_text.setFont(font);
		health_text.setFillColor(sf::Color::White);
		health_text.setCharacterSize(14);
		aa_sprite.setTexture(aa_texture);
		ls_sprite.setTexture(ls_texture_list);
		aa_sprite.setTextureRect(IntRect(0, 0, aa_frame_width, aa_frame_height));
		ls_sprite.setTextureRect(IntRect(0, 0, ls_frame_width, ls_frame_height));
		trail.sprite.setTextureRect(IntRect(0, 0, trail.slowing_frameWidth, trail.slowing_frameHeight));
		setRight_LeftTexture();
		return true;
	}

	void update_slowing() {
		if (!trail.isActive) return;

		float frame_duration = float(slowing_duration_FROMPLAYER) / 24;

		if (is_watching_back || is_watching_right) {
			trail.sprite.setTexture(trail.slowing_trail_left);
			if (is_watching_back) {
				trail.sprite.setPosition(sprite.getPosition().x - 69.f, sprite.getPosition().y + 41.f);
			}
			else {
				trail.sprite.setPosition(sprite.getPosition().x - 81.f, sprite.getPosition().y + 68.f);
			}
		}
		else if (is_watching_front || is_watching_left) {
			trail.sprite.setTexture(trail.slowing_trail_right);
			if (is_watching_front) {
				trail.sprite.setPosition(sprite.getPosition().x + 110.f, sprite.getPosition().y + 68.f);
			}
			else {
				trail.sprite.setPosition(sprite.getPosition().x + 97.f, sprite.getPosition().y + 65.f);
			}
		}

		if (trail.slowing_clock.getElapsedTime().asSeconds() < slowing_duration_FROMPLAYER) {
			if (trail.animation_clock.getElapsedTime().asSeconds() > frame_duration) {
				trail.sprite.setTextureRect(IntRect(trail.current_slowing_frame * trail.slowing_frameWidth, 0, trail.slowing_frameWidth, trail.slowing_frameHeight));
				trail.current_slowing_frame++;
				if (trail.current_slowing_frame >= trail.max_slowing_frames) {
					trail.current_slowing_frame = 0;
					trail.isActive = false;
					speed = normal_speed;
				}
				trail.animation_clock.restart();
			}
		}
		else {
			trail.isActive = false;
			speed = normal_speed;
		}

	}

	void update_animation(MainPlayer& player) {
		if (current_state == WALKING) {
			if (animClock.getElapsedTime().asSeconds() > switchTime) {
				right_leg = !right_leg;
				animClock.restart();

				sf::Vector2f direction = player.sprite.getPosition() - sprite.getPosition();
				float angle = std::atan2(direction.y, direction.x);

				// Углы направления в радианах
				if (angle >= -0.75f * M_PI && angle <= -0.25f * M_PI) {
					// вверх
					is_watching_front = true;
					is_watching_back = is_watching_left = is_watching_right = false;
					if (right_leg) setBackRightTexture();
					else setBackLeftTexture();
				}
				else if (angle >= 0.25f * M_PI && angle <= 0.75f * M_PI) {
					// вниз
					is_watching_back = true;
					is_watching_front = is_watching_left = is_watching_right = false;
					if (right_leg) setFrontRightTexture();
					else setFrontLeftTexture();
				}
				else if (angle <= -0.75f * M_PI || angle >= 0.75f * M_PI) {
					// влево
					is_watching_left = true;
					is_watching_back = is_watching_front = is_watching_right = false;
					if (right_leg) setLeft_LeftTexture();
					else setLeft_RightTexture();
				}
				else {
					// вправо
					is_watching_right = true;
					is_watching_back = is_watching_front = is_watching_left = false;
					if (right_leg) setRight_RightTexture();
					else setRight_LeftTexture();
				}
			}
		}
	}

	void draw_health(RenderWindow& window) {
		window.draw(health_back_bar);
		window.draw(health_bar);
		window.draw(health_text);
	}

	void spawn() {
		aaIsActive = true;	
	}

	void attack2() {
		attack2_isActive = true;
	}

	void update_spawn_animation() {
		if (aaIsActive && !spawned) {
			if (aa_clock.getElapsedTime().asSeconds() > 0.3) {
				aa_sprite.setTextureRect(IntRect(current_aa_frame * aa_frame_width, 0, aa_frame_width, aa_frame_width));
				current_aa_frame++;
				aa_clock.restart();
			}
		}
		if (current_aa_frame >= 5) {
			current_aa_frame = 5;
			spawned = true;
			aaIsActive = false;
			current_state = WALKING;
		}
	}
	
	void update_attack2(MainPlayer &player) {
		if (attack2_isActive) {
			current_state = ATTACK_2;
			if (is_watching_back) {
				sprite.setTexture(front_attack2);
			}
			else if (is_watching_front) {
				sprite.setTexture(back_attack2);
			}
			else if (is_watching_left) {
				sprite.setTexture(left_attack2);
			}
			else {
				sprite.setTexture(right_attack2);
			}

			if (attack2_clock.getElapsedTime().asSeconds() > 1) {
				if (is_watching_back || is_watching_front) {
					sprite.setTextureRect(IntRect(current_attack2_frame * frontnback_framewidth, 0, frontnback_framewidth, attack2_frameheight));
				}
				else if (is_watching_left || is_watching_right) {
					sprite.setTextureRect(IntRect(current_attack2_frame * leftnright_framewidth, 0, leftnright_framewidth, attack2_frameheight));
				}
				current_attack2_frame++;
				attack2_clock.restart();
			}
			if (current_attack2_frame >= 3) {
				attack2_isActive = false;
				current_attack2_frame = 0;
			}
			if (current_attack2_frame == 2) {
				fireball = std::make_unique<fire_ball>(fireball_damage, fireball_speed, *this, this->fireball_texture_name); 
				if (fireball) {
					fireball->activate_fireball(player.sprite.getPosition()); 
					attack_cooldown.restart();
				}
			}
		}
	}

	bool load_tornado(const string& one, const string& two, const string& three, const string& four, const string& five,
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
			if (cur_tornado_frame < tornado_textures.size() - 1)
				cur_tornado_frame++;
			else {
				stop_drawing_tornado = true;
				cur_tornado_frame = tornado_textures.size() - 1;
			}
			tornado_clock.restart();
		}
	}

	void hit_with_lightning(MainPlayer& player) {
		if (light_strikeActive) {
			if (is_watching_back) {
				sprite.setTexture(wizard_striking_front);
			}
			else if (is_watching_front) {
				sprite.setTexture(wizard_striking_back);
			}
			else if (is_watching_left) {
				sprite.setTexture(wizard_striking_left);
			}
			else {
				sprite.setTexture(wizard_striking_right);
			}

			current_state = ATTACK_1;
			
			ls_sprite.setPosition(player.sprite.getPosition() + Vector2f(-74.f, -163.f));
			if (ls_clock.getElapsedTime().asSeconds() > 0.09) {
				ls_sprite.setTextureRect(IntRect(current_ls_frame * ls_frame_width, 0, ls_frame_width, ls_frame_height));
				current_ls_frame++;
				ls_clock.restart();
			}
			if (current_ls_frame == 5 && !lightning_strike_damaged) {
				for (auto& effect : player.active_effects) {
					if (auto* reflect = dynamic_cast<Reflect_damage*>(effect.get())) {
						isHitted = true;
						int own_damage = lightning_damage * reflect->reflection_percentage;
						health -= own_damage;
						sprite.setColor(sf::Color::Red);
						ch_timer.restart();
					}
				}
				player.current_health -= lightning_damage;
				lightning_strike_damaged = true;
			}
			if (current_ls_frame >= 10) {
				current_ls_frame = 0;
				lightning_cooldown.restart();
				light_strikeActive = false;
				lightning_strike_damaged = false;
			}
		}
	}

	void update_moving(float dt, MainPlayer& player, const std::vector<sf::RectangleShape>& walls) {
		current_state = WALKING;

		float current_begin_distance = distance(position, player.sprite.getPosition());
		if (current_begin_distance > attack_range) {
			if (two_sec_clock.getElapsedTime().asSeconds() > 2 && !light_strikeActive) {
				cur_lightning_value = (rand() % 100) + 1;
				two_sec_clock.restart();
			}
			if (cur_lightning_value > 75 && !light_strikeActive && lightning_cooldown.getElapsedTime().asSeconds() > 5) {
				light_strikeActive = true;
				current_state = ATTACK_1;
			}
			else {
				light_strikeActive = false;
			}
		}
		if (current_begin_distance < attack_range) {
			if (attack_cooldown.getElapsedTime().asSeconds() >= 1.f) {
				current_state = ATTACK_2;
				attack2();
			}
		}

		float dx, dy;
		float minimal_distance = FLT_MAX;
		Vector2f final_position = position;

		for (dx = -1; dx <= 1; dx++) {
			for (dy = -1; dy <= 1; dy++) {
				if (dx == 0 && dy == 0) continue;
				float len = sqrt(pow(dx, 2) + pow(dy, 2));
				if (len != 0) {
					dx /= len;
					dy /= len;
				}

				Vector2f test_position = position + Vector2f(dx, dy) * speed * dt;
				
				bool colide = false;
				for (auto& wall : walls) {
					if (sprite.getGlobalBounds().intersects(wall.getGlobalBounds())) {
						colide = true;
						break;
					}
				}

				if (!colide) {
					float current_distance = distance(test_position, player.sprite.getPosition());
					if (current_distance > attack_range) {
						if (current_distance < minimal_distance) {
							minimal_distance = current_distance;
							final_position = test_position;
						}
					}
				}
			}
		}
		sprite.setPosition(final_position);
		position = final_position;
	}

	void get_damage(SwordSlashEffect& swordEffect, Sword* sword, MainPlayer& player) {
		if (swordEffect.sprite.getGlobalBounds().intersects(sprite.getGlobalBounds())) {
			isHitted = true;
			sprite.setColor(Color::Red);
			int final_damage = sword->damage;
			if (sword->crit_chance > 0 && sword->crit_multiplier > 1) {
				int roll = rand() % 100;
				if (roll < sword->crit_chance) {
					final_damage = sword->damage * sword->crit_multiplier;
					swordEffect.active_crits.emplace_back(swordEffect.crit_texture, player, swordEffect);
				}
			}
			if (sword->slowing_percent > 0) {
				float buf_slowing_percent = speed * sword->slowing_percent / 100;
				speed -= buf_slowing_percent;
				slowing_duration_FROMPLAYER = sword->slowing_duration;
				trail.isActive = true;
				trail.slowing_clock.restart();
				trail.animation_clock.restart();
			}
			if (sword->poison_damage > 0) {
				poison_damage_FROMPLAYER = sword->poison_damage;
				poison_duration_FROMPLAYER = sword->poison_duration;
				poison_timer_FROMPLAYER = sword->poison_timer;
				poison_tick_clock_FROMPLAYER = sword->poison_tick_clock;
				if (!isPoisoned) {
					isPoisoned = true;
					poison_timer_FROMPLAYER.restart();
				}
				else {
					poison_timer_FROMPLAYER.restart();
				}
			}
			health -= final_damage;
			ch_timer.restart();
		}
	}

	void updateDamageVisuals() {
		if (isHitted && ch_timer.getElapsedTime().asSeconds() > 0.2f) {
			sprite.setColor(Color::White);
			isHitted = false;
		}
	}

	void update_poison() {
		if (isPoisoned) {
			if (poison_timer_FROMPLAYER.getElapsedTime().asSeconds() < poison_duration_FROMPLAYER) {
				if (poison_tick_clock_FROMPLAYER.getElapsedTime().asSeconds() >= 1) {
					isHitted = true;
					health -= poison_damage_FROMPLAYER;
					sprite.setColor(Color::Red);
					ch_timer.restart();
					poison_tick_clock_FROMPLAYER.restart();
				}
			}
		}
	}

	void update(float dt, const sf::Vector2f& playerPos, MainPlayer& player, const std::vector<sf::RectangleShape>& walls, Sword* sword,  float maxHealth) {
		if (is_watching_front || is_watching_back) {
			health_bar.setPosition(sprite.getPosition().x, sprite.getPosition().y - 17.f);
			health_back_bar.setPosition(sprite.getPosition().x, sprite.getPosition().y - 17.f);
		}
		else if (is_watching_left || is_watching_right) {
			health_bar.setPosition(sprite.getPosition().x - 23.f, sprite.getPosition().y - 17.f);
			health_back_bar.setPosition(sprite.getPosition().x - 23.f, sprite.getPosition().y - 17.f);
		}
		health_text.setPosition(health_back_bar.getPosition().x + 75.f, health_back_bar.getPosition().y - 3.f);
		health_text.setString(std::to_string(health));
		float currentHP = health;
		float width = (currentHP / maxHealth) * maxWidth;
		health_bar.setSize(sf::Vector2f(width, 12.f));

		update_spawn_animation();
		update_animation(player);
		if(current_state == WALKING)
		update_moving(dt, player, walls);
		update_attack2(player);
		update_poison();
		updateDamageVisuals();
		if (light_strikeActive)
			hit_with_lightning(player);
		if (fireball != nullptr) {
			fireball->update(dt, 300.f, *this, player);
		}
		update_slowing();
	}

	void setBackLeftTexture() {
		sprite.setTexture(back_left);
		sprite.setTextureRect(IntRect(0, 0, 163, 172));
	}

	void setBackRightTexture() {
		sprite.setTexture(back_right);
		sprite.setTextureRect(IntRect(0, 0, 163, 172));
	}

	void setFrontLeftTexture() {
		sprite.setTexture(front_left);
		sprite.setTextureRect(IntRect(0, 0, 163, 172));
	}

	void setFrontRightTexture() {
		sprite.setTexture(front_right);
		sprite.setTextureRect(IntRect(0, 0, 163, 172));
	}

	void setLeft_LeftTexture() {
		sprite.setTexture(left_left_leg);
		sprite.setTextureRect(IntRect(0, 0, 120, 172));
	}

	void setLeft_RightTexture() {
		sprite.setTexture(left_right_leg);
		sprite.setTextureRect(IntRect(0, 0, 120, 172));
	}

	void setRight_RightTexture() {
		sprite.setTexture(right_right_leg);
		sprite.setTextureRect(IntRect(0, 0, 117, 172));
	}

	void setRight_LeftTexture() {
		sprite.setTexture(right_left_leg);
		sprite.setTextureRect(IntRect(0, 0, 117, 172));
	}

};

class Stone_Golem {
private :
	// POISONING FROM PLAYER
	int poison_damage_FROMPLAYER = 0;
	int poison_duration_FROMPLAYER = 0;
	Clock poison_timer_FROMPLAYER;
	Clock poison_tick_clock_FROMPLAYER; // one tick
	bool isPoisoned = false;
	// POISONING FROM PLAYER
public :
	Sprite sprite;
	Texture front, back, left, right;

	Clock ch_timer;
	bool isHitted = false; // ударил ли его?

	// health
	sf::RectangleShape health_bar;
	sf::RectangleShape health_back_bar;
	sf::Text health_text;
	float maxWidth = 200.f;
	int health;
	// health

	sf::Texture slash_texture_right;
	sf::Texture slash_texture_left;
	sf::Texture slash_texture_back;
	sf::Texture slash_texture_front;
	sf::Clock lifetimeClock;
	float duration = 0.13f; // эффект длится 0.18 секунды
	bool active = false;
	bool isKilled = false;

	sf::Vector2f position;
	float speed; // скорость движения

	sf::Clock attackCooldownClock;
	float attackDelay;

	Sprite slash_sprite;
	int damage;

	sf::RectangleShape frontandback;
	sf::RectangleShape leftandright;

	bool is_watching_front = false;
	bool is_watching_back = false;
	bool is_watching_right = false;
	bool is_watching_left = false;
	bool isMoving = false;

	Stone_Golem(int damage, int health, float speed, float attackDelay) : damage(damage), health(health), speed(speed), attackDelay(attackDelay) {};

	void update(float dt, const sf::Vector2f& playerPos, MainPlayer& player, const std::vector<sf::RectangleShape>& walls, Sword* sword, float maxHealth) {
		health_bar.setPosition(sprite.getPosition().x + 8.f, sprite.getPosition().y - 17.f);
		health_back_bar.setPosition(sprite.getPosition().x + 8.f, sprite.getPosition().y - 17.f);
		health_text.setPosition(sprite.getPosition().x + 110.f, health_bar.getPosition().y - 2.f);
		health_text.setString(std::to_string(health));

		//float maxWidth = 200.f; // ширина при полном здоровье
		float currentHP = health;
		float width = (currentHP / maxHealth) * maxWidth;
		health_bar.setSize(sf::Vector2f(width, 12.f));

		update_animation(player);
		update_attack_rects();
		updateDamageVisuals();
		update_poison();
		update_moving(dt, playerPos, player, walls);
		slash_update();
	}

	void update_poison() {
		if (isPoisoned) {
			if (poison_timer_FROMPLAYER.getElapsedTime().asSeconds() < poison_duration_FROMPLAYER) {
				if (poison_tick_clock_FROMPLAYER.getElapsedTime().asSeconds() >= 1) {
					isHitted = true;
					health -= poison_damage_FROMPLAYER;
					sprite.setColor(Color::Red);
					ch_timer.restart();
					poison_tick_clock_FROMPLAYER.restart();
				}
			}
		}
	}

	void update_animation(MainPlayer& player) {
		if (isMoving) {
			sf::Vector2f direction = player.sprite.getPosition() - sprite.getPosition();
			float angle = std::atan2(direction.y, direction.x);
			// углы направления в радианах
			if (angle >= -0.75f * M_PI && angle <= -0.25f * M_PI) {
				// вверх
				is_watching_front = true;
				is_watching_back = is_watching_left = is_watching_right = false;
				setBack();
				sprite.setTextureRect(sf::IntRect(0, 0, 190, 153));
			}
			else if (angle >= 0.25f * M_PI && angle <= 0.75f * M_PI) {
				// вниз
				is_watching_back = true;
				is_watching_front = is_watching_left = is_watching_right = false;
				setFront();
				sprite.setTextureRect(sf::IntRect(0, 0, 190, 150));
			}
			else if (angle <= -0.75f * M_PI || angle >= 0.75f * M_PI) {
				// влево
				is_watching_left = true;
				is_watching_back = is_watching_front = is_watching_right = false;
				setLeft();
				sprite.setTextureRect(sf::IntRect(0, 0, 120, 160));
			}
			else {
				// вправо
				is_watching_right = true;
				is_watching_back = is_watching_front = is_watching_left = false;
				setRight();
				sprite.setTextureRect(sf::IntRect(0, 0, 106, 150));
			}
		}
	}

	void update_moving(float dt, const sf::Vector2f& playerPos, MainPlayer& player, const std::vector<sf::RectangleShape>& walls) {
		isMoving = true;

		float dx = playerPos.x - position.x;
		float dy = playerPos.y - position.y;

		sf::Vector2f bestMove = position;
		float minDist = FLT_MAX;

		for (dx = -1; dx <= 1; ++dx) {
			for (dy = -1; dy <= 1; ++dy) {
				if (dx == 0 && dy == 0) continue;

				sf::Vector2f testPos = position + sf::Vector2f(dx, dy) * speed * dt;
				sprite.setPosition(testPos);

				bool collide = false;
				for (const auto& wall : walls) {
					if (sprite.getGlobalBounds().intersects(wall.getGlobalBounds())) {
						collide = true;
						break;
					}
				}

				if (!collide) {
					float dist = distance(testPos, playerPos);
					if (dist < minDist) {
						minDist = dist;
						bestMove = testPos;
					}
				}
			}
		}
		position = bestMove;
		sprite.setPosition(bestMove);
	}

	void slash_update() {
		if (active && lifetimeClock.getElapsedTime().asSeconds() > duration)
			active = false;
	}

	void attack(MainPlayer& player) {
		if (attackCooldownClock.getElapsedTime().asSeconds() < attackDelay)	return;
		
		if (is_watching_back)
			slash_sprite.setPosition(sprite.getPosition().x + 14.f, sprite.getPosition().y + 46.f);
		if (is_watching_front)
			slash_sprite.setPosition(sprite.getPosition().x - 3.f, sprite.getPosition().y - 52.f);
		if (is_watching_left)
			slash_sprite.setPosition(sprite.getPosition().x - 40.f, sprite.getPosition().y - 8.f);
		if (is_watching_right)
			slash_sprite.setPosition(sprite.getPosition().x + 13.f, sprite.getPosition().y);

		if (is_watching_back && frontandback.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
			slash_sprite.setTexture(slash_texture_front);
			active = true;
			isMoving = false;
			lifetimeClock.restart();
		}
		else if (is_watching_front && frontandback.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
			slash_sprite.setTexture(slash_texture_back);
			active = true;
			isMoving = false;
			lifetimeClock.restart();
		}
		else if (is_watching_left && leftandright.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
			slash_sprite.setTexture(slash_texture_left);
			active = true;
			isMoving = false;
			lifetimeClock.restart();
		}
		else if (is_watching_right && leftandright.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
			slash_sprite.setTexture(slash_texture_right);
			active = true;
			isMoving = false;
			lifetimeClock.restart();
		}
		else {
			isMoving = true;
		}

		if (slash_sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
			int final_damage = damage;
			if (player.armor_slot.armor != nullptr) {
				if (player.armor_slot.armor->armor > 0) {
					final_damage = damage * (50 / (50 + player.armor_slot.armor->armor));
				}
				if (player.armor_slot.armor->evasion > 0) {
					int random_chance = rand() % 100;
					if (random_chance < player.armor_slot.armor->evasion) {
						final_damage = 0;
					}
				}
			}

			for (auto& effect : player.active_effects) {
				if (auto* shield = dynamic_cast<Shield*>(effect.get())) {
					final_damage -= (final_damage * shield->dmg_reduction / 100.f);
				}
			} 

			for (auto& effect : player.active_effects) {
				if (auto* reflect = dynamic_cast<Reflect_damage*>(effect.get())) {
					isHitted = true;
					int own_damage = damage * reflect->reflection_percentage;
					health -= own_damage;
					sprite.setColor(sf::Color::Red);
					ch_timer.restart();
				}
			}

			if (final_damage < 0) {
				final_damage = 0;
			}

			player.current_health -= final_damage;
			attackCooldownClock.restart();
		}
	}

	void update_attack_rects() {
		if (is_watching_back) {
			frontandback.setPosition(sprite.getPosition().x + 8.f, sprite.getPosition().y + 102.f);
			frontandback.setSize({ 190.f, 63.f });
			leftandright.setSize({ 0.f, 0.f });
		}
		else if (is_watching_front) {
			frontandback.setPosition(sprite.getPosition().x, sprite.getPosition().y - 60.f);
			frontandback.setSize({ 190.f, 63.f });
			leftandright.setSize({ 0.f, 0.f });
		}
		else if (is_watching_left) {
			leftandright.setPosition(sprite.getPosition().x - 8.f, sprite.getPosition().y - 7);
			frontandback.setSize({ 0.f, 0.f });
			leftandright.setSize({ 55.f, 132.f});
		}
		else if (is_watching_right) {
			leftandright.setPosition(sprite.getPosition().x + 83.f, sprite.getPosition().y + 9.f);
			frontandback.setSize({ 0.f, 0.f });
			leftandright.setSize({ 55.f, 132.f });
		}
		else {
			frontandback.setPosition(10000.f, 10000.f );
			leftandright.setPosition(10000.f, 10000.f);
			leftandright.setSize({ 0.f, 0.f });
			frontandback.setSize({ 0.f, 0.f });
		}
	}

	bool loadTexture(Font &font) {
		if (!back.loadFromFile("textures/chapone/evil/golem/golem_back1.png") || 
			!front.loadFromFile("textures/chapone/evil/golem/golem_front1.png") ||
			!left.loadFromFile("textures/chapone/evil/golem/golem_left1.png") ||
			!right.loadFromFile("textures/chapone/evil/golem/golem_right1.png") ||
			!slash_texture_right.loadFromFile("textures/chapone/evil/golem/hit_right.png") ||
			!slash_texture_left.loadFromFile("textures/chapone/evil/golem/hit_left.png") ||
			!slash_texture_back.loadFromFile("textures/chapone/evil/golem/hit_back.png") ||
			!slash_texture_front.loadFromFile("textures/chapone/evil/golem/hit_front.png")) {
			return false;
		}
		sprite.setColor(Color::White);
		health_bar.setFillColor(sf::Color(218, 24, 24));
		health_back_bar.setFillColor(sf::Color(37, 40, 89));
		health_text.setFont(font);
		health_text.setFillColor(sf::Color::White);
		health_text.setCharacterSize(14);
		frontandback.setSize(sf::Vector2f(190.f, 63.f));
		leftandright.setSize(sf::Vector2f(55.f, 132.f));
		health_back_bar.setSize(sf::Vector2f(200.f, 12.f));
		frontandback.setFillColor(sf::Color(0, 0, 0, 0));
		leftandright.setFillColor(sf::Color(0, 0, 0, 0));
		left.setSmooth(false);
		right.setSmooth(false);
		back.setSmooth(false);
		front.setSmooth(false);
		return true;
	}

	void SetHealth(int health) {
		this->health = health;
	}

	void get_damage(SwordSlashEffect& swordEffect, Sword* sword, MainPlayer &player) {
		if (swordEffect.sprite.getGlobalBounds().intersects(sprite.getGlobalBounds())) {
			isHitted = true;
			sprite.setColor(Color::Red);
			int final_damage = sword->damage;
			if (sword->crit_chance > 0 && sword->crit_multiplier > 1) {
				int roll = rand() % 100;
				if (roll < sword->crit_chance) {
					final_damage = sword->damage * sword->crit_multiplier;
					swordEffect.active_crits.emplace_back(swordEffect.crit_texture, player, swordEffect);
				} 
			}
			if (sword->poison_damage > 0) {
				poison_damage_FROMPLAYER = sword->poison_damage;
				poison_duration_FROMPLAYER = sword->poison_duration;
				poison_timer_FROMPLAYER = sword->poison_timer;
				poison_tick_clock_FROMPLAYER = sword->poison_tick_clock;
				if (!isPoisoned) {
					isPoisoned = true;
					poison_timer_FROMPLAYER.restart();
				}
				else {
					poison_timer_FROMPLAYER.restart();
				}
			}
			if (health >= 37) {
				health -= final_damage / 2 - 1;
			}
			else {
				health -= final_damage;
			}
			ch_timer.restart();
		}
	}

	void updateDamageVisuals() {
		if (isHitted && ch_timer.getElapsedTime().asSeconds() > 0.2f) {
			sprite.setColor(Color::White);
			isHitted = false;
		}
	}

	void setLeft() {
		sprite.setTexture(left);
	}

	void setRight() {
		sprite.setTexture(right);
	}

	void setFront() {
		sprite.setTexture(front);
	}

	void setBack() {
		sprite.setTexture(back);
	}

};

class Skeleton { // класс скелетов
private:
	// POISONING FROM PLAYER
	int poison_damage_FROMPLAYER = 0;
	int poison_duration_FROMPLAYER = 0;
	Clock poison_timer_FROMPLAYER;
	Clock poison_tick_clock_FROMPLAYER; // one tick
	// POISONING FROM PLAYER

	// SLOWING FROM PLAYER
	int slowing_duration_FROMPLAYER = 0; // s
	bool isPoisoned = false;
	// SLOWING FROM PLAYER
public:
	Sprite sprite;
	Texture front_right, front_left, back_right, back_left, left_left_leg, left_right_leg, right_left_leg, right_right_leg;

	Clock ch_timer;

	// health
	sf::RectangleShape health_bar;
	sf::RectangleShape health_back_bar;
	sf::Text health_text;
	float maxWidth = 100.f;
	// health

	sf::Texture slash_texture_right;
	sf::Texture slash_texture_left;
	sf::Texture slash_texture_back;
	sf::Texture slash_texture_front;
	sf::Clock lifetimeClock;
	float duration = 0.09f; // эффект длится 0.18 секунды
	bool active = false;

	sf::Clock attackCooldownClock;
	float attackDelay;
	Sprite slash_sprite;
	bool foundMove = false;

	sf::RectangleShape frontandback;
	sf::RectangleShape leftandright;

	sf::Clock animClock;
	float switchTime = 0.25f;

	bool right_leg = true;
	int health = 0;
	bool is_watching_front = false;
	bool is_watching_back = true;
	bool is_watching_right = false;
	bool is_watching_left = false;
	bool isMoving = false;
	bool isHitted = false;
	bool isKilled = false;

	sf::Vector2f position;
	float speed; // скорость движения
	float normal_speed;

	// slowing
	Slowing_trail trail;
	// slowing

	int damage;

	Skeleton(int dmg, int health, float speed, float attackDelay, const Vector2f& pos) {
		damage = dmg;
		this->health = health;
		this->speed = speed;
		normal_speed = speed;
		this->attackDelay = attackDelay;
		position = pos;
		sprite.setPosition(position);
	}

	void update_slowing() {
		if (!trail.isActive) return;

		float frame_duration = float(slowing_duration_FROMPLAYER) / 24;

		if (is_watching_back || is_watching_right) {
			trail.sprite.setTexture(trail.slowing_trail_left);
			if (is_watching_back) {
				if (right_leg) {
					trail.sprite.setPosition(sprite.getPosition().x - 88.f, sprite.getPosition().y + 28.f);
				}
				else {
					trail.sprite.setPosition(sprite.getPosition().x - 91.f, sprite.getPosition().y + 29.f);
				}
			}
			else {
				if (right_leg) {
					trail.sprite.setPosition(sprite.getPosition().x - 97.f, sprite.getPosition().y + 28.f);
				}
				else {
					trail.sprite.setPosition(sprite.getPosition().x - 100.f, sprite.getPosition().y + 30.f);
				}

			}
		}
		else if (is_watching_front || is_watching_left) {
			trail.sprite.setTexture(trail.slowing_trail_right);
			if (is_watching_front) {
				if (right_leg) {
					trail.sprite.setPosition(sprite.getPosition().x + 47.f, sprite.getPosition().y + 26.f);
				}
				else {
					trail.sprite.setPosition(sprite.getPosition().x + 45.f, sprite.getPosition().y + 22.f);
				}
			}
			else {
				if (right_leg) {
					trail.sprite.setPosition(sprite.getPosition().x + 41.f, sprite.getPosition().y + 25.f);
				}
				else {
					trail.sprite.setPosition(sprite.getPosition().x + 41.f, sprite.getPosition().y + 31.f);
				}
			}
		}

		if (trail.slowing_clock.getElapsedTime().asSeconds() < slowing_duration_FROMPLAYER) {
			if (trail.animation_clock.getElapsedTime().asSeconds() > frame_duration) {
				trail.sprite.setTextureRect(IntRect(trail.current_slowing_frame * trail.slowing_frameWidth, 0, trail.slowing_frameWidth, trail.slowing_frameHeight));
				trail.current_slowing_frame++;
				if (trail.current_slowing_frame >= trail.max_slowing_frames) {
					trail.current_slowing_frame = 0;
					trail.isActive = false;
					speed = normal_speed;
				}
				trail.animation_clock.restart();
			}
		}
		else {
			trail.isActive = false;
			speed = normal_speed;
		}

	}

	void update_animation(MainPlayer& player) {
		if (isMoving) {
			if (animClock.getElapsedTime().asSeconds() > switchTime) {
				right_leg = !right_leg;
				animClock.restart();

				sf::Vector2f direction = player.sprite.getPosition() - sprite.getPosition();
				float angle = std::atan2(direction.y, direction.x);

				// Углы направления в радианах
				if (angle >= -0.75f * M_PI && angle <= -0.25f * M_PI) {
					// вверх
					is_watching_front = true;
					is_watching_back = is_watching_left = is_watching_right = false;
					if (right_leg) setBackRightTexture();
					else setBackLeftTexture();
				}
				else if (angle >= 0.25f * M_PI && angle <= 0.75f * M_PI) {
					// вниз
					is_watching_back = true;
					is_watching_front = is_watching_left = is_watching_right = false;
					if (right_leg) setFrontRightTexture();
					else setFrontLeftTexture();
				}
				else if (angle <= -0.75f * M_PI || angle >= 0.75f * M_PI) {
					// влево
					is_watching_left = true;
					is_watching_back = is_watching_front = is_watching_right = false;
					if (right_leg) setLeft_LeftTexture();
					else setLeft_RightTexture();
				}
				else {
					// вправо
					is_watching_right = true;
					is_watching_back = is_watching_front = is_watching_left = false;
					if (right_leg) setRight_RightTexture();
					else setRight_LeftTexture();
				}
			}
		}
	}

	void update_moving(float dt, const sf::Vector2f& playerPos, MainPlayer& player, const std::vector<sf::RectangleShape>& walls) {
		isMoving = true;

		sf::Vector2f dir = playerPos - position;
		float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

		if (length != 0)
			dir /= length; // нормализация вектора

		sf::Vector2f bestMove = position;
		float minDist = FLT_MAX;
		bool foundMove = false;

		for (int x = -1; x <= 1; ++x) {
			for (int y = -1; y <= 1; ++y) {
				if (x == 0 && y == 0) continue;

				// направление для проверки
				sf::Vector2f stepDir = sf::Vector2f((float)x, (float)y);
				float stepLen = std::sqrt(stepDir.x * stepDir.x + stepDir.y * stepDir.y);
				if (stepLen != 0)
					stepDir /= stepLen; // нормализация

				sf::Vector2f testPos = position + stepDir * speed * dt;
				sprite.setPosition(testPos);

				bool collide = false;
				for (const auto& wall : walls) {
					if (sprite.getGlobalBounds().intersects(wall.getGlobalBounds())) {
						collide = true;
						break;
					}
				}

				if (!collide) {
					float dist = distance(testPos, playerPos);
					if (dist < minDist) {
						minDist = dist;
						bestMove = testPos;
						foundMove = true;
					}
				}
			}
		}


		if (foundMove) {
			position = bestMove;
		}

		sprite.setPosition(position);
	}

	void slash_update() {
		if (active && lifetimeClock.getElapsedTime().asSeconds() > duration)
			active = false;
	}

	void attack(MainPlayer& player) {
		if (attackCooldownClock.getElapsedTime().asSeconds() < attackDelay)
			return;

		if (is_watching_back)
			slash_sprite.setPosition(sprite.getPosition().x - 8.f, sprite.getPosition().y + 72.f);
		if (is_watching_front)
			slash_sprite.setPosition(sprite.getPosition().x - 2.f, sprite.getPosition().y - 34.f);
		if (is_watching_left)
			slash_sprite.setPosition(sprite.getPosition().x - 29.f, sprite.getPosition().y + 15.f);
		if (is_watching_right)
			slash_sprite.setPosition(sprite.getPosition().x + 35.f, sprite.getPosition().y + 15.f);

		if (is_watching_back && frontandback.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
			slash_sprite.setTexture(slash_texture_front);
			slash_sprite.setTextureRect(IntRect(0,0, 100, 70));
			active = true;
			isMoving = false;
			lifetimeClock.restart();
		}
		else if (is_watching_front && frontandback.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
			slash_sprite.setTexture(slash_texture_back);
			slash_sprite.setTextureRect(IntRect(0, 0, 100, 70));
			active = true;
			isMoving = false;
			lifetimeClock.restart();
		}
		else if (is_watching_left && leftandright.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
			slash_sprite.setTexture(slash_texture_left);
			slash_sprite.setTextureRect(IntRect(0, 0, 70, 100));
			active = true;
			isMoving = false;
			lifetimeClock.restart();
		}
		else if (is_watching_right && leftandright.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
			slash_sprite.setTexture(slash_texture_right);
			slash_sprite.setTextureRect(IntRect(0, 0, 77, 101));
			active = true;
			isMoving = false;
			lifetimeClock.restart();
		}
		else {
			isMoving = true;
		}

		if (slash_sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
			int final_damage = damage;
			if (player.armor_slot.armor != nullptr) {
				if (player.armor_slot.armor->armor > 0) {
					final_damage = damage * (50 / (50 + player.armor_slot.armor->armor));
				}		
				if (player.armor_slot.armor->evasion > 0) {
					int random_chance = rand() % 100;
					if (random_chance < player.armor_slot.armor->evasion) {
						final_damage = 0;
					}
				}
			}

			for (auto& effect : player.active_effects) {
				if (auto* shield = dynamic_cast<Shield*>(effect.get())) {
					final_damage -= (final_damage * shield->dmg_reduction / 100.f);
				}
			}
			
			for (auto& effect : player.active_effects) {
				if (auto* reflect = dynamic_cast<Reflect_damage*>(effect.get())) {
					isHitted = true;				
					int own_damage = damage * reflect->reflection_percentage;
					health -= own_damage;
					sprite.setColor(sf::Color::Red);
					ch_timer.restart();
				}
			}

			if (final_damage < 0) {
				final_damage = 0;
			}

			player.current_health -= final_damage;
			attackCooldownClock.restart();
		}

	}

	void update_attack_rects() {
		if (is_watching_back) {
			frontandback.setPosition(sprite.getPosition().x - 12.f, sprite.getPosition().y + 120.f);
			leftandright.setPosition(0.f, 0.f);
		}
		else if (is_watching_front) {
			frontandback.setPosition(sprite.getPosition().x - 12.f, sprite.getPosition().y - 60.f);
			leftandright.setPosition(0.f, 0.f);
		}
		else if (is_watching_left) {
			leftandright.setPosition(sprite.getPosition().x - 29.f, sprite.getPosition().y);
			frontandback.setPosition(0.f, 0.f);
		}
		else if (is_watching_right) {
			leftandright.setPosition(sprite.getPosition().x + 57.f, sprite.getPosition().y);
			frontandback.setPosition(0.f, 0.f);
		}
	}

	void update(float dt, const sf::Vector2f& playerPos, MainPlayer& player, const std::vector<sf::RectangleShape>& walls, Sword* sword, float maxHealth) {
		health_bar.setPosition(sprite.getPosition().x + 8.f, sprite.getPosition().y - 17.f);
		health_back_bar.setPosition(sprite.getPosition().x + 8.f, sprite.getPosition().y - 17.f);
		health_text.setPosition(sprite.getPosition().x + 50.f, health_bar.getPosition().y - 2.f);
		health_text.setString(std::to_string(health));

		float currentHP = health;
		float widthBar = float(currentHP / maxHealth) * maxWidth;
		health_bar.setSize(sf::Vector2f(widthBar, 12.f));

		update_animation(player);
		update_attack_rects();
		updateDamageVisuals();
		update_slowing();
		update_moving(dt, playerPos, player, walls);
		update_poison();
		slash_update();
	}

	void update_poison() {
		if (isPoisoned) {
			if (poison_timer_FROMPLAYER.getElapsedTime().asSeconds() < poison_duration_FROMPLAYER) {
				if (poison_tick_clock_FROMPLAYER.getElapsedTime().asSeconds() >= 1) {
					isHitted = true;
					health -= poison_damage_FROMPLAYER;
					sprite.setColor(Color::Red);
					ch_timer.restart();
					poison_tick_clock_FROMPLAYER.restart();
				}
			}
		}
	}

	bool loadTexture(const string& front_right, const string& front_left, const string& back_right, const string& back_left,
		const string& left_left_leg, const string& left_right_leg, const string& right_right_leg, const string& right_left_leg, Font &font) {
		if (!this->front_right.loadFromFile(front_right) ||
			!this->front_left.loadFromFile(front_left) ||
			!this->back_right.loadFromFile(back_right) ||
			!this->back_left.loadFromFile(back_left) ||
			!this->left_left_leg.loadFromFile(left_left_leg) ||
			!this->left_right_leg.loadFromFile(left_right_leg) ||
			!this->right_right_leg.loadFromFile(right_right_leg) ||
			!this->right_left_leg.loadFromFile(right_left_leg) ||
			
			!slash_texture_right.loadFromFile("textures/chapone/evil/skeleton/hit_right.png") ||
			!slash_texture_left.loadFromFile("textures/chapone/evil/skeleton/hit_left.png") ||
			!slash_texture_back.loadFromFile("textures/chapone/evil/skeleton/hit_back.png") ||
			!slash_texture_front.loadFromFile("textures/chapone/evil/skeleton/hit_front.png") || !trail.load()) {
			return false;
		}
		sprite.setColor(Color::White);
		health_bar.setFillColor(sf::Color(218, 24, 24));
		health_back_bar.setFillColor(sf::Color(37, 40, 89));
		health_back_bar.setSize(sf::Vector2f(maxWidth, 12.f));
		health_text.setFont(font);
		health_text.setFillColor(sf::Color::White);
		health_text.setCharacterSize(14);
		trail.sprite.setTextureRect(IntRect(0, 0, trail.slowing_frameWidth, trail.slowing_frameHeight));
		frontandback.setSize(sf::Vector2f(108.f, 63.f));
		leftandright.setSize(sf::Vector2f(55.f, 132.f));
		return true;
	}

	void SetHealth(int health) {
		this->health = health;
	}

	void get_damage(SwordSlashEffect& swordEffect, Sword* sword, MainPlayer& player) {
		if (swordEffect.sprite.getGlobalBounds().intersects(sprite.getGlobalBounds())) {
			isHitted = true;
			sprite.setColor(sf::Color::Red);
			int final_damage = sword->damage;
			if (sword->crit_chance > 0 && sword->crit_multiplier > 1) {
				int roll = rand() % 100;
				if (roll < sword->crit_chance) {
					final_damage = sword->damage * sword->crit_multiplier;
					swordEffect.active_crits.emplace_back(swordEffect.crit_texture, player, swordEffect);
				}
			}
			if (sword->slowing_percent > 0) {
				float buf_slowing_percent = speed * sword->slowing_percent / 100;
				speed -= buf_slowing_percent;
				slowing_duration_FROMPLAYER = sword->slowing_duration;
				trail.isActive = true;
				trail.slowing_clock.restart();
				trail.animation_clock.restart();
			}
			if (sword->poison_damage > 0) {
				poison_damage_FROMPLAYER = sword->poison_damage;
				poison_duration_FROMPLAYER = sword->poison_duration;
				poison_timer_FROMPLAYER = sword->poison_timer;
				poison_tick_clock_FROMPLAYER = sword->poison_tick_clock;
				if (!isPoisoned) {
					isPoisoned = true;
					poison_timer_FROMPLAYER.restart();
				}
				else {
					poison_timer_FROMPLAYER.restart();
				}
			}
			health -= final_damage;
			ch_timer.restart();
		}
	}

	void updateDamageVisuals() {
		if (isHitted && ch_timer.getElapsedTime().asSeconds() > 0.2f) {
			isHitted = false;
			sprite.setColor(Color::White);
		}
	}

	void setBackLeftTexture() {
		sprite.setTexture(back_left);
		sprite.setTextureRect(IntRect(0, 0, 76, 132));
	}

	void setBackRightTexture() {
		sprite.setTexture(back_right);
		sprite.setTextureRect(IntRect(0, 0, 76, 132));
	}

	void setFrontLeftTexture() {
		sprite.setTexture(front_left);
		sprite.setTextureRect(IntRect(0, 0, 86, 132));
	}

	void setFrontRightTexture() {
		sprite.setTexture(front_right);
		sprite.setTextureRect(IntRect(0, 0, 86, 132));
	}

	void setLeft_LeftTexture() {
		sprite.setTexture(left_left_leg);
		sprite.setTextureRect(IntRect(0, 0, 67, 132));
	}

	void setLeft_RightTexture() {
		sprite.setTexture(left_right_leg);
		sprite.setTextureRect(IntRect(0, 0, 67, 132));
	}

	void setRight_RightTexture() {
		sprite.setTexture(right_right_leg);
		sprite.setTextureRect(IntRect(0, 0, 66, 132));
	}

	void setRight_LeftTexture() {
		sprite.setTexture(right_left_leg);
		sprite.setTextureRect(IntRect(0, 0, 66, 132));
	}
};