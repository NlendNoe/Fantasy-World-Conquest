#ifndef EXPLORATION_CPP
#define EXPLORATION_CPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../structure.h"
#include "../ui_utils.h"
#include "../audio.h"
#include "../sauvegarde.h"
#include "../bestiaire/bestiaire.cpp"
#include "../combat/combat.cpp"

// -------------------------------------------------------------
// 1. BOUTIQUE DE LA CITÉ EN SFML
// -------------------------------------------------------------
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

    struct ItemBoutique {
        std::string nom;
        std::string effet;
        int prix;
        int type; // 0: arme, 1: armure, 2: potion, 3: mana, 4: grande potion, 5: parchemin, 6: sac
        int valeur;
    };

    std::vector<ItemBoutique> items = {
        {"Épée de Fer", "+5 ATQ permanente", 15, 0, 5},
        {"Épée d'Acier", "+15 ATQ permanente", 40, 0, 15},
        {"Lame Runique", "+25 ATQ permanente", 75, 0, 25},
        {"Claymore Vorpale", "+40 ATQ permanente", 130, 0, 40},
        {"Épée du Héros", "+75 ATQ permanente", 250, 0, 75},
        {"Bouclier en Bois", "+1 Bouclier défensif", 35, 1, 1},
        {"Bouclier d'Acier", "+2 Boucliers défensifs", 65, 1, 2},
        {"Cotte de Mailles", "+20 PV Max & Soins", 80, 1, 20},
        {"Armure de Plaques", "+45 PV Max & +10 DEF", 180, 1, 45},
        {"Potion de Soin", "+35 PV en combat/explo", 20, 2, 1},
        {"Grande Potion", "Restaure 100% des PV", 50, 4, 1},
        {"Potion de Mana", "+35 MP en combat", 25, 3, 1},
        {"Parchemin de Force", "+6 ATQ permanent", 100, 5, 6},
        {"Agrandir Sac", "+2 Emplacements d'objets", 70, 6, 2}
    };

    float cardW = screenW * 0.42f;
    float cardH = screenH * 0.065f;
    float col1X = screenW * 0.06f;
    float col2X = screenW * 0.52f;
    float startY = screenH * 0.17f;
    float gapY = screenH * 0.082f;

    std::string messageNotification = "Bienvenue a la Boutique Royale ! Cliquez sur un article pour acheter.";
    sf::Color couleurNotif = UI::GoldBright;

    float btnRetW = screenW * 0.22f;
    float btnRetH = screenH * 0.065f;
    sf::RectangleShape btnRetour(sf::Vector2f(btnRetW, btnRetH));
    btnRetour.setOrigin(btnRetW / 2.0f, btnRetH / 2.0f);
    btnRetour.setPosition(centerX, screenH * 0.93f);
    btnRetour.setFillColor(UI::Gold);
    btnRetour.setOutlineThickness(2.0f);
    btnRetour.setOutlineColor(UI::GoldBright);

    sf::Text txtRetour("RETOUR A LA CITE", font, static_cast<unsigned int>(btnRetH * 0.40f));
    txtRetour.setFillColor(sf::Color::Black);
    sf::FloatRect bRet = txtRetour.getLocalBounds();
    txtRetour.setOrigin(bRet.left + bRet.width / 2.0f, bRet.top + bRet.height / 2.0f);
    txtRetour.setPosition(btnRetour.getPosition());

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
                    float x = (i < 7) ? col1X : col2X;
                    float y = startY + (i % 7) * gapY;
                    sf::FloatRect boundsItem(x, y, cardW, cardH);

                    if (boundsItem.contains(mPos.x, mPos.y))
                    {
                        if (joueur.orJoueur >= items[i].prix)
                        {
                            joueur.orJoueur -= items[i].prix;

                            if (items[i].type == 0) // Arme
                            {
                                joueur.attaque += items[i].valeur;
                                messageNotification = "Achete: " + items[i].nom + " (+ " + std::to_string(items[i].valeur) + " ATQ) !";
                            }
                            else if (items[i].type == 1) // Armure / Bouclier
                            {
                                if (items[i].nom.find("Bouclier") != std::string::npos)
                                {
                                    joueur.inventaire.nombreBouclier += items[i].valeur;
                                    messageNotification = "Achete: " + items[i].nom + " (+ " + std::to_string(items[i].valeur) + " Bouclier) !";
                                }
                                else if (items[i].nom.find("Cotte") != std::string::npos)
                                {
                                    joueur.vieMax += items[i].valeur;
                                    joueur.vie += items[i].valeur;
                                    messageNotification = "Achete: " + items[i].nom + " (+ " + std::to_string(items[i].valeur) + " PV Max) !";
                                }
                                else // Armure plaques
                                {
                                    joueur.vieMax += items[i].valeur;
                                    joueur.vie += items[i].valeur;
                                    joueur.defense += 10;
                                    messageNotification = "Achete: " + items[i].nom + " (+45 PV Max, +10 DEF) !";
                                }
                            }
                            else if (items[i].type == 2) // Potion
                            {
                                joueur.inventaire.potionsNormales += items[i].valeur;
                                messageNotification = "Achete: Potion de Soin ajoutee a votre sac !";
                            }
                            else if (items[i].type == 3) // Mana
                            {
                                joueur.inventaire.potionsMana += items[i].valeur;
                                messageNotification = "Achete: Potion de Mana ajoutee a votre sac !";
                            }
                            else if (items[i].type == 4) // Grande potion
                            {
                                joueur.inventaire.grandesPotions += items[i].valeur;
                                messageNotification = "Achete: Grande Potion ajoutee a votre sac !";
                            }
                            else if (items[i].type == 5) // Parchemin
                            {
                                joueur.attaque += items[i].valeur;
                                messageNotification = "Achete: Parchemin de Force (+6 ATQ permanent) !";
                            }
                            else if (items[i].type == 6) // Agrandir sac
                            {
                                joueur.inventaire.capaciteSac += items[i].valeur;
                                messageNotification = "Achete: Sac agrandi (+2 emplacements) !";
                            }

                            couleurNotif = UI::GreenHP;
                        }
                        else
                        {
                            messageNotification = "Or insuffisant pour acheter: " + items[i].nom + " !";
                            couleurNotif = UI::Crimson;
                        }
                    }
                }
            }
        }

        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        window.clear(sf::Color(12, 14, 20));
        if (fondCharge) window.draw(spriteFond);

        sf::RectangleShape voile(sf::Vector2f(screenW, screenH));
        voile.setFillColor(sf::Color(10, 12, 22, 210));
        window.draw(voile);

        // En-tête
        UI::drawPanel(window, screenW * 0.05f, screenH * 0.02f, screenW * 0.90f, screenH * 0.09f);
        sf::Text txtTitreShop("BOUTIQUE ROYALE DE RIVENOIR", font, static_cast<unsigned int>(screenH * 0.038f));
        txtTitreShop.setFillColor(UI::GoldBright);
        txtTitreShop.setPosition(screenW * 0.08f, screenH * 0.035f);
        window.draw(txtTitreShop);

        sf::Text txtOrShop("Bourse : " + std::to_string(joueur.orJoueur) + " PO", font, static_cast<unsigned int>(screenH * 0.032f));
        txtOrShop.setFillColor(UI::AmberXP);
        sf::FloatRect bOrS = txtOrShop.getLocalBounds();
        txtOrShop.setPosition(screenW * 0.92f - bOrS.width, screenH * 0.04f);
        window.draw(txtOrShop);

        // Bandeau de notification
        sf::Text txtNotif(messageNotification, font, static_cast<unsigned int>(screenH * 0.022f));
        txtNotif.setFillColor(couleurNotif);
        sf::FloatRect bN = txtNotif.getLocalBounds();
        txtNotif.setOrigin(bN.left + bN.width / 2.0f, bN.top + bN.height / 2.0f);
        txtNotif.setPosition(centerX, screenH * 0.135f);
        window.draw(txtNotif);

        // Liste des articles
        for (size_t i = 0; i < items.size(); ++i)
        {
            float x = (i < 7) ? col1X : col2X;
            float y = startY + (i % 7) * gapY;
            sf::FloatRect boundsItem(x, y, cardW, cardH);
            bool isHov = boundsItem.contains(mPos.x, mPos.y);

            sf::RectangleShape card(sf::Vector2f(cardW, cardH));
            card.setPosition(x, y);
            card.setFillColor(isHov ? sf::Color(35, 45, 75, 230) : UI::DarkPanel);
            card.setOutlineThickness(1.5f);
            card.setOutlineColor(isHov ? UI::GoldBright : UI::Gold);
            window.draw(card);

            sf::Text txtNom(items[i].nom, font, static_cast<unsigned int>(cardH * 0.38f));
            txtNom.setFillColor(isHov ? UI::GoldBright : UI::TextWhite);
            txtNom.setPosition(x + 12.0f, y + 6.0f);
            window.draw(txtNom);

            sf::Text txtEffet(items[i].effet, font, static_cast<unsigned int>(cardH * 0.28f));
            txtEffet.setFillColor(UI::TextMuted);
            txtEffet.setPosition(x + 14.0f, y + cardH * 0.50f);
            window.draw(txtEffet);

            sf::Text txtPrix(std::to_string(items[i].prix) + " PO", font, static_cast<unsigned int>(cardH * 0.38f));
            txtPrix.setFillColor(joueur.orJoueur >= items[i].prix ? UI::AmberXP : UI::Crimson);
            sf::FloatRect bP = txtPrix.getLocalBounds();
            txtPrix.setPosition(x + cardW - bP.width - 15.0f, y + cardH * 0.28f);
            window.draw(txtPrix);
        }

        // Bouton retour
        UI::drawButton(window, btnRetour, txtRetour, btnRetour.getGlobalBounds().contains(mPos.x, mPos.y));

        window.display();
    }

    return true;
}

