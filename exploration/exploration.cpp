#ifndef EXPLORATION_CPP
#define EXPLORATION_CPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#include "../structure.h"
#include "../ui_utils.h"
#include "../audio.h"
#include "../sauvegarde.h"
#include "../bestiaire/bestiaire.cpp"
#include "../combat/combat.cpp"

inline bool afficherBoutiqueSFML(sf::RenderWindow &window, sf::Font &font, Joueur &joueur)
{
    float screenW = static_cast<float>(window.getSize().x);
    float screenH = static_cast<float>(window.getSize().y);
    float centerX = screenW / 2.0f;

    sf::Texture texFond;
    bool fondCharge = texFond.loadFromFile("assets/backgrounds/fond-principale.jpg");
    sf::Sprite spriteFond;
    if (fondCharge)
    {
        spriteFond.setTexture(texFond);
        sf::Vector2u sz = texFond.getSize();
        spriteFond.setScale(screenW / sz.x, screenH / sz.y);
    }

    sf::Texture texEpee, texBouclier, texArmure, texPotionSoin, texPotionMana, texGrandePotion, texParchemin, texSac;
    texEpee.loadFromFile("assets/pictures/epee/epee.png");
    texBouclier.loadFromFile("assets/pictures/bouclier/bouclier.png");
    texArmure.loadFromFile("assets/pictures/armure/armure.png");
    texPotionSoin.loadFromFile("assets/pictures/potion_soin/potion_soin.png");
    texPotionMana.loadFromFile("assets/pictures/potion_mana/potion_mana.png");
    texGrandePotion.loadFromFile("assets/pictures/grande_potion/grande_potion.png");
    texParchemin.loadFromFile("assets/pictures/parchemin/parchemin.png");
    texSac.loadFromFile("assets/pictures/sac/sac.png");

    sf::Texture* texturesObjets[8] = {
        &texEpee, &texBouclier, &texArmure, &texPotionSoin,
        &texPotionMana, &texGrandePotion, &texParchemin, &texSac
    };

    struct ItemBoutique {
        std::string nom;
        std::string categorie;
        std::string description;
        int prix;
        int type;
        int valeur;
        int iconIdx;
    };

    std::vector<ItemBoutique> items = {
        {"Épée de Fer", "Arme", "Lame forgee en fer solide.\nAugmente l'Attaque permanente de 5 points.", 15, 0, 5, 0},
        {"Épée d'Acier", "Arme", "Fine epee trempee dans l'acier.\nAugmente l'Attaque permanente de 15 points.", 40, 0, 15, 0},
        {"Lame Runique", "Arme", "Lame gravee d'anciennes runes de bravoure.\nAugmente l'Attaque permanente de 25 points.", 75, 0, 25, 0},
        {"Claymore Vorpale", "Arme", "Lourde epee capable de fendre les armures.\nAugmente l'Attaque permanente de 40 points.", 130, 0, 40, 0},
        {"Épée du Héros", "Arme", "Arme mythique doree des champions du royaume.\nAugmente l'Attaque permanente de 75 points.", 250, 0, 75, 0},
        {"Bouclier en Bois", "Bouclier", "Rondache en chene renforcee de ferrures.\nAjoute 1 Bouclier dans l'inventaire.", 35, 1, 1, 1},
        {"Bouclier d'Acier", "Bouclier", "Robuste ecu en acier trempe.\nAjoute 2 Boucliers dans l'inventaire.", 65, 1, 2, 1},
        {"Cotte de Mailles", "Armure", "Armure de mailles souple et protectrice.\nAccroît les PV Max de 20 et restaure 20 PV.", 80, 1, 20, 2},
        {"Armure de Plaques", "Armure", "Harnois complet d'acier massif.\nAccroît les PV Max de 45 et la Defense de 10.", 180, 1, 45, 2},
        {"Potion de Soin", "Consommable", "Fiole curative restaure 35 PV en combat\nou lors de vos explorations.", 20, 2, 1, 3},
        {"Grande Potion", "Consommable", "Precieuse liqueur restaurant instantanement\n100% de la sante maximale du heros.", 50, 4, 1, 5},
        {"Potion de Mana", "Consommable", "Fiole d'energie arcanique restituant\n35 Points de Mana lors des affrontements.", 25, 3, 1, 4},
        {"Parchemin de Force", "Magie", "Parchemin runique millenaire.\nConfere un gain definitif de 6 points d'Attaque.", 100, 5, 6, 6},
        {"Agrandir Sac", "Amélioration", "Ceinture de cuir et sacoches suplementaires.\nPermet de transporter 2 objets de plus.", 70, 6, 2, 7}
    };

    int itemSelectionne = 0;
    std::string messageNotif = "Cliquez sur un article pour voir ses details et l'acquerir.";
    sf::Color couleurNotif = UI::Silver;

    float listX = screenW * 0.05f;
    float listY = screenH * 0.16f;
    float listW = screenW * 0.42f;
    float rowH = screenH * 0.048f;
    float gapRow = screenH * 0.005f;

    float rightX = screenW * 0.50f;
    float rightY = screenH * 0.16f;
    float rightW = screenW * 0.45f;
    float rightH = screenH * 0.74f;

    float btnRetW = screenW * 0.20f;
    float btnRetH = screenH * 0.060f;
    sf::RectangleShape btnRetour(sf::Vector2f(btnRetW, btnRetH));
    btnRetour.setPosition(listX, screenH * 0.915f);
    btnRetour.setFillColor(UI::DarkPanelLight);
    btnRetour.setOutlineThickness(0.0f);

    sf::Text txtRetour("RETOUR A LA CITE", font, static_cast<unsigned int>(btnRetH * 0.40f));
    txtRetour.setFillColor(UI::TextWhite);
    sf::FloatRect bRet = txtRetour.getLocalBounds();
    txtRetour.setOrigin(bRet.left + bRet.width / 2.0f, bRet.top + bRet.height / 2.0f);
    txtRetour.setPosition(btnRetour.getPosition().x + btnRetW / 2.0f, btnRetour.getPosition().y + btnRetH / 2.0f);

    float btnAchatW = rightW * 0.85f;
    float btnAchatH = screenH * 0.065f;
    sf::RectangleShape btnAcheter(sf::Vector2f(btnAchatW, btnAchatH));
    btnAcheter.setPosition(rightX + (rightW - btnAchatW) / 2.0f, rightY + rightH - btnAchatH - screenH * 0.035f);
    btnAcheter.setFillColor(sf::Color(35, 115, 60));
    btnAcheter.setOutlineThickness(0.0f);

    sf::Text txtAcheter("ACHETER L'OBJET", font, static_cast<unsigned int>(btnAchatH * 0.40f));
    txtAcheter.setFillColor(UI::TextWhite);
    sf::FloatRect bAch = txtAcheter.getLocalBounds();
    txtAcheter.setOrigin(bAch.left + bAch.width / 2.0f, bAch.top + bAch.height / 2.0f);
    txtAcheter.setPosition(btnAcheter.getPosition().x + btnAchatW / 2.0f, btnAcheter.getPosition().y + btnAchatH / 2.0f);

    sf::Sprite spriteApercu;
    spriteApercu.setOrigin(64.0f, 64.0f);
    spriteApercu.setScale(1.4f, 1.4f);
    spriteApercu.setPosition(rightX + rightW / 2.0f, rightY + screenH * 0.16f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return false;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (btnRetour.getGlobalBounds().contains(mPos.x, mPos.y))
                {
                    return true;
                }

                for (size_t i = 0; i < items.size(); ++i)
                {
                    float y = listY + i * (rowH + gapRow);
                    sf::FloatRect boundsRow(listX, y, listW, rowH);
                    if (boundsRow.contains(mPos.x, mPos.y))
                    {
                        itemSelectionne = static_cast<int>(i);
                    }
                }

                if (btnAcheter.getGlobalBounds().contains(mPos.x, mPos.y))
                {
                    ItemBoutique &itemActuel = items[itemSelectionne];
                    if (joueur.orJoueur >= itemActuel.prix)
                    {
                        joueur.orJoueur -= itemActuel.prix;

                        if (itemActuel.type == 0)
                        {
                            joueur.attaque += itemActuel.valeur;
                            messageNotif = "Succes: " + itemActuel.nom + " equipee (+ " + std::to_string(itemActuel.valeur) + " ATQ) !";
                        }
                        else if (itemActuel.type == 1)
                        {
                            if (itemActuel.nom.find("Bouclier") != std::string::npos)
                            {
                                joueur.inventaire.nombreBouclier += itemActuel.valeur;
                                messageNotif = "Succes: " + itemActuel.nom + " ajoute au sac (+ " + std::to_string(itemActuel.valeur) + ") !";
                            }
                            else if (itemActuel.nom.find("Cotte") != std::string::npos)
                            {
                                joueur.vieMax += itemActuel.valeur;
                                joueur.vie += itemActuel.valeur;
                                messageNotif = "Succes: " + itemActuel.nom + " portee (+ " + std::to_string(itemActuel.valeur) + " PV Max) !";
                            }
                            else
                            {
                                joueur.vieMax += itemActuel.valeur;
                                joueur.vie += itemActuel.valeur;
                                joueur.defense += 10;
                                messageNotif = "Succes: " + itemActuel.nom + " (+45 PV Max, +10 DEF) !";
                            }
                        }
                        else if (itemActuel.type == 2)
                        {
                            joueur.inventaire.potionsNormales += itemActuel.valeur;
                            messageNotif = "Succes: Potion de Soin rangee dans votre sac !";
                        }
                        else if (itemActuel.type == 3)
                        {
                            joueur.inventaire.potionsMana += itemActuel.valeur;
                            messageNotif = "Succes: Potion de Mana rangee dans votre sac !";
                        }
                        else if (itemActuel.type == 4)
                        {
                            joueur.inventaire.grandesPotions += itemActuel.valeur;
                            messageNotif = "Succes: Grande Potion rangee dans votre sac !";
                        }
                        else if (itemActuel.type == 5)
                        {
                            joueur.attaque += itemActuel.valeur;
                            messageNotif = "Succes: Parchemin de Force utilise (+6 ATQ permanent) !";
                        }
                        else if (itemActuel.type == 6)
                        {
                            joueur.inventaire.capaciteSac += itemActuel.valeur;
                            messageNotif = "Succes: Capacite du sac etendue (+2 places) !";
                        }

                        couleurNotif = UI::GreenHP;
                    }
                    else
                    {
                        messageNotif = "Or insuffisant pour acheter: " + itemActuel.nom + " !";
                        couleurNotif = UI::Crimson;
                    }
                }
            }
        }

        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        window.clear(sf::Color(10, 12, 18));
        if (fondCharge) window.draw(spriteFond);

        sf::RectangleShape voile(sf::Vector2f(screenW, screenH));
        voile.setFillColor(sf::Color(10, 14, 24, 215));
        window.draw(voile);

        UI::drawPanel(window, screenW * 0.05f, screenH * 0.02f, screenW * 0.90f, screenH * 0.09f);
        sf::Text txtTitreShop("BOUTIQUE ROYALE DE RIVENOIR", font, static_cast<unsigned int>(screenH * 0.038f));
        txtTitreShop.setFillColor(UI::Silver);
        txtTitreShop.setPosition(screenW * 0.08f, screenH * 0.035f);
        window.draw(txtTitreShop);

        sf::Text txtOrShop("Bourse : " + std::to_string(joueur.orJoueur) + " PO", font, static_cast<unsigned int>(screenH * 0.030f));
        txtOrShop.setFillColor(UI::AmberGold);
        sf::FloatRect bOrS = txtOrShop.getLocalBounds();
        txtOrShop.setPosition(screenW * 0.92f - bOrS.width, screenH * 0.04f);
        window.draw(txtOrShop);

        sf::Text txtNotif(messageNotif, font, static_cast<unsigned int>(screenH * 0.020f));
        txtNotif.setFillColor(couleurNotif);
        sf::FloatRect bN = txtNotif.getLocalBounds();
        txtNotif.setOrigin(bN.left + bN.width / 2.0f, bN.top + bN.height / 2.0f);
        txtNotif.setPosition(centerX, screenH * 0.130f);
        window.draw(txtNotif);

        for (size_t i = 0; i < items.size(); ++i)
        {
            float y = listY + i * (rowH + gapRow);
            sf::FloatRect boundsRow(listX, y, listW, rowH);
            bool isSelected = (itemSelectionne == static_cast<int>(i));
            bool isHovered = boundsRow.contains(mPos.x, mPos.y);

            sf::RectangleShape row(sf::Vector2f(listW, rowH));
            row.setPosition(listX, y);
            row.setOutlineThickness(0.0f);

            if (isSelected)
                row.setFillColor(sf::Color(45, 75, 135, 240));
            else if (isHovered)
                row.setFillColor(sf::Color(32, 45, 70, 210));
            else
                row.setFillColor(UI::DarkPanel);

            window.draw(row);

            sf::Text txtNom(items[i].nom, font, static_cast<unsigned int>(rowH * 0.44f));
            txtNom.setFillColor(isSelected ? UI::Azure : UI::TextWhite);
            txtNom.setPosition(listX + 15.0f, y + rowH * 0.22f);
            window.draw(txtNom);

            sf::Text txtCat(items[i].categorie, font, static_cast<unsigned int>(rowH * 0.36f));
            txtCat.setFillColor(UI::TextMuted);
            txtCat.setPosition(listX + listW * 0.58f, y + rowH * 0.26f);
            window.draw(txtCat);

            sf::Text txtPrix(std::to_string(items[i].prix) + " PO", font, static_cast<unsigned int>(rowH * 0.44f));
            txtPrix.setFillColor(joueur.orJoueur >= items[i].prix ? UI::AmberGold : UI::Crimson);
            sf::FloatRect bP = txtPrix.getLocalBounds();
            txtPrix.setPosition(listX + listW - bP.width - 15.0f, y + rowH * 0.22f);
            window.draw(txtPrix);
        }

        UI::drawPanel(window, rightX, rightY, rightW, rightH, "FICHE DETAILLEE DE L'ARTICLE", &font, 20);

        ItemBoutique &itemActuel = items[itemSelectionne];

        float frameBoxSz = screenH * 0.22f;
        sf::RectangleShape boxImage(sf::Vector2f(frameBoxSz, frameBoxSz));
        boxImage.setOrigin(frameBoxSz / 2.0f, frameBoxSz / 2.0f);
        boxImage.setPosition(rightX + rightW / 2.0f, rightY + screenH * 0.16f);
        boxImage.setFillColor(sf::Color(18, 24, 38, 240));
        boxImage.setOutlineThickness(1.0f);
        boxImage.setOutlineColor(UI::BorderMuted);
        window.draw(boxImage);

        int iconId = itemActuel.iconIdx;
        spriteApercu.setTexture(*texturesObjets[iconId]);
        spriteApercu.setPosition(boxImage.getPosition());
        window.draw(spriteApercu);

        sf::Text txtNomGrand(itemActuel.nom, font, static_cast<unsigned int>(screenH * 0.034f));
        txtNomGrand.setFillColor(UI::Silver);
        sf::FloatRect bNG = txtNomGrand.getLocalBounds();
        txtNomGrand.setOrigin(bNG.left + bNG.width / 2.0f, bNG.top + bNG.height / 2.0f);
        txtNomGrand.setPosition(rightX + rightW / 2.0f, rightY + screenH * 0.32f);
        window.draw(txtNomGrand);

        sf::Text txtType("Categorie : " + itemActuel.categorie, font, static_cast<unsigned int>(screenH * 0.022f));
        txtType.setFillColor(UI::Azure);
        sf::FloatRect bT = txtType.getLocalBounds();
        txtType.setOrigin(bT.left + bT.width / 2.0f, bT.top + bT.height / 2.0f);
        txtType.setPosition(rightX + rightW / 2.0f, rightY + screenH * 0.365f);
        window.draw(txtType);

        sf::Text txtPrixGrand("Prix d'acquisition : " + std::to_string(itemActuel.prix) + " Pieces d'Or", font, static_cast<unsigned int>(screenH * 0.026f));
        txtPrixGrand.setFillColor(joueur.orJoueur >= itemActuel.prix ? UI::AmberGold : UI::Crimson);
        sf::FloatRect bPG = txtPrixGrand.getLocalBounds();
        txtPrixGrand.setOrigin(bPG.left + bPG.width / 2.0f, bPG.top + bPG.height / 2.0f);
        txtPrixGrand.setPosition(rightX + rightW / 2.0f, rightY + screenH * 0.41f);
        window.draw(txtPrixGrand);

        sf::RectangleShape sepDesc(sf::Vector2f(rightW * 0.85f, 1.0f));
        sepDesc.setOrigin(sepDesc.getSize().x / 2.0f, 0.5f);
        sepDesc.setPosition(rightX + rightW / 2.0f, rightY + screenH * 0.455f);
        sepDesc.setFillColor(UI::BorderMuted);
        window.draw(sepDesc);

        sf::Text txtDesc(itemActuel.description, font, static_cast<unsigned int>(screenH * 0.023f));
        txtDesc.setFillColor(UI::TextWhite);
        sf::FloatRect bD = txtDesc.getLocalBounds();
        txtDesc.setOrigin(bD.left + bD.width / 2.0f, bD.top + bD.height / 2.0f);
        txtDesc.setPosition(rightX + rightW / 2.0f, rightY + screenH * 0.525f);
        window.draw(txtDesc);

        std::string statutActuel = "";
        if (itemActuel.type == 0) statutActuel = "Votre Attaque actuelle : " + std::to_string(joueur.attaque);
        else if (itemActuel.type == 1 && itemActuel.nom.find("Bouclier") != std::string::npos) statutActuel = "Boucliers en stock : " + std::to_string(joueur.inventaire.nombreBouclier);
        else if (itemActuel.type == 1) statutActuel = "Vos PV Max actuels : " + std::to_string(joueur.vieMax) + " | DEF: " + std::to_string(joueur.defense);
        else if (itemActuel.type == 2) statutActuel = "Potions de soin en stock : " + std::to_string(joueur.inventaire.potionsNormales);
        else if (itemActuel.type == 3) statutActuel = "Potions de mana en stock : " + std::to_string(joueur.inventaire.potionsMana);
        else if (itemActuel.type == 4) statutActuel = "Grandes potions en stock : " + std::to_string(joueur.inventaire.grandesPotions);
        else if (itemActuel.type == 6) statutActuel = "Capacite de votre sac : " + std::to_string(joueur.inventaire.capaciteSac) + " objets";

        if (!statutActuel.empty())
        {
            sf::Text txtStatut(statutActuel, font, static_cast<unsigned int>(screenH * 0.021f));
            txtStatut.setFillColor(UI::TextMuted);
            sf::FloatRect bS = txtStatut.getLocalBounds();
            txtStatut.setOrigin(bS.left + bS.width / 2.0f, bS.top + bS.height / 2.0f);
            txtStatut.setPosition(rightX + rightW / 2.0f, rightY + screenH * 0.60f);
            window.draw(txtStatut);
        }

        UI::drawButton(window, btnAcheter, txtAcheter, btnAcheter.getGlobalBounds().contains(mPos.x, mPos.y),
                       joueur.orJoueur >= itemActuel.prix ? sf::Color(35, 120, 60) : sf::Color(70, 70, 75),
                       joueur.orJoueur >= itemActuel.prix ? sf::Color(48, 160, 80) : sf::Color(80, 80, 85),
                       UI::TextWhite,
                       UI::TextWhite);

        UI::drawButton(window, btnRetour, txtRetour, btnRetour.getGlobalBounds().contains(mPos.x, mPos.y));

        window.display();
    }

    return true;
}

