#include "Enemy.h"

// Nacitani textur nepratel
void Enemies::loadTextures()
{
    
    crawlid1_right.loadFromFile("Textures/Crawlid/Crawlid1_right.png");
    crawlid1_left.loadFromFile("Textures/Crawlid/Crawlid1_left.png");
    crawlid2_right.loadFromFile("Textures/Crawlid/Crawlid2_right.png");
    crawlid2_left.loadFromFile("Textures/Crawlid/Crawlid2_left.png");

    vengefly1_right.loadFromFile("Textures/Vengefly/Vengefly1_right.png");
    vengefly1_left.loadFromFile("Textures/Vengefly/Vengefly1_left.png");
    vengefly2_right.loadFromFile("Textures/Vengefly/Vengefly2_right.png");
    vengefly2_left.loadFromFile("Textures/Vengefly/Vengefly2_left.png");
}

// Inicializace nepritele
// Typ nepritele: 0 = Crawlid ( pozemni ), 1 = Mosqit ( letajici )
Enemy::Enemy(sf::Vector2f size, sf::Vector2f position, int type)
{
    enemy.setSize(size);
    enemy.setPosition(position);
}

// Logika nepritele     ! - nepritele 2 beru podle jeho velikosti, coz v podstate jde a nemelo by to nikdy delat problem ale je to divny
void Enemy::handleEnemy(float deltaTime, Player& player, sf::RenderWindow& window, const sf::Texture& leftTexture1, const sf::Texture& rightTexture1, const sf::Texture& leftTexture2, 
    const sf::Texture& rightTexture2, const sf::Texture& leftTexture3, const sf::Texture& rightTexture3, const sf::Texture& leftTexture4, const sf::Texture& rightTexture4)
{
    // Nepritel zije, pouziti logiky
    if (isAlive() == true)
    {
        movementTimer += deltaTime;
        flyingTimer += deltaTime;

        // Zmena smeru pohybu nepritele
        if (movementTimer >= movementDuration)
        {
            movingLeft = !movingLeft;
            movementTimer = 0;
        }

        // Casovani pohybu letajiciho nepritele
        if (flyingTimer >= flyingDuration && enemy.getSize() == sf::Vector2f(40, 40))   // !
        {
            movingUp = !movingUp;
            flyingTimer = 0;

        }

        // Pohyb letajiciho nepritele nahoru
        if (movingUp && enemy.getSize() == sf::Vector2f(40, 40))
        {
            move({0, speed * deltaTime * multiplier});
        }

        // Pohyb letajiciho nepritele dolu
        else if (!movingUp && enemy.getSize() == sf::Vector2f(40, 40))
        {
            move({0, -speed * deltaTime * multiplier});
        }

        // Pohyb nepritele vlevo ( pozemniho + letajiciho )
        if (movingLeft)
        {
            move({ -speed * deltaTime * multiplier, 0 });
            if (enemy.getSize() == sf::Vector2f(40, 40)) // !
                enemy.setTexture(&leftTexture3);
            else
                enemy.setTexture(&leftTexture1);

            // Casovac na zmenu textury ( animace )
            if (runTimer.getElapsedTime().asSeconds() > run_animationTime)
            {
                if (enemy.getSize() == sf::Vector2f(40, 40)) // !
                {
                    if (enemy.getTexture() == &leftTexture3)
                        enemy.setTexture(&leftTexture4);
                    else
                        enemy.setTexture(&leftTexture3);
                    if (runTimer.getElapsedTime().asSeconds() > run_animationTime * 2)
                        runTimer.restart();
                }
                else
                {
                    if (enemy.getTexture() == &leftTexture1)
                        enemy.setTexture(&leftTexture2);
                    else
                        enemy.setTexture(&leftTexture1);
                    if (runTimer.getElapsedTime().asSeconds() > run_animationTime * 2)
                        runTimer.restart();
                }
            }
        }
        // Pohyb nepritele vpravo ( pozemniho + letajiciho )
        if (!movingLeft)
        {
            move({ speed * deltaTime * multiplier, 0 });
            if (enemy.getSize() == sf::Vector2f(40, 40)) // !
                enemy.setTexture(&rightTexture3);
            else
                enemy.setTexture(&rightTexture1);
            if (runTimer.getElapsedTime().asSeconds() > run_animationTime)
            {
                if (enemy.getSize() == sf::Vector2f(40, 40)) // !
                {
                    if (enemy.getTexture() == &rightTexture3)
                        enemy.setTexture(&rightTexture4);
                    else
                        enemy.setTexture(&rightTexture3);
                    if (runTimer.getElapsedTime().asSeconds() > run_animationTime * 2)
                        runTimer.restart();
                }
                else
                {
                    if (enemy.getTexture() == &rightTexture1)
                        enemy.setTexture(&rightTexture2);
                    else
                        enemy.setTexture(&rightTexture1);
                    if (runTimer.getElapsedTime().asSeconds() > run_animationTime * 2)
                        runTimer.restart();
                }
            }
        }

        // Letajici nepritel detekuje hrace pod nim
        if (player.getY() <= getY() - 50 && enemy.getSize() == sf::Vector2f(40, 40))    // !
        {
            movingUp = false;
        }
        // Letajici nepritel detekuje hrace nad nim
        if (player.getY() >= getY() + 50 && enemy.getSize() == sf::Vector2f(40, 40))    // !
        {
            movingUp = true;
        }
        // Detekce hrace vlevo ( hrac je blize jak 300 pixelu )
        if (player.getX() >= getX() - 300 && player.getX() <= getX() && player.getY() <= getY())
        {
            movingLeft = true;
        }
        // Detekce hrace vpravo ( hrac je blize jak 300 pixelu  )
        if (player.getX() <= getX() + 300 && player.getX() >= getX() && player.getY() <= getY())
        {
            movingLeft = false;
        }      
    }
}

