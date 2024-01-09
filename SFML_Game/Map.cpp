#include "Map.h"

// Nacitani textur mapy
void Map::loadTextures()
{
    backgroundTexture1.loadFromFile("Textures/Map/background1.png");
    backgroundTexture2.loadFromFile("Textures/Map/background2.png");
    backgroundTexture3.loadFromFile("Textures/Map/background3.png");
    backgroundTexture4.loadFromFile("Textures/Map/background4.png");
    floorTexture1.loadFromFile("Textures/Map/stoneFloor.png");
    floorTexture2.loadFromFile("Textures/Map/stoneFloorSmall.png");
    floorTexture3.loadFromFile("Textures/Map/greenFloor.png");
    floorTexture4.loadFromFile("Textures/Map/stoneBossFloor.png");
    platformTexture.loadFromFile("Textures/Map/platformTexture.png");
    bossCageTexture.loadFromFile("Textures/Map/cageDoor.png");
    bossArenaTexture.loadFromFile("Textures/Map/bossBackground.png");
    bossCageWallTexture.loadFromFile("Textures/Map/cageWall.png");

    // Nastaveni textur jednotlivym spritum pozadi
    background1.setTexture(backgroundTexture1);
    background2.setTexture(backgroundTexture2);
    background3.setTexture(backgroundTexture3);
    background4.setTexture(backgroundTexture4);
    stoneFloor1.setTexture(floorTexture1);
    stoneFloor2.setTexture(floorTexture2);
    greenFloor.setTexture(floorTexture3);
    bossFloor.setTexture(floorTexture4);
    cageDoor1.setTexture(&bossCageTexture);
    cageDoor2.setTexture(&bossCageTexture);
    bossArena.setTexture(bossArenaTexture);

    // Nastaveni pozice obou klecovych dveri
    cageDoor1.setSize({ 10, 320 });
    cageDoor1.setPosition({ 1050, -1000 });   

    cageDoor2.setSize({ 10, 320 });
    cageDoor2.setPosition({ 1950, -820 });

    cageWall1.setSize({ 100, 200 });
    cageWall2.setSize({ 100, 200 });
    cageWall1.setPosition({ 1000, -1000 });
    cageWall2.setPosition({ 1900, -1000 });
    cageWall1.setTexture(&bossCageWallTexture);
    cageWall2.setTexture(&bossCageWallTexture);
}

Map::Map()
{
    // Zde se vytvareji logicke zeme
    sf::RectangleShape floor1 = setFloor(floor1X, 10, 0, floor1Height);
    sf::RectangleShape floor2 = setFloor(floor2X, 10, 0, floor2Height);
    sf::RectangleShape floor3 = setFloor(floor3X, 10, 600, floor3Height);


    // Zde se vytvareji platformy
    sf::RectangleShape platform11 = setPlatform(100, 10, 1900, 720);
    sf::RectangleShape platform12 = setPlatform(100, 10, 2000, 550); 
    sf::RectangleShape platform13 = setPlatform(100, 10, 2000, 370);
    sf::RectangleShape platform21 = setPlatform(100, 10, 1300, 180);
    sf::RectangleShape platform22 = setPlatform(100, 10, 150, -400);
    sf::RectangleShape platform23 = setPlatform(100, 10, 0, -40);
    sf::RectangleShape platform24 = setPlatform(100, 10, 400, 70);
    sf::RectangleShape platform25 = setPlatform(100, 10, 400, -210);
    

    // Nastaveni zvetseni pozadi ( pouze pro estetiku )
    background1.setScale(1.0f, 1.5f);
}

// Setup logicke zeme ( ne jeji textury )
sf::RectangleShape Map::setFloor(float width, float height, float x, float y)
{
    sf::RectangleShape floor(sf::Vector2f(width, height));
    floor.setPosition(x, y);
    floors.push_back(floor);

    return floor;
}

// Setup platformy vrchni + spodni a push back do vektoru vrchnich nebo spodnich platforem
sf::RectangleShape Map::setPlatform(float width, float height, float x, float y)
{
    sf::RectangleShape platformA(sf::Vector2f(width, height));
    platformA.setPosition(x, y);
    sf::RectangleShape platformB(sf::Vector2f(width, height));
    platformB.setPosition(x, y + 10);

    platformsTop.push_back(platformA);
    platformsBot.push_back(platformB);

    return platformA;
}