inline bool explorerMondeSFML(sf::RenderWindow &window, sf::Font &font, Joueur &joueur, int &zoneActuelle, int &territoiresConquis)
{
    float screenW = static_cast<float>(window.getSize().x);
    float screenH = static_cast<float>(window.getSize().y);
    float centerX = screenW / 2.0f;

    AudioJeu::jouer("assets/Music/Woodland Fantasy _ OpenGameArt.org.ogg", 30.0f, true);

    sf::Texture texFond;
    bool fondCharge = texFond.loadFromFile("assets/backgrounds/font2.jpg");
    sf::Sprite spriteFond;
    if (fondCharge)
    {
        spriteFond.setTexture(texFond);
        sf::Vector2u sz = texFond.getSize();
        spriteFond.setScale(screenW / sz.x, screenH / sz.y);
    }

    sf::Texture texHero;
    std::string cheminPerso = "assets/pictures/dark_skinned_knight.png";
    if (joueur.classeIndex == 1) cheminPerso = "assets/pictures/robe.png";
    else if (joueur.classeIndex == 2) cheminPerso = "assets/pictures/leather_armor.png";
    else if (joueur.classeIndex == 3) cheminPerso = "assets/pictures/plate_armor.png";
    else if (joueur.classeIndex == 4) cheminPerso = "assets/pictures/robed_skeleton_spellcast.png";
    else if (joueur.classeIndex == 5) cheminPerso = "assets/pictures/chain_armor_bandit.png";

    texHero.loadFromFile(cheminPerso);
    sf::Sprite spriteHero(texHero);
    spriteHero.setTextureRect(sf::IntRect(0, 128, 64, 64));
    spriteHero.setOrigin(32.0f, 32.0f);
    spriteHero.setScale(screenH * 0.007f, screenH * 0.007f);
    spriteHero.setPosition(centerX, screenH * 0.44f);

    std::string zoneNom = getNomZone(zoneActuelle);
    std::string narration = "Vous avancez a pas de loup au coeur de " + zoneNom + "...\nLe vent souffle a travers les arbres centenaires.";

    std::vector<UI::FloatingText> floatingTexts;

    float btnW = screenW * 0.22f;
    float btnH = screenH * 0.065f;
    float actionsY = screenH * 0.88f;

    sf::RectangleShape btnAvancer(sf::Vector2f(btnW, btnH));
    btnAvancer.setOrigin(btnW / 2.0f, btnH / 2.0f);
    btnAvancer.setPosition(screenW * 0.26f, actionsY);
    btnAvancer.setFillColor(UI::DarkPanelLight);
    btnAvancer.setOutlineThickness(0.0f);

    sf::Text txtAvancer("1. AVANCER", font, static_cast<unsigned int>(btnH * 0.40f));
    txtAvancer.setFillColor(UI::TextWhite);
    sf::FloatRect bAv = txtAvancer.getLocalBounds();
    txtAvancer.setOrigin(bAv.left + bAv.width / 2.0f, bAv.top + bAv.height / 2.0f);
    txtAvancer.setPosition(btnAvancer.getPosition());

    sf::RectangleShape btnPotion(sf::Vector2f(btnW, btnH));
    btnPotion.setOrigin(btnW / 2.0f, btnH / 2.0f);
    btnPotion.setPosition(screenW * 0.50f, actionsY);
    btnPotion.setFillColor(UI::DarkPanelLight);
    btnPotion.setOutlineThickness(0.0f);

    sf::Text txtPotion("2. BOIRE POTION", font, static_cast<unsigned int>(btnH * 0.40f));
    txtPotion.setFillColor(UI::TextWhite);
    sf::FloatRect bPot = txtPotion.getLocalBounds();
    txtPotion.setOrigin(bPot.left + bPot.width / 2.0f, bPot.top + bPot.height / 2.0f);
    txtPotion.setPosition(btnPotion.getPosition());

    sf::RectangleShape btnVille(sf::Vector2f(btnW, btnH));
    btnVille.setOrigin(btnW / 2.0f, btnH / 2.0f);
    btnVille.setPosition(screenW * 0.74f, actionsY);
    btnVille.setFillColor(UI::DarkPanelLight);
    btnVille.setOutlineThickness(0.0f);

    sf::Text txtVille("3. RETOUR CITE", font, static_cast<unsigned int>(btnH * 0.40f));
    txtVille.setFillColor(UI::TextWhite);
    sf::FloatRect bVi = txtVille.getLocalBounds();
    txtVille.setOrigin(bVi.left + bVi.width / 2.0f, bVi.top + bVi.height / 2.0f);
    txtVille.setPosition(btnVille.getPosition());

    sf::Clock animClock;
    sf::Clock deltaClock;

    while (window.isOpen() && joueur.vie > 0)
    {
        float dt = deltaClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return false;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (btnVille.getGlobalBounds().contains(mPos.x, mPos.y))
                {
                    AudioJeu::jouer("assets/Music/Town Theme RPG _ OpenGameArt.org.ogg", 30.0f, true);
                    return true;
                }
                else if (btnPotion.getGlobalBounds().contains(mPos.x, mPos.y))
                {
                    if (joueur.inventaire.potionsNormales > 0 && joueur.vie < joueur.vieMax)
                    {
                        joueur.inventaire.potionsNormales--;
                        int soin = 35;
                        joueur.vie = std::min(joueur.vie + soin, joueur.vieMax);
                        floatingTexts.push_back({"+" + std::to_string(soin) + " PV", {centerX, screenH * 0.40f}, UI::GreenHP, 1.2f, 1.2f});
                        narration = "Vous buvez une Potion de Soin. Vos forces sont revigorees (" + std::to_string(joueur.vie) + "/" + std::to_string(joueur.vieMax) + " PV) !";
                    }
                    else if (joueur.inventaire.grandesPotions > 0 && joueur.vie < joueur.vieMax)
                    {
                        joueur.inventaire.grandesPotions--;
                        joueur.vie = joueur.vieMax;
                        floatingTexts.push_back({"VIE 100% !", {centerX, screenH * 0.40f}, UI::GreenHP, 1.2f, 1.2f});
                        narration = "Grande Potion consommee ! Vos PV sont entierement restaures !";
                    }
                    else
                    {
                        floatingTexts.push_back({"AUCUNE POTION !", {centerX, screenH * 0.40f}, UI::Crimson, 1.0f, 1.0f});
                    }
                }
                else if (btnAvancer.getGlobalBounds().contains(mPos.x, mPos.y))
                {
                    int roll = rand() % 100;

                    if (roll < 38)
                    {
                        Monstre monstre;
                        genererMonstre(zoneActuelle, rand() % 4, monstre);
                        bool gagne = lancerCombatSFML(window, font, joueur, monstre, zoneActuelle, false);

                        AudioJeu::jouer("assets/Music/Woodland Fantasy _ OpenGameArt.org.ogg", 30.0f, true);

                        if (joueur.vie <= 0)
                        {
                            return true;
                        }

                        if (gagne)
                        {
                            narration = "Apres une rude empoignade, vous fouillez le cadavre du " + monstre.nom + " et continuez votre chemin !";
                        }
                        else
                        {
                            narration = "Vous avez reussi a fuir le combat et vous vous cachez dans un fourre...";
                        }
                    }
                    else if (roll < 65)
                    {
                        int typeTresor = rand() % 4;
                        if (typeTresor == 0)
                        {
                            int orTrouve = (rand() % 25) + 15 + (zoneActuelle * 10);
                            joueur.orJoueur += orTrouve;
                            floatingTexts.push_back({"+" + std::to_string(orTrouve) + " PO", {centerX, screenH * 0.40f}, UI::AmberGold, 1.3f, 1.3f});
                            narration = "DECOUVERTE ! Vous denichez un coffre en bois mousseux contenant " + std::to_string(orTrouve) + " pieces d'or !";
                        }
                        else if (typeTresor == 1)
                        {
                            joueur.inventaire.potionsNormales++;
                            floatingTexts.push_back({"+1 POTION", {centerX, screenH * 0.40f}, UI::GreenHP, 1.3f, 1.3f});
                            narration = "TRESOR ! Une Potion de Soin etait cachee sous une dalle de pierre antique !";
                        }
                        else if (typeTresor == 2)
                        {
                            joueur.inventaire.potionsMana++;
                            floatingTexts.push_back({"+1 POTION MANA", {centerX, screenH * 0.40f}, UI::BlueMana, 1.3f, 1.3f});
                            narration = "MAGIE ! Vous trouvez une fiole d'Elixir de Mana etincelante !";
                        }
                        else
                        {
                            joueur.inventaire.nombreBouclier++;
                            floatingTexts.push_back({"+1 BOUCLIER", {centerX, screenH * 0.40f}, UI::Silver, 1.3f, 1.3f});
                            narration = "EQUIPEMENT ! Un bouclier renforce etait abandonne contre un tertre !";
                        }
                    }
                    else if (roll < 78)
                    {
                        int degatsPiege = (rand() % 12) + (zoneActuelle * 6);
                        joueur.vie = std::max(1, joueur.vie - degatsPiege);
                        floatingTexts.push_back({"PIEGE ! -" + std::to_string(degatsPiege) + " PV", {centerX, screenH * 0.40f}, UI::Crimson, 1.3f, 1.3f});
                        narration = "PIEGE ! Un piege dissimule se declenche ! Vous subissez " + std::to_string(degatsPiege) + " degats !";
                    }
                    else if (roll < 90)
                    {
                        int bonusAtq = (rand() % 4) + 2;
                        joueur.attaque += bonusAtq;
                        floatingTexts.push_back({"+" + std::to_string(bonusAtq) + " ATQ PERMANENTE !", {centerX, screenH * 0.40f}, UI::Azure, 1.4f, 1.4f});
                        narration = "ARTEFACT ! Vous affûtez votre arme sur une Pierre Sacree. Attaque permanente +" + std::to_string(bonusAtq) + " !";
                    }
                    else
                    {
                        int soin = 40;
                        int mana = 30;
                        joueur.vie = std::min(joueur.vie + soin, joueur.vieMax);
                        joueur.mana = std::min(joueur.mana + mana, joueur.manaMax);
                        floatingTexts.push_back({"+40 PV  +30 MP", {centerX, screenH * 0.40f}, UI::BlueMana, 1.4f, 1.4f});
                        narration = "SANCTUAIRE ! Une source feerique restaure 40 PV et 30 Mana a votre heros !";
                    }
                }
            }
        }

        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        float bobbing = std::sin(animClock.getElapsedTime().asSeconds() * 3.5f) * 2.5f;
        spriteHero.setPosition(centerX, screenH * 0.44f + bobbing);

        window.clear(sf::Color(10, 14, 20));
        if (fondCharge) window.draw(spriteFond);

        sf::RectangleShape voile(sf::Vector2f(screenW, screenH));
        voile.setFillColor(sf::Color(10, 15, 25, 160));
        window.draw(voile);

        UI::drawPanel(window, screenW * 0.04f, screenH * 0.02f, screenW * 0.92f, screenH * 0.10f);

        sf::Text txtZone(zoneNom + " (Zone " + std::to_string(zoneActuelle) + ")", font, static_cast<unsigned int>(screenH * 0.032f));
        txtZone.setFillColor(UI::Silver);
        txtZone.setPosition(screenW * 0.06f, screenH * 0.032f);
        window.draw(txtZone);

        float hudBarW = screenW * 0.22f;
        UI::drawProgressBar(window, screenW * 0.44f, screenH * 0.035f, hudBarW, 16.0f, joueur.vie, joueur.vieMax, UI::GreenHP, sf::Color(55, 20, 20), "PV: " + std::to_string(joueur.vie) + " / " + std::to_string(joueur.vieMax), font, 12);
        UI::drawProgressBar(window, screenW * 0.44f, screenH * 0.065f, hudBarW, 14.0f, joueur.mana, joueur.manaMax, UI::BlueMana, sf::Color(20, 30, 55), "Mana: " + std::to_string(joueur.mana) + " / " + std::to_string(joueur.manaMax), font, 11);

        sf::Text txtBourse("Bourse : " + std::to_string(joueur.orJoueur) + " PO | Niv. " + std::to_string(joueur.niveau), font, static_cast<unsigned int>(screenH * 0.024f));
        txtBourse.setFillColor(UI::AmberGold);
        sf::FloatRect bB = txtBourse.getLocalBounds();
        txtBourse.setPosition(screenW * 0.94f - bB.width, screenH * 0.04f);
        window.draw(txtBourse);

        window.draw(spriteHero);

        UI::drawPanel(window, screenW * 0.10f, screenH * 0.65f, screenW * 0.80f, screenH * 0.16f, "REGISTRE D'EXPLORATION", &font, 18);
        sf::Text txtNar(narration, font, static_cast<unsigned int>(screenH * 0.023f));
        txtNar.setFillColor(UI::TextWhite);
        txtNar.setPosition(screenW * 0.12f, screenH * 0.72f);
        window.draw(txtNar);

        UI::drawButton(window, btnAvancer, txtAvancer, btnAvancer.getGlobalBounds().contains(mPos.x, mPos.y));
        UI::drawButton(window, btnPotion, txtPotion, btnPotion.getGlobalBounds().contains(mPos.x, mPos.y));
        UI::drawButton(window, btnVille, txtVille, btnVille.getGlobalBounds().contains(mPos.x, mPos.y));

        UI::updateAndDrawFloatingTexts(window, font, floatingTexts, dt);

        window.display();
    }

    AudioJeu::jouer("assets/Music/Town Theme RPG _ OpenGameArt.org.ogg", 30.0f, true);
    return true;
}