// -------------------------------------------------------------
// 2. EXPLORATION DE CONTREE EN SFML
// -------------------------------------------------------------
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

    // Sprite du joueur en marche
    sf::Texture texHero;
    std::string cheminPerso = "assets/pictures/dark_skinned_knight.png";
    if (joueur.classeIndex == 1) cheminPerso = "assets/pictures/robe.png";
    else if (joueur.classeIndex == 2) cheminPerso = "assets/pictures/leather_armor.png";
    else if (joueur.classeIndex == 3) cheminPerso = "assets/pictures/plate_armor.png";
    else if (joueur.classeIndex == 4) cheminPerso = "assets/pictures/robed_skeleton_spellcast.png";
    else if (joueur.classeIndex == 5) cheminPerso = "assets/pictures/chain_armor_bandit.png";

    texHero.loadFromFile(cheminPerso);
    sf::Sprite spriteHero(texHero);
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
    btnAvancer.setOutlineThickness(2.0f);
    btnAvancer.setOutlineColor(UI::Gold);

    sf::Text txtAvancer("1. AVANCER", font, static_cast<unsigned int>(btnH * 0.40f));
    txtAvancer.setFillColor(UI::TextWhite);
    sf::FloatRect bAv = txtAvancer.getLocalBounds();
    txtAvancer.setOrigin(bAv.left + bAv.width / 2.0f, bAv.top + bAv.height / 2.0f);
    txtAvancer.setPosition(btnAvancer.getPosition());

    sf::RectangleShape btnPotion(sf::Vector2f(btnW, btnH));
    btnPotion.setOrigin(btnW / 2.0f, btnH / 2.0f);
    btnPotion.setPosition(screenW * 0.50f, actionsY);
    btnPotion.setFillColor(UI::DarkPanelLight);
    btnPotion.setOutlineThickness(2.0f);
    btnPotion.setOutlineColor(UI::Gold);

    sf::Text txtPotion("2. BOIRE POTION", font, static_cast<unsigned int>(btnH * 0.40f));
    txtPotion.setFillColor(UI::TextWhite);
    sf::FloatRect bPot = txtPotion.getLocalBounds();
    txtPotion.setOrigin(bPot.left + bPot.width / 2.0f, bPot.top + bPot.height / 2.0f);
    txtPotion.setPosition(btnPotion.getPosition());

    sf::RectangleShape btnVille(sf::Vector2f(btnW, btnH));
    btnVille.setOrigin(btnW / 2.0f, btnH / 2.0f);
    btnVille.setPosition(screenW * 0.74f, actionsY);
    btnVille.setFillColor(UI::DarkPanelLight);
    btnVille.setOutlineThickness(2.0f);
    btnVille.setOutlineColor(UI::Gold);

    sf::Text txtVille("3. RETOUR CITE", font, static_cast<unsigned int>(btnH * 0.40f));
    txtVille.setFillColor(UI::TextWhite);
    sf::FloatRect bVi = txtVille.getLocalBounds();
    txtVille.setOrigin(bVi.left + bVi.width / 2.0f, bVi.top + bVi.height / 2.0f);
    txtVille.setPosition(btnVille.getPosition());

    sf::Clock animClock;
    int animFrame = 0;
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

                    if (roll < 38) // 38% Combat de monstre
                    {
                        Monstre monstre;
                        genererMonstre(zoneActuelle, rand() % 4, monstre);
                        bool gagne = lancerCombatSFML(window, font, joueur, monstre, zoneActuelle, false);

                        AudioJeu::jouer("assets/Music/Woodland Fantasy _ OpenGameArt.org.ogg", 30.0f, true);

                        if (joueur.vie <= 0)
                        {
                            return true; // Retour ville après mort
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
                    else if (roll < 65) // 27% Coffre au trésor
                    {
                        int typeTresor = rand() % 4;
                        if (typeTresor == 0)
                        {
                            int orTrouve = (rand() % 25) + 15 + (zoneActuelle * 10);
                            joueur.orJoueur += orTrouve;
                            floatingTexts.push_back({"+" + std::to_string(orTrouve) + " PO", {centerX, screenH * 0.40f}, UI::AmberXP, 1.3f, 1.3f});
                            narration = "★ DECOUVERTE ! Vous denichez un coffre en bois mousseux contenant " + std::to_string(orTrouve) + " pièces d'or !";
                        }
                        else if (typeTresor == 1)
                        {
                            joueur.inventaire.potionsNormales++;
                            floatingTexts.push_back({"+1 POTION", {centerX, screenH * 0.40f}, UI::GreenHP, 1.3f, 1.3f});
                            narration = "★ TRESOR ! Une Potion de Soin etait cachee sous une dalle de pierre antique !";
                        }
                        else if (typeTresor == 2)
                        {
                            joueur.inventaire.potionsMana++;
                            floatingTexts.push_back({"+1 POTION MANA", {centerX, screenH * 0.40f}, UI::BlueMana, 1.3f, 1.3f});
                            narration = "★ MAGIE ! Vous trouvez une fiole d'Elixir de Mana étincelante !";
                        }
                        else
                        {
                            joueur.inventaire.nombreBouclier++;
                            floatingTexts.push_back({"+1 BOUCLIER", {centerX, screenH * 0.40f}, UI::GoldBright, 1.3f, 1.3f});
                            narration = "★ EQUIPEMENT ! Un bouclier renforce etait abandonne contre un tertre !";
                        }
                    }
                    else if (roll < 78) // 13% Piège
                    {
                        int degatsPiege = (rand() % 12) + (zoneActuelle * 6);
                        joueur.vie = std::max(1, joueur.vie - degatsPiege);
                        floatingTexts.push_back({"PIEGE ! -" + std::to_string(degatsPiege) + " PV", {centerX, screenH * 0.40f}, UI::Crimson, 1.3f, 1.3f});
                        narration = "⚠ PIEGE ! Un piege dissimule se declenche ! Vous subissez " + std::to_string(degatsPiege) + " degats de pointes rouillees !";
                    }
                    else if (roll < 90) // 12% Écho d'Épée / Bénédiction
                    {
                        int bonusAtq = (rand() % 4) + 2;
                        joueur.attaque += bonusAtq;
                        floatingTexts.push_back({"+" + std::to_string(bonusAtq) + " ATQ PERMANENTE !", {centerX, screenH * 0.40f}, UI::GoldBright, 1.4f, 1.4f});
                        narration = "⚔ ARTEFACT ! Vous affûtez votre arme sur une Pierre de Guerre Sacree. Attaque permamentee +" + std::to_string(bonusAtq) + " !";
                    }
                    else // 10% Fontaine de Jouvence
                    {
                        int soin = 40;
                        int mana = 30;
                        joueur.vie = std::min(joueur.vie + soin, joueur.vieMax);
                        joueur.mana = std::min(joueur.mana + mana, joueur.manaMax);
                        floatingTexts.push_back({"+40 PV  +30 MP", {centerX, screenH * 0.40f}, UI::BlueMana, 1.4f, 1.4f});
                        narration = "✨ SANCTUAIRE ! Une source feerique restaure 40 PV et 30 Mana a votre hero !";
                    }
                }
            }
        }

        // Animation du marcheur
        if (animClock.getElapsedTime().asSeconds() > 0.16f)
        {
            animFrame = (animFrame + 1) % 9;
            animClock.restart();
        }

        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        window.clear(sf::Color(10, 15, 20));
        if (fondCharge) window.draw(spriteFond);

        // Voile atmosphérique
        sf::RectangleShape voile(sf::Vector2f(screenW, screenH));
        voile.setFillColor(sf::Color(10, 15, 25, 150));
        window.draw(voile);

        // Barre supérieure HUD
        UI::drawPanel(window, screenW * 0.04f, screenH * 0.02f, screenW * 0.92f, screenH * 0.10f);

        sf::Text txtZone(zoneNom + " (Zone " + std::to_string(zoneActuelle) + ")", font, static_cast<unsigned int>(screenH * 0.032f));
        txtZone.setFillColor(UI::GoldBright);
        txtZone.setPosition(screenW * 0.06f, screenH * 0.032f);
        window.draw(txtZone);

        // Barres de vie / Mana du héros dans le HUD
        float hudBarW = screenW * 0.22f;
        UI::drawProgressBar(window, screenW * 0.44f, screenH * 0.035f, hudBarW, 16.0f, joueur.vie, joueur.vieMax, UI::GreenHP, sf::Color(60, 20, 20), "PV: " + std::to_string(joueur.vie) + " / " + std::to_string(joueur.vieMax), font, 12);
        UI::drawProgressBar(window, screenW * 0.44f, screenH * 0.065f, hudBarW, 14.0f, joueur.mana, joueur.manaMax, UI::BlueMana, sf::Color(20, 30, 60), "Mana: " + std::to_string(joueur.mana) + " / " + std::to_string(joueur.manaMax), font, 11);

        sf::Text txtBourse("Bourse : " + std::to_string(joueur.orJoueur) + " PO | Niv. " + std::to_string(joueur.niveau), font, static_cast<unsigned int>(screenH * 0.024f));
        txtBourse.setFillColor(UI::AmberXP);
        sf::FloatRect bB = txtBourse.getLocalBounds();
        txtBourse.setPosition(screenW * 0.94f - bB.width, screenH * 0.04f);
        window.draw(txtBourse);

        // Sprite animé au centre
        spriteHero.setTextureRect(sf::IntRect(animFrame * 64, 128, 64, 64)); // walkcycle
        window.draw(spriteHero);

        // Panneau de narration
        UI::drawPanel(window, screenW * 0.10f, screenH * 0.65f, screenW * 0.80f, screenH * 0.16f, "REGISTRE D'EXPLORATION", &font, 18);
        sf::Text txtNar(narration, font, static_cast<unsigned int>(screenH * 0.023f));
        txtNar.setFillColor(UI::TextWhite);
        txtNar.setPosition(screenW * 0.12f, screenH * 0.72f);
        window.draw(txtNar);

        // Boutons d'action
        UI::drawButton(window, btnAvancer, txtAvancer, btnAvancer.getGlobalBounds().contains(mPos.x, mPos.y));
        UI::drawButton(window, btnPotion, txtPotion, btnPotion.getGlobalBounds().contains(mPos.x, mPos.y));
        UI::drawButton(window, btnVille, txtVille, btnVille.getGlobalBounds().contains(mPos.x, mPos.y));

        // Textes flottants
        UI::updateAndDrawFloatingTexts(window, font, floatingTexts, dt);

        window.display();
    }

    AudioJeu::jouer("assets/Music/Town Theme RPG _ OpenGameArt.org.ogg", 30.0f, true);
    return true;
}

