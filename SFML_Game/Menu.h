class Menu 
{
public:

    Menu(float width, float height) 
    {
        // Nacteni fontu a pozadi menu
        font.loadFromFile("font.ttf");
        backgroundTexture.loadFromFile("Textures/background.png");
        backgroundLogoTexture.loadFromFile("Textures/logo.png");

        // Nastaveni textury pozadi
        background.setTexture(backgroundTexture);
        background.setPosition(-30, 0);

        // Nastaveni textury loga
        logo.setTexture(backgroundLogoTexture);
        logo.setPosition(160, 370);

        // Jednotlive moznosti v menu
        addItem("Play", sf::Vector2f(width / 2 + 200, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));
        addItem("Boss rush", sf::Vector2f(width / 2 + 200, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));
        addItem("Exit", sf::Vector2f(width / 2 + 200, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));
    }

    // Kresleni pozadi a jednotlivych moznosti v menu
    void draw(sf::RenderWindow& window) 
    {
        window.draw(background);
        window.draw(logo);

        for (const auto& item : menuItems) 
        {
            window.draw(item.text);
        }

        // Zlute zvyrazneni vybrane moznosti
        if (selectedItemIndex >= 0 && selectedItemIndex < menuItems.size()) 
        {
            menuItems[selectedItemIndex].text.setFillColor(sf::Color::Yellow);
        }
    }

    // Funkce na pohyb a zapnuti hry v menu
    void handleEvents(sf::RenderWindow& window, sf::Event& event) 
    {
        if (event.type == sf::Event::KeyPressed) 
        {
            if (event.key.code == sf::Keyboard::Up) 
            {
                moveUp();
            }
            else if (event.key.code == sf::Keyboard::Down) 
            {
                moveDown();
            }
            else if (event.key.code == sf::Keyboard::Enter) 
            {
                if (selectedItemIndex == 0) 
                {
                    startGame = true;
                }
                else if (selectedItemIndex == 1)
                {
                    bossRush = true;
                }
                else if (selectedItemIndex == 2) 
                {
                    exitGame = true;
                }
            }
        }
    }
    // Funkce na pohyb nahoru v menu
    void moveUp() 
    {
        if (selectedItemIndex == 1) 
        {
            menuItems[selectedItemIndex].text.setFillColor(sf::Color::White);
            selectedItemIndex = 0;
        }
        if (selectedItemIndex == 2)
        {
            menuItems[selectedItemIndex].text.setFillColor(sf::Color::White);
            selectedItemIndex = 1;
        }
    }
    // Funkce na pohyb dolu v menu
    void moveDown() 
    {
        if (selectedItemIndex == 0) 
        {
            menuItems[selectedItemIndex].text.setFillColor(sf::Color::White);
            selectedItemIndex = 1;
        }
        if (selectedItemIndex == 1)
        {
            menuItems[selectedItemIndex].text.setFillColor(sf::Color::White);
            selectedItemIndex = 2;
        }
    }

    bool startGame = false;
    bool bossRush = false;
    bool exitGame = false;

private:

    // Struktura na druhy moznosti v menu ( asi neni uplne potreba )
    struct MenuItem 
    {
        sf::Text text;
    };

    // Maximum poctu moznosti ( kvuli mezeram ve vykreslovani ) a cislo aktualne vybrane moznosti
    int MAX_NUMBER_OF_ITEMS = 3;
    int selectedItemIndex = 0;

    // Font, texture a sprite pozadi, vektor vsech moznosti
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Texture backgroundLogoTexture;
    sf::Sprite background;
    sf::Sprite logo;
    std::vector<MenuItem> menuItems;
    
    // Konstrukter moznosti
    void addItem(const std::string& text, const sf::Vector2f& position) 
    {
        MenuItem newItem;
        newItem.text.setString(text);
        newItem.text.setFont(font);
        newItem.text.setCharacterSize(40);
        newItem.text.setFillColor(sf::Color::White);
        newItem.text.setPosition(position);

        menuItems.push_back(newItem);
    }
};