inline void afficherVictoireAbsolueSFML(sf::RenderWindow &window, sf::Font &font, Joueur &joueur)
{
    float screenW = static_cast<float>(window.getSize().x);
    float screenH = static_cast<float>(window.getSize().y);
    float centerX = screenW / 2.0f;

    AudioJeu::jouer("assets/Music/Orchestral Epic Fantasy Music _ OpenGameArt.org.OGG", 40.0f, true);

    sf::Texture texFond;
    bool fondCharge = texFond.loadFromFile("assets/backgrounds/chargement.jpg");
    sf::Sprite spriteFond;
    if (fondCharge)
    {
        spriteFond.setTexture(texFond);
        sf::Vector2u sz = texFond.getSize();
        spriteFond.setScale(screenW / sz.x, screenH / sz.y);
    }

    float btnW = screenW * 0.28f;
    float btnH = screenH * 0.07f;
    sf::RectangleShape btnFin(sf::Vector2f(btnW, btnH));
    btnFin.setOrigin(btnW / 2.0f, btnH / 2.0f);
    btnFin.setPosition(centerX, screenH * 0.85f);
    btnFin.setFillColor(UI::DarkPanelLight);
    btnFin.setOutlineThickness(0.0f);

    sf::Text txtFin("GLOIRE ETERNELLE (MENU)", font, static_cast<unsigned int>(btnH * 0.40f));
    txtFin.setFillColor(UI::TextWhite);
    sf::FloatRect bF = txtFin.getLocalBounds();
    txtFin.setOrigin(bF.left + bF.width / 2.0f, bF.top + bF.height / 2.0f);
    txtFin.setPosition(btnFin.getPosition());

    bool quitter = false;

    while (window.isOpen() && !quitter)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (btnFin.getGlobalBounds().contains(mPos.x, mPos.y))
                {
                    quitter = true;
                }
            }
        }

        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        window.clear(sf::Color(10, 10, 15));
        if (fondCharge) window.draw(spriteFond);

        sf::RectangleShape voile(sf::Vector2f(screenW, screenH));
        voile.setFillColor(sf::Color(12, 16, 26, 220));
        window.draw(voile);

        UI::drawPanel(window, screenW * 0.15f, screenH * 0.10f, screenW * 0.70f, screenH * 0.68f, "VICTOIRE ABSOLUE DU ROYAUME", &font, 26);

        sf::Text txtVictoire("★ FANTASY WORLD CONQUEST EST ACCOMPLI ! ★", font, static_cast<unsigned int>(screenH * 0.045f));
        txtVictoire.setFillColor(UI::Silver);
        sf::FloatRect bV = txtVictoire.getLocalBounds();
        txtVictoire.setOrigin(bV.left + bV.width / 2.0f, bV.top + bV.height / 2.0f);
        txtVictoire.setPosition(centerX, screenH * 0.22f);
        window.draw(txtVictoire);

        std::string epilogue = "Toutes les 6 contrees ont ete liberees du joug des tenebres !\n"
            "La Reine des Fees Malefique et l'Empereur des Ombres ont ete terrasses.\n\n"
            "Champion : " + joueur.nom + " (" + joueur.classeNom + ")\n"
            "Niveau Final : " + std::to_string(joueur.niveau) + "  |  Monstres Terrasses : " + std::to_string(joueur.monstresVaincus) + "\n"
            "Fortune Amassee : " + std::to_string(joueur.orJoueur) + " Pieces d'Or\n\n"
            "Votre nom restera grave a jamais dans les chroniques de ce monde.";

        sf::Text txtEpi(epilogue, font, static_cast<unsigned int>(screenH * 0.026f));
        txtEpi.setFillColor(UI::TextWhite);
        sf::FloatRect bE = txtEpi.getLocalBounds();
        txtEpi.setOrigin(bE.left + bE.width / 2.0f, bE.top + bE.height / 2.0f);
        txtEpi.setPosition(centerX, screenH * 0.46f);
        window.draw(txtEpi);

        UI::drawButton(window, btnFin, txtFin, btnFin.getGlobalBounds().contains(mPos.x, mPos.y));

        window.display();
    }
}