// -------------------------------------------------------------
// 3. ÉCRAN DE VICTOIRE ABSOLUE DU JEU
// -------------------------------------------------------------
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
    btnFin.setFillColor(UI::Gold);
    btnFin.setOutlineThickness(2.0f);
    btnFin.setOutlineColor(UI::GoldBright);

    sf::Text txtFin("GLOIRE ETERNELLE (MENU)", font, static_cast<unsigned int>(btnH * 0.40f));
    txtFin.setFillColor(sf::Color::Black);
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
        voile.setFillColor(sf::Color(15, 18, 30, 220));
        window.draw(voile);

        // Panneau central
        UI::drawPanel(window, screenW * 0.15f, screenH * 0.10f, screenW * 0.70f, screenH * 0.68f, "VICTOIRE ABSOLUE DU ROYAUME", &font, 26);

        sf::Text txtVictoire("★ FANTASY WORLD CONQUEST EST ACCOMPLI ! ★", font, static_cast<unsigned int>(screenH * 0.045f));
        txtVictoire.setFillColor(UI::GoldBright);
        sf::FloatRect bV = txtVictoire.getLocalBounds();
        txtVictoire.setOrigin(bV.left + bV.width / 2.0f, bV.top + bV.height / 2.0f);
        txtVictoire.setPosition(centerX, screenH * 0.22f);
        window.draw(txtVictoire);

        std::string epilogue = "Toutes les 6 contrees ont ete liberees du joug des tenebres !\n"
            "La Reine des Fees Malefique et l'Empereur des Ombres ont ete terrassee.\n\n"
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

