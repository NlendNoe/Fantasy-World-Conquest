#ifndef COMBAT_CPP
#define COMBAT_CPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include "../structure.h"
#include "../ui_utils.h"
#include "../audio.h"

enum class CombatState
{
    PLAYER_TURN,
    ACTION_RESOLVING,
    ENEMY_TURN,
    VICTORY_SCREEN,
    DEFEAT_SCREEN
};

inline bool lancerCombatSFML(sf::RenderWindow &window, sf::Font &font, Joueur &joueur, Monstre &monstre, int zoneActuelle, bool isBossRaid = false)
{
    float screenW = static_cast<float>(window.getSize().x);
    float screenH = static_cast<float>(window.getSize().y);
    float centerX = screenW / 2.0f;
    float centerY = screenH / 2.0f;

    // Musique du combat
    if (isBossRaid)
        AudioJeu::jouer("assets/Music/Orchestral Epic Fantasy Music _ OpenGameArt.org.OGG", 35.0f, true);
    else
        AudioJeu::jouer("assets/Music/RPG Battle Theme - The Last Encounter _ OpenGameArt.org.ogg", 30.0f, true);

    // Fond d'arène
    sf::Texture texFond;
    bool fondCharge = texFond.loadFromFile("assets/backgrounds/introBg.jpg");
    sf::Sprite spriteFond;
    if (fondCharge)
    {
        spriteFond.setTexture(texFond);
        sf::Vector2u sz = texFond.getSize();
        spriteFond.setScale(screenW / sz.x, screenH / sz.y);
    }

    // Chargement des textures des sprites LPC
    sf::Texture texGuerrier, texMage, texArcher, texPaladin, texNecro, texAssassin;
    texGuerrier.loadFromFile("assets/pictures/dark_skinned_knight.png");
    texMage.loadFromFile("assets/pictures/robe.png");
    texArcher.loadFromFile("assets/pictures/leather_armor.png");
    texPaladin.loadFromFile("assets/pictures/plate_armor.png");
    texNecro.loadFromFile("assets/pictures/robed_skeleton_spellcast.png");
    texAssassin.loadFromFile("assets/pictures/chain_armor_bandit.png");

    sf::Texture* texturesHero[6] = {&texGuerrier, &texMage, &texArcher, &texPaladin, &texNecro, &texAssassin};
    int heroTexIdx = std::clamp(joueur.classeIndex, 0, 5);

    sf::Sprite spriteHero(*texturesHero[heroTexIdx]);
    spriteHero.setOrigin(32.0f, 32.0f);
    float heroScale = screenH * 0.0055f;
    spriteHero.setScale(heroScale, heroScale);
    sf::Vector2f heroBasePos(screenW * 0.25f, screenH * 0.45f);
    spriteHero.setPosition(heroBasePos);

    // Texture du monstre
    sf::Texture* texMonstre = texturesHero[monstre.spriteType % 6];
    sf::Sprite spriteMonstre(*texMonstre);
    spriteMonstre.setOrigin(32.0f, 32.0f);
    float monstreScale = (isBossRaid ? screenH * 0.0070f : screenH * 0.0055f);
    // Inverser horizontalement pour faire face au héros
    spriteMonstre.setScale(-monstreScale, monstreScale);
    sf::Vector2f monstreBasePos(screenW * 0.75f, screenH * 0.45f);
    spriteMonstre.setPosition(monstreBasePos);

    // Journal de combat (log des messages)
    std::vector<std::string> journalCombat;
    journalCombat.push_back("Un redoutable " + monstre.nom + " surgit !");
    journalCombat.push_back("Que le combat commence ! A votre tour d'agir.");

    // Textes flottants
    std::vector<UI::FloatingText> floatingTexts;

    CombatState etat = CombatState::PLAYER_TURN;
    bool postureDefensive = false;
    bool menuObjetsOuvert = false;
    bool combatTermine = false;
    bool victoireJoueur = false;
    bool fuiteReussie = false;

    sf::Clock horlogeTour;
    sf::Clock horlogeAnim;
    float dureeAttente = 0.8f;
    int heroFrame = 0;
    int heroActionRow = 2; // 2 = marche/repos, 3 = attaque physique, 0 = magie
    int monstreFrame = 0;

    float shakeHero = 0.0f;
    float shakeMonstre = 0.0f;

    // Dimensions des boutons de commande
    float btnW = screenW * 0.20f;
    float btnH = screenH * 0.065f;
    float deckX = screenW * 0.06f;
    float deckY = screenH * 0.68f;
    float gapY = screenH * 0.075f;

    sf::RectangleShape btnAttaque(sf::Vector2f(btnW, btnH));
    btnAttaque.setPosition(deckX, deckY);
    btnAttaque.setOutlineThickness(2.0f);
    btnAttaque.setOutlineColor(UI::Gold);

    sf::Text txtAttaque("1. ATTAQUER", font, static_cast<unsigned int>(btnH * 0.40f));
    txtAttaque.setFillColor(UI::TextWhite);
    txtAttaque.setPosition(deckX + 15.0f, deckY + btnH * 0.25f);

    sf::RectangleShape btnSort(sf::Vector2f(btnW, btnH));
    btnSort.setPosition(deckX, deckY + gapY);
    btnSort.setOutlineThickness(2.0f);
    btnSort.setOutlineColor(UI::Gold);

    std::string nomSortCourt = joueur.competenceSpeciale;
    if (nomSortCourt.length() > 18) nomSortCourt = nomSortCourt.substr(0, 16) + "..";
    sf::Text txtSort("2. " + nomSortCourt, font, static_cast<unsigned int>(btnH * 0.36f));
    txtSort.setFillColor(UI::TextWhite);
    txtSort.setPosition(deckX + 15.0f, deckY + gapY + btnH * 0.25f);

    sf::RectangleShape btnObjets(sf::Vector2f(btnW, btnH));
    btnObjets.setPosition(deckX + btnW + screenW * 0.02f, deckY);
    btnObjets.setOutlineThickness(2.0f);
    btnObjets.setOutlineColor(UI::Gold);

    sf::Text txtObjets("3. INVENTAIRE", font, static_cast<unsigned int>(btnH * 0.40f));
    txtObjets.setFillColor(UI::TextWhite);
    txtObjets.setPosition(deckX + btnW + screenW * 0.02f + 15.0f, deckY + btnH * 0.25f);

    sf::RectangleShape btnFuir(sf::Vector2f(btnW, btnH));
    btnFuir.setPosition(deckX + btnW + screenW * 0.02f, deckY + gapY);
    btnFuir.setOutlineThickness(2.0f);
    btnFuir.setOutlineColor(UI::Gold);

    sf::Text txtFuir(isBossRaid ? "4. FUITE IMPOSSIBLE" : "4. FUIR", font, static_cast<unsigned int>(btnH * 0.40f));
    txtFuir.setFillColor(isBossRaid ? sf::Color(140, 140, 140) : UI::TextWhite);
    txtFuir.setPosition(deckX + btnW + screenW * 0.02f + 15.0f, deckY + gapY + btnH * 0.25f);

    // Boutons de sous-menu Objets
    float popW = screenW * 0.28f;
    float popH = screenH * 0.26f;
    float popX = deckX + btnW * 0.5f;
    float popY = deckY - popH - screenH * 0.02f;

    sf::RectangleShape popPanel(sf::Vector2f(popW, popH));
    popPanel.setPosition(popX, popY);
    popPanel.setFillColor(UI::DarkPanel);
    popPanel.setOutlineThickness(2.5f);
    popPanel.setOutlineColor(UI::GoldBright);

    float itemBtnH = popH * 0.20f;
    sf::RectangleShape btnItemSoin(sf::Vector2f(popW - 20.0f, itemBtnH));
    btnItemSoin.setPosition(popX + 10.0f, popY + 12.0f);
    btnItemSoin.setFillColor(UI::DarkPanelLight);
    btnItemSoin.setOutlineThickness(1.5f);
    btnItemSoin.setOutlineColor(UI::Gold);

    sf::Text txtItemSoin("", font, static_cast<unsigned int>(itemBtnH * 0.48f));
    txtItemSoin.setPosition(popX + 20.0f, popY + 15.0f);

    sf::RectangleShape btnItemGrande(sf::Vector2f(popW - 20.0f, itemBtnH));
    btnItemGrande.setPosition(popX + 10.0f, popY + 16.0f + itemBtnH);
    btnItemGrande.setFillColor(UI::DarkPanelLight);
    btnItemGrande.setOutlineThickness(1.5f);
    btnItemGrande.setOutlineColor(UI::Gold);

    sf::Text txtItemGrande("", font, static_cast<unsigned int>(itemBtnH * 0.48f));
    txtItemGrande.setPosition(popX + 20.0f, popY + 19.0f + itemBtnH);

    sf::RectangleShape btnItemMana(sf::Vector2f(popW - 20.0f, itemBtnH));
    btnItemMana.setPosition(popX + 10.0f, popY + 20.0f + itemBtnH * 2.0f);
    btnItemMana.setFillColor(UI::DarkPanelLight);
    btnItemMana.setOutlineThickness(1.5f);
    btnItemMana.setOutlineColor(UI::Gold);

    sf::Text txtItemMana("", font, static_cast<unsigned int>(itemBtnH * 0.48f));
    txtItemMana.setPosition(popX + 20.0f, popY + 23.0f + itemBtnH * 2.0f);

    sf::RectangleShape btnItemBouclier(sf::Vector2f(popW - 20.0f, itemBtnH));
    btnItemBouclier.setPosition(popX + 10.0f, popY + 24.0f + itemBtnH * 3.0f);
    btnItemBouclier.setFillColor(UI::DarkPanelLight);
    btnItemBouclier.setOutlineThickness(1.5f);
    btnItemBouclier.setOutlineColor(UI::Gold);

    sf::Text txtItemBouclier("", font, static_cast<unsigned int>(itemBtnH * 0.48f));
    txtItemBouclier.setPosition(popX + 20.0f, popY + 27.0f + itemBtnH * 3.0f);

    // Bouton de fin de combat
    float btnFinW = screenW * 0.24f;
    float btnFinH = screenH * 0.07f;
    sf::RectangleShape btnFin(sf::Vector2f(btnFinW, btnFinH));
    btnFin.setOrigin(btnFinW / 2.0f, btnFinH / 2.0f);
    btnFin.setPosition(centerX, screenH * 0.70f);
    btnFin.setFillColor(UI::Gold);
    btnFin.setOutlineThickness(2.0f);
    btnFin.setOutlineColor(UI::GoldBright);

    sf::Text txtFin("CONTINUER", font, static_cast<unsigned int>(btnFinH * 0.42f));
    txtFin.setFillColor(sf::Color::Black);
    sf::FloatRect bFin = txtFin.getLocalBounds();
    txtFin.setOrigin(bFin.left + bFin.width / 2.0f, bFin.top + bFin.height / 2.0f);
    txtFin.setPosition(btnFin.getPosition());

    sf::Clock deltaClock;

    while (window.isOpen() && !combatTermine)
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

                if (etat == CombatState::PLAYER_TURN)
                {
                    if (menuObjetsOuvert)
                    {
                        if (btnItemSoin.getGlobalBounds().contains(mPos.x, mPos.y))
                        {
                            if (joueur.inventaire.potionsNormales > 0)
                            {
                                joueur.inventaire.potionsNormales--;
                                int soin = 35;
                                joueur.vie = std::min(joueur.vie + soin, joueur.vieMax);
                                floatingTexts.push_back({"+" + std::to_string(soin) + " PV", heroBasePos, UI::GreenHP, 1.2f, 1.2f});
                                journalCombat.push_back(joueur.nom + " boit une Potion de Soin (+35 PV).");
                                menuObjetsOuvert = false;
                                etat = CombatState::ACTION_RESOLVING;
                                horlogeTour.restart();
                            }
                        }
                        else if (btnItemGrande.getGlobalBounds().contains(mPos.x, mPos.y))
                        {
                            if (joueur.inventaire.grandesPotions > 0)
                            {
                                joueur.inventaire.grandesPotions--;
                                joueur.vie = joueur.vieMax;
                                floatingTexts.push_back({"VIE 100% !", heroBasePos, UI::GreenHP, 1.2f, 1.2f});
                                journalCombat.push_back(joueur.nom + " consomme une Grande Potion (Soins 100%).");
                                menuObjetsOuvert = false;
                                etat = CombatState::ACTION_RESOLVING;
                                horlogeTour.restart();
                            }
                        }
                        else if (btnItemMana.getGlobalBounds().contains(mPos.x, mPos.y))
                        {
                            if (joueur.inventaire.potionsMana > 0)
                            {
                                joueur.inventaire.potionsMana--;
                                int manaRendu = 35;
                                joueur.mana = std::min(joueur.mana + manaRendu, joueur.manaMax);
                                floatingTexts.push_back({"+" + std::to_string(manaRendu) + " MP", heroBasePos, UI::BlueMana, 1.2f, 1.2f});
                                journalCombat.push_back(joueur.nom + " boit un Élixir de Mana (+35 MP).");
                                menuObjetsOuvert = false;
                                etat = CombatState::ACTION_RESOLVING;
                                horlogeTour.restart();
                            }
                        }
                        else if (btnItemBouclier.getGlobalBounds().contains(mPos.x, mPos.y))
                        {
                            if (joueur.inventaire.nombreBouclier > 0)
                            {
                                joueur.inventaire.nombreBouclier--;
                                postureDefensive = true;
                                floatingTexts.push_back({"BOUCLIER ACTIF !", heroBasePos, UI::GoldBright, 1.2f, 1.2f});
                                journalCombat.push_back(joueur.nom + " lève son Bouclier (Dégâts /2 au prochain coup) !");
                                menuObjetsOuvert = false;
                                etat = CombatState::ACTION_RESOLVING;
                                horlogeTour.restart();
                            }
                        }
                        else
                        {
                            menuObjetsOuvert = false;
                        }
                    }
                    else
                    {
                        // Action 1: Attaquer
                        if (btnAttaque.getGlobalBounds().contains(mPos.x, mPos.y))
                        {
                            heroActionRow = 3; // Slash
                            heroFrame = 0;

                            bool isCrit = (rand() % 100) < joueur.chanceCrit;
                            int degatsBruts = joueur.attaque + (rand() % 5);
                            if (isCrit) degatsBruts = static_cast<int>(degatsBruts * 1.7f);

                            int degatsFinaux = std::max(1, degatsBruts - (monstre.defense / 3));
                            monstre.vie = std::max(0, monstre.vie - degatsFinaux);

                            shakeMonstre = 0.25f;
                            std::string txtDmg = (isCrit ? "CRITIQUE ! -" : "-") + std::to_string(degatsFinaux);
                            floatingTexts.push_back({txtDmg, monstreBasePos, isCrit ? UI::GoldBright : UI::Crimson, 1.2f, 1.2f});

                            journalCombat.push_back(joueur.nom + " frappe " + monstre.nom + " pour " + std::to_string(degatsFinaux) + " degats !" + (isCrit ? " (Coup Critique !)" : ""));

                            etat = CombatState::ACTION_RESOLVING;
                            horlogeTour.restart();
                        }
                        // Action 2: Sort Spécial
                        else if (btnSort.getGlobalBounds().contains(mPos.x, mPos.y))
                        {
                            int coutMana = 15;
                            if (joueur.classeIndex == 0) coutMana = 10;
                            else if (joueur.classeIndex == 1) coutMana = 25;
                            else if (joueur.classeIndex == 4) coutMana = 20;

                            if (joueur.mana >= coutMana)
                            {
                                joueur.mana -= coutMana;
                                heroActionRow = 0; // Spellcast
                                heroFrame = 0;

                                int degatsSort = 0;
                                if (joueur.classeIndex == 0) // Guerrier: Coup Puissant
                                {
                                    degatsSort = static_cast<int>(joueur.attaque * 2.3f);
                                    monstre.vie = std::max(0, monstre.vie - degatsSort);
                                    floatingTexts.push_back({"FRACAS ! -" + std::to_string(degatsSort), monstreBasePos, UI::GoldBright, 1.3f, 1.3f});
                                    journalCombat.push_back(joueur.nom + " assène un Coup Puissant devastateur (" + std::to_string(degatsSort) + " degats) !");
                                }
                                else if (joueur.classeIndex == 1) // Mage: Boule de Feu
                                {
                                    degatsSort = static_cast<int>(joueur.attaque * 2.6f);
                                    monstre.vie = std::max(0, monstre.vie - degatsSort);
                                    floatingTexts.push_back({"INFERNO ! -" + std::to_string(degatsSort), monstreBasePos, sf::Color(255, 100, 30), 1.3f, 1.3f});
                                    journalCombat.push_back(joueur.nom + " dechaîne une furieuse Boule de Feu (" + std::to_string(degatsSort) + " degats) !");
                                }
                                else if (joueur.classeIndex == 2) // Archer: Tir de Précision
                                {
                                    degatsSort = static_cast<int>(joueur.attaque * 2.0f);
                                    monstre.vie = std::max(0, monstre.vie - degatsSort);
                                    floatingTexts.push_back({"PERFORANT ! -" + std::to_string(degatsSort), monstreBasePos, UI::GoldBright, 1.3f, 1.3f});
                                    journalCombat.push_back(joueur.nom + " décoche une flèche précise ignorant l'armure (" + std::to_string(degatsSort) + " degats) !");
                                }
                                else if (joueur.classeIndex == 3) // Paladin: Soin Divin + Frappe
                                {
                                    int soin = 45;
                                    joueur.vie = std::min(joueur.vie + soin, joueur.vieMax);
                                    degatsSort = static_cast<int>(joueur.attaque * 1.4f);
                                    monstre.vie = std::max(0, monstre.vie - degatsSort);
                                    floatingTexts.push_back({"+" + std::to_string(soin) + " PV", heroBasePos, UI::GreenHP, 1.2f, 1.2f});
                                    floatingTexts.push_back({"CHATIMENT ! -" + std::to_string(degatsSort), monstreBasePos, UI::GoldBright, 1.2f, 1.2f});
                                    journalCombat.push_back(joueur.nom + " invoque la Lumiere : +" + std::to_string(soin) + " PV et " + std::to_string(degatsSort) + " degats infliges !");
                                }
                                else if (joueur.classeIndex == 4) // Nécromancien: Malédiction (vol de vie)
                                {
                                    degatsSort = static_cast<int>(joueur.attaque * 1.8f);
                                    monstre.vie = std::max(0, monstre.vie - degatsSort);
                                    int volVie = degatsSort / 2;
                                    joueur.vie = std::min(joueur.vie + volVie, joueur.vieMax);
                                    floatingTexts.push_back({"DRAIN ! -" + std::to_string(degatsSort), monstreBasePos, sf::Color(180, 80, 240), 1.2f, 1.2f});
                                    floatingTexts.push_back({"+" + std::to_string(volVie) + " PV", heroBasePos, UI::GreenHP, 1.2f, 1.2f});
                                    journalCombat.push_back(joueur.nom + " draine la force vitale : -" + std::to_string(degatsSort) + " PV infliges, +" + std::to_string(volVie) + " PV absorbes !");
                                }
                                else // Assassin: Attaque Sournoise
                                {
                                    degatsSort = static_cast<int>(joueur.attaque * 2.5f);
                                    monstre.vie = std::max(0, monstre.vie - degatsSort);
                                    floatingTexts.push_back({"ASSASSINAT ! -" + std::to_string(degatsSort), monstreBasePos, UI::Crimson, 1.3f, 1.3f});
                                    journalCombat.push_back(joueur.nom + " fond depuis l'ombre et porte un coup mortel (" + std::to_string(degatsSort) + " degats) !");
                                }

                                shakeMonstre = 0.3f;
                                etat = CombatState::ACTION_RESOLVING;
                                horlogeTour.restart();
                            }
                            else
                            {
                                floatingTexts.push_back({"MANA INSUFFISANT !", heroBasePos, UI::BlueMana, 1.0f, 1.0f});
                            }
                        }
                        // Action 3: Ouvrir Inventaire
                        else if (btnObjets.getGlobalBounds().contains(mPos.x, mPos.y))
                        {
                            menuObjetsOuvert = true;
                        }
                        // Action 4: Fuir
                        else if (btnFuir.getGlobalBounds().contains(mPos.x, mPos.y))
                        {
                            if (isBossRaid)
                            {
                                floatingTexts.push_back({"IMPOSSIBLE DE FUIR LE BOSS !", heroBasePos, UI::Crimson, 1.2f, 1.2f});
                            }
                            else
                            {
                                if ((rand() % 100) < 75)
                                {
                                    journalCombat.push_back(joueur.nom + " profite d'une ouverture et s'enfuit avec succes !");
                                    fuiteReussie = true;
                                    combatTermine = true;
                                    victoireJoueur = false;
                                }
                                else
                                {
                                    journalCombat.push_back("Votre tentative de fuite echoue ! Le monstre bloque le passage.");
                                    floatingTexts.push_back({"FUITE ECHOUEE !", heroBasePos, UI::Crimson, 1.2f, 1.2f});
                                    etat = CombatState::ACTION_RESOLVING;
                                    horlogeTour.restart();
                                }
                            }
                        }
                    }
                }
                else if (etat == CombatState::VICTORY_SCREEN || etat == CombatState::DEFEAT_SCREEN)
                {
                    if (btnFin.getGlobalBounds().contains(mPos.x, mPos.y))
                    {
                        combatTermine = true;
                    }
                }
            }
        }

        // Gestion de l'état et résolution des tours
        if (etat == CombatState::ACTION_RESOLVING)
        {
            if (horlogeTour.getElapsedTime().asSeconds() >= dureeAttente)
            {
                if (monstre.vie <= 0)
                {
                    etat = CombatState::VICTORY_SCREEN;
                    victoireJoueur = true;
                    joueur.monstresVaincus++;
                    joueur.orJoueur += monstre.orRecompense;
                    joueur.xp += monstre.xpRecompense;

                    journalCombat.push_back("VICTOIRE ! " + monstre.nom + " s'effondre dans la poussiere !");
                    journalCombat.push_back("Vous ramassez " + std::to_string(monstre.orRecompense) + " PO et gagnez " + std::to_string(monstre.xpRecompense) + " XP !");

                    // Vérifier Level Up
                    if (joueur.xp >= joueur.xpSeuil)
                    {
                        joueur.niveau++;
                        joueur.xp -= joueur.xpSeuil;
                        joueur.xpSeuil += 60;
                        joueur.vieMax += 25;
                        joueur.manaMax += 10;
                        joueur.attaque += 5;
                        joueur.defense += 3;
                        joueur.vie = joueur.vieMax;
                        joueur.mana = joueur.manaMax;
                        journalCombat.push_back("★ LEVEL UP ! Vous atteignez le Niveau " + std::to_string(joueur.niveau) + " ! Stats augmenteed & Soins complets !");
                    }
                }
                else
                {
                    etat = CombatState::ENEMY_TURN;
                    horlogeTour.restart();
                }
            }
        }
        else if (etat == CombatState::ENEMY_TURN)
        {
            if (horlogeTour.getElapsedTime().asSeconds() >= 0.5f)
            {
                // Attaque du monstre
                int degatsEnnemi = monstre.attaque + (rand() % 4);
                if (postureDefensive)
                {
                    degatsEnnemi /= 2;
                    postureDefensive = false;
                    floatingTexts.push_back({"BOUCLIER ACTIF !", heroBasePos, UI::Gold, 1.0f, 1.0f});
                }

                degatsEnnemi = std::max(1, degatsEnnemi - (joueur.defense / 4));
                joueur.vie = std::max(0, joueur.vie - degatsEnnemi);

                shakeHero = 0.25f;
                floatingTexts.push_back({"-" + std::to_string(degatsEnnemi), heroBasePos, UI::Crimson, 1.2f, 1.2f});
                journalCombat.push_back(monstre.nom + " utilise [" + monstre.capaciteSpeciale + "] et vous inflige " + std::to_string(degatsEnnemi) + " degats !");

                if (joueur.vie <= 0)
                {
                    etat = CombatState::DEFEAT_SCREEN;
                    victoireJoueur = false;
                    journalCombat.push_back("Vous tombez a terre... Vos forces vous abandonnent.");
                }
                else
                {
                    etat = CombatState::PLAYER_TURN;
                    heroActionRow = 2; // Repos
                }
            }
        }

        // Animations des sprites
        if (horlogeAnim.getElapsedTime().asSeconds() > 0.15f)
        {
            heroFrame = (heroFrame + 1) % 6;
            monstreFrame = (monstreFrame + 1) % 6;
            horlogeAnim.restart();
        }

        // Tremblement d'impact
        sf::Vector2f curHeroPos = heroBasePos;
        if (shakeHero > 0.0f)
        {
            shakeHero -= dt;
            curHeroPos.x += (rand() % 11 - 5);
            curHeroPos.y += (rand() % 7 - 3);
        }
        spriteHero.setPosition(curHeroPos);

        sf::Vector2f curMonstrePos = monstreBasePos;
        if (shakeMonstre > 0.0f)
        {
            shakeMonstre -= dt;
            curMonstrePos.x += (rand() % 11 - 5);
            curMonstrePos.y += (rand() % 7 - 3);
        }
        spriteMonstre.setPosition(curMonstrePos);

        // Rectangles des textures de sprites
        spriteHero.setTextureRect(sf::IntRect(heroFrame * 64, heroActionRow * 64, 64, 64));
        spriteMonstre.setTextureRect(sf::IntRect(monstreFrame * 64, 128, 64, 64)); // row 2

        // Rendu
        window.clear(sf::Color(10, 10, 15));

        if (fondCharge) window.draw(spriteFond);

        // Voile d'ambiance
        sf::RectangleShape voile(sf::Vector2f(screenW, screenH));
        voile.setFillColor(sf::Color(12, 16, 24, 170));
        window.draw(voile);

        // En-tête de combat
        UI::drawPanel(window, screenW * 0.05f, screenH * 0.03f, screenW * 0.90f, screenH * 0.08f);
        sf::Text txtTitreCombat(isBossRaid ? "⚔ COMBAT DE BOSS DE ZONE : " + monstre.nom : "⚔ ENGAGEMENT : " + monstre.nom, font, static_cast<unsigned int>(screenH * 0.035f));
        txtTitreCombat.setFillColor(isBossRaid ? UI::Crimson : UI::GoldBright);
        sf::FloatRect bTC = txtTitreCombat.getLocalBounds();
        txtTitreCombat.setOrigin(bTC.left + bTC.width / 2.0f, bTC.top + bTC.height / 2.0f);
        txtTitreCombat.setPosition(centerX, screenH * 0.07f);
        window.draw(txtTitreCombat);

        // Cartes de statistiques au-dessus des combattants
        float cardW = screenW * 0.32f;
        float cardH = screenH * 0.12f;

        // Fiche Héros (Haut Gauche)
        UI::drawPanel(window, screenW * 0.08f, screenH * 0.14f, cardW, cardH, joueur.nom + " (" + joueur.classeNom + " Niv." + std::to_string(joueur.niveau) + ")", &font, 18);
        UI::drawProgressBar(window, screenW * 0.10f, screenH * 0.20f, cardW - screenW * 0.04f, 18.0f, joueur.vie, joueur.vieMax, UI::GreenHP, sf::Color(60, 20, 20), "PV: " + std::to_string(joueur.vie) + " / " + std::to_string(joueur.vieMax), font, 13);
        UI::drawProgressBar(window, screenW * 0.10f, screenH * 0.23f, cardW - screenW * 0.04f, 14.0f, joueur.mana, joueur.manaMax, UI::BlueMana, sf::Color(20, 30, 60), "Mana: " + std::to_string(joueur.mana) + " / " + std::to_string(joueur.manaMax), font, 11);

        // Fiche Monstre (Haut Droite)
        UI::drawPanel(window, screenW * 0.60f, screenH * 0.14f, cardW, cardH, monstre.nom + (isBossRaid ? " [BOSS]" : ""), &font, 18);
        UI::drawProgressBar(window, screenW * 0.62f, screenH * 0.20f, cardW - screenW * 0.04f, 18.0f, monstre.vie, monstre.vieMax, UI::Crimson, sf::Color(50, 50, 50), "PV: " + std::to_string(monstre.vie) + " / " + std::to_string(monstre.vieMax), font, 13);
        sf::Text txtStatsM("ATQ: " + std::to_string(monstre.attaque) + " | DEF: " + std::to_string(monstre.defense) + " | " + monstre.capaciteSpeciale, font, 14);
        txtStatsM.setFillColor(UI::TextMuted);
        txtStatsM.setPosition(screenW * 0.62f, screenH * 0.23f);
        window.draw(txtStatsM);

        // Dessin des sprites des combattants
        window.draw(spriteHero);
        window.draw(spriteMonstre);

        // Panneau Journal de Combat (Bas Droite)
        float logW = screenW * 0.44f;
        float logH = screenH * 0.28f;
        float logX = screenW * 0.50f;
        float logY = screenH * 0.66f;
        UI::drawPanel(window, logX, logY, logW, logH, "CHRONIQUE DE LA BATAILLE", &font, 18);

        int maxLignes = 6;
        int debut = std::max(0, static_cast<int>(journalCombat.size()) - maxLignes);
        for (size_t i = debut; i < journalCombat.size(); ++i)
        {
            sf::Text txtLigne(journalCombat[i], font, static_cast<unsigned int>(screenH * 0.020f));
            if (i == journalCombat.size() - 1)
                txtLigne.setFillColor(UI::GoldBright);
            else
                txtLigne.setFillColor(UI::TextWhite);

            txtLigne.setPosition(logX + 15.0f, logY + 45.0f + (i - debut) * (screenH * 0.035f));
            window.draw(txtLigne);
        }

        // Command Deck (Bas Gauche)
        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (etat == CombatState::PLAYER_TURN)
        {
            UI::drawButton(window, btnAttaque, txtAttaque, btnAttaque.getGlobalBounds().contains(mPos.x, mPos.y));
            UI::drawButton(window, btnSort, txtSort, btnSort.getGlobalBounds().contains(mPos.x, mPos.y));
            UI::drawButton(window, btnObjets, txtObjets, btnObjets.getGlobalBounds().contains(mPos.x, mPos.y));
            UI::drawButton(window, btnFuir, txtFuir, (!isBossRaid && btnFuir.getGlobalBounds().contains(mPos.x, mPos.y)));

            // Sous-menu inventaire
            if (menuObjetsOuvert)
            {
                window.draw(popPanel);

                txtItemSoin.setString("1. Potion de Soin (+35 PV)  x" + std::to_string(joueur.inventaire.potionsNormales));
                txtItemGrande.setString("2. Grande Potion (100% PV)  x" + std::to_string(joueur.inventaire.grandesPotions));
                txtItemMana.setString("3. Potion de Mana (+35 MP)  x" + std::to_string(joueur.inventaire.potionsMana));
                txtItemBouclier.setString("4. Bouclier (Dégâts /2)    x" + std::to_string(joueur.inventaire.nombreBouclier));

                UI::drawButton(window, btnItemSoin, txtItemSoin, btnItemSoin.getGlobalBounds().contains(mPos.x, mPos.y));
                UI::drawButton(window, btnItemGrande, txtItemGrande, btnItemGrande.getGlobalBounds().contains(mPos.x, mPos.y));
                UI::drawButton(window, btnItemMana, txtItemMana, btnItemMana.getGlobalBounds().contains(mPos.x, mPos.y));
                UI::drawButton(window, btnItemBouclier, txtItemBouclier, btnItemBouclier.getGlobalBounds().contains(mPos.x, mPos.y));
            }
        }

        // Écran de Victoire / Défaite superposé
        if (etat == CombatState::VICTORY_SCREEN || etat == CombatState::DEFEAT_SCREEN)
        {
            sf::RectangleShape popFin(sf::Vector2f(screenW * 0.50f, screenH * 0.40f));
            popFin.setOrigin(popFin.getSize().x / 2.0f, popFin.getSize().y / 2.0f);
            popFin.setPosition(centerX, centerY);
            popFin.setFillColor(UI::DarkPanel);
            popFin.setOutlineThickness(3.0f);
            popFin.setOutlineColor(etat == CombatState::VICTORY_SCREEN ? UI::GoldBright : UI::Crimson);
            window.draw(popFin);

            sf::Text txtTitreFin(etat == CombatState::VICTORY_SCREEN ? "★ VICTOIRE TRIOMPHALE ★" : "☠ VOUS AVEZ PÉRI ☠", font, static_cast<unsigned int>(screenH * 0.045f));
            txtTitreFin.setFillColor(etat == CombatState::VICTORY_SCREEN ? UI::GoldBright : UI::Crimson);
            sf::FloatRect bTF = txtTitreFin.getLocalBounds();
            txtTitreFin.setOrigin(bTF.left + bTF.width / 2.0f, bTF.top + bTF.height / 2.0f);
            txtTitreFin.setPosition(centerX, centerY - screenH * 0.12f);
            window.draw(txtTitreFin);

            std::string resumeText;
            if (etat == CombatState::VICTORY_SCREEN)
            {
                resumeText = "Le monstre " + monstre.nom + " a été vaincu !\n\n"
                    + "Gains : +" + std::to_string(monstre.orRecompense) + " Pièces d'Or | +" + std::to_string(monstre.xpRecompense) + " XP\n"
                    + "Niveau actuel : " + std::to_string(joueur.niveau) + " (" + std::to_string(joueur.xp) + "/" + std::to_string(joueur.xpSeuil) + " XP)";
            }
            else
            {
                resumeText = "Vos blessures sont trop profondes...\nLes prêtres de la Cité vous rapatrient au sanctuaire.";
            }

            sf::Text txtResume(resumeText, font, static_cast<unsigned int>(screenH * 0.024f));
            txtResume.setFillColor(UI::TextWhite);
            sf::FloatRect bR = txtResume.getLocalBounds();
            txtResume.setOrigin(bR.left + bR.width / 2.0f, bR.top + bR.height / 2.0f);
            txtResume.setPosition(centerX, centerY - screenH * 0.01f);
            window.draw(txtResume);

            btnFin.setPosition(centerX, centerY + screenH * 0.12f);
            txtFin.setPosition(btnFin.getPosition());
            txtFin.setString(etat == CombatState::VICTORY_SCREEN ? "CONTINUER" : "RESSUSCITER");
            sf::FloatRect bF2 = txtFin.getLocalBounds();
            txtFin.setOrigin(bF2.left + bF2.width / 2.0f, bF2.top + bF2.height / 2.0f);

            UI::drawButton(window, btnFin, txtFin, btnFin.getGlobalBounds().contains(mPos.x, mPos.y),
                           etat == CombatState::VICTORY_SCREEN ? UI::Gold : UI::Crimson,
                           UI::GoldBright,
                           sf::Color::Black,
                           sf::Color::Black);
        }

        // Textes flottants
        UI::updateAndDrawFloatingTexts(window, font, floatingTexts, dt);

        window.display();
    }

    if (joueur.vie <= 0)
    {
        // Résurrection avec 50% de PV
        joueur.vie = std::max(1, joueur.vieMax / 2);
    }

    return victoireJoueur;
}

#endif