inline bool afficherMenuVilleSFML(sf::RenderWindow &window, sf::Font &font, Joueur &joueur, int &zoneActuelle, int &territoiresConquis)
{
    float screenW = static_cast<float>(window.getSize().x);
    float screenH = static_cast<float>(window.getSize().y);
    float centerX = screenW / 2.0f;

    AudioJeu::jouer("assets/Music/Town Theme RPG _ OpenGameArt.org.ogg", 30.0f, true);

    sf::Texture texFond;
    bool fondCharge = texFond.loadFromFile("assets/backgrounds/fond-principale.jpg");
    sf::Sprite spriteFond;
    if (fondCharge)
    {
        spriteFond.setTexture(texFond);
        sf::Vector2u sz = texFond.getSize();
        spriteFond.setScale(screenW / sz.x, screenH / sz.y);
    }

    sf::Texture texHero;
    std::string cheminPerso = "assets/pictures/dark_skinned_knight.png";
    if (joueur.classeIndex == 1) cheminPerso = "assets/pictures/robe.png";
    else if (joueur.classeIndex == 2) cheminPerso = "assets/pictures/leather_armor.png";
    else if (joueur.classeIndex == 3) cheminPerso = "assets/pictures/plate_armor.png";
    else if (joueur.classeIndex == 4) cheminPerso = "assets/pictures/robed_skeleton_spellcast.png";
    else if (joueur.classeIndex == 5) cheminPerso = "assets/pictures/chain_armor_bandit.png";

    texHero.loadFromFile(cheminPerso);
    sf::Sprite spriteHero(texHero);
    spriteHero.setTextureRect(sf::IntRect(0, 128, 64, 64));
    spriteHero.setOrigin(32.0f, 32.0f);
    spriteHero.setScale(screenH * 0.0035f, screenH * 0.0035f);

    std::string notifVille = "Bienvenue au Bastion de Rivenoir, aventurier.";
    sf::Color couleurNotif = UI::Silver;

    float btnW = screenW * 0.46f;
    float btnH = screenH * 0.072f;
    float startY = screenH * 0.19f;
    float gapY = screenH * 0.088f;
    float btnX = screenW * 0.44f;

    sf::RectangleShape btnExplo(sf::Vector2f(btnW, btnH)); btnExplo.setPosition(btnX, startY); btnExplo.setOutlineThickness(0.0f);
    sf::RectangleShape btnBoutique(sf::Vector2f(btnW, btnH)); btnBoutique.setPosition(btnX, startY + gapY); btnBoutique.setOutlineThickness(0.0f);
    sf::RectangleShape btnAuberge(sf::Vector2f(btnW, btnH)); btnAuberge.setPosition(btnX, startY + gapY * 2.0f); btnAuberge.setOutlineThickness(0.0f);
    sf::RectangleShape btnBoss(sf::Vector2f(btnW, btnH)); btnBoss.setPosition(btnX, startY + gapY * 3.0f); btnBoss.setOutlineThickness(0.0f);
    sf::RectangleShape btnSave(sf::Vector2f(btnW, btnH)); btnSave.setPosition(btnX, startY + gapY * 4.0f); btnSave.setOutlineThickness(0.0f);
    sf::RectangleShape btnQuitter(sf::Vector2f(btnW, btnH)); btnQuitter.setPosition(btnX, startY + gapY * 5.0f); btnQuitter.setOutlineThickness(0.0f);

    unsigned int szBtn = static_cast<unsigned int>(btnH * 0.38f);
    sf::Text txtExplo("1. EXPLORER LA CONTREE", font, szBtn); txtExplo.setPosition(btnX + 20.0f, startY + btnH * 0.28f);
    sf::Text txtBoutique("2. BOUTIQUE DE LA CITE", font, szBtn); txtBoutique.setPosition(btnX + 20.0f, startY + gapY + btnH * 0.28f);
    sf::Text txtAuberge("3. SE REPOSER A L'AUBERGE (20 PO)", font, szBtn); txtAuberge.setPosition(btnX + 20.0f, startY + gapY * 2.0f + btnH * 0.28f);
    sf::Text txtBoss("4. RAID CONTRE LE BOSS DE ZONE", font, szBtn); txtBoss.setPosition(btnX + 20.0f, startY + gapY * 3.0f + btnH * 0.28f);
    sf::Text txtSave("5. SAUVEGARDER LA PARTIE", font, szBtn); txtSave.setPosition(btnX + 20.0f, startY + gapY * 4.0f + btnH * 0.28f);
    sf::Text txtQuitter("6. RETOUR AU MENU PRINCIPAL", font, szBtn); txtQuitter.setPosition(btnX + 20.0f, startY + gapY * 5.0f + btnH * 0.28f);

    bool modalBossOuvert = false;
    Monstre bossZone;

    float modalW = screenW * 0.50f;
    float modalH = screenH * 0.40f;
    float modalX = centerX - modalW / 2.0f;
    float modalY = screenH * 0.30f;

    sf::RectangleShape btnAssaut(sf::Vector2f(modalW * 0.42f, screenH * 0.065f));
    btnAssaut.setPosition(modalX + modalW * 0.05f, modalY + modalH * 0.75f);
    btnAssaut.setFillColor(UI::Crimson);
    btnAssaut.setOutlineThickness(0.0f);

    sf::Text txtAssaut("LANCER L'ASSAUT", font, static_cast<unsigned int>(screenH * 0.026f));
    txtAssaut.setFillColor(UI::TextWhite);
    sf::FloatRect bAss = txtAssaut.getLocalBounds();
    txtAssaut.setOrigin(bAss.left + bAss.width / 2.0f, bAss.top + bAss.height / 2.0f);
    txtAssaut.setPosition(btnAssaut.getPosition().x + (modalW * 0.21f), btnAssaut.getPosition().y + (screenH * 0.032f));

    sf::RectangleShape btnAnnulerBoss(sf::Vector2f(modalW * 0.42f, screenH * 0.065f));
    btnAnnulerBoss.setPosition(modalX + modalW * 0.53f, modalY + modalH * 0.75f);
    btnAnnulerBoss.setFillColor(UI::DarkPanelLight);
    btnAnnulerBoss.setOutlineThickness(0.0f);

    sf::Text txtAnnulerBoss("SE REPLIER", font, static_cast<unsigned int>(screenH * 0.026f));
    txtAnnulerBoss.setFillColor(UI::TextWhite);
    sf::FloatRect bAnn = txtAnnulerBoss.getLocalBounds();
    txtAnnulerBoss.setOrigin(bAnn.left + bAnn.width / 2.0f, bAnn.top + bAnn.height / 2.0f);
    txtAnnulerBoss.setPosition(btnAnnulerBoss.getPosition().x + (modalW * 0.21f), btnAnnulerBoss.getPosition().y + (screenH * 0.032f));

    sf::Clock animClock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return false;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (modalBossOuvert)
                {
                    if (btnAssaut.getGlobalBounds().contains(mPos.x, mPos.y))
                    {
                        modalBossOuvert = false;
                        bool victoireBoss = lancerCombatSFML(window, font, joueur, bossZone, zoneActuelle, true);

                        AudioJeu::jouer("assets/Music/Town Theme RPG _ OpenGameArt.org.ogg", 30.0f, true);

                        if (victoireBoss)
                        {
                            territoiresConquis++;
                            sauvegarderPartie(joueur, zoneActuelle, territoiresConquis);

                            if (zoneActuelle >= 6 || territoiresConquis >= 6)
                            {
                                afficherVictoireAbsolueSFML(window, font, joueur);
                                return false;
                            }
                            else
                            {
                                zoneActuelle++;
                                notifVille = "CONQUETE REUSSIE ! Boss terrasse. Zone " + std::to_string(zoneActuelle) + " deverrouillee !";
                                couleurNotif = UI::Azure;
                                sauvegarderPartie(joueur, zoneActuelle, territoiresConquis);
                            }
                        }
                        else
                        {
                            notifVille = "Defaite contre le Boss de Zone... Entraînez-vous encore !";
                            couleurNotif = UI::Crimson;
                        }
                    }
                    else if (btnAnnulerBoss.getGlobalBounds().contains(mPos.x, mPos.y))
                    {
                        modalBossOuvert = false;
                    }
                }
                else
                {
                    if (btnExplo.getGlobalBounds().contains(mPos.x, mPos.y))
                    {
                        explorerMondeSFML(window, font, joueur, zoneActuelle, territoiresConquis);
                    }
                    else if (btnBoutique.getGlobalBounds().contains(mPos.x, mPos.y))
                    {
                        afficherBoutiqueSFML(window, font, joueur);
                    }
                    else if (btnAuberge.getGlobalBounds().contains(mPos.x, mPos.y))
                    {
                        if (joueur.orJoueur >= 20)
                        {
                            joueur.orJoueur -= 20;
                            joueur.vie = joueur.vieMax;
                            joueur.mana = joueur.manaMax;
                            notifVille = "Vous passez une nuit paisible a l'auberge. PV et Mana entierement restaures (-20 PO) !";
                            couleurNotif = UI::GreenHP;
                        }
                        else
                        {
                            notifVille = "Or insuffisant pour louer une chambre a l'auberge (20 PO requis) !";
                            couleurNotif = UI::Crimson;
                        }
                    }
                    else if (btnBoss.getGlobalBounds().contains(mPos.x, mPos.y))
                    {
                        genererMonstre(zoneActuelle, 4, bossZone);
                        modalBossOuvert = true;
                    }
                    else if (btnSave.getGlobalBounds().contains(mPos.x, mPos.y))
                    {
                        if (sauvegarderPartie(joueur, zoneActuelle, territoiresConquis))
                        {
                            notifVille = "Partie sauvegardee avec succes dans les archives du Bastion !";
                            couleurNotif = UI::GreenHP;
                        }
                        else
                        {
                            notifVille = "Erreur lors de l'enregistrement du parchemin de sauvegarde.";
                            couleurNotif = UI::Crimson;
                        }
                    }
                    else if (btnQuitter.getGlobalBounds().contains(mPos.x, mPos.y))
                    {
                        return false;
                    }
                }
            }
        }

        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        window.clear(sf::Color(12, 14, 22));
        if (fondCharge) window.draw(spriteFond);

        sf::RectangleShape voile(sf::Vector2f(screenW, screenH));
        voile.setFillColor(sf::Color(12, 16, 26, 205));
        window.draw(voile);

        UI::drawPanel(window, screenW * 0.05f, screenH * 0.02f, screenW * 0.90f, screenH * 0.09f);
        sf::Text titreVille("LA CITE DE RIVENOIR - BASTION DU MONDE", font, static_cast<unsigned int>(screenH * 0.038f));
        titreVille.setFillColor(UI::Silver);
        titreVille.setPosition(screenW * 0.08f, screenH * 0.035f);
        window.draw(titreVille);

        std::string nomZoneActuelle = getNomZone(zoneActuelle);
        sf::Text sousTitre("Zone " + std::to_string(zoneActuelle) + " : " + nomZoneActuelle + " | Conquetes : " + std::to_string(territoiresConquis) + "/6", font, static_cast<unsigned int>(screenH * 0.025f));
        sousTitre.setFillColor(UI::Azure);
        sf::FloatRect bST = sousTitre.getLocalBounds();
        sousTitre.setPosition(screenW * 0.92f - bST.width, screenH * 0.042f);
        window.draw(sousTitre);

        sf::Text txtNotif(notifVille, font, static_cast<unsigned int>(screenH * 0.022f));
        txtNotif.setFillColor(couleurNotif);
        sf::FloatRect bN = txtNotif.getLocalBounds();
        txtNotif.setOrigin(bN.left + bN.width / 2.0f, bN.top + bN.height / 2.0f);
        txtNotif.setPosition(centerX, screenH * 0.14f);
        window.draw(txtNotif);

        float cardHerosW = screenW * 0.34f;
        float cardHerosH = screenH * 0.72f;
        UI::drawPanel(window, screenW * 0.06f, startY, cardHerosW, cardHerosH, "FEUILLE DU HEROS", &font, 20);

        float bobbing = std::sin(animClock.getElapsedTime().asSeconds() * 3.0f) * 2.0f;
        spriteHero.setPosition(screenW * 0.11f, startY + 80.0f + bobbing);
        window.draw(spriteHero);

        sf::Text txtIdentite(joueur.nom + "\n" + joueur.classeNom + " - Niv. " + std::to_string(joueur.niveau), font, static_cast<unsigned int>(screenH * 0.024f));
        txtIdentite.setFillColor(UI::Silver);
        txtIdentite.setPosition(screenW * 0.16f, startY + 55.0f);
        window.draw(txtIdentite);

        float barX = screenW * 0.08f;
        float barW = cardHerosW - screenW * 0.04f;
        float curY = startY + 130.0f;

        UI::drawProgressBar(window, barX, curY, barW, 18.0f, joueur.vie, joueur.vieMax, UI::GreenHP, sf::Color(55, 20, 20), "PV : " + std::to_string(joueur.vie) + " / " + std::to_string(joueur.vieMax), font, 13);
        curY += 26.0f;
        UI::drawProgressBar(window, barX, curY, barW, 14.0f, joueur.mana, joueur.manaMax, UI::BlueMana, sf::Color(20, 30, 55), "Mana : " + std::to_string(joueur.mana) + " / " + std::to_string(joueur.manaMax), font, 12);
        curY += 24.0f;
        UI::drawProgressBar(window, barX, curY, barW, 14.0f, joueur.xp, joueur.xpSeuil, UI::Azure, sf::Color(30, 45, 65), "XP : " + std::to_string(joueur.xp) + " / " + std::to_string(joueur.xpSeuil), font, 11);
        curY += 30.0f;

        std::string statsTexte = "Attaque : " + std::to_string(joueur.attaque) + "       Defense : " + std::to_string(joueur.defense) + "\n\n"
            + "Critique : " + std::to_string(joueur.chanceCrit) + "%      Sort : " + joueur.competenceSpeciale + "\n\n"
            + "Bourse : " + std::to_string(joueur.orJoueur) + " PO       Monstres terrasses : " + std::to_string(joueur.monstresVaincus) + "\n\n"
            + "----------------- SAC DE VOYAGE -----------------\n"
            + "Potions de soin (+35 PV) : " + std::to_string(joueur.inventaire.potionsNormales) + "\n"
            + "Grandes Potions (100%)    : " + std::to_string(joueur.inventaire.grandesPotions) + "\n"
            + "Potions de Mana (+35 MP)  : " + std::to_string(joueur.inventaire.potionsMana) + "\n"
            + "Boucliers protecteurs     : " + std::to_string(joueur.inventaire.nombreBouclier) + "\n"
            + "Capacite du Sac           : " + std::to_string(joueur.inventaire.capaciteSac) + " emplacements";

        sf::Text txtStats(statsTexte, font, static_cast<unsigned int>(screenH * 0.020f));
        txtStats.setFillColor(UI::TextWhite);
        txtStats.setPosition(barX, curY);
        window.draw(txtStats);

        UI::drawButton(window, btnExplo, txtExplo, btnExplo.getGlobalBounds().contains(mPos.x, mPos.y));
        UI::drawButton(window, btnBoutique, txtBoutique, btnBoutique.getGlobalBounds().contains(mPos.x, mPos.y));
        UI::drawButton(window, btnAuberge, txtAuberge, btnAuberge.getGlobalBounds().contains(mPos.x, mPos.y));
        UI::drawButton(window, btnBoss, txtBoss, btnBoss.getGlobalBounds().contains(mPos.x, mPos.y), sf::Color(140, 35, 35), sf::Color(180, 45, 45), UI::TextWhite, UI::TextWhite);
        UI::drawButton(window, btnSave, txtSave, btnSave.getGlobalBounds().contains(mPos.x, mPos.y));
        UI::drawButton(window, btnQuitter, txtQuitter, btnQuitter.getGlobalBounds().contains(mPos.x, mPos.y));

        if (modalBossOuvert)
        {
            UI::drawPanel(window, modalX, modalY, modalW, modalH, "ORDRE DE BATAILLE : BOSS DE ZONE", &font, 22);

            std::string bossInfo = "Cible Majeure : " + bossZone.nom + "\n\n"
                + "Points de Vie du Boss : " + std::to_string(bossZone.vieMax) + " PV\n"
                + "Puissance d'Attaque : " + std::to_string(bossZone.attaque) + " ATQ | Armure : " + std::to_string(bossZone.defense) + " DEF\n"
                + "Attaque Speciale : " + bossZone.capaciteSpeciale + "\n\n"
                + "Recompense de Victoire : +" + std::to_string(bossZone.orRecompense) + " PO | +" + std::to_string(bossZone.xpRecompense) + " XP\n"
                + "Attention : La fuite sera impossible une fois engage !";

            sf::Text txtBInfo(bossInfo, font, static_cast<unsigned int>(screenH * 0.021f));
            txtBInfo.setFillColor(UI::TextWhite);
            txtBInfo.setPosition(modalX + 25.0f, modalY + 55.0f);
            window.draw(txtBInfo);

            UI::drawButton(window, btnAssaut, txtAssaut, btnAssaut.getGlobalBounds().contains(mPos.x, mPos.y), sf::Color(140, 35, 35), sf::Color(180, 45, 45), UI::TextWhite, UI::TextWhite);
            UI::drawButton(window, btnAnnulerBoss, txtAnnulerBoss, btnAnnulerBoss.getGlobalBounds().contains(mPos.x, mPos.y));
        }

        window.display();
    }

    return true;
}

#endif