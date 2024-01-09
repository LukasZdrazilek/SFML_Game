#include "Boss.h"

// Nacitani textur bosse a jeho utoku
void Boss::loadTextures()
{
	boss_right.loadFromFile("Textures/Boss/Boss_right.png");
	boss_left.loadFromFile("Textures/Boss/Boss_left.png");
	boss_jump_right.loadFromFile("Textures/Boss/Boss_jump_right.png");
	boss_jump_left.loadFromFile("Textures/Boss/Boss_jump_left.png");
	boss_attack1_right.loadFromFile("Textures/Boss/Boss_attack1_right.png");
	boss_attack1_left.loadFromFile("Textures/Boss/Boss_attack1_left.png");
	boss_attack1_charge_right.loadFromFile("Textures/Boss/Boss_attack1_charge_right.png");
	boss_attack1_charge_left.loadFromFile("Textures/Boss/Boss_attack1_charge_left.png");
	boss_attack2_wave_right.loadFromFile("Textures/Boss/Boss_attack2_wave_right.png");
	boss_attack2_wave_left.loadFromFile("Textures/Boss/Boss_attack2_wave_left.png");
	boss_downed.loadFromFile("Textures/Boss/Boss_downed.png");
	boss_death.loadFromFile("Textures/Boss/Boss_death.png");

	attackWave.setTexture(boss_attack2_wave_left);
	attackWave.setPosition({ 1750, -600 });
}

// Logika bosse
void Boss::handleBoss(Player& player, sf::RenderWindow& window, Interface& interface, sf::RectangleShape attackHitbox, float& deltaTime, float& multiplier, sf::Sound& playerHitSound)
{
	if (player.enterBossfight == true)
	{
		boss.setTexture(&boss_left);
		handlePlayerCollision(player, interface, window, playerHitSound);
		handlePlayerAttackCollision(player, interface, window, attackHitbox);
		handleTimedAttacks(player, interface, deltaTime, multiplier);
		handlePlayerWaveAttackCollision(player, interface, window, attackWave, playerHitSound);
	}	
}

// Korekce bossova hitboxu pro ruzne textury  !!
 sf::FloatRect Boss::getBounds()
{
	if (boss.getTexture() == &boss_left)				  return getGlobalBounds1();
	if (boss.getTexture() == &boss_right)				  return getGlobalBounds2();
	if (boss.getTexture() == &boss_attack1_left)		  return getGlobalBounds3();
}

// Kontrola kolize bosse a hrace
bool Boss::checkPlayerCollision(Player& player)
{
	return getBounds().intersects((player.getGlobalBounds()));
}

// Kontrola kolizi hracova utoku a bosse
bool Boss::checkPlayerAttackCollision(Player& player, sf::RectangleShape attackHitbox)
{
	return getBounds().intersects(attackHitbox.getGlobalBounds());
}

// Kontrola kolizi hrace a bossuv wave attack
bool Boss::checkPlayerWaveAttackCollision(Player& player, sf::Sprite attackWave)
{
	return attackWave.getGlobalBounds().intersects((player.getGlobalBounds()));
}

// Nepritel utoci na hrace, hrac zapne ochranu ( aka blikani po hitu akorat bez blikani ! ), zmeni se mu HP o 1 a zapne zvuk hitu 
void Boss::handlePlayerCollision(Player& player, Interface& interface, sf::RenderWindow& window, sf::Sound& playerHitSound)
{
	if (checkPlayerCollision(player) && isAlive(interface) == true && playerHitCooldown.getElapsedTime().asSeconds() > hit_cooldown)
	{
		playerHitCooldown.restart();
		interface.hitPoints--;
		playerHitSound.play();

		if (player.facingLeft == false)
			player.move({ -80, -20 });
		else
			player.move({ +80, -20 });
	}
}

// Pokud utok hrace zasahne nepritele a ten je nazivu, enemy se posune po smeru utoku, zmeni se mu HP o 1 a zapne se zvuk hitu a hraci se navysi bar healing pointu
void Boss::handlePlayerAttackCollision(Player& player, Interface& interface, sf::RenderWindow& window, sf::RectangleShape attackHitbox)
{
	if (player.isAttacking)
	{
		if (checkPlayerAttackCollision(player, attackHitbox) && isAlive(interface) == true)
		{
			takeDamage(1, interface);
			if (interface.healingPoints < interface.maxHealingPoints)
				interface.healingPoints += 1;
		}
	}
}