// -------------------------------------------------------------
// 4. MENU DE LA VILLE / BASTION SFML
// -------------------------------------------------------------
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

    // Avatar miniature du joueur
    sf::Texture texHero;
    std::string cheminPerso = "assets/pictures/dark_skinned_knight.png";
    if (joueur.classeIndex == 1) cheminPerso = "assets/pictures/robe.png";
    else if (joueur.classeIndex == 2) cheminPerso = "assets/pictures/leather_armor.png";
    else if (joueur.classeIndex == 3) cheminPerso = "assets/pictures/plate_armor.png";
    else if (joueur.classeIndex == 4) cheminPerso = "assets/pictures/robed_skeleton_spellcast.png";
    else if (joueur.classeIndex == 5) cheminPerso = "assets/pictures/chain_armor_bandit.png";

    texHero.loadFromFile(cheminPerso);
    sf::Sprite spriteHero(texHero);
    spriteHero.setOrigin(32.0f, 32.0f);
    spriteHero.setScale(screenH * 0.0035f, screenH * 0.0035f);

    std::string notifVille = "Bienvenue au Bastion de Rivenoir, aventurier.";
    sf::Color couleurNotif = UI::GoldBright;

    // Dimensions des boutons de commande de la cité
    float btnW = screenW * 0.46f;
    float btnH = screenH * 0.072f;
    float startY = screenH * 0.19f;
    float gapY = screenH * 0.088f;
    float btnX = screenW * 0.44f;

    sf::RectangleShape btnExplo(sf::Vector2f(btnW, btnH)); btnExplo.setPosition(btnX, startY);
    sf::RectangleShape btnBoutique(sf::Vector2f(btnW, btnH)); btnBoutique.setPosition(btnX, startY + gapY);
    sf::RectangleShape btnAuberge(sf::Vector2f(btnW, btnH)); btnAuberge.setPosition(btnX, startY + gapY * 2.0f);
    sf::RectangleShape btnBoss(sf::Vector2f(btnW, btnH)); btnBoss.setPosition(btnX, startY + gapY * 3.0f);
    sf::RectangleShape btnSave(sf::Vector2f(btnW, btnH)); btnSave.setPosition(btnX, startY + gapY * 4.0f);
    sf::RectangleShape btnQuitter(sf::Vector2f(btnW, btnH)); btnQuitter.setPosition(btnX, startY + gapY * 5.0f);

    unsigned int szBtn = static_cast<unsigned int>(btnH * 0.38f);
    sf::Text txtExplo("1. EXPLORER LA CONTREE", font, szBtn); txtExplo.setPosition(btnX + 20.0f, startY + btnH * 0.28f);
    sf::Text txtBoutique("2. BOUTIQUE DE LA CITE", font, szBtn); txtBoutique.setPosition(btnX + 20.0f, startY + gapY + btnH * 0.28f);
    sf::Text txtAuberge("3. SE REPOSER A L'AUBERGE (20 PO)", font, szBtn); txtAuberge.setPosition(btnX + 20.0f, startY + gapY * 2.0f + btnH * 0.28f);
    sf::Text txtBoss("4. RAID CONTRE LE BOSS DE ZONE", font, szBtn); txtBoss.setPosition(btnX + 20.0f, startY + gapY * 3.0f + btnH * 0.28f);
    sf::Text txtSave("5. SAUVEGARDER LA PARTIE", font, szBtn); txtSave.setPosition(btnX + 20.0f, startY + gapY * 4.0f + btnH * 0.28f);
    sf::Text txtQuitter("6. RETOUR AU MENU PRINCIPAL", font, szBtn); txtQuitter.setPosition(btnX + 20.0f, startY + gapY * 5.0f + btnH * 0.28f);

    // Modal de confirmation du Boss
    bool modalBossOuvert = false;
    Monstre bossZone;

    float modalW = screenW * 0.50f;
    float modalH = screenH * 0.40f;
    float modalX = centerX - modalW / 2.0f;
    float modalY = screenH * 0.30f;

    sf::RectangleShape btnAssaut(sf::Vector2f(modalW * 0.42f, screenH * 0.065f));
    btnAssaut.setPosition(modalX + modalW * 0.05f, modalY + modalH * 0.75f);
    btnAssaut.setFillColor(UI::Crimson);
    btnAssaut.setOutlineThickness(2.0f);
    btnAssaut.setOutlineColor(UI::GoldBright);

    sf::Text txtAssaut("LANCER L'ASSAUT", font, static_cast<unsigned int>(screenH * 0.026f));
    txtAssaut.setFillColor(UI::TextWhite);
    sf::FloatRect bAss = txtAssaut.getLocalBounds();
    txtAssaut.setOrigin(bAss.left + bAss.width / 2.0f, bAss.top + bAss.height / 2.0f);
    txtAssaut.setPosition(btnAssaut.getPosition().x + (modalW * 0.21f), btnAssaut.getPosition().y + (screenH * 0.032f));

    sf::RectangleShape btnAnnulerBoss(sf::Vector2f(modalW * 0.42f, screenH * 0.065f));
    btnAnnulerBoss.setPosition(modalX + modalW * 0.53f, modalY + modalH * 0.75f);
    btnAnnulerBoss.setFillColor(UI::DarkPanelLight);
    btnAnnulerBoss.setOutlineThickness(2.0f);
    btnAnnulerBoss.setOutlineColor(UI::Gold);

    sf::Text txtAnnulerBoss("SE REPLIER", font, static_cast<unsigned int>(screenH * 0.026f));
    txtAnnulerBoss.setFillColor(UI::TextWhite);
    sf::FloatRect bAnn = txtAnnulerBoss.getLocalBounds();
    txtAnnulerBoss.setOrigin(bAnn.left + bAnn.width / 2.0f, bAnn.top + bAnn.height / 2.0f);
    txtAnnulerBoss.setPosition(btnAnnulerBoss.getPosition().x + (modalW * 0.21f), btnAnnulerBoss.getPosition().y + (screenH * 0.032f));

    sf::Clock animClock;
    int animFrame = 0;

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
                                return false; // Fin triomphale
                            }
                            else
                            {
                                zoneActuelle++;
                                notifVille = "★ CONQUETE REUSSIE ! Boss terrasse. Zone " + std::to_string(zoneActuelle) + " deverrouillee !";
                                couleurNotif = UI::GoldBright;
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

        // Animation du sprite héros dans la fiche
        if (animClock.getElapsedTime().asSeconds() > 0.18f)
        {
            animFrame = (animFrame + 1) % 9;
            animClock.restart();
        }

        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        window.clear(sf::Color(12, 14, 22));
        if (fondCharge) window.draw(spriteFond);

        sf::RectangleShape voile(sf::Vector2f(screenW, screenH));
        voile.setFillColor(sf::Color(12, 16, 26, 200));
        window.draw(voile);

        // Titre de la Ville
        UI::drawPanel(window, screenW * 0.05f, screenH * 0.02f, screenW * 0.90f, screenH * 0.09f);
        sf::Text titreVille("LA CITE DE RIVENOIR - BASTION DU MONDE", font, static_cast<unsigned int>(screenH * 0.038f));
        titreVille.setFillColor(UI::GoldBright);
        titreVille.setPosition(screenW * 0.08f, screenH * 0.035f);
        window.draw(titreVille);

        std::string nomZoneActuelle = getNomZone(zoneActuelle);
        sf::Text sousTitre("Zone " + std::to_string(zoneActuelle) + " : " + nomZoneActuelle + " | Conquetes : " + std::to_string(territoiresConquis) + "/6", font, static_cast<unsigned int>(screenH * 0.025f));
        sousTitre.setFillColor(UI::AmberXP);
        sf::FloatRect bST = sousTitre.getLocalBounds();
        sousTitre.setPosition(screenW * 0.92f - bST.width, screenH * 0.042f);
        window.draw(sousTitre);

        // Bandeau de notification
        sf::Text txtNotif(notifVille, font, static_cast<unsigned int>(screenH * 0.022f));
        txtNotif.setFillColor(couleurNotif);
        sf::FloatRect bN = txtNotif.getLocalBounds();
        txtNotif.setOrigin(bN.left + bN.width / 2.0f, bN.top + bN.height / 2.0f);
        txtNotif.setPosition(centerX, screenH * 0.14f);
        window.draw(txtNotif);

        // Panneau Gauche : Fiche Héros
        float cardHerosW = screenW * 0.34f;
        float cardHerosH = screenH * 0.72f;
        UI::drawPanel(window, screenW * 0.06f, startY, cardHerosW, cardHerosH, "FEUILLE DU HEROS", &font, 20);

        // Sprite animé
        spriteHero.setPosition(screenW * 0.11f, startY + 80.0f);
        spriteHero.setTextureRect(sf::IntRect(animFrame * 64, 128, 64, 64));
        window.draw(spriteHero);

        // Identité
        sf::Text txtIdentite(joueur.nom + "\n" + joueur.classeNom + " - Niv. " + std::to_string(joueur.niveau), font, static_cast<unsigned int>(screenH * 0.024f));
        txtIdentite.setFillColor(UI::GoldBright);
        txtIdentite.setPosition(screenW * 0.16f, startY + 55.0f);
        window.draw(txtIdentite);

        // Barres de progression
        float barX = screenW * 0.08f;
        float barW = cardHerosW - screenW * 0.04f;
        float curY = startY + 130.0f;

        UI::drawProgressBar(window, barX, curY, barW, 18.0f, joueur.vie, joueur.vieMax, UI::GreenHP, sf::Color(60, 20, 20), "PV : " + std::to_string(joueur.vie) + " / " + std::to_string(joueur.vieMax), font, 13);
        curY += 26.0f;
        UI::drawProgressBar(window, barX, curY, barW, 16.0f, joueur.mana, joueur.manaMax, UI::BlueMana, sf::Color(20, 30, 60), "Mana : " + std::to_string(joueur.mana) + " / " + std::to_string(joueur.manaMax), font, 12);
        curY += 24.0f;
        UI::drawProgressBar(window, barX, curY, barW, 14.0f, joueur.xp, joueur.xpSeuil, UI::AmberXP, sf::Color(50, 40, 20), "XP : " + std::to_string(joueur.xp) + " / " + std::to_string(joueur.xpSeuil), font, 11);
        curY += 30.0f;

        // Statistiques détaillées
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

        // Boutons de la Cité
        UI::drawButton(window, btnExplo, txtExplo, btnExplo.getGlobalBounds().contains(mPos.x, mPos.y));
        UI::drawButton(window, btnBoutique, txtBoutique, btnBoutique.getGlobalBounds().contains(mPos.x, mPos.y));
        UI::drawButton(window, btnAuberge, txtAuberge, btnAuberge.getGlobalBounds().contains(mPos.x, mPos.y));
        UI::drawButton(window, btnBoss, txtBoss, btnBoss.getGlobalBounds().contains(mPos.x, mPos.y), UI::Crimson, UI::GoldBright, UI::Gold, sf::Color::Black);
        UI::drawButton(window, btnSave, txtSave, btnSave.getGlobalBounds().contains(mPos.x, mPos.y));
        UI::drawButton(window, btnQuitter, txtQuitter, btnQuitter.getGlobalBounds().contains(mPos.x, mPos.y));

        // Modal de Raid de Boss
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

            UI::drawButton(window, btnAssaut, txtAssaut, btnAssaut.getGlobalBounds().contains(mPos.x, mPos.y), UI::Crimson, UI::GoldBright, UI::Gold, sf::Color::Black);
            UI::drawButton(window, btnAnnulerBoss, txtAnnulerBoss, btnAnnulerBoss.getGlobalBounds().contains(mPos.x, mPos.y));
        }

        window.display();
    }

    return true;
}

#endif