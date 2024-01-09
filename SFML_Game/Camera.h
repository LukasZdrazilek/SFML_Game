#include <SFML/Graphics.hpp>
#include "Player.h"
#ifndef CAMERA_H
#define CAMERA_H
#pragma once

// Real time pohyb kamery podle pozice hrace
void handleCamera(sf::View& camera, Player& player, sf::RenderWindow& window)
{
    camera.setSize(sf::Vector2f(800, 450));
    camera.setCenter(player.getPosition().x + player.playerWidth / 2, player.getPosition().y + player.playerHeight / 2 - 20);
    window.setView(camera);

    // Zmena kamery po vstupu do boss areny
    if (player.getY() <= -500 && player.getX() >= 1000 )  // x a y podle areny
    {
        camera.setSize(sf::Vector2f(950, 550));
        camera.setCenter(player.getPosition().x + player.playerWidth / 2, -650);
        window.setView(camera);
        if (player.getX() >= 1200)
            player.enterBossfight = true;
    }
}

#endif