#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Interface.h"
#ifndef BOSS_H
#define BOSS_H

class Boss
{
public:

	Boss()
	{
		boss.setSize({ 350, 250 });
		boss.setPosition({ 1500, -1240 });
	}

	void loadTextures();

	void handleBoss(Player& player, sf::RenderWindow& window, Interface& interface, sf::RectangleShape attackHitbox, float& deltaTime, float& multiplier, sf::Sound& playerHitSound);

	void drawBoss(sf::RenderWindow& window, Interface& interface, Player& player)
	{
		window.draw(boss);
		// Kresleni textury wave utoku bosse, pokud je bool true
		if (isAlive(interface) && player.enterBossfight == true)
		{
			if (attackWaveDraw == true)
				window.draw(attackWave);
		}
	}

	// Jednoduche funkce se pro prehlednost nachazi v hlavickovem .h souboru

	void move(sf::Vector2f distance)
	{
		boss.move(distance);
	}

	void setPosition(sf::Vector2f newPos)
	{
		boss.setPosition(newPos);
	}

	float getY()
	{
		return boss.getPosition().y;
	}

	float getX()
	{
		return boss.getPosition().x;
	}

	sf::Vector2f getSize()
	{
		return boss.getSize();
	}

	// Fix kolize na boss_left texturu
	sf::FloatRect getGlobalBounds1()
	{
		sf::Vector2f position((float)getX() + 75, (float)getY());
		sf::Vector2f size(245, 250);
		return sf::FloatRect(position, size);
	}
	// Fix kolize na boss_right texturu
	sf::FloatRect getGlobalBounds2()
	{
		sf::Vector2f position((float)getX() + 25, (float)getY());
		sf::Vector2f size(245, 250);
		return sf::FloatRect(position, size);
	}

	// Fix kolize na boss_attack1_left texturu
	sf::FloatRect getGlobalBounds3()
	{
		sf::Vector2f position((float)getX() - 250, (float)getY() + 20);
		sf::Vector2f size(245, 250);
		return sf::FloatRect(position, size);
	}

	void takeDamage(int damage, Interface& interface)
	{
		interface.bossHitPoints -= damage;
	}

	bool isAlive(Interface& interface)
	{
		if (interface.bossHitPoints > 0)
			return true;
		else
			return false;
	}

	bool checkPlayerCollision(Player& player);

	bool checkPlayerAttackCollision(Player& player, sf::RectangleShape attackHitbox);

	bool checkPlayerWaveAttackCollision(Player& player, sf::Sprite attackWave);

	void handlePlayerCollision(Player& player, Interface& interface, sf::RenderWindow& window, sf::Sound& playerHitSound);

	void handlePlayerAttackCollision(Player& player, Interface& interface, sf::RenderWindow& window, sf::RectangleShape attackHitbox);

	void handleTimedAttacks(Player& player, Interface& interface, float& deltaTime, float& multiplier);

	void handlePlayerWaveAttackCollision(Player& player, Interface& interface, sf::RenderWindow& window, sf::Sprite attackWave, sf::Sound& playerHitSound);

	sf::FloatRect getBounds();

private:

	// Prvni a druhy bool na kontrolu hrace v boss areny a bool na kresleni textury vlnoveho utoku bosse
	bool enteredBossfight = false;
	bool entered = false;
	bool attackWaveDraw = false;
	bool startDeathTimer = false;

	// Sprite bosse
	sf::RectangleShape boss;

	// Hodiny a casovac na nesmrtelnost hrace na 1 sec po hitu nepritelem
	sf::Clock playerHitCooldown;
	float hit_cooldown = 1.5f;

	// Hodiny na casovani 'spadnuti' nepritele do areny a casovac utoku bosse
	sf::Clock entranceTimer;
	sf::Clock attacksTimer;
	sf::Clock deathTimer;

	// Textury bosse a jeho utoku
	sf::Texture boss_right;
	sf::Texture boss_left;
	sf::Texture boss_jump_right;
	sf::Texture boss_jump_left;
	sf::Texture boss_attack1_right;
	sf::Texture boss_attack1_left;
	sf::Texture boss_attack1_charge_right;
	sf::Texture boss_attack1_charge_left;
	sf::Texture boss_attack2_wave_right;
	sf::Texture boss_attack2_wave_left;
	sf::Texture boss_downed;
	sf::Texture boss_death;
	sf::Sprite attackWave;

};
#endif