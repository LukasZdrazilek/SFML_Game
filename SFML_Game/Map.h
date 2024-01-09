#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"
#pragma once
#ifndef MAP_H
#define MAP_H

class Map 
{
public:
    Map();

    void draw(sf::RenderWindow& window, Player& player, float deltaTime, float multiplier);

    void drawPlatform(sf::RenderWindow& window, sf::RectangleShape& platform);
    
    sf::RectangleShape setPlatform(float width, float height, float x, float y);

    sf::RectangleShape setFloor(float width, float height, float x, float y);

    void loadTextures();

    void handleCollisions(Player& player);

private:

    // Bool kdyz hrac vejde do boss areny a delky zemi jednotlivych podlazi
    bool enteredBossfight = false;
    float floor1X = 2500;
    float floor2X = 1900;
    float floor3X = 1400;

    // Vysky ( pozice y ) jednotlivych podlazi
    float floor1Height = 900;
    float floor2Height = 200;
    float floor3Height = -500;

    // Casovac na pohyb mrizovych dveri
    sf::Clock doorTimer;

    // Textury jednotlivych zemi a mrizovych dveri
    sf::Texture floorTexture1;
    sf::Texture floorTexture2;    
    sf::Texture floorTexture3;
    sf::Texture floorTexture4;
    sf::Texture bossCageTexture;
    sf::Texture bossCageWallTexture;

    // Sprity jednotlivych zemi a obou mrizovych dveri
    sf::Sprite stoneFloor1;
    sf::Sprite stoneFloor2;
    sf::Sprite greenFloor;
    sf::Sprite bossFloor;
    sf::RectangleShape cageWall1;
    sf::RectangleShape cageWall2;
    sf::RectangleShape cageDoor1;
    sf::RectangleShape cageDoor2;

    // Textury 4 pozadi a platform
    sf::Texture backgroundTexture1;
    sf::Texture backgroundTexture2;
    sf::Texture backgroundTexture3;
    sf::Texture backgroundTexture4;
    sf::Texture bossArenaTexture;
    sf::Texture platformTexture;

    // Sprity 4 pozadi
    sf::Sprite background1;
    sf::Sprite background2;
    sf::Sprite background3;
    sf::Sprite background4;
    sf::Sprite bossArena;

    // Vektory pozadi a obou casti platform
    std::vector<sf::RectangleShape> floors;
    std::vector<sf::RectangleShape> platformsTop;
    std::vector<sf::RectangleShape> platformsBot;

};

#endif