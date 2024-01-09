#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Map.h"
#include "Interface.h"
#include "Camera.h"
#include "Menu.h"

//////////// Edit po dokonceni hry /////////////////////////////////////////////////////////////////
///////////// Vykricnik ( ! ) u komentare == chtelo by zmenit / ne moc dobre reseni ////////////////
////////////// Duvody - malo casu, musel jsem jiz pracovat na Pythonu, kdybych mel grafika /////////
/////////////// co by mi kreslil textury tak bych to mozna stihl, zprvu jsem netusil jak na to /////
//////////////// a dalsi podobne vymluvy ///////////////////////////////////////////////////////////

enum class GameState 
{
    MENU,
    GAMEPLAY
};

int main() 
{
    // Okno hry a FPS limit ( zde neni nutne potreba )
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Song of the Knights of Silk");
    window.setFramerateLimit(60);

    // Inicializace class
    Map map;
    Boss boss;
    Enemy enemy({1,1},{1,1},1);
    Enemies enemies;
    Player player;
    sf::View camera;
    Enemies allEnemies;
    Interface interface;
    // Inicializace textury utoku ( pro lepsi dostupnost )
    sf::Texture hitpointTexture;

    // Inicializace sound bufferu a jednotlivych zvuku
    sf::SoundBuffer hitBuffer;
    sf::SoundBuffer getHitBuffer;
    sf::SoundBuffer music1Buffer;
    sf::SoundBuffer music2Buffer;
    sf::Sound hitSound;
    sf::Sound getHitSound;
    sf::Sound music1;
    sf::Sound music2;

    // 'Real time' hodiny, casovac utoku, delta time + multiplier proti zavislosti hre na hardwaru
    sf::Clock clock;
    sf::Clock attackTimer;
    float deltaTime;
    float multiplier = 70.0f;

    // Nacitani textur
    player.loadTextures();
    boss.loadTextures();
    map.loadTextures();
    interface.loadTextures();
    allEnemies.loadTextures();

    // Nacitani zvuku
    hitBuffer.loadFromFile("Hit.wav");
    getHitBuffer.loadFromFile("GetHit.wav");
    music1Buffer.loadFromFile("Music1.wav");
    music2Buffer.loadFromFile("Music2.mp3");

    // Nahravani zvuku na buffer
    hitSound.setBuffer(hitBuffer);
    getHitSound.setBuffer(getHitBuffer);
    music1.setBuffer(music1Buffer);
    music2.setBuffer(music2Buffer);
    music1.setLoop(true);
    // Kontrola zacatku hudby
    bool music1Started = false;
    bool music2Started = false;


    GameState gameState = GameState::MENU;
    Menu mainMenu((float)window.getSize().x, (float)window.getSize().y);

    // Game loop
    while (window.isOpen())
    {
        // Zacatek hudby menu a hry + kontrola proti loopovani
        if (!music1Started && player.enterBossfight == false)
        {
            music1.play();
            music1Started = true;
        }
        // Zacatek boss hudby + kontrola proti loopovani
        if (player.enterBossfight == true && !music2Started)
        {
            music1.stop();
            music2.play();
            music2Started = true;
        }

        sf::Event event;
        deltaTime = clock.restart().asSeconds(); 

        // MENU
        if (gameState == GameState::MENU) 
        {
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                mainMenu.handleEvents(window, event);
                // Moznost zacatek hry
                if (mainMenu.startGame == true)
                {
                    gameState = GameState::GAMEPLAY;
                }
                // Moznost modu Boss Rush
                else if (mainMenu.bossRush == true)
                {
                    gameState = GameState::GAMEPLAY;
                    player.setPosition({ 700, -800 });
                }
                // Moznost Exit
                else if (mainMenu.exitGame == true)
                {
                    window.close();
                }
            }

            window.clear();
            mainMenu.draw(window);
            window.display();
        }
        // GAMEPLAY
        else if (gameState == GameState::GAMEPLAY) 
        {
            // Pokud hrac zemre ale neni v boss arene
            // Hrac se resetuje na zacatek a nepratele resetuji sve pozice a zivoty
            if (interface.isPlayerAlive(player) == false and player.enterBossfight == false)
            {
                interface.playerReset(player);
                enemies.resetEnemies();
            }
            // Pokud hrac zemre v boss arene
            // ! Konec hry ( git gud ) 
            if (interface.isPlayerAlive(player) == false and player.enterBossfight == true)
            {
                window.close();
            }

            // Pouziti logiky kamery, interface, nepratel, hrace, mapy a bosse
            handleCamera(camera, player, window);
            interface.handleUI(window, player);
            if (mainMenu.bossRush == false)
                allEnemies.handleEnemies(deltaTime, player, window, interface, player.attackHitbox, hitSound, getHitSound);
            player.handlePlayer(deltaTime, multiplier, hitSound);
            map.handleCollisions(player);
            boss.handleBoss(player, window, interface, player.attackHitbox, deltaTime, multiplier, getHitSound);

            // Pouziti eventu pro ukonceni hry a zavreni okna 
            while (window.pollEvent(event)) 
            {
                switch (event.type)
                {
                    case sf::Event::Closed:
                        window.close();

                    case sf::Event::KeyReleased:
                        if (event.key.code == sf::Keyboard::Escape)
                            window.close();
                }
            }

            // Kresleni jednotlivych textur class mapy, bosse, interface, hrace a nepratel
            window.clear();
            map.draw(window, player, deltaTime, multiplier);
            boss.drawBoss(window, interface, player);
            interface.draw(window);
            player.drawPlayer(window);
            allEnemies.drawEnemies(window);
            window.display();
        }
    }
    return 0;
}