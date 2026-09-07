#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>

#include "structure.h"
#include "personnages/personnage.cpp"
#include "bestiaire/bestiaire.cpp"
#include "combat/combat.cpp"
#include "exploration/exploration.cpp"

using namespace std;

int main()
{
    srand(time(0));

    Joueur joueur;
    joueur.inventaire.sac = new int[joueur.inventaire.capaciteSac]{0};

    int territoiresConquis = 0;
    int zoneActuelle = 1;
    int option = 0;

    // Plein écran
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "FANTASY WORLD CONQUEST", sf::Style::Fullscreen);

    // Charger la police
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/pixelart.ttf"))
    {
        cout << "Erreur: Impossible de charger pixelart.ttf" << endl;
        if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
            cout << "Impossible de charger la police systeme." << endl;
    }

    sf::Texture textureFond;
    bool fondCharge = textureFond.loadFromFile("assets/backgrounds/Bg.jpg");
    sf::Sprite spriteFond;
    if (fondCharge)
    {
        spriteFond.setTexture(textureFond);
        sf::Vector2u tailleImage = textureFond.getSize();
        spriteFond.setScale((float)window.getSize().x / tailleImage.x, (float)window.getSize().y / tailleImage.y);
    }

    // Calcul des centres dynamiques de l'écran
    float centerX = window.getSize().x / 2.0f;
    float centerY = window.getSize().y / 2.0f;

    sf::Text titre("FANTASY WORLD CONQUEST", font, 36);
    titre.setFillColor(sf::Color::White);
    titre.setOrigin(titre.getLocalBounds().width / 2.0f, 0.0f);
    titre.setPosition(centerX, centerY - 200.0f);

    sf::RectangleShape btnJouer(sf::Vector2f(250.0f, 50.0f));
    btnJouer.setOrigin(125.0f, 25.0f);
    btnJouer.setPosition(centerX, centerY - 30.0f);
    btnJouer.setFillColor(sf::Color::White);

    sf::Text txtJouer("Commencer", font, 20);
    txtJouer.setOrigin(txtJouer.getLocalBounds().width / 2.0f, txtJouer.getLocalBounds().height / 2.0f);
    txtJouer.setPosition(centerX, centerY - 35.0f);

    sf::RectangleShape btnQuitter(sf::Vector2f(250.0f, 50.0f));
    btnQuitter.setOrigin(125.0f, 25.0f);
    btnQuitter.setPosition(centerX, centerY + 50.0f);
    btnQuitter.setFillColor(sf::Color(180, 40, 40));

    sf::Text txtQuitter("Quitter", font, 20);
    txtQuitter.setOrigin(txtQuitter.getLocalBounds().width / 2.0f, txtQuitter.getLocalBounds().height / 2.0f);
    txtQuitter.setPosition(centerX, centerY + 45.0f);

    bool jeuDemarre = false;

    // BOUCLE DU MENU PRINCIPAL
    while (window.isOpen() && !jeuDemarre)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (btnJouer.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    jeuDemarre = true;
                }
                else if (btnQuitter.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    window.close();
                }
            }
        }

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (btnJouer.getGlobalBounds().contains(mousePos.x, mousePos.y))
            btnJouer.setFillColor(sf::Color(60, 200, 60));
        else
            btnJouer.setFillColor(sf::Color(40, 140, 40));

        if (btnQuitter.getGlobalBounds().contains(mousePos.x, mousePos.y))
            btnQuitter.setFillColor(sf::Color(230, 60, 60));
        else
            btnQuitter.setFillColor(sf::Color(180, 40, 40));

        window.clear(sf::Color(15, 15, 20));

        if (fondCharge)
            window.draw(spriteFond);

        window.draw(titre);
        window.draw(btnJouer);
        window.draw(txtJouer);
        window.draw(btnQuitter);
        window.draw(txtQuitter);
        window.display();
    }

    if (jeuDemarre)
    {
        creerPersonnageSFML(window, font, joueur);

        while (joueur.vie > 0 && territoiresConquis < 6)
        {
            explorerMonde(territoiresConquis, zoneActuelle, option, joueur);
        }

        if (joueur.vie <= 0)
        {
            cout << "\n [GAME OVER] VOUS ETES MORT SANS ECRIRE VOTRE LEGENDE...\n";
        }
        else
        {
            cout << "\n [VICTOIRE ABSOLUE] L'EMPEREUR DES OMBRES EST VAINCU !\n";
        }
    }

    delete[] joueur.inventaire.sac;
    return 0;
}