// Funkce kolizi
void Map::handleCollisions(Player& player)
{
    // Pohyb klecovych dveri
    if (enteredBossfight == false && player.enterBossfight == true && player.getX() >= 1200)
    {
        doorTimer.restart();
        enteredBossfight = true;
    }

    if (enteredBossfight == true && player.getX() >= 1200 && doorTimer.getElapsedTime().asSeconds() < 1.0f)
    {
        float movementSpeed = 3.0f;
        float distanceToMove = movementSpeed;

        cageDoor1.move({ 0, distanceToMove });
    }

    // Kolize klecovych dveri a hrace
    if (player.getGlobalBounds().intersects(cageDoor1.getGlobalBounds()))
    {
        player.setPosition({ cageDoor1.getGlobalBounds().left + 10, player.getY()});
    }

    if (player.getGlobalBounds().intersects(cageDoor2.getGlobalBounds()))
    {
        player.setPosition({ cageDoor2.getGlobalBounds().left - player.playerWidth, player.getY() });
    }

    // Kolize zeme a hrace
    for (sf::RectangleShape floor : floors)
    {
        if (player.getGlobalBounds().intersects(floor.getGlobalBounds()))
        {
            float overlap = player.getGlobalBounds().top + player.getGlobalBounds().height - floor.getGlobalBounds().top;
            if (overlap < player.playerHeight)
            {
                player.setPosition({ player.getPosition().x, floor.getGlobalBounds().top - player.playerHeight });
                player.isJumping = false;
                player.jumpsLeft = 1;
            }
            player.floorHeight = floor.getGlobalBounds().top;
        }
        // Zmena pozice logicke zeme z pohledu hrace, kvuli zmene gravitaci na jednotlivych podlazi
        else
        {
            player.floorHeight = floor1Height;

            if (player.getY() <= floor2Height && player.getX() <= floor2X)
                player.floorHeight = floor2Height;

            if (player.getY() <= floor3Height && player.getX() >= 540)
                player.floorHeight = floor3Height;
        }    
    }
    
    // Kolize platformy a hrace zvrchu platformy ( muze na ni skocit )
    for (sf::RectangleShape& platform : platformsTop)
    {
        if (player.getGlobalBounds().intersects(platform.getGlobalBounds()))
        {
            player.floorHeight = platform.getGlobalBounds().top;
            float overlap = player.getGlobalBounds().top + player.getGlobalBounds().height - platform.getGlobalBounds().top;

            if (overlap < player.playerHeight)
            {
                player.setPosition({ player.getPosition().x, platform.getGlobalBounds().top - player.playerHeight + 1 });
                player.isJumping = false;
                player.jumpsLeft = 1;
            }
        }
    }

    // Kolize platformy a hrace zespodu platformy ( nemuze na ni skocit, po kolizi zacne padat )
    for (sf::RectangleShape& platform : platformsBot)
    {
        if (player.getGlobalBounds().intersects(platform.getGlobalBounds()))   
        {
            player.isJumping = false;
        }
    }
}

// Kresleni platformy
void Map::drawPlatform(sf::RenderWindow& window, sf::RectangleShape& platform)
{
    window.draw(platform);
}

// Kresleni mapy
void Map::draw(sf::RenderWindow& window, Player& player, float deltaTime, float multiplier)
{
    float x = -400;
    float xx = 0;
    float xxx = 0;
    float xxxx = 600;

    // Kresleni pozadi
    for (int i = 0; i < 4; i++)
    {
        background1.setPosition(x, 250);
        background2.setPosition(x, -350);

        window.draw(background1);
        window.draw(background2);

        x += backgroundTexture1.getSize().x;
    }

    background3.setPosition(-1000, -950);
    window.draw(background3);

    for (int i = 0; i < 1501; i += 1500)
    {
        background4.setPosition(i, -950);
        window.draw(background4);
    }
    
    bossArena.setPosition(1000, -935);
    window.draw(bossArena);
    
    // Kresleni textur zeme
    for (int i = 0; i < 25; i++)      //  Prvni zeme 2500 wide
    {
        stoneFloor1.setPosition(xx, 900);
        window.draw(stoneFloor1);
        xx += floorTexture1.getSize().x;
    }
    for (int j = 0; j < 19; j++)     //  Druha zeme 1900 wide
    {
        greenFloor.setPosition(xxx, 200);
        window.draw(greenFloor);
        xxx += floorTexture3.getSize().x;
    }

    for (int i = 0; i < 14; i++)      // Treti zeme 1400 wide
    {
        bossFloor.setPosition(xxxx, -500);
        window.draw(bossFloor);
        xxxx += floorTexture1.getSize().x;
    }

    // Kresleni klecovych dveri
    window.draw(cageDoor2);
    window.draw(cageDoor1);

    // Kresleni klecovych zdi
    window.draw(cageWall1);
    window.draw(cageWall2);

    // Vektor na vrchni cast platformy
    for (sf::RectangleShape& platform : platformsTop)
    {
        drawPlatform(window, platform);
        platform.setTexture(&platformTexture);
    }

    // Vektor na spodni cast platformy
    for (sf::RectangleShape& platform : platformsBot)
    {
        drawPlatform(window, platform);
        platform.setTexture(&platformTexture);
    }
}