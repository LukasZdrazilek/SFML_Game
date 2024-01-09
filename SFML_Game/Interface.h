#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#ifndef INTERFACE_H
#define INTERFACE_H


class Interface
{
public:

	void handleUI(sf::RenderWindow& window, Player& player);

	void updatePositions(Player& player);

	void draw(sf::RenderWindow& window);

	void healing(Player& player);

	void handleNpc(Player& player);

	bool isPlayerAlive(Player& player);

	int hitPoints = 3;

	int bossHitPoints = 300;

	int maxHealingPoints = 100;

	int healingPoints = 20;

	void loadTextures();

	void playerReset(Player& player);

private:

	// Booly na check zmizeni textu tutorialu
	bool learnedAttacking = false;
	bool learnedHealing = false;
	bool learnedShifting = false;
	bool enteredBossfight = false;
	bool npcTextTimerStart = true;
	bool drawNpcText = false;
	bool bossDeathTextTimerStart = false;

	// Promenne pro healovani
	sf::Clock healingTimer;
	sf::Clock npcAnimationTimer;
	sf::Clock npcTextTimer;

	bool cKeyPressed = false;
	const sf::Time holdDuration = sf::seconds(1.0f);

	sf::Clock bossDeathTextTimer;

	// Textury HP a pozadi healing baru
	sf::Texture hitpointTexture;
	sf::Texture npcTexture1;
	sf::Texture npcTexture2;
	sf::Sprite hitPoint1;
	sf::Sprite hitPoint2;
	sf::Sprite hitPoint3;
	sf::Sprite npc;
	sf::RectangleShape healthBackground;
	sf::RectangleShape bossHealthBackground;

	// Font a texty
	sf::Font font;
	sf::Text healthBarText;
	sf::Text tutorialAttackText;
	sf::Text tutorialHealingText;
	sf::Text tutorialShiftjumpText;
	sf::Text bossHealthBarText;
	sf::Text bossNameText;
	sf::Text endingText;
	sf::Text npcText;
	sf::Text bossDeathText;

	// Vektory na HP a body v healing baru
	std::vector<sf::Sprite> HP;
	std::vector<char> healingBar;

};

#endif
