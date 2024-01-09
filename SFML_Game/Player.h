#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:

	// Inicializace hrace
	Player()
	{
		player.setSize({ 60, 70 });
		attackHitbox.setSize(sf::Vector2f(playerWidth, playerHeight));
		player.setPosition({ 100, 400 });
	}

	// Kresleni hrace
	void drawPlayer(sf::RenderWindow& window)
	{
		window.draw(player);
		window.draw(attackEffect);
		if (isAttacking)
			window.draw(attackHitbox);
	}

	// Pouziti logiky hrace
	void handlePlayer(float deltaTime, float multiplier, sf::Sound& attackSound);

	// Jednoduche funkce se pro prehlednost nachazi v hlavickovem .h souboru

	void move(sf::Vector2f distance)
	{
		player.move(distance);
	}

	void setPosition(sf::Vector2f newPos)
	{
		player.setPosition(newPos);
	}

	float getY()
	{
		return player.getPosition().y;
	}

	float getX()
	{
		return player.getPosition().x;
	}

	sf::Vector2f getPosition()
	{
		return player.getPosition();
	}

	sf::Vector2f getSize()
	{
		return player.getSize();
	}

	sf::FloatRect getGlobalBounds()
	{
		sf::Vector2f position = player.getPosition();
		sf::Vector2f size = player.getSize();
		return sf::FloatRect(position, size);
	}

	void loadTextures();

	// Booly a nastaveni velikosti spritu hrace
	bool facingLeft = false;
	float playerHeight = 70;
	float playerWidth = 60;
	bool wasAttackPressed = false;
	bool isAttacking = false;

	float moveSpeed = 5;
	float floorHeight = 900;
	float gravitySpeed = 9;
	bool moving_left = false;
	bool moving_right = false;

	float jumpSpeed = 7;
	int jumpsLeft = 1;
	bool isJumping = false;

	bool enterBossfight = false;

	// Casovac skoku, animace behu, jump boostu, 2x casovac proti 'spamu' utoku a cooldown utoku
	sf::Clock jumpTimer;
	sf::Clock runTimer;
	sf::Clock jumpBoostTimer;
	sf::Clock attackAnimationTimer;
	sf::Clock attackAnimationTime;
	sf::Clock attackCooldown;

	// Sprite attack hitboxu
	sf::RectangleShape attackHitbox;

private:

	// Sprite hrace a jeho attack effectu
	sf::RectangleShape player;
	sf::RectangleShape attackEffect;

	// Casovac animace behu a bool attack animace
	float run_animationTime = 0.3f;
	bool attackAnimation = false;

	// Textury hrace a jeho utoku, leceni
	sf::Texture playerTexture_right;
	sf::Texture playerTexture_left;
	sf::Texture playerAttack_right;
	sf::Texture playerAttack_left;
	sf::Texture playerFall_right;
	sf::Texture playerFall_left;
	sf::Texture playerJump_left;
	sf::Texture playerJump_right;
	sf::Texture playerRun1_left;
	sf::Texture playerRun1_right;
	sf::Texture playerRun2_left;
	sf::Texture playerRun2_right;
	sf::Texture playerAttackEffect_right;
	sf::Texture playerAttackEffect_left;
	sf::Texture playerHealing;

};

#endif
