#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "../structure.h"

using namespace std;

void creerPersonnageSFML(sf::RenderWindow& window, sf::Font& font, Joueur& joueur)
{
    std::string nomSaisi = "";
    int classeChoisie = 0; 
    bool creationTerminee = false;

    sf::Texture textureFond;
    bool fondCharge = textureFond.loadFromFile("fond_creation.png");
    sf::Sprite spriteFond;
    if (fondCharge) {
        spriteFond.setTexture(textureFond);
        sf::Vector2u tailleImage = textureFond.getSize();
        spriteFond.setScale(800.0f / tailleImage.x, 600.0f / tailleImage.y);
    }

    sf::Text titre("CREATION DE L'AVATAR", font, 28);
    titre.setFillColor(sf::Color::White);
    titre.setPosition(200.0f, 30.0f);

    sf::Text consigneNom("1. Entrez le nom de votre avatar :", font, 18);
    consigneNom.setPosition(150.0f, 90.0f);

    sf::RectangleShape champNom(sf::Vector2f(400.0f, 40.0f));
    champNom.setPosition(200.0f, 120.0f);
    champNom.setFillColor(sf::Color(20, 20, 40, 200));
    champNom.setOutlineThickness(2.0f);
    champNom.setOutlineColor(sf::Color::Cyan);

    sf::Text texteNom("", font, 20);
    texteNom.setPosition(210.0f, 128.0f);

    sf::Text consigneClasse("2. Choisissez votre classe :", font, 18);
    consigneClasse.setPosition(150.0f, 190.0f);

    sf::RectangleShape btnGuerrier(sf::Vector2f(180.0f, 80.0f));
    btnGuerrier.setPosition(100.0f, 230.0f);
    sf::Text txtGuerrier("Guerrier\n\nPV: 120\nAtq: 12", font, 16);
    txtGuerrier.setPosition(120.0f, 240.0f);

    sf::RectangleShape btnMage(sf::Vector2f(180.0f, 80.0f));
    btnMage.setPosition(310.0f, 230.0f);
    sf::Text txtMage("Mage\n\nPV: 80\nAtq: 22", font, 16);
    txtMage.setPosition(340.0f, 240.0f);

    sf::RectangleShape btnVoleur(sf::Vector2f(180.0f, 80.0f));
    btnVoleur.setPosition(520.0f, 230.0f);
    sf::Text txtVoleur("Barbare\n\nPV: 60\nAtq: 27", font, 16);
    txtVoleur.setPosition(540.0f, 240.0f);

    sf::RectangleShape btnValider(sf::Vector2f(220.0f, 50.0f));
    btnValider.setPosition(290.0f, 360.0f);
    btnValider.setFillColor(sf::Color(50, 150, 50));

    sf::Text txtValider("CONFIRMER", font, 20);
    txtValider.setPosition(335.0f, 372.0f);

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
                    if (!nomSaisi.empty()) nomSaisi.pop_back();
                }
                else if (event.text.unicode > 32 && event.text.unicode < 127 && nomSaisi.length() < 15) {
                    nomSaisi += static_cast<char>(event.text.unicode);
                }
                texteNom.setString(nomSaisi);
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (btnGuerrier.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    classeChoisie = 1;
                }
                else if (btnMage.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    classeChoisie = 2;
                }
                else if (btnVoleur.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    classeChoisie = 3;
                }
                else if (btnValider.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (!nomSaisi.empty() && classeChoisie != 0) {
                        creationTerminee = true;
                    }
                }
            }
        }

        btnGuerrier.setFillColor((classeChoisie == 1) ? sf::Color(80, 120, 220) : sf::Color(40, 40, 60, 200));
        btnMage.setFillColor((classeChoisie == 2) ? sf::Color(80, 120, 220) : sf::Color(40, 40, 60, 200));
        btnVoleur.setFillColor((classeChoisie == 3) ? sf::Color(80, 120, 220) : sf::Color(40, 40, 60, 200));

        window.clear(sf::Color(15, 15, 10));

        if (fondCharge) window.draw(spriteFond);

        window.draw(titre);
        window.draw(consigneNom);
        window.draw(champNom);
        window.draw(texteNom);

        window.draw(consigneClasse);
        window.draw(btnGuerrier);
        window.draw(txtGuerrier);
        window.draw(btnMage);
        window.draw(txtMage);
        window.draw(btnVoleur);
        window.draw(txtVoleur);

        if (!nomSaisi.empty() && classeChoisie != 0) {
            window.draw(btnValider);
            window.draw(txtValider);
        }

        window.display();
    }

    joueur.nom = nomSaisi;
    joueur.niveau = 1;

    if (classeChoisie == 1) { // Guerrier
        joueur.vie = 120;
        joueur.vieMax = 120;
        joueur.attaque = 12;
    }
    else if (classeChoisie == 2) { // Mage
        joueur.vie = 80;
        joueur.vieMax = 80;
        joueur.attaque = 22;
    }
    else if (classeChoisie == 3) { // Voleur
        joueur.vie = 95;
        joueur.vieMax = 95;
        joueur.attaque = 16;
    }
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