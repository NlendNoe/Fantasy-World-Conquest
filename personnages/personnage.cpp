#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "../structure.h"

using namespace std;

void creerPersonnageSFML(sf::RenderWindow& window, sf::Font& font, Joueur& joueur)
{
    std::string nomSaisi = "";
    bool creationTerminee = false;

    sf::Text titre("CREATION DU PERSONNAGE", font, 28);
    titre.setFillColor(sf::Color::Yellow);
    titre.setPosition(200.0f, 60.0f);

    sf::Text consigne("Entrez le nom de votre avatar\n(Pas d'espace, puis appuyez sur ENTREE ou CONFIRMER) :", font, 18);
    consigne.setPosition(150.0f, 130.0f);

    sf::RectangleShape champNom(sf::Vector2f(400.0f, 45.0f));
    champNom.setPosition(200.0f, 210.0f);
    champNom.setFillColor(sf::Color(40, 40, 60));
    champNom.setOutlineThickness(2.0f);
    champNom.setOutlineColor(sf::Color::Cyan);

    sf::Text texteNom("", font, 22);
    texteNom.setPosition(210.0f, 220.0f);
    texteNom.setFillColor(sf::Color::White);

    sf::RectangleShape btnValider(sf::Vector2f(200.0f, 50.0f));
    btnValider.setPosition(300.0f, 300.0f);
    btnValider.setFillColor(sf::Color(50, 150, 50));

    sf::Text txtValider("CONFIRMER", font, 20);
    txtValider.setPosition(335.0f, 312.0f);

    while (window.isOpen() && !creationTerminee)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == 8 || event.text.unicode == 127) {
                    if (!nomSaisi.empty()) {
                        nomSaisi.pop_back();
                    }
                }
                else if (event.text.unicode == 13 || event.text.unicode == 10) {
                    if (!nomSaisi.empty()) {
                        creationTerminee = true;
                    }
                }
                else if (event.text.unicode > 32 && event.text.unicode < 127 && nomSaisi.length() < 15) {
                    nomSaisi += static_cast<char>(event.text.unicode);
                }

                texteNom.setString(nomSaisi);
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (btnValider.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (!nomSaisi.empty()) {
                        creationTerminee = true;
                    }
                }
            }
        }

        window.clear(sf::Color(20, 20, 30));
        window.draw(titre);
        window.draw(consigne);
        window.draw(champNom);
        window.draw(texteNom);

        if (!nomSaisi.empty()) {
            window.draw(btnValider);
            window.draw(txtValider);
        }

        window.display();
    }

    joueur.nom = nomSaisi;
    joueur.vie = 100;
    joueur.vieMax = 100;
    joueur.attaque = 10;
    joueur.niveau = 1;
}

void afficherPersonnage(const Joueur &joueur)
{
    cout << "===================\n";
    cout << "\n JOUEUR \n";
    cout << "===================\n";

    cout << "Nom : " << joueur.nom << "\n";
    cout << "Vie : " << joueur.vie << "/" << joueur.vieMax << "\n";
    cout << "Attaque : " << joueur.attaque << "\n";
    cout << "Niveau : " << joueur.niveau << "\n";
}