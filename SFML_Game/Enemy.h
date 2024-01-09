#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Player.h"
#include "Interface.h"
#pragma once
#ifndef ENEMY_H
#define ENEMY_H


class Enemy
{
public:

	Enemy(sf::Vector2f size, sf::Vector2f position, int type);

	void handleEnemy(float deltaTime, Player& player, sf::RenderWindow& window, const sf::Texture& leftTexture1, const sf::Texture& rightTexture1, const sf::Texture& leftTexture2,
		const sf::Texture& rightTexture2, const sf::Texture& leftTexture3, const sf::Texture& rightTexture3, const sf::Texture& leftTexture4, const sf::Texture& rightTexture4);

	// Jednoduche funkce se pro prehlednost nachazi v hlavickovem .h souboru

	void drawEnemy(sf::RenderWindow& window)
	{
		window.draw(enemy);
	}

	void move(sf::Vector2f distance)
	{
		enemy.move(distance);
	}

	void setPosition(sf::Vector2f newPos)
	{
		enemy.setPosition(newPos);
	}

	float getY()
	{
		return enemy.getPosition().y;
	}

	float getX()
	{
		return enemy.getPosition().x;
	}

	sf::Vector2f getPosition()
	{
		return enemy.getPosition();
	}

	sf::Vector2f getSize()
	{
		return enemy.getSize();
	}

	int getHitPoints() 
	{
		return hitPoints;
	}

	void takeDamage(int damage) 
	{
		hitPoints -= damage;
	}

	bool isAlive() 
	{
		if (hitPoints > 0)
			return true;
		else
			return false;
	}


	bool checkPlayerCollision(Player& player);

	bool checkPlayerAttackCollision(Player& player, sf::RectangleShape attackHitbox);

	void handlePlayerCollision(Player& player, Interface& interface, sf::RenderWindow& window, sf::Sound& playerHitSound);

	void handlePlayerAttackCollision(Player& player, Interface& interface, sf::RenderWindow& window, sf::RectangleShape attackHitbox, sf::Sound& enemyHitSound);

	// Funkce na kolize nepritele
	sf::FloatRect getGlobalBounds()
	{
		sf::Vector2f position = enemy.getPosition();
		sf::Vector2f size = enemy.getSize();
		return sf::FloatRect(position, size);
	}

	// Enemy HP
	int hitPoints = 3;

	// Casovace pro pohyb nepratel a booly na pohyb vlevo/vpravo nahoru/dolu
	float movementTimer = 0;      
	float movementDuration = 2.0f;
	float flyingTimer = 0;
	float flyingDuration = 1.0f;
	bool movingLeft = true;
	bool movingUp = true;

private:

	float speed = 2.0f;

	float multiplier = 60.0f;

	// Sprite nepritele
	sf::RectangleShape enemy;
 
	// Casovac animace pohybu a jeji rychlost
	sf::Clock runTimer;
	float run_animationTime = 0.3f;

	// Nesmrtelnost hrace na 1s po hitu nepritelem
	sf::Clock playerHitCooldown;
	float hit_cooldown = 1.5f;

	// Nastaveni zvuku - zatim nefunguje
	sf::SoundBuffer hitSoundBuffer;
	sf::Sound hitSound;
};

// Class Enemies pro lepsi manipulaci se vsemi neprateli
class Enemies
{
public:
	Enemies();

	void loadTextures();

	void handleEnemies(float deltaTime, Player& player, sf::RenderWindow& window, Interface& interface, sf::RectangleShape attackHitbox, sf::Sound& enemyHitSound, sf::Sound& playerHitSound);

	void drawEnemies(sf::RenderWindow& window);

	void resetEnemies();

private:

	// Textury nepratel
	sf::Texture crawlid1_right;
	sf::Texture crawlid1_left;
	sf::Texture crawlid2_right;
	sf::Texture crawlid2_left;
	sf::Texture vengefly1_right;
	sf::Texture vengefly1_left;
	sf::Texture vengefly2_right;
	sf::Texture vengefly2_left;

	// Vektor vsech nepratel
	std::vector<Enemy> enemies;
};
#endif