// Stejne jako kolize hrace a bosse, akorat zde je to kolize hrace a wave utoku bosse
void Boss::handlePlayerWaveAttackCollision(Player& player, Interface& interface, sf::RenderWindow& window, sf::Sprite attackWave, sf::Sound& playerHitSound)
{
	if (checkPlayerWaveAttackCollision(player, attackWave) && attackWaveDraw == true && isAlive(interface) == true && playerHitCooldown.getElapsedTime().asSeconds() > hit_cooldown)
	{
		playerHitCooldown.restart();
		interface.hitPoints--;
		playerHitSound.play();

		if (player.facingLeft == false)
			player.move({ -80, -20 });
		else
			player.move({ +80, -20 });
	}
}

// Logika bosse a jeho utoku
void Boss::handleTimedAttacks(Player& player, Interface& interface, float& deltaTime, float& multiplier)
{
	// Reseni timeru aby se nerestartoval kazdou sekundu ( funkce se nachazi v game loopu )
	if (player.enterBossfight == true)
	{
		if (enteredBossfight == false)
		{
			entranceTimer.restart();
			attacksTimer.restart();

			enteredBossfight = true;
		}	
	}

	// Korekce bossovy pozice kdyz nahodou spadne pod zem ( stava se z nejakeho duvodu )
	if (getY() > -746.0f)
	{
		boss.setPosition({ getX(), -746.0f });
	}


	// Prichod bosse do areny
	if (entranceTimer.getElapsedTime().asSeconds() >= 2)
	{
		if (entranceTimer.getElapsedTime().asSeconds() <= 2.3)
		{
			move({ 0, 24 * deltaTime * multiplier});
		}
	}

	// Pokud je boss nazivu
	if (interface.bossHitPoints > 0)
	{
		// Timer celeho setu utoku
		if (attacksTimer.getElapsedTime().asSeconds() < 33.2)
		{

			if (attacksTimer.getElapsedTime().asSeconds() > 4 && attacksTimer.getElapsedTime().asSeconds() <= 6)
			{
				boss.setTexture(&boss_attack1_charge_left);
			}

			if (attacksTimer.getElapsedTime().asSeconds() > 5.9 && attacksTimer.getElapsedTime().asSeconds() < 6)
			{
				move({ -70 * deltaTime * multiplier, 0 });
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 6 && attacksTimer.getElapsedTime().asSeconds() < 8)
			{
				boss.setTexture(&boss_attack1_left);
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 8 && attacksTimer.getElapsedTime().asSeconds() < 10)
			{
				boss.setTexture(&boss_right);
			}

			else if (attacksTimer.getElapsedTime().asSeconds() >= 10 && attacksTimer.getElapsedTime().asSeconds() <= 10.3)
			{
				move({ 5 * deltaTime * multiplier, -25 * deltaTime * multiplier });
			}

			else if (attacksTimer.getElapsedTime().asSeconds() >= 11.3 && attacksTimer.getElapsedTime().asSeconds() <= 11.6)
			{
				boss.setTexture(&boss_jump_right);
				move({ 21 * deltaTime * multiplier, 30 * deltaTime * multiplier });
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 12 && attacksTimer.getElapsedTime().asSeconds() < 13)
			{
				boss.setTexture(&boss_left);
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 13 && attacksTimer.getElapsedTime().asSeconds() < 15)
			{
				boss.setTexture(&boss_attack1_charge_left);
			}

			// Zacatek rychlych kombo utoku po sobe
			else if (attacksTimer.getElapsedTime().asSeconds() > 15 && attacksTimer.getElapsedTime().asSeconds() < 20)
			{
				if (attacksTimer.getElapsedTime().asSeconds() > 15 && attacksTimer.getElapsedTime().asSeconds() <= 15.1)
				{
					move({ -40 * deltaTime * multiplier, 0 });
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 15.1 && attacksTimer.getElapsedTime().asSeconds() <= 15.4)
				{
					boss.setTexture(&boss_attack1_left);
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 15.4 && attacksTimer.getElapsedTime().asSeconds() <= 15.7)
				{
					boss.setTexture(&boss_attack1_charge_right);
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 15.7 && attacksTimer.getElapsedTime().asSeconds() <= 15.8)
				{
					move({ 40 * deltaTime * multiplier, 0 });
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 15.8 && attacksTimer.getElapsedTime().asSeconds() <= 16.1)
				{
					boss.setTexture(&boss_attack1_right);
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 16.1 && attacksTimer.getElapsedTime().asSeconds() <= 16.4)
				{
					boss.setTexture(&boss_attack1_charge_left);
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 16.4 && attacksTimer.getElapsedTime().asSeconds() <= 16.5)
				{
					move({ -40 * deltaTime * multiplier, 0 });
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 16.5 && attacksTimer.getElapsedTime().asSeconds() <= 16.8)
				{
					boss.setTexture(&boss_attack1_left);
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 16.8 && attacksTimer.getElapsedTime().asSeconds() <= 17.1)
				{
					boss.setTexture(&boss_attack1_charge_right);
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 17.1 && attacksTimer.getElapsedTime().asSeconds() <= 17.2)
				{
					move({ 40 * deltaTime * multiplier, 0 });
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 17.2 && attacksTimer.getElapsedTime().asSeconds() <= 17.5)
				{
					boss.setTexture(&boss_attack1_right);
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 17.5 && attacksTimer.getElapsedTime().asSeconds() <= 17.8)
				{
					boss.setTexture(&boss_attack1_charge_left);
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 17.8 && attacksTimer.getElapsedTime().asSeconds() <= 17.9)
				{
					move({ -40 * deltaTime * multiplier, 0 });
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 17.9 && attacksTimer.getElapsedTime().asSeconds() <= 18.2)
				{
					boss.setTexture(&boss_attack1_left);
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 18.2 && attacksTimer.getElapsedTime().asSeconds() <= 18.5)
				{
					boss.setTexture(&boss_attack1_charge_right);
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 18.5 && attacksTimer.getElapsedTime().asSeconds() <= 18.6)
				{
					move({ 40 * deltaTime * multiplier, 0 });
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 18.6 && attacksTimer.getElapsedTime().asSeconds() <= 18.9)
				{
					boss.setTexture(&boss_attack1_right);
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 18.6 && attacksTimer.getElapsedTime().asSeconds() <= 18.9)
				{
					boss.setTexture(&boss_attack1_right);
				}

				else if (attacksTimer.getElapsedTime().asSeconds() > 18.9 && attacksTimer.getElapsedTime().asSeconds() <= 19.4)
				{
					boss.setTexture(&boss_attack1_charge_left);
				}
			}
			// Zacatek vlnoveho utoku
			else if (attacksTimer.getElapsedTime().asSeconds() > 19.4 && attacksTimer.getElapsedTime().asSeconds() <= 20.9)
			{
				boss.setTexture(&boss_attack1_left);
				attackWaveDraw = true;
				attackWave.move({ -15 * deltaTime * multiplier, 0 });
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 20.9 && attacksTimer.getElapsedTime().asSeconds() <= 21.4)
			{
				attackWaveDraw = false;
				attackWave.move({ 20 * deltaTime * multiplier, 0 });
				boss.setTexture(&boss_attack1_charge_left);
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 21.4 && attacksTimer.getElapsedTime().asSeconds() <= 22.9)
			{
				boss.setTexture(&boss_attack1_left);
				attackWaveDraw = true;
				attackWave.move({ -12 * deltaTime * multiplier, 0 });
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 22.9 && attacksTimer.getElapsedTime().asSeconds() <= 23.5)
			{
				attackWaveDraw = false;
				attackWave.move({ 28 * deltaTime * multiplier, 0 });
				boss.setTexture(&boss_attack1_charge_left);
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 23.5 && attacksTimer.getElapsedTime().asSeconds() <= 25)
			{
				boss.setTexture(&boss_attack1_left);
				attackWaveDraw = true;
				attackWave.move({ -12 * deltaTime * multiplier, 0 });
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 25 && attacksTimer.getElapsedTime().asSeconds() <= 27)
			{
				boss.setTexture(&boss_attack1_charge_left);
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 27 && attacksTimer.getElapsedTime().asSeconds() < 27.1)
			{
				move({ -70 * deltaTime * multiplier, 0 });
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 27.1 && attacksTimer.getElapsedTime().asSeconds() <= 29.1)
			{
				boss.setTexture(&boss_attack1_left);
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 29.1 && attacksTimer.getElapsedTime().asSeconds() <= 31.1)
			{
				boss.setTexture(&boss_attack1_charge_right);
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 31.1 && attacksTimer.getElapsedTime().asSeconds() < 31.2)
			{
				move({ 70 * deltaTime * multiplier, 0 });
			}

			else if (attacksTimer.getElapsedTime().asSeconds() > 31.2 && attacksTimer.getElapsedTime().asSeconds() <= 33.2)
			{
				boss.setTexture(&boss_attack1_right);
			}

		}
		// Restart cyklu, vraceni vlnoveho utoku na pocatecni pozici
		else
		{
			attacksTimer.restart();
			attackWave.setPosition({ 1750, -600 });
			attackWaveDraw = false;
		}
			
	}
	// Smrt bosse, zapne se timer pro zmenu textur
	else
	{
		if (startDeathTimer == false)
		{
			deathTimer.restart();
			startDeathTimer = true;
		}

		boss.setTexture(&boss_downed);

		if (deathTimer.getElapsedTime().asSeconds() > 9)
			boss.setTexture(&boss_death);
	}
}
