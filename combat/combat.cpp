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

    if (isBossRaid)
        AudioJeu::jouer("assets/Music/Orchestral Epic Fantasy Music _ OpenGameArt.org.OGG", 35.0f, true);
    else
        AudioJeu::jouer("assets/Music/RPG Battle Theme - The Last Encounter _ OpenGameArt.org.ogg", 30.0f, true);

    sf::Texture texFond;
    bool fondCharge = texFond.loadFromFile("assets/backgrounds/introBg.jpg");
    sf::Sprite spriteFond;
    if (fondCharge)
    {
        spriteFond.setTexture(texFond);
        sf::Vector2u sz = texFond.getSize();
        spriteFond.setScale(screenW / sz.x, screenH / sz.y);
    }

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
    spriteHero.setTextureRect(sf::IntRect(0, 192, 64, 64));
    spriteHero.setOrigin(32.0f, 32.0f);
    float heroScale = screenH * 0.0055f;
    spriteHero.setScale(heroScale, heroScale);
    sf::Vector2f heroBasePos(screenW * 0.25f, screenH * 0.45f);
    spriteHero.setPosition(heroBasePos);

    // Image dediee du monstre si disponible, sinon sprite generique de secours
    sf::Texture texMonstreDediee;
    bool monstreDedieeChargee = false;
    if (!monstre.sprite.empty())
        monstreDedieeChargee = texMonstreDediee.loadFromFile(monstre.sprite);

    float tailleSpriteMonstre = (isBossRaid ? screenH * 0.28f : screenH * 0.18f);

    sf::Sprite spriteMonstre;
    if (monstreDedieeChargee)
    {
        spriteMonstre.setTexture(texMonstreDediee);
        sf::Vector2u szM = texMonstreDediee.getSize();
        spriteMonstre.setOrigin(szM.x / 2.0f, szM.y / 2.0f);
        spriteMonstre.setScale(tailleSpriteMonstre / szM.x, tailleSpriteMonstre / szM.x);
    }
    else
    {
        spriteMonstre.setTexture(*texturesHero[monstre.spriteType % 6]);
        spriteMonstre.setTextureRect(sf::IntRect(0, 64, 64, 64));
        spriteMonstre.setOrigin(32.0f, 32.0f);
        float monstreScale = (isBossRaid ? screenH * 0.0070f : screenH * 0.0055f);
        spriteMonstre.setScale(monstreScale, monstreScale);
    }
    sf::Vector2f monstreBasePos(screenW * 0.75f, screenH * 0.45f);
    spriteMonstre.setPosition(monstreBasePos);

    // Aura pulsante derriere les boss : signal visuel de danger
    float rayonAura = tailleSpriteMonstre * 0.62f;
    sf::CircleShape auraBoss(rayonAura);
    auraBoss.setOrigin(rayonAura, rayonAura);
    auraBoss.setPosition(monstreBasePos);
    auraBoss.setFillColor(sf::Color(215, 50, 50, 80));
    auraBoss.setOutlineThickness(3.0f);
    auraBoss.setOutlineColor(sf::Color(255, 90, 90, 160));

    // Intro dramatique des boss : flash rouge + secousse de l'ecran
    float introBoss = isBossRaid ? 1.2f : 0.0f;

    std::vector<std::string> journalCombat;
    journalCombat.push_back("Un redoutable " + monstre.nom + " surgit !");
    journalCombat.push_back("Que le combat commence ! A votre tour d'agir.");

    std::vector<UI::FloatingText> floatingTexts;

    CombatState etat = CombatState::PLAYER_TURN;
    int niveauAvantCombat = joueur.niveau; // pour soigner seulement si level up reel
    bool postureDefensive = false;
    bool menuObjetsOuvert = false;
    bool combatTermine = false;
    bool victoireJoueur = false;

    sf::Clock horlogeTour;
    sf::Clock horlogeTotal;
    float dureeAttente = 0.8f;

    float shakeHero = 0.0f;
    float shakeMonstre = 0.0f;
    float heroLunge = 0.0f;
    float monstreLunge = 0.0f;

    float btnW = screenW * 0.20f;
    float btnH = screenH * 0.065f;
    float deckX = screenW * 0.06f;
    float deckY = screenH * 0.68f;
    float gapY = screenH * 0.075f;

    sf::RectangleShape btnAttaque(sf::Vector2f(btnW, btnH));
    btnAttaque.setPosition(deckX, deckY);
    btnAttaque.setOutlineThickness(0.0f);

    sf::Text txtAttaque("1. ATTAQUER", font, static_cast<unsigned int>(btnH * 0.40f));
    txtAttaque.setFillColor(UI::TextWhite);
    txtAttaque.setPosition(deckX + 15.0f, deckY + btnH * 0.25f);

    sf::RectangleShape btnSort(sf::Vector2f(btnW, btnH));
    btnSort.setPosition(deckX, deckY + gapY);
    btnSort.setOutlineThickness(0.0f);

    std::string nomSortCourt = joueur.competenceSpeciale;
    if (nomSortCourt.length() > 18) nomSortCourt = nomSortCourt.substr(0, 16) + "..";
    sf::Text txtSort("2. " + nomSortCourt, font, static_cast<unsigned int>(btnH * 0.36f));
    txtSort.setFillColor(UI::TextWhite);
    txtSort.setPosition(deckX + 15.0f, deckY + gapY + btnH * 0.25f);

    sf::RectangleShape btnObjets(sf::Vector2f(btnW, btnH));
    btnObjets.setPosition(deckX + btnW + screenW * 0.02f, deckY);
    btnObjets.setOutlineThickness(0.0f);

    sf::Text txtObjets("3. INVENTAIRE", font, static_cast<unsigned int>(btnH * 0.40f));
    txtObjets.setFillColor(UI::TextWhite);
    txtObjets.setPosition(deckX + btnW + screenW * 0.02f + 15.0f, deckY + btnH * 0.25f);

    sf::RectangleShape btnFuir(sf::Vector2f(btnW, btnH));
    btnFuir.setPosition(deckX + btnW + screenW * 0.02f, deckY + gapY);
    btnFuir.setOutlineThickness(0.0f);

    sf::Text txtFuir(isBossRaid ? "4. FUITE IMPOSSIBLE" : "4. FUIR", font, static_cast<unsigned int>(btnH * 0.40f));
    txtFuir.setFillColor(isBossRaid ? sf::Color(140, 140, 140) : UI::TextWhite);
    txtFuir.setPosition(deckX + btnW + screenW * 0.02f + 15.0f, deckY + gapY + btnH * 0.25f);

    float popW = screenW * 0.28f;
    float popH = screenH * 0.26f;
    float popX = deckX + btnW * 0.5f;
    float popY = deckY - popH - screenH * 0.02f;

    sf::RectangleShape popPanel(sf::Vector2f(popW, popH));
    popPanel.setPosition(popX, popY);
    popPanel.setFillColor(UI::DarkPanel);
    popPanel.setOutlineThickness(1.5f);
    popPanel.setOutlineColor(UI::BorderMuted);

    float itemBtnH = popH * 0.20f;
    sf::RectangleShape btnItemSoin(sf::Vector2f(popW - 20.0f, itemBtnH));
    btnItemSoin.setPosition(popX + 10.0f, popY + 12.0f);
    btnItemSoin.setFillColor(UI::DarkPanelLight);
    btnItemSoin.setOutlineThickness(0.0f);

    sf::Text txtItemSoin("", font, static_cast<unsigned int>(itemBtnH * 0.48f));
    txtItemSoin.setPosition(popX + 20.0f, popY + 15.0f);

    sf::RectangleShape btnItemGrande(sf::Vector2f(popW - 20.0f, itemBtnH));
    btnItemGrande.setPosition(popX + 10.0f, popY + 16.0f + itemBtnH);
    btnItemGrande.setFillColor(UI::DarkPanelLight);
    btnItemGrande.setOutlineThickness(0.0f);

    sf::Text txtItemGrande("", font, static_cast<unsigned int>(itemBtnH * 0.48f));
    txtItemGrande.setPosition(popX + 20.0f, popY + 19.0f + itemBtnH);

    sf::RectangleShape btnItemMana(sf::Vector2f(popW - 20.0f, itemBtnH));
    btnItemMana.setPosition(popX + 10.0f, popY + 20.0f + itemBtnH * 2.0f);
    btnItemMana.setFillColor(UI::DarkPanelLight);
    btnItemMana.setOutlineThickness(0.0f);

    sf::Text txtItemMana("", font, static_cast<unsigned int>(itemBtnH * 0.48f));
    txtItemMana.setPosition(popX + 20.0f, popY + 23.0f + itemBtnH * 2.0f);

    sf::RectangleShape btnItemBouclier(sf::Vector2f(popW - 20.0f, itemBtnH));
    btnItemBouclier.setPosition(popX + 10.0f, popY + 24.0f + itemBtnH * 3.0f);
    btnItemBouclier.setFillColor(UI::DarkPanelLight);
    btnItemBouclier.setOutlineThickness(0.0f);

    sf::Text txtItemBouclier("", font, static_cast<unsigned int>(itemBtnH * 0.48f));
    txtItemBouclier.setPosition(popX + 20.0f, popY + 27.0f + itemBtnH * 3.0f);

    float btnFinW = screenW * 0.24f;
    float btnFinH = screenH * 0.07f;
    sf::RectangleShape btnFin(sf::Vector2f(btnFinW, btnFinH));
    btnFin.setOrigin(btnFinW / 2.0f, btnFinH / 2.0f);
    btnFin.setPosition(centerX, screenH * 0.70f);
    btnFin.setFillColor(UI::DarkPanelLight);
    btnFin.setOutlineThickness(0.0f);

    sf::Text txtFin("CONTINUER", font, static_cast<unsigned int>(btnFinH * 0.42f));
    txtFin.setFillColor(UI::TextWhite);
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
                                // Utiliser un objet ne consomme pas le tour : l'ennemi n'attaque pas apres
                                menuObjetsOuvert = false;
                                etat = CombatState::PLAYER_TURN;
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
                                // Utiliser un objet ne consomme pas le tour : l'ennemi n'attaque pas apres
                                menuObjetsOuvert = false;
                                etat = CombatState::PLAYER_TURN;
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
                                journalCombat.push_back(joueur.nom + " boit un Elixir de Mana (+35 MP).");
                                // Utiliser un objet ne consomme pas le tour : l'ennemi n'attaque pas apres
                                menuObjetsOuvert = false;
                                etat = CombatState::PLAYER_TURN;
                            }
                        }
                        else if (btnItemBouclier.getGlobalBounds().contains(mPos.x, mPos.y))
                        {
                            if (joueur.inventaire.nombreBouclier > 0)
                            {
                                joueur.inventaire.nombreBouclier--;
                                postureDefensive = true;
                                floatingTexts.push_back({"BOUCLIER ACTIF !", heroBasePos, UI::Silver, 1.2f, 1.2f});
                                journalCombat.push_back(joueur.nom + " leve son Bouclier (Degats /2 au prochain coup) !");
                                // Le bouclier est un vrai choix tactique : il consomme le tour,
                                // contrairement aux potions qui restent gratuites.
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
                        if (btnAttaque.getGlobalBounds().contains(mPos.x, mPos.y))
                        {
                            heroLunge = 0.25f;
                            bool isCrit = (rand() % 100) < joueur.chanceCrit;
                            int degatsBruts = joueur.attaque + (rand() % 5);
                            if (isCrit) degatsBruts = static_cast<int>(degatsBruts * 1.7f);

                            int degatsFinaux = std::max(1, degatsBruts - (monstre.defense / 3));
                            monstre.vie = std::max(0, monstre.vie - degatsFinaux);

                            shakeMonstre = 0.25f;
                            std::string txtDmg = (isCrit ? "CRITIQUE ! -" : "-") + std::to_string(degatsFinaux);
                            floatingTexts.push_back({txtDmg, monstreBasePos, isCrit ? UI::Azure : UI::Crimson, 1.2f, 1.2f});

                            journalCombat.push_back(joueur.nom + " frappe " + monstre.nom + " pour " + std::to_string(degatsFinaux) + " degats !" + (isCrit ? " (Coup Critique !)" : ""));

                            etat = CombatState::ACTION_RESOLVING;
                            horlogeTour.restart();
                        }
                        else if (btnSort.getGlobalBounds().contains(mPos.x, mPos.y))
                        {
                            int coutMana = 20;
                            if (joueur.classeIndex == 0) coutMana = 15;      // Guerrier - Coup Puissant
                            else if (joueur.classeIndex == 1) coutMana = 25; // Mage - Boule de Feu
                            else if (joueur.classeIndex == 2) coutMana = 15; // Archer - Tir de Precision
                            else if (joueur.classeIndex == 3) coutMana = 20; // Paladin - Soin Divin
                            else if (joueur.classeIndex == 4) coutMana = 25; // Necromancien - Malediction
                            // Assassin : 20

                            if (joueur.mana >= coutMana)
                            {
                                joueur.mana -= coutMana;
                                heroLunge = 0.35f;

                                // Degats de sort = multiplicateur x ATQ, avec une part d'aléatoire
                                // (+0 a +5) comme l'attaque normale, pour eviter les valeurs figees.
                                int degatsSort = 0;
                                if (joueur.classeIndex == 0)
                                {
                                    degatsSort = static_cast<int>(joueur.attaque * 1.6f) + (rand() % 6);
                                    monstre.vie = std::max(0, monstre.vie - degatsSort);
                                    floatingTexts.push_back({"FRACAS ! -" + std::to_string(degatsSort), monstreBasePos, UI::Azure, 1.3f, 1.3f});
                                    journalCombat.push_back(joueur.nom + " assene un Coup Puissant devastateur (" + std::to_string(degatsSort) + " degats) !");
                                }
                                else if (joueur.classeIndex == 1)
                                {
                                    degatsSort = static_cast<int>(joueur.attaque * 1.8f) + (rand() % 6);
                                    monstre.vie = std::max(0, monstre.vie - degatsSort);
                                    floatingTexts.push_back({"INFERNO ! -" + std::to_string(degatsSort), monstreBasePos, sf::Color(255, 120, 50), 1.3f, 1.3f});
                                    journalCombat.push_back(joueur.nom + " dechaîne une furieuse Boule de Feu (" + std::to_string(degatsSort) + " degats) !");
                                }
                                else if (joueur.classeIndex == 2)
                                {
                                    degatsSort = static_cast<int>(joueur.attaque * 1.5f) + (rand() % 6);
                                    monstre.vie = std::max(0, monstre.vie - degatsSort);
                                    floatingTexts.push_back({"PERFORANT ! -" + std::to_string(degatsSort), monstreBasePos, UI::Azure, 1.3f, 1.3f});
                                    journalCombat.push_back(joueur.nom + " decoche une fleche precise ignorant l'armure (" + std::to_string(degatsSort) + " degats) !");
                                }
                                else if (joueur.classeIndex == 3)
                                {
                                    int soin = 35;
                                    joueur.vie = std::min(joueur.vie + soin, joueur.vieMax);
                                    degatsSort = static_cast<int>(joueur.attaque * 1.2f) + (rand() % 6);
                                    monstre.vie = std::max(0, monstre.vie - degatsSort);
                                    floatingTexts.push_back({"+" + std::to_string(soin) + " PV", heroBasePos, UI::GreenHP, 1.2f, 1.2f});
                                    floatingTexts.push_back({"CHATIMENT ! -" + std::to_string(degatsSort), monstreBasePos, UI::Azure, 1.2f, 1.2f});
                                    journalCombat.push_back(joueur.nom + " invoque la Lumiere : +" + std::to_string(soin) + " PV et " + std::to_string(degatsSort) + " degats infliges !");
                                }
                                else if (joueur.classeIndex == 4)
                                {
                                    degatsSort = static_cast<int>(joueur.attaque * 1.5f) + (rand() % 6);
                                    monstre.vie = std::max(0, monstre.vie - degatsSort);
                                    int volVie = degatsSort / 2;
                                    joueur.vie = std::min(joueur.vie + volVie, joueur.vieMax);
                                    floatingTexts.push_back({"DRAIN ! -" + std::to_string(degatsSort), monstreBasePos, sf::Color(190, 95, 255), 1.2f, 1.2f});
                                    floatingTexts.push_back({"+" + std::to_string(volVie) + " PV", heroBasePos, UI::GreenHP, 1.2f, 1.2f});
                                    journalCombat.push_back(joueur.nom + " draine la force vitale : -" + std::to_string(degatsSort) + " PV infliges, +" + std::to_string(volVie) + " PV absorbes !");
                                }
                                else
                                {
                                    degatsSort = static_cast<int>(joueur.attaque * 1.8f) + (rand() % 6);
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
                        else if (btnObjets.getGlobalBounds().contains(mPos.x, mPos.y))
                        {
                            menuObjetsOuvert = true;
                        }
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

                    // Boucle while : gere le franchissement de plusieurs niveaux d'un coup
                    while (joueur.xp >= joueur.xpSeuil)
                    {
                        joueur.niveau++;
                        joueur.xp -= joueur.xpSeuil;
                        joueur.xpSeuil += 60;
                        joueur.vieMax += 15;
                        joueur.manaMax += 5;
                        joueur.attaque += 4;
                        joueur.defense += 2;
                        journalCombat.push_back("★ LEVEL UP ! Vous atteignez le Niveau " + std::to_string(joueur.niveau) + " ! Stats augmentees !");
                    }
                    if (joueur.niveau > niveauAvantCombat)
                    {
                        joueur.vie = joueur.vieMax;
                        joueur.mana = joueur.manaMax;
                        journalCombat.push_back("Soins complets : vos PV et votre Mana sont restaures !");
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
                monstreLunge = 0.25f;
                int degatsEnnemi = monstre.attaque + (rand() % 4);
                if (postureDefensive)
                {
                    degatsEnnemi /= 2;
                    postureDefensive = false;
                    floatingTexts.push_back({"BOUCLIER ACTIF !", heroBasePos, UI::Silver, 1.0f, 1.0f});
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
                }
            }
        }

        float totalTime = horlogeTotal.getElapsedTime().asSeconds();
        float bobHero = std::sin(totalTime * 3.0f) * 2.0f;
        float bobMonstre = std::sin((totalTime + 1.0f) * 3.0f) * 2.0f;

        sf::Vector2f curHeroPos = heroBasePos;
        curHeroPos.y += bobHero;
        if (heroLunge > 0.0f)
        {
            heroLunge -= dt;
            curHeroPos.x += 35.0f;
        }
        if (shakeHero > 0.0f)
        {
            shakeHero -= dt;
            curHeroPos.x += (rand() % 9 - 4);
            curHeroPos.y += (rand() % 7 - 3);
        }
        spriteHero.setPosition(curHeroPos);

        sf::Vector2f curMonstrePos = monstreBasePos;
        curMonstrePos.y += bobMonstre;
        if (monstreLunge > 0.0f)
        {
            monstreLunge -= dt;
            curMonstrePos.x -= 35.0f;
        }
        if (shakeMonstre > 0.0f)
        {
            shakeMonstre -= dt;
            curMonstrePos.x += (rand() % 9 - 4);
            curMonstrePos.y += (rand() % 7 - 3);
        }
        spriteMonstre.setPosition(curMonstrePos);

        window.clear(sf::Color(10, 12, 18));

        // Secousse de l'ecran amortie pendant l'intro du boss
        if (introBoss > 0.0f)
        {
            introBoss -= dt;
            float intensite = std::max(0.0f, introBoss) / 1.2f;
            sf::View vueEcran = window.getDefaultView();
            vueEcran.move((rand() % 13 - 6) * intensite, (rand() % 13 - 6) * intensite);
            window.setView(vueEcran);
        }
        else
        {
            window.setView(window.getDefaultView());
        }

        if (fondCharge) window.draw(spriteFond);

        sf::RectangleShape voile(sf::Vector2f(screenW, screenH));
        voile.setFillColor(sf::Color(12, 16, 24, 180));
        window.draw(voile);

        // Flash rouge decroissant pendant l'intro du boss
        if (introBoss > 0.0f)
        {
            sf::RectangleShape flashRouge(sf::Vector2f(screenW, screenH));
            flashRouge.setFillColor(sf::Color(160, 20, 20, static_cast<sf::Uint8>(130 * (introBoss / 1.2f))));
            window.draw(flashRouge);
        }

        UI::drawPanel(window, screenW * 0.05f, screenH * 0.03f, screenW * 0.90f, screenH * 0.08f);
        sf::Text txtTitreCombat(isBossRaid ? "COMBAT DE BOSS DE ZONE : " + monstre.nom : "ENGAGEMENT : " + monstre.nom, font, static_cast<unsigned int>(screenH * 0.035f));
        txtTitreCombat.setFillColor(isBossRaid ? UI::Crimson : UI::Silver);
        sf::FloatRect bTC = txtTitreCombat.getLocalBounds();
        txtTitreCombat.setOrigin(bTC.left + bTC.width / 2.0f, bTC.top + bTC.height / 2.0f);
        txtTitreCombat.setPosition(centerX, screenH * 0.07f);
        window.draw(txtTitreCombat);

        float cardW = screenW * 0.32f;
        float cardH = screenH * 0.12f;

        UI::drawPanel(window, screenW * 0.08f, screenH * 0.14f, cardW, cardH, joueur.nom + " (" + joueur.classeNom + " Niv." + std::to_string(joueur.niveau) + ")", &font, 20);
        UI::drawProgressBar(window, screenW * 0.10f, screenH * 0.20f, cardW - screenW * 0.04f, 18.0f, joueur.vie, joueur.vieMax, UI::GreenHP, sf::Color(55, 20, 20), "PV: " + std::to_string(joueur.vie) + " / " + std::to_string(joueur.vieMax), font, 15);
        UI::drawProgressBar(window, screenW * 0.10f, screenH * 0.23f, cardW - screenW * 0.04f, 14.0f, joueur.mana, joueur.manaMax, UI::BlueMana, sf::Color(20, 30, 55), "Mana: " + std::to_string(joueur.mana) + " / " + std::to_string(joueur.manaMax), font, 13);

        UI::drawPanel(window, screenW * 0.60f, screenH * 0.14f, cardW, cardH, monstre.nom + (isBossRaid ? " [BOSS]" : ""), &font, 20);
        UI::drawProgressBar(window, screenW * 0.62f, screenH * 0.20f, cardW - screenW * 0.04f, 18.0f, monstre.vie, monstre.vieMax, UI::Crimson, sf::Color(45, 45, 45), "PV: " + std::to_string(monstre.vie) + " / " + std::to_string(monstre.vieMax), font, 15);
        sf::Text txtStatsM("ATQ: " + std::to_string(monstre.attaque) + " | DEF: " + std::to_string(monstre.defense) + " | " + monstre.capaciteSpeciale, font, 16);
        txtStatsM.setFillColor(UI::TextMuted);
        txtStatsM.setPosition(screenW * 0.62f, screenH * 0.23f);
        window.draw(txtStatsM);

        window.draw(spriteHero);

        // Aura pulsante dessinee derriere le boss
        if (isBossRaid)
        {
            sf::Color cAura(215, 50, 50);
            cAura.a = static_cast<sf::Uint8>(90 + 45 * std::sin(totalTime * 2.5f));
            auraBoss.setFillColor(cAura);
            float pulsation = 1.0f + 0.06f * std::sin(totalTime * 2.5f);
            auraBoss.setScale(pulsation, pulsation);
            window.draw(auraBoss);
        }

        window.draw(spriteMonstre);

        float logW = screenW * 0.44f;
        float logH = screenH * 0.28f;
        float logX = screenW * 0.50f;
        float logY = screenH * 0.66f;
        UI::drawPanel(window, logX, logY, logW, logH, "CHRONIQUE DE LA BATAILLE", &font, 20);

        int maxLignes = 6;
        int debut = std::max(0, static_cast<int>(journalCombat.size()) - maxLignes);
        for (size_t i = debut; i < journalCombat.size(); ++i)
        {
            sf::Text txtLigne(journalCombat[i], font, static_cast<unsigned int>(screenH * 0.024f));
            if (i == journalCombat.size() - 1)
                txtLigne.setFillColor(UI::Azure);
            else
                txtLigne.setFillColor(UI::TextWhite);

            txtLigne.setPosition(logX + 15.0f, logY + 45.0f + (i - debut) * (screenH * 0.035f));
            window.draw(txtLigne);
        }

        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (etat == CombatState::PLAYER_TURN)
        {
            UI::drawButton(window, btnAttaque, txtAttaque, btnAttaque.getGlobalBounds().contains(mPos.x, mPos.y));
            UI::drawButton(window, btnSort, txtSort, btnSort.getGlobalBounds().contains(mPos.x, mPos.y));
            UI::drawButton(window, btnObjets, txtObjets, btnObjets.getGlobalBounds().contains(mPos.x, mPos.y));
            UI::drawButton(window, btnFuir, txtFuir, (!isBossRaid && btnFuir.getGlobalBounds().contains(mPos.x, mPos.y)));

            if (menuObjetsOuvert)
            {
                window.draw(popPanel);

                txtItemSoin.setString("1. Potion de Soin (+35 PV)  x" + std::to_string(joueur.inventaire.potionsNormales));
                txtItemGrande.setString("2. Grande Potion (100% PV)  x" + std::to_string(joueur.inventaire.grandesPotions));
                txtItemMana.setString("3. Potion de Mana (+35 MP)  x" + std::to_string(joueur.inventaire.potionsMana));
                txtItemBouclier.setString("4. Bouclier (Degats /2)    x" + std::to_string(joueur.inventaire.nombreBouclier));

                UI::drawButton(window, btnItemSoin, txtItemSoin, btnItemSoin.getGlobalBounds().contains(mPos.x, mPos.y));
                UI::drawButton(window, btnItemGrande, txtItemGrande, btnItemGrande.getGlobalBounds().contains(mPos.x, mPos.y));
                UI::drawButton(window, btnItemMana, txtItemMana, btnItemMana.getGlobalBounds().contains(mPos.x, mPos.y));
                UI::drawButton(window, btnItemBouclier, txtItemBouclier, btnItemBouclier.getGlobalBounds().contains(mPos.x, mPos.y));
            }
        }

        if (etat == CombatState::VICTORY_SCREEN || etat == CombatState::DEFEAT_SCREEN)
        {
            sf::RectangleShape popFin(sf::Vector2f(screenW * 0.50f, screenH * 0.40f));
            popFin.setOrigin(popFin.getSize().x / 2.0f, popFin.getSize().y / 2.0f);
            popFin.setPosition(centerX, centerY);
            popFin.setFillColor(UI::DarkPanel);
            popFin.setOutlineThickness(2.0f);
            popFin.setOutlineColor(etat == CombatState::VICTORY_SCREEN ? UI::Azure : UI::Crimson);
            window.draw(popFin);

            sf::Text txtTitreFin(etat == CombatState::VICTORY_SCREEN ? "★ VICTOIRE TRIOMPHALE ★" : "VOUS AVEZ PERI", font, static_cast<unsigned int>(screenH * 0.045f));
            txtTitreFin.setFillColor(etat == CombatState::VICTORY_SCREEN ? UI::Silver : UI::Crimson);
            sf::FloatRect bTF = txtTitreFin.getLocalBounds();
            txtTitreFin.setOrigin(bTF.left + bTF.width / 2.0f, bTF.top + bTF.height / 2.0f);
            txtTitreFin.setPosition(centerX, centerY - screenH * 0.12f);
            window.draw(txtTitreFin);

            std::string resumeText;
            if (etat == CombatState::VICTORY_SCREEN)
            {
                resumeText = "Le monstre " + monstre.nom + " a ete vaincu !\n\n"
                    + "Gains : +" + std::to_string(monstre.orRecompense) + " Pieces d'Or | +" + std::to_string(monstre.xpRecompense) + " XP\n"
                    + "Niveau actuel : " + std::to_string(joueur.niveau) + " (" + std::to_string(joueur.xp) + "/" + std::to_string(joueur.xpSeuil) + " XP)";
            }
            else
            {
                resumeText = "Vos blessures sont trop profondes...\nLes pretres de la Cite vous rapatrient au sanctuaire.";
            }

            sf::Text txtResume(resumeText, font, static_cast<unsigned int>(screenH * 0.028f));
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
                           etat == CombatState::VICTORY_SCREEN ? sf::Color(35, 75, 120) : sf::Color(120, 35, 35),
                           etat == CombatState::VICTORY_SCREEN ? sf::Color(50, 100, 160) : sf::Color(160, 45, 45),
                           UI::TextWhite,
                           UI::Silver);
        }

        UI::updateAndDrawFloatingTexts(window, font, floatingTexts, dt);

        window.display();
    }

    if (joueur.vie <= 0)
    {
        joueur.vie = std::max(1, joueur.vieMax / 2);
    }

    return victoireJoueur;
}

#endif