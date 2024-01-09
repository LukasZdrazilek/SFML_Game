#include "Player.h"

// Nacitani textur hrace
void Player::loadTextures()
{
    playerTexture_right.loadFromFile("Textures/Hornet/Hornet_right.png");
    playerTexture_left.loadFromFile("Textures/Hornet/Hornet_left.png");
    playerAttack_right.loadFromFile("Textures/Hornet/Hornet_attack1_right.png");
    playerAttack_left.loadFromFile("Textures/Hornet/Hornet_attack1_left.png");
    playerFall_right.loadFromFile("Textures/Hornet/Hornet_fall_right.png");
    playerFall_left.loadFromFile("Textures/Hornet/Hornet_fall_left.png");
    playerJump_right.loadFromFile("Textures/Hornet/Hornet_jump_right.png");
    playerJump_left.loadFromFile("Textures/Hornet/Hornet_jump_left.png");
    playerRun1_right.loadFromFile("Textures/Hornet/Hornet_run1_right.png");
    playerRun1_left.loadFromFile("Textures/Hornet/Hornet_run1_left.png");
    playerAttackEffect_right.loadFromFile("Textures/Hornet/Attack_right.png");
    playerAttackEffect_left.loadFromFile("Textures/Hornet/Attack_left.png");
    playerRun2_right.loadFromFile("Textures/Hornet/Hornet_run2_right.png");
    playerRun2_left.loadFromFile("Textures/Hornet/Hornet_run2_left.png");  
    playerHealing.loadFromFile("Textures/Hornet/Hornet_healing.png");
}

// Logika hrace
void Player::handlePlayer(float deltaTime, float multiplier, sf::Sound& attackSound) 
{
    // Idle textury hrace kdyz se nehybe
    if (facingLeft == false)
        player.setTexture(&playerTexture_right);
    else
        player.setTexture(&playerTexture_left);
       

    // Utok na nepritele
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !isAttacking && attackCooldown.getElapsedTime().asMilliseconds() > 600) 
    {
        isAttacking = true;
        attackCooldown.restart();
        attackAnimationTime.restart();
        attackSound.play();

        // Textury spritu utoku podle pozice hrace
        if (!facingLeft) 
            attackHitbox.setPosition(player.getPosition().x + (player.getGlobalBounds().width), player.getPosition().y);

        else 
            attackHitbox.setPosition(player.getPosition().x - (player.getGlobalBounds().width), player.getPosition().y);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) == false)
    {
        isAttacking = false;
    }
    
    // Zakaz spamovani utoku
    if (isAttacking && attackCooldown.getElapsedTime().asMilliseconds() > 200) 
        isAttacking = false;

    // Aktivace textur utoku a textur utociciho hrace
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)  && attackAnimationTime.getElapsedTime().asMilliseconds() < 200)
    {
        if (facingLeft == false )
        {
            attackHitbox.setTexture(&playerAttackEffect_right);
            player.setTexture(&playerAttack_right);
        }
        else
        {
            attackHitbox.setTexture(&playerAttackEffect_left);
            player.setTexture(&playerAttack_left);
        }      
    }

    // Jump boost powerup - Drzte skok pri drzeni LShiftu pro vetsi skok 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && isJumping && (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
    {
        jumpBoostTimer.restart();
        if (jumpBoostTimer.getElapsedTime().asSeconds() < 1.0)
        {
            if (facingLeft == false)
                player.move({ 4 * deltaTime * multiplier, 0 });
            else
                player.move({ -4 * deltaTime * multiplier, 0 });
        }
    }

    // Pohyb vlevo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
    {
        move({ -moveSpeed * deltaTime * multiplier, 0 });
        facingLeft = true;

        // Nastaveni textur utociciho hrace pri pohybu vlevo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && attackAnimationTime.getElapsedTime().asMilliseconds() < 200)
        {

            if (facingLeft == false) 
                player.setTexture(&playerAttack_right);
            else
                player.setTexture(&playerAttack_left);
        }
        else
        {   // Animace behani vlevo ( zmena dvou textur po case )
            player.setTexture(&playerRun1_left);
            if (runTimer.getElapsedTime().asSeconds() > run_animationTime)
            {
                if (player.getTexture() == &playerRun1_left)
                    player.setTexture(&playerRun2_left);
                else
                    player.setTexture(&playerRun1_left);

                if (runTimer.getElapsedTime().asSeconds() > run_animationTime * 2)
                    runTimer.restart();
                

            }
        } 
    }

    // Pohyb vpravo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
    {
        move({ +moveSpeed * deltaTime * multiplier, 0 });
        facingLeft = false;

        // Nastaveni textur utociciho hrace pri pohybu vpravo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && attackAnimationTime.getElapsedTime().asMilliseconds() < 200)
        {
            if (facingLeft == false)
                player.setTexture(&playerAttack_right);
            else
                player.setTexture(&playerAttack_left);
        }
        else
        {   // Animace behani vpravo ( zmena dvou textur po case )
            player.setTexture(&playerRun1_right);
            if (runTimer.getElapsedTime().asSeconds() > run_animationTime)
            {
                if (player.getTexture() == &playerRun1_right)
                    player.setTexture(&playerRun2_right);
                else
                    player.setTexture(&playerRun1_right);

                if (runTimer.getElapsedTime().asSeconds() > run_animationTime * 2)
                    runTimer.restart();
            }
        }        
    }

    // Aktivace textur healovani
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && !isJumping && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
    {
        player.setTexture(&playerHealing);
    }

    // Gravitace - pokud je hrac vys nez zeme + nedrzi mezernik     !!
    if (getY() + playerHeight < floorHeight && isJumping == false)
    {
        player.move({ 0, gravitySpeed * deltaTime * multiplier });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        {
            if (facingLeft == false)
                player.setTexture(&playerAttack_right);
            else
                player.setTexture(&playerAttack_left);
        }
        else
        {
            if (facingLeft == false)
                player.setTexture(&playerFall_right);
            else
                player.setTexture(&playerFall_left);
        }
    }

    // funkce pro "zapnuti" skoku, zapne se casovac a zakaze skok znovu
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && jumpsLeft > 0)
    {
        if (isJumping == false)   
        {
            isJumping = true;
            jumpTimer.restart();
            jumpsLeft = 0; 
        }
    }

    // Funkce na skok
    if (isJumping)
    {
        float jumpTime = jumpTimer.getElapsedTime().asSeconds();
        if (jumpTime < 0.4)        // Max vyska skoku
        {
            player.move({ 0, -jumpSpeed * deltaTime * multiplier });
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
            {
                if (facingLeft == false)
                    player.setTexture(&playerAttack_right);
                else
                    player.setTexture(&playerAttack_left);
            }
            else
            {
                if (facingLeft == false)
                    player.setTexture(&playerJump_right);
                else
                    player.setTexture(&playerJump_left);
            }
        }
        else
        {
            isJumping = false;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) == false)
    {
        isJumping = false;
    }

    // Bounds mapy zleva
    if (getX() <= 0)
        setPosition({ 0, (float)getY() });

    // Bounds mapy zprava
    if (getX() >= (2500 - playerWidth))
        setPosition({ 2500 - (playerWidth), (float)getY()});
}
