#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "../structure.h"

using namespace std;

bool creerPersonnageSFML(sf::RenderWindow &window, sf::Font &font, Joueur &joueur)
{
    std::string nomSaisi = "";
    int classeChoisie = 0;
    bool creationTerminee = false;

    float centerX = window.getSize().x / 2.0f;

    sf::Texture textureFond;
    bool fondCharge = textureFond.loadFromFile("assets/backgrounds/aaaaaa.jpg");
    sf::Sprite spriteFond;
    if (fondCharge)
    {
        spriteFond.setTexture(textureFond);
        sf::Vector2u tailleImage = textureFond.getSize();
        spriteFond.setScale((float)window.getSize().x / tailleImage.x, (float)window.getSize().y / tailleImage.y);
    }

    sf::RectangleShape btnRetour(sf::Vector2f(150.0f, 50.0f));
    btnRetour.setPosition(30.0f, 20.0f);
    btnRetour.setFillColor(sf::Color(180, 40, 40));

    sf::Text txtRetour("RETOUR", font, 18);
    txtRetour.setOrigin(txtRetour.getLocalBounds().width / 2.0f, txtRetour.getLocalBounds().height / 2.0f);
    txtRetour.setPosition(30.0f + 75.0f, 20.0f + 25.0f);
    


    sf::Texture texturePerso, texturePersoDeux, texturePersoTrois, textureQuatre, textureCinq, textureSix;
    texturePerso.loadFromFile("assets/pictures/dark_skinned_knight.png");
    texturePersoDeux.loadFromFile("assets/pictures/robe.png");
    texturePersoTrois.loadFromFile("assets/pictures/leather_armor.png");
    textureQuatre.loadFromFile("assets/pictures/plate_armor.png");
    textureCinq.loadFromFile("assets/pictures/robed_skeleton_spellcast.png");
    textureSix.loadFromFile("assets/pictures/chain_armor_bandit.png");

    sf::Sprite spritePerso(texturePerso), spritePersoDeux(texturePersoDeux), spritePersoTrois(texturePersoTrois),
               spritePersoQuatre(textureQuatre), spritePersoCinq(textureCinq), spritePersoSix(textureSix);

    sf::Sprite* sprites[6] = {&spritePerso, &spritePersoDeux, &spritePersoTrois, &spritePersoQuatre, &spritePersoCinq, &spritePersoSix};
    for(int i = 0; i < 6; i++) {
        sprites[i]->setOrigin(32.0f, 32.0f);
        sprites[i]->setPosition(centerX, window.getSize().y - 280.0f);
        sprites[i]->setScale(4.5f, 4.5f);
    }

    sf::IntRect imgPersonnage(0, 128, 64, 64);

    // Titre principal
    sf::Text titre("CREATION DE L'AVATAR", font, 38);
    titre.setFillColor(sf::Color::White);
    titre.setOrigin(titre.getLocalBounds().width / 2.0f, 0.0f);
    titre.setPosition(centerX, 20.0f);

    // Champ Nom
    sf::Text consigneNom("1. Entrez le nom de votre avatar :", font, 18);
    consigneNom.setOrigin(consigneNom.getLocalBounds().width / 2.0f, 0.0f);
    consigneNom.setPosition(centerX, 65.0f);

    float largeurNom = 520.0f;
    float hauteurNom = 50.0f;
    sf::RectangleShape champNom(sf::Vector2f(largeurNom, hauteurNom));
    champNom.setPosition(centerX - (largeurNom / 2.0f), 95.0f);
    champNom.setFillColor(sf::Color(20, 20, 40, 200));
    champNom.setOutlineThickness(3.0f);
    champNom.setOutlineColor(sf::Color::Cyan);

    sf::Text texteNom("", font, 22);
    texteNom.setPosition(centerX - (largeurNom / 2.0f) + 15.0f, 107.0f);

    // Choix de classe
    sf::Text consigneClasse("2. Choisissez votre classe :", font, 18);
    consigneClasse.setOrigin(consigneClasse.getLocalBounds().width / 2.0f, 0.0f);
    consigneClasse.setPosition(centerX, 165.0f);

    float largeurBtn = 260.0f;
    float hauteurBtn = 90.0f;
    float espacementH = 35.0f;
    float espacementV = 25.0f;

    float col1 = centerX - (1.5f * largeurBtn + espacementH);
    float col2 = centerX - (0.5f * largeurBtn);
    float col3 = centerX + (0.5f * largeurBtn + espacementH);

    float row1Y = 200.0f;
    float row2Y = row1Y + hauteurBtn + espacementV;

    sf::RectangleShape btnGuerrier(sf::Vector2f(largeurBtn, hauteurBtn)); btnGuerrier.setPosition(col1, row1Y);
    sf::Text txtGuerrier("Guerrier\nPV : 120 | Atq : 12", font, 18); txtGuerrier.setPosition(col1 + 20.0f, row1Y + 20.0f);

    sf::RectangleShape btnMage(sf::Vector2f(largeurBtn, hauteurBtn)); btnMage.setPosition(col2, row1Y);
    sf::Text txtMage("Mage\nPV : 80 | Atq : 22", font, 18); txtMage.setPosition(col2 + 20.0f, row1Y + 20.0f);

    sf::RectangleShape btnArcher(sf::Vector2f(largeurBtn, hauteurBtn)); btnArcher.setPosition(col3, row1Y);
    sf::Text txtArcher("Archer\nPV : 95 | Atq : 16", font, 18); txtArcher.setPosition(col3 + 20.0f, row1Y + 20.0f);

    sf::RectangleShape btnPaladin(sf::Vector2f(largeurBtn, hauteurBtn)); btnPaladin.setPosition(col1, row2Y);
    sf::Text txtPaladin("Paladin\nPV : 140 | Atq : 10", font, 18); txtPaladin.setPosition(col1 + 20.0f, row2Y + 20.0f);

    sf::RectangleShape btnNecro(sf::Vector2f(largeurBtn, hauteurBtn)); btnNecro.setPosition(col2, row2Y);
    sf::Text txtNecro("Necromancien\nPV : 75 | Atq : 25", font, 18); txtNecro.setPosition(col2 + 20.0f, row2Y + 20.0f);

    sf::RectangleShape btnAssassin(sf::Vector2f(largeurBtn, hauteurBtn)); btnAssassin.setPosition(col3, row2Y);
    sf::Text txtAssassin("Assassin\nPV : 85 | Atq : 20", font, 18); txtAssassin.setPosition(col3 + 20.0f, row2Y + 20.0f);

    sf::RectangleShape btnValider(sf::Vector2f(260.0f, 55.0f));
    btnValider.setPosition(centerX - 130.0f, window.getSize().y - 85.0f);
    btnValider.setFillColor(sf::Color(50, 150, 50));

    sf::Text txtValider("CONFIRMER", font, 22);
    txtValider.setPosition(centerX - 65.0f, window.getSize().y - 70.0f);

    while (window.isOpen() && !creationTerminee)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return false;
            }

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == 8 || event.text.unicode == 127)
                {
                    if (!nomSaisi.empty()) nomSaisi.pop_back();
                }
                else if (event.text.unicode > 32 && event.text.unicode < 127 && nomSaisi.length() < 15)
                {
                    nomSaisi += static_cast<char>(event.text.unicode);
                }
                texteNom.setString(nomSaisi);
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (btnRetour.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    return false;
                }
                else if (btnGuerrier.getGlobalBounds().contains(mousePos.x, mousePos.y)) classeChoisie = 1;
                else if (btnMage.getGlobalBounds().contains(mousePos.x, mousePos.y)) classeChoisie = 2;
                else if (btnArcher.getGlobalBounds().contains(mousePos.x, mousePos.y)) classeChoisie = 3;
                else if (btnPaladin.getGlobalBounds().contains(mousePos.x, mousePos.y)) classeChoisie = 4;
                else if (btnNecro.getGlobalBounds().contains(mousePos.x, mousePos.y)) classeChoisie = 5;
                else if (btnAssassin.getGlobalBounds().contains(mousePos.x, mousePos.y)) classeChoisie = 6;
                else if (btnValider.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    if (!nomSaisi.empty() && classeChoisie != 0) creationTerminee = true;
                }
            }
        }

        sf::Vector2f mousePosHover = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (btnRetour.getGlobalBounds().contains(mousePosHover.x, mousePosHover.y))
            btnRetour.setFillColor(sf::Color(230, 60, 60));
        else
            btnRetour.setFillColor(sf::Color(180, 40, 40));

        btnGuerrier.setFillColor((classeChoisie == 1) ? sf::Color(80, 120, 220) : sf::Color(40, 40, 60, 200));
        btnMage.setFillColor((classeChoisie == 2) ? sf::Color(80, 120, 220) : sf::Color(40, 40, 60, 200));
        btnArcher.setFillColor((classeChoisie == 3) ? sf::Color(80, 120, 220) : sf::Color(40, 40, 60, 200));
        btnPaladin.setFillColor((classeChoisie == 4) ? sf::Color(80, 120, 220) : sf::Color(40, 40, 60, 200));
        btnNecro.setFillColor((classeChoisie == 5) ? sf::Color(80, 120, 220) : sf::Color(40, 40, 60, 200));
        btnAssassin.setFillColor((classeChoisie == 6) ? sf::Color(80, 120, 220) : sf::Color(40, 40, 60, 200));

        window.clear(sf::Color(15, 15, 10));

        if (fondCharge) window.draw(spriteFond);

        window.draw(btnRetour);
        window.draw(txtRetour);

        window.draw(titre);
        window.draw(consigneNom); window.draw(champNom); window.draw(texteNom);
        window.draw(consigneClasse);

        window.draw(btnGuerrier); window.draw(txtGuerrier);
        window.draw(btnMage);     window.draw(txtMage);
        window.draw(btnArcher);   window.draw(txtArcher);
        window.draw(btnPaladin);  window.draw(txtPaladin);
        window.draw(btnNecro);    window.draw(txtNecro);
        window.draw(btnAssassin); window.draw(txtAssassin);

        if (classeChoisie >= 1 && classeChoisie <= 6)
        {
            sprites[classeChoisie - 1]->setTextureRect(imgPersonnage);
            window.draw(*sprites[classeChoisie - 1]);
        }

        if (!nomSaisi.empty() && classeChoisie != 0)
        {
            window.draw(btnValider);
            window.draw(txtValider);
        }

        window.display();
    }

    joueur.nom = nomSaisi;
    joueur.niveau = 1;

    if (classeChoisie == 1)      { joueur.vie = 120; joueur.vieMax = 120; joueur.attaque = 12; joueur.defense = 75; joueur.mana = 5;  joueur.manaMax = 10;   joueur.competenceSpeciale = "Coup puissant"; }
    else if (classeChoisie == 2) { joueur.vie = 80;  joueur.vieMax = 80;  joueur.attaque = 22; joueur.defense = 15; joueur.mana = 50; joueur.manaMax = 1000; joueur.competenceSpeciale = "Boule de feu"; }
    else if (classeChoisie == 3) { joueur.vie = 95;  joueur.vieMax = 95;  joueur.attaque = 16; joueur.defense = 35; joueur.mana = 20; joueur.manaMax = 50;   joueur.competenceSpeciale = "Tir de Precision"; }
    else if (classeChoisie == 4) { joueur.vie = 140; joueur.vieMax = 140; joueur.attaque = 10; joueur.defense = 85; joueur.mana = 30; joueur.manaMax = 60;   joueur.competenceSpeciale = "Soin Divin"; }
    else if (classeChoisie == 5) { joueur.vie = 75;  joueur.vieMax = 75;  joueur.attaque = 25; joueur.defense = 10; joueur.mana = 80; joueur.manaMax = 200;  joueur.competenceSpeciale = "Malédiction"; }
    else if (classeChoisie == 6) { joueur.vie = 85;  joueur.vieMax = 85;  joueur.attaque = 20; joueur.defense = 25; joueur.mana = 15; joueur.manaMax = 30;   joueur.competenceSpeciale = "Attaque Sournoise"; }

    return true;
}