// Kontrola kolize hrace a nepritele
bool Enemy::checkPlayerCollision(Player& player)
{
    return getGlobalBounds().intersects(player.getGlobalBounds());
}

// Kontrola utoku hrace na nepritele
bool Enemy::checkPlayerAttackCollision(Player& player, sf::RectangleShape attackHitbox)
{
    return getGlobalBounds().intersects(attackHitbox.getGlobalBounds());
}

// Logika utoku nepritele na hrace
void Enemy::handlePlayerCollision(Player& player, Interface& interface, sf::RenderWindow& window, sf::Sound& playerHitSound)
{
    // Nepritel utoci na hrace, hrac zapne ochranu ( aka blikani po hitu akorat bez blikani ! ), zmeni se mu HP o 1 a zapne zvuk hitu 
    if (checkPlayerCollision(player) && isAlive() == true && playerHitCooldown.getElapsedTime().asSeconds() > hit_cooldown)
    {
        playerHitCooldown.restart();
        interface.hitPoints--;
        playerHitSound.play();

        // hrac se posune po smeru hitu a trochu nahoru
        if (player.facingLeft == false)
            player.move({ -80, -20 });
        else
            player.move({ +80, -20 });
    }
}

// Logika utoku hrace na nepritele
void Enemy::handlePlayerAttackCollision(Player& player, Interface& interface, sf::RenderWindow& window, sf::RectangleShape attackHitbox, sf::Sound& enemyHitSound)
{
    if (player.isAttacking)
    {   
        // Pokud utok hrace zasahne nepritele a ten je nazivu, enemy se posune po smeru utoku, zmeni se mu HP o 1, zapne se zvuk hitu a hraci se navysi bar healing pointu
        if (checkPlayerAttackCollision(player, attackHitbox) && isAlive() == true)
        {
            if (player.facingLeft == false)
                enemy.move({ 60, 0 });
            else
                enemy.move({ -60, 0 });
            takeDamage(1);
            enemyHitSound.play();
            if (interface.healingPoints < interface.maxHealingPoints)
                interface.healingPoints += 10;
        }
    }
}

// Zde se vytvari nepratele
Enemies::Enemies()
{
    enemies.push_back(Enemy({ 70, 40 }, { 900, 860 }, 0));
    enemies.push_back(Enemy({ 70, 40 }, { 1000, 160 }, 0));
    enemies.push_back(Enemy({ 70, 40 }, { 900, 160 }, 0));
    enemies.push_back(Enemy({ 40, 40 }, { 1200, 750 }, 1));
    enemies.push_back(Enemy({ 40, 40 }, { 2200, 750 }, 1));
    enemies.push_back(Enemy({ 40, 40 }, { 2280, 770 }, 1));
}

// Funkce na nepratele
void Enemies::handleEnemies(float deltaTime, Player& player, sf::RenderWindow& window, Interface& interface, sf::RectangleShape attackHitbox, sf::Sound& enemyHitSound, sf::Sound& playerHitSound)
{
    for (auto& enemy : enemies)
    {
        enemy.handleEnemy(deltaTime, player, window, crawlid1_left, crawlid1_right, crawlid2_left, crawlid2_right, vengefly1_left, vengefly1_right, vengefly2_left, vengefly2_right);
        enemy.handlePlayerCollision(player, interface, window, playerHitSound);
        enemy.handlePlayerAttackCollision(player, interface, window, attackHitbox, enemyHitSound);
    }
}

// Kresleni enemy
void Enemies::drawEnemies(sf::RenderWindow& window)
{
    for (auto& enemy : enemies)
    {
        if (enemy.isAlive() == true)
            enemy.drawEnemy(window);
    }
}

// Reset nepratel pokud hrac zemre mimo boss arenu
void Enemies::resetEnemies()
{
    for (auto& enemy : enemies)
    {
        enemy.hitPoints = 3;
    }
}
