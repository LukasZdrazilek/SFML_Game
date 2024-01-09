#include "Interface.h"

// Nacitani textur a textu + fontu
void Interface::loadTextures()
{
	hitpointTexture.loadFromFile("Textures/HP.png");
	npcTexture1.loadFromFile("Textures/npc2.png");
	npcTexture2.loadFromFile("Textures/npc1.png");

	font.loadFromFile("font.ttf");

	healthBarText.setFont(font);
	healthBarText.setCharacterSize(16);
	healthBarText.setLetterSpacing(-1);
	healthBarText.setFillColor(sf::Color::White);

	bossHealthBarText.setFont(font);
	bossHealthBarText.setCharacterSize(16);
	bossHealthBarText.setLetterSpacing(-1);
	bossHealthBarText.setFillColor(sf::Color::White);

	tutorialAttackText.setFont(font);
	tutorialAttackText.setCharacterSize(12);
	tutorialAttackText.setFillColor(sf::Color::White);
	tutorialAttackText.setString("Press X to attack");

	tutorialHealingText.setFont(font);
	tutorialHealingText.setCharacterSize(12);
	tutorialHealingText.setFillColor(sf::Color::White);
	tutorialHealingText.setString("Hold C to heal 1 HP if bar is half \n 2 HP if the bar is full");

	tutorialShiftjumpText.setFont(font);
	tutorialShiftjumpText.setCharacterSize(12);
	tutorialShiftjumpText.setFillColor(sf::Color::White);
	tutorialShiftjumpText.setString("Hold shift while jumping \n to jump extra distance");

	healthBackground.setFillColor(sf::Color::Black);
	healthBackground.setSize({ 134, 13 });

	bossHealthBackground.setFillColor(sf::Color::Black);
	bossHealthBackground.setSize({ 400, 15 });

	bossNameText.setFont(font);
	bossNameText.setCharacterSize(16);
	bossNameText.setFillColor(sf::Color::White);
	bossNameText.setString("Perjurer Knight");

	bossDeathText.setFont(font);
	bossDeathText.setCharacterSize(16);
	bossDeathText.setFillColor(sf::Color::White);
	bossDeathText.setString("My lord... I have failed you yet again");

	endingText.setFont(font);
	endingText.setCharacterSize(64);
	endingText.setFillColor(sf::Color::Yellow);
	endingText.setString("			 THE END \n Thank You for playing \n     press ESC to quit");

	npcText.setFont(font);
	npcText.setCharacterSize(13);
	npcText.setFillColor(sf::Color::White);

}

