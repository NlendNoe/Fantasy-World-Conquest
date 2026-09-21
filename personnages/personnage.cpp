#ifndef PERSONNAGE_CPP
#define PERSONNAGE_CPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include "../structure.h"
#include "../ui_utils.h"

inline bool creerPersonnageSFML(sf::RenderWindow &window, sf::Font &font, Joueur &joueur)
{
    std::string nomSaisi = "";
    int classeChoisie = 1;
    bool creationTerminee = false;

    float screenW = static_cast<float>(window.getSize().x);
    float screenH = static_cast<float>(window.getSize().y);
    float centerX = screenW / 2.0f;

    sf::Texture textureFond;
    bool fondCharge = textureFond.loadFromFile("assets/backgrounds/perso.jpg");
    sf::Sprite spriteFond;
    if (fondCharge)
    {
        spriteFond.setTexture(textureFond);
        sf::Vector2u sz = textureFond.getSize();
        spriteFond.setScale(screenW / sz.x, screenH / sz.y);
        spriteFond.setColor(sf::Color(170, 175, 190));
    }

    float btnRetourW = screenW * 0.11f;
    float btnRetourH = screenH * 0.05f;
    sf::RectangleShape btnRetour(sf::Vector2f(btnRetourW, btnRetourH));
    btnRetour.setPosition(screenW * 0.03f, screenH * 0.03f);
    btnRetour.setFillColor(sf::Color(140, 35, 35));
    btnRetour.setOutlineThickness(0.0f);

    sf::Text txtRetour("RETOUR", font, static_cast<unsigned int>(btnRetourH * 0.40f));
    txtRetour.setFillColor(UI::TextWhite);
    sf::FloatRect boundsRetour = txtRetour.getLocalBounds();
    txtRetour.setOrigin(boundsRetour.left + boundsRetour.width / 2.0f, boundsRetour.top + boundsRetour.height / 2.0f);
    txtRetour.setPosition(btnRetour.getPosition().x + (btnRetourW / 2.0f), btnRetour.getPosition().y + (btnRetourH / 2.0f));

    sf::Texture texGuerrier, texMage, texArcher, texPaladin, texNecro, texAssassin;
    texGuerrier.loadFromFile("assets/pictures/dark_skinned_knight.png");
    texMage.loadFromFile("assets/pictures/robe.png");
    texArcher.loadFromFile("assets/pictures/leather_armor.png");
    texPaladin.loadFromFile("assets/pictures/plate_armor.png");
    texNecro.loadFromFile("assets/pictures/robed_skeleton_spellcast.png");
    texAssassin.loadFromFile("assets/pictures/chain_armor_bandit.png");

    sf::Sprite sprites[6];
    sprites[0].setTexture(texGuerrier);
    sprites[1].setTexture(texMage);
    sprites[2].setTexture(texArcher);
    sprites[3].setTexture(texPaladin);
    sprites[4].setTexture(texNecro);
    sprites[5].setTexture(texAssassin);

    for (int i = 0; i < 6; ++i)
    {
        sprites[i].setTextureRect(sf::IntRect(0, 128, 64, 64));
        sprites[i].setOrigin(32.0f, 32.0f);
        sprites[i].setScale(screenH * 0.0055f, screenH * 0.0055f);
        sprites[i].setPosition(centerX, screenH * 0.72f);
    }

    sf::Text titre("CREATION DU HEROS", font, static_cast<unsigned int>(screenH * 0.05f));
    titre.setFillColor(UI::Silver);
    sf::FloatRect bTitre = titre.getLocalBounds();
    titre.setOrigin(bTitre.left + bTitre.width / 2.0f, bTitre.top + bTitre.height / 2.0f);
    titre.setPosition(centerX, screenH * 0.05f);

    sf::Text consigneNom("1. Entrez le nom de votre champion :", font, static_cast<unsigned int>(screenH * 0.024f));
    consigneNom.setFillColor(UI::TextMuted);
    sf::FloatRect bNom = consigneNom.getLocalBounds();
    consigneNom.setOrigin(bNom.left + bNom.width / 2.0f, bNom.top + bNom.height / 2.0f);
    consigneNom.setPosition(centerX, screenH * 0.11f);

    float largeurNom = screenW * 0.36f;
    float hauteurNom = screenH * 0.055f;
    sf::RectangleShape champNom(sf::Vector2f(largeurNom, hauteurNom));
    champNom.setOrigin(largeurNom / 2.0f, hauteurNom / 2.0f);
    champNom.setPosition(centerX, screenH * 0.155f);
    champNom.setFillColor(UI::DarkPanel);
    champNom.setOutlineThickness(1.0f);
    champNom.setOutlineColor(UI::BorderMuted);

    sf::Text texteNom("", font, static_cast<unsigned int>(hauteurNom * 0.45f));
    texteNom.setFillColor(UI::TextWhite);

    sf::Text consigneClasse("2. Choisissez votre ordre de combat :", font, static_cast<unsigned int>(screenH * 0.024f));
    consigneClasse.setFillColor(UI::TextMuted);
    sf::FloatRect bClasse = consigneClasse.getLocalBounds();
    consigneClasse.setOrigin(bClasse.left + bClasse.width / 2.0f, bClasse.top + bClasse.height / 2.0f);
    consigneClasse.setPosition(centerX, screenH * 0.21f);

    float largeurBtn = screenW * 0.17f;
    float hauteurBtn = screenH * 0.085f;
    float espacementH = screenW * 0.015f;
    float espacementV = screenH * 0.02f;

    float col1 = centerX - (1.5f * largeurBtn + espacementH);
    float col2 = centerX - (0.5f * largeurBtn);
    float col3 = centerX + (0.5f * largeurBtn + espacementH);

    float row1Y = screenH * 0.25f;
    float row2Y = row1Y + hauteurBtn + espacementV;

    unsigned int szClasseTxt = static_cast<unsigned int>(hauteurBtn * 0.22f);

    sf::RectangleShape btnClasses[6];
    sf::Text txtClasses[6];

    struct InfoClasse {
        std::string nom;
        std::string stats;
        float x, y;
    } infos[6] = {
        {"Guerrier", "PV: 130 | ATQ: 14 | DEF: 25\nSort: Coup Puissant", col1, row1Y},
        {"Mage", "PV: 90 | ATQ: 25 | DEF: 10\nSort: Boule de Feu", col2, row1Y},
        {"Archer", "PV: 105 | ATQ: 18 | DEF: 15\nSort: Tir de Precision", col3, row1Y},
        {"Paladin", "PV: 150 | ATQ: 12 | DEF: 35\nSort: Soin Divin", col1, row2Y},
        {"Necromancien", "PV: 85 | ATQ: 22 | DEF: 12\nSort: Malediction", col2, row2Y},
        {"Assassin", "PV: 95 | ATQ: 21 | DEF: 14\nSort: Attaque Sournoise", col3, row2Y}
    };

    for (int i = 0; i < 6; ++i)
    {
        btnClasses[i].setSize(sf::Vector2f(largeurBtn, hauteurBtn));
        btnClasses[i].setPosition(infos[i].x, infos[i].y);
        btnClasses[i].setOutlineThickness(0.0f);

        txtClasses[i].setFont(font);
        txtClasses[i].setCharacterSize(szClasseTxt);
        txtClasses[i].setString(infos[i].nom + "\n" + infos[i].stats);
        txtClasses[i].setPosition(infos[i].x + 12.0f, infos[i].y + 10.0f);
    }

    float btnValW = screenW * 0.22f;
    float btnValH = screenH * 0.065f;
    sf::RectangleShape btnValider(sf::Vector2f(btnValW, btnValH));
    btnValider.setOrigin(btnValW / 2.0f, btnValH / 2.0f);
    btnValider.setPosition(centerX, screenH * 0.92f);
    btnValider.setFillColor(sf::Color(35, 120, 55));
    btnValider.setOutlineThickness(0.0f);

    sf::Text txtValider("COMMENCER L'AVENTURE", font, static_cast<unsigned int>(btnValH * 0.40f));
    txtValider.setFillColor(UI::TextWhite);
    sf::FloatRect bVal = txtValider.getLocalBounds();
    txtValider.setOrigin(bVal.left + bVal.width / 2.0f, bVal.top + bVal.height / 2.0f);
    txtValider.setPosition(btnValider.getPosition());

    sf::Clock horlogeAnim;

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
                else if (event.text.unicode >= 32 && event.text.unicode < 127 && nomSaisi.length() < 16)
                {
                    nomSaisi += static_cast<char>(event.text.unicode);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (btnRetour.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    return false;
                }

                for (int i = 0; i < 6; ++i)
                {
                    if (btnClasses[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        classeChoisie = i + 1;
                    }
                }

                if (btnValider.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    if (nomSaisi.empty())
                    {
                        nomSaisi = "Légendaire";
                    }
                    creationTerminee = true;
                }
            }
        }

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (btnRetour.getGlobalBounds().contains(mousePos.x, mousePos.y))
            btnRetour.setFillColor(sf::Color(180, 45, 45));
        else
            btnRetour.setFillColor(sf::Color(135, 30, 30));

        for (int i = 0; i < 6; ++i)
        {
            if (classeChoisie == i + 1)
            {
                btnClasses[i].setFillColor(sf::Color(45, 80, 140, 240));
                txtClasses[i].setFillColor(UI::Azure);
            }
            else if (btnClasses[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                btnClasses[i].setFillColor(sf::Color(35, 48, 75, 220));
                txtClasses[i].setFillColor(UI::TextWhite);
            }
            else
            {
                btnClasses[i].setFillColor(UI::DarkPanel);
                txtClasses[i].setFillColor(UI::TextMuted);
            }
        }

        if (btnValider.getGlobalBounds().contains(mousePos.x, mousePos.y))
            btnValider.setFillColor(sf::Color(45, 155, 70));
        else
            btnValider.setFillColor(sf::Color(32, 115, 50));

        window.clear(sf::Color(10, 12, 18));
        if (fondCharge) window.draw(spriteFond);

        sf::RectangleShape overlay(sf::Vector2f(screenW, screenH));
        overlay.setFillColor(sf::Color(10, 12, 20, 175));
        window.draw(overlay);

        window.draw(btnRetour);
        window.draw(txtRetour);

        window.draw(titre);
        window.draw(consigneNom);
        window.draw(champNom);

        std::string displayNom = nomSaisi.empty() ? "Cliquez et tapez votre nom..." : nomSaisi;
        texteNom.setString(displayNom);
        texteNom.setFillColor(nomSaisi.empty() ? UI::TextMuted : UI::Azure);
        sf::FloatRect bTN = texteNom.getLocalBounds();
        texteNom.setOrigin(bTN.left + bTN.width / 2.0f, bTN.top + bTN.height / 2.0f);
        texteNom.setPosition(champNom.getPosition());
        window.draw(texteNom);

        window.draw(consigneClasse);

        for (int i = 0; i < 6; ++i)
        {
            window.draw(btnClasses[i]);
            window.draw(txtClasses[i]);
        }

        int idx = classeChoisie - 1;
        float bobbing = std::sin(horlogeAnim.getElapsedTime().asSeconds() * 3.5f) * 2.5f;
        sprites[idx].setPosition(centerX, screenH * 0.72f + bobbing);
        sprites[idx].setTextureRect(sf::IntRect(0, 128, 64, 64));
        window.draw(sprites[idx]);

        window.draw(btnValider);
        window.draw(txtValider);

        window.display();
    }

    if (nomSaisi.empty()) nomSaisi = "Héros";

    joueur.nom = nomSaisi;
    joueur.niveau = 1;
    joueur.classeIndex = classeChoisie - 1;
    joueur.xp = 0;
    joueur.xpSeuil = 100;
    joueur.orJoueur = 50;
    joueur.monstresVaincus = 0;

    joueur.inventaire.potionsNormales = 3;
    joueur.inventaire.grandesPotions = 1;
    joueur.inventaire.nombreBouclier = 2;
    joueur.inventaire.potionsMana = 2;
    joueur.inventaire.capaciteSac = 5;

    if (classeChoisie == 1)
    {
        joueur.classeNom = "Guerrier";
        joueur.vie = 130; joueur.vieMax = 130;
        joueur.attaque = 14; joueur.defense = 25;
        joueur.mana = 20; joueur.manaMax = 20;
        joueur.chanceCrit = 15;
        joueur.competenceSpeciale = "Coup Puissant";
    }
    else if (classeChoisie == 2)
    {
        joueur.classeNom = "Mage";
        joueur.vie = 90; joueur.vieMax = 90;
        joueur.attaque = 25; joueur.defense = 10;
        joueur.mana = 70; joueur.manaMax = 70;
        joueur.chanceCrit = 10;
        joueur.competenceSpeciale = "Boule de Feu";
    }
    else if (classeChoisie == 3)
    {
        joueur.classeNom = "Archer";
        joueur.vie = 105; joueur.vieMax = 105;
        joueur.attaque = 18; joueur.defense = 15;
        joueur.mana = 30; joueur.manaMax = 30;
        joueur.chanceCrit = 30;
        joueur.competenceSpeciale = "Tir de Precision";
    }
    else if (classeChoisie == 4)
    {
        joueur.classeNom = "Paladin";
        joueur.vie = 150; joueur.vieMax = 150;
        joueur.attaque = 12; joueur.defense = 35;
        joueur.mana = 40; joueur.manaMax = 40;
        joueur.chanceCrit = 10;
        joueur.competenceSpeciale = "Soin Divin";
    }
    else if (classeChoisie == 5)
    {
        joueur.classeNom = "Necromancien";
        joueur.vie = 85; joueur.vieMax = 85;
        joueur.attaque = 22; joueur.defense = 12;
        joueur.mana = 75; joueur.manaMax = 75;
        joueur.chanceCrit = 15;
        joueur.competenceSpeciale = "Malediction";
    }
    else if (classeChoisie == 6)
    {
        joueur.classeNom = "Assassin";
        joueur.vie = 95; joueur.vieMax = 95;
        joueur.attaque = 21; joueur.defense = 14;
        joueur.mana = 30; joueur.manaMax = 30;
        joueur.chanceCrit = 35;
        joueur.competenceSpeciale = "Attaque Sournoise";
    }

    return true;
}

#endif