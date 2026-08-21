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

    // Création de la fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Fantasy World Conquest");

    // Charger la police
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        cout << "Erreur: Impossible de charger arial.ttf" << endl;
        if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
            cout << "Impossible de charger la police systeme." << endl;
    }

    sf::Text titre("FANTASY WORLD CONQUEST", font, 32);
    titre.setFillColor(sf::Color::White);
    titre.setPosition(180.0f, 80.0f);

    sf::RectangleShape btnJouer(sf::Vector2f(250.0f, 50.0f));
    btnJouer.setPosition(275.0f, 220.0f);
    btnJouer.setFillColor(sf::Color::White);

    sf::Text txtJouer("Commencer", font, 20);
    txtJouer.setPosition(320.0f, 230.0f);

    sf::RectangleShape btnQuitter(sf::Vector2f(250.0f, 50.0f));
    btnQuitter.setPosition(275.0f, 310.0f);
    btnQuitter.setFillColor(sf::Color(180, 40, 40)); // Rouge

    sf::Text txtQuitter("Quitter", font, 20);
    txtQuitter.setPosition(350.0f, 320.0f);

    bool jeuDemarre = false;

    while (window.isOpen() && !jeuDemarre)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // --- GESTION DES CLICS DE SOURIS ---
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (btnJouer.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    jeuDemarre = true;
                }
                else if (btnQuitter.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    delete[] joueur.inventaire.sac;
                    window.close();
                    return 0;
                }
            }
        }

        // --- GESTION DU SURVOL (HOVER) ---
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (btnJouer.getGlobalBounds().contains(mousePos.x, mousePos.y))
            btnJouer.setFillColor(sf::Color(60, 200, 60)); // Vert clair

        else
            btnJouer.setFillColor(sf::Color(40, 140, 40)); // Vert foncé

        if (btnQuitter.getGlobalBounds().contains(mousePos.x, mousePos.y))
            btnQuitter.setFillColor(sf::Color(230, 60, 60)); // Rouge clair

        else
            btnQuitter.setFillColor(sf::Color(180, 40, 40)); // Rouge foncé

        window.clear(sf::Color(15, 15, 20));
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