// Kresleni jednotlivych objektu
void Interface::draw(sf::RenderWindow& window)
{
	window.draw(healthBackground);
	window.draw(healthBarText);
	window.draw(npc);

	// Kresleni NPC textu pokud je hrac dostatecne blizko
	if (drawNpcText == true)
		window.draw(npcText);

	// Kresleni bosse pokud je hrac v arene a boss zije
	if (enteredBossfight == true && bossHitPoints > 0)
	{
		window.draw(bossHealthBackground);
		window.draw(bossHealthBarText);
		window.draw(bossNameText);
	}

	// Tutorial utoku, zde nejakym zpusobem zatezuje mene pameti nez v samotne funkci
	if (learnedAttacking == false)
	{
		tutorialAttackText.setPosition({ 700, 800 });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		learnedAttacking = true;
	if (learnedAttacking == false)
		window.draw(tutorialAttackText);

	// Tutorial leceni
	if (learnedHealing == false)
	{
		tutorialHealingText.setPosition({ 1300, 800 });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		learnedHealing = true;
	if (learnedHealing == false)
		window.draw(tutorialHealingText);

	// Tutorial shift jump boost
	if (learnedShifting == false)
	{
		tutorialShiftjumpText.setPosition({ 200, 100 });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		learnedShifting = true;
	if (learnedShifting == false)
		window.draw(tutorialShiftjumpText);

	// Vykresleni textur HP podle zivotu hrace
	if (hitPoints == 3)
	{
		window.draw(hitPoint1);
		window.draw(hitPoint2);
		window.draw(hitPoint3);
	}
	else if (hitPoints == 2)
	{
		window.draw(hitPoint1);
		window.draw(hitPoint2);
	}
	else
		window.draw(hitPoint1);

	// Kresleni textu umirajiciho bosse a ending textu dle casu
	if (bossHitPoints <= 0)
	{
		if (bossDeathTextTimerStart == false)
		{
			bossDeathTextTimer.restart();
			bossDeathTextTimerStart = true;
		}

		if (bossDeathTextTimer.getElapsedTime().asSeconds() < 9)
			window.draw(bossDeathText);
		else if (bossDeathTextTimer.getElapsedTime().asSeconds() > 13)
			window.draw(endingText);
	}
}

// Abilita leceni
void Interface::healing(Player& player) 
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) 
	{
        if (!cKeyPressed) 
		{
            healingTimer.restart();
            cKeyPressed = true;
        }

		// Leceni po 2 sekundach drzeni 'C'
        if (healingTimer.getElapsedTime() >= holdDuration) 
		{
            if (hitPoints < 3 && healingPoints >= 50 && !player.isJumping && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
            {
            hitPoints += 1;
			healingPoints -= 50;
			}
        }
    } 
	else 
		cKeyPressed = false;
    
}

// Real time update interface dle pozice hrace
void Interface::updatePositions(Player& player)
{
	// Logika vykreslovani baru na leceni
	std::string health;
	for (int i = 0; i < healingPoints; i++)
	{
		health.append("|");
	}
	healthBarText.setString(health);

	// Logika vykreslovani HP bosse
	std::string bossHealth;
	for (int j = 0; j < bossHitPoints; j++)
	{
		bossHealth.append("|");
	}
	bossHealthBarText.setString(bossHealth);

	// Korekce pozice interfacu po zmene kamery v boss arene
	if (player.enterBossfight == true)
	{
		enteredBossfight = true;
		hitPoint1.setPosition({ (float)player.getX() - 390, -850 });
		hitPoint2.setPosition({ (float)player.getX() - 340, -850 });
		hitPoint3.setPosition({ (float)player.getX() - 290, -850 });

		healthBarText.setPosition({ (float)player.getX() - 388, -800 });
		healthBackground.setPosition((float)player.getX() - 387, (float)player.getY() - 805);

		bossHealthBarText.setPosition({ (float)player.getX() - 170, -465 });
		bossHealthBackground.setPosition({ (float)player.getX() - 170, -462 });
		bossNameText.setPosition({ (float)player.getX() - 170, -480 });
	}

	// Neni v boss arene = klasicke pozice
	else
	{
		hitPoint1.setPosition({ (float)player.getX() - 340, (float)player.getY() - 160 });
		hitPoint2.setPosition({ (float)player.getX() - 290, (float)player.getY() - 160 });
		hitPoint3.setPosition({ (float)player.getX() - 240, (float)player.getY() - 160 });

		healthBarText.setPosition({ (float)player.getX() - 338, (float)player.getY() - 115 });

		healthBackground.setPosition((float)player.getX() - 337, (float)player.getY() - 110);
	}

	// Nastaveni pozic pro boss a ending text
	if (bossHitPoints <= 0)
	{
		bossDeathText.setPosition({ 1450, -450});
		endingText.setPosition({ 1150, -900 });
	}
}

// Funkce ktera vraci jestli je hrac nazivu
bool Interface::isPlayerAlive(Player& player)
{
	if (hitPoints == 0)
		return false;
	else
		return true;
}

// Logika NPC, nastaveni pozice, textury a animace
void Interface::handleNpc(Player& player)
{
	npc.setPosition({ 2450, 840 });
	npc.setTexture(npcTexture1);

	if (npcAnimationTimer.getElapsedTime().asSeconds() > 1)
	{
		npc.setTexture(npcTexture2);
		if (npcAnimationTimer.getElapsedTime().asSeconds() > 2)
		{
			npc.setTexture(npcTexture1);
			npcAnimationTimer.restart();
		}
	}

	// Nastaveni pozice NPC textu
	npcText.setPosition({ 2400, 780 });

	// ! Pokud je hrac blizko NPC, kresli se text dle casu
	if (player.getX() >= 2300)
	{
		drawNpcText = true;
		npcText.setString("Thank you for saving me\nfrom those pesky beasts");
		if (npcTextTimerStart == true)
		{
			npcTextTimer.restart();
			npcTextTimerStart = false;
		}
		if (npcTextTimer.getElapsedTime().asSeconds() > 6)
			npcText.setString("   I got cornered, there\nwas no other way but down");
		if (npcTextTimer.getElapsedTime().asSeconds() > 13)
			npcText.setString("I was so scared... it's\nthat nasty teeth of theirs");
		if (npcTextTimer.getElapsedTime().asSeconds() > 19)
			npcText.setString("  It's dangerous here\nif you're heading up, beware...");
		if (npcTextTimer.getElapsedTime().asSeconds() > 26)
		{
			npcText.setStyle(sf::Text::Bold);
			npcText.setString("Beware he who once served");
		}
		if (npcTextTimer.getElapsedTime().asSeconds() > 29)
		{
			npcText.setStyle(sf::Text::Regular);
			npcText.setString("			...");
		}
		if (npcTextTimer.getElapsedTime().asSeconds() > 36)
		{
			npcText.setString("  Phew, that nasty little teeth");
		}

	}

	// Hrac odejde, text zmizi
	else
		drawNpcText = false;
}

// Logika celeho interface
void Interface::handleUI(sf::RenderWindow& window, Player& player)
{
	// Push textur HP do vektoru
	HP.push_back(hitPoint1);
	HP.push_back(hitPoint2);
	HP.push_back(hitPoint3);

	// Nastaveni textur HP
	hitPoint1.setTexture(hitpointTexture);
	hitPoint3.setTexture(hitpointTexture);
	hitPoint2.setTexture(hitpointTexture);

	// Pouziti jednotlivych logik interface
	handleNpc(player);
	updatePositions(player);
	healing(player);
}

// Reset hrace na zacatek hry
void Interface::playerReset(Player& player)
{
	hitPoints = 3;
	healingPoints = 20;
	player.setPosition({ 100, 400 });
}