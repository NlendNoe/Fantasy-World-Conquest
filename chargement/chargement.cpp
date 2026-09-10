#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <cstdlib>

#include "../structure.h"

using namespace std;

void ecranChargementSFML(sf::RenderWindow &window, sf::Font &font)
{
    float screenW = static_cast<float>(window.getSize().x);
    float screenH = static_cast<float>(window.getSize().y);
    float centerX = screenW / 2.0f;

    sf::Texture textureFond;
    bool fondCharge = textureFond.loadFromFile("assets/backgrounds/font2.jpg");
    sf::Sprite spriteFond;
    if (fondCharge)
    {
        spriteFond.setTexture(textureFond);
        sf::Vector2u tailleImage = textureFond.getSize();
        spriteFond.setScale(screenW / tailleImage.x, screenH / tailleImage.y);
    }

    // ---------- 2. La barre de chargement ----------
    float barreW = screenW * 0.40f;
    float barreH = screenH * 0.035f;
    float barreX = centerX - (barreW / 2.0f);
    float barreY = screenH * 0.82f;

    sf::RectangleShape contourBarre(sf::Vector2f(barreW, barreH));
    contourBarre.setPosition(barreX, barreY);
    contourBarre.setFillColor(sf::Color(0, 0, 0, 150));   
    contourBarre.setOutlineThickness(3.0f);
    contourBarre.setOutlineColor(sf::Color::White);

    sf::RectangleShape remplissageBarre(sf::Vector2f(0.0f, barreH)); 
    remplissageBarre.setPosition(barreX, barreY);
    remplissageBarre.setFillColor(sf::Color(120, 200, 80));         

    unsigned int szTexte = static_cast<unsigned int>(screenH * 0.030f);
    sf::Text texteChargement("Chargement... 0%", font, szTexte);
    sf::FloatRect boundsTexte = texteChargement.getLocalBounds();
    texteChargement.setOrigin(boundsTexte.left + boundsTexte.width / 2.0f, boundsTexte.top + boundsTexte.height / 2.0f);
    texteChargement.setPosition(centerX, barreY + barreH + (screenH * 0.04f)); 

    int progression = 0;           
    sf::Clock horloge;             
    float prochaineEtape = 0.2f;   
    bool chargementFini = false;
    sf::Clock horlogeFin;      

    while (window.isOpen() && !chargementFini)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (progression < 100 && horloge.getElapsedTime().asSeconds() >= prochaineEtape)
        {
            horloge.restart();
            prochaineEtape = 0.1f + static_cast<float>(rand() % 30) / 100.0f;

            int saut = 2 + rand() % 8;
            progression += saut;
            if (progression > 100)
                progression = 100;

            if (progression >= 100)
                horlogeFin.restart();

            float largeurRemplie = barreW * (progression / 100.0f);
            remplissageBarre.setSize(sf::Vector2f(largeurRemplie, barreH));

            texteChargement.setString("Chargement... " + to_string(progression) + "%");
            sf::FloatRect bounds = texteChargement.getLocalBounds();
            texteChargement.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        }

        if (progression >= 100 && horlogeFin.getElapsedTime().asSeconds() >= 0.6f)
            chargementFini = true;

        window.clear(sf::Color::Black);

        if (fondCharge)
            window.draw(spriteFond);

        window.draw(contourBarre);
        window.draw(remplissageBarre);
        window.draw(texteChargement);

        window.display();
    }
}
