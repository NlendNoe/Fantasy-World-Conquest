#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "structure.h"
#include "audio.h"
#include "ui_utils.h"
#include "sauvegarde.h"
#include "chargement/chargement.cpp"
#include "personnages/personnage.cpp"
#include "bestiaire/bestiaire.cpp"
#include "combat/combat.cpp"
#include "exploration/exploration.cpp"

using namespace std;

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    Joueur joueur;
    int territoiresConquis = 0;
    int zoneActuelle = 1;

    // Plein écran
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "FANTASY WORLD CONQUEST", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/GamePocket-Regular.ttf"))
    {
        if (!font.loadFromFile("assets/fonts/pixelart.ttf"))
        {
            if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
                cout << "[Erreur] Impossible de charger la police systeme." << endl;
        }
    }

    sf::Texture textureFond;
    bool fondCharge = textureFond.loadFromFile("assets/backgrounds/font.jpg");
    sf::Sprite spriteFond;
    if (fondCharge)
    {
        spriteFond.setTexture(textureFond);
        sf::Vector2u sz = textureFond.getSize();
        spriteFond.setScale((float)window.getSize().x / sz.x, (float)window.getSize().y / sz.y);
    }

    float screenW = static_cast<float>(window.getSize().x);
    float screenH = static_cast<float>(window.getSize().y);
    float centerX = screenW / 2.0f;
    float centerY = screenH / 2.0f;

    float btnW = screenW * 0.26f;          
    float btnH = screenH * 0.072f;         
    float gapY = screenH * 0.088f; 
    float startBtnY = centerY - screenH * 0.02f;

    // Titre principal
    unsigned int taillePoliceTitre = static_cast<unsigned int>(screenH * 0.075f);
    sf::Text titre("FANTASY WORLD CONQUEST", font, taillePoliceTitre);
    titre.setFillColor(UI::GoldBright);
    sf::FloatRect boundsTitre = titre.getLocalBounds();
    titre.setOrigin(boundsTitre.left + boundsTitre.width / 2.0f, boundsTitre.top + boundsTitre.height / 2.0f);
    titre.setPosition(centerX, screenH * 0.22f);

    // Sous-titre
    sf::Text sousTitre("L'Epopee des Six Contrees", font, static_cast<unsigned int>(screenH * 0.028f));
    sousTitre.setFillColor(UI::TextMuted);
    sf::FloatRect boundsSous = sousTitre.getLocalBounds();
    sousTitre.setOrigin(boundsSous.left + boundsSous.width / 2.0f, boundsSous.top + boundsSous.height / 2.0f);
    sousTitre.setPosition(centerX, screenH * 0.29f);

    // Bouton 1: NOUVELLE PARTIE
    sf::RectangleShape btnJouer(sf::Vector2f(btnW, btnH));
    btnJouer.setOrigin(btnW / 2.0f, btnH / 2.0f);
    btnJouer.setPosition(centerX, startBtnY);
    btnJouer.setOutlineThickness(2.0f);
    btnJouer.setOutlineColor(UI::Gold);

    sf::Text txtJouer("NOUVELLE AVENTURE", font, static_cast<unsigned int>(btnH * 0.38f));
    txtJouer.setFillColor(UI::TextWhite);
    sf::FloatRect bJ = txtJouer.getLocalBounds();
    txtJouer.setOrigin(bJ.left + bJ.width / 2.0f, bJ.top + bJ.height / 2.0f);
    txtJouer.setPosition(btnJouer.getPosition());

    // Bouton 2: CONTINUER
    sf::RectangleShape btnContinuer(sf::Vector2f(btnW, btnH));
    btnContinuer.setOrigin(btnW / 2.0f, btnH / 2.0f);
    btnContinuer.setPosition(centerX, startBtnY + gapY);
    btnContinuer.setOutlineThickness(2.0f);
    btnContinuer.setOutlineColor(UI::Gold);

    sf::Text txtContinuer("CONTINUER LA PARTIE", font, static_cast<unsigned int>(btnH * 0.38f));
    txtContinuer.setFillColor(UI::TextWhite);
    sf::FloatRect bC = txtContinuer.getLocalBounds();
    txtContinuer.setOrigin(bC.left + bC.width / 2.0f, bC.top + bC.height / 2.0f);
    txtContinuer.setPosition(btnContinuer.getPosition());

    // Bouton 3: QUITTER
    sf::RectangleShape btnQuitter(sf::Vector2f(btnW, btnH));
    btnQuitter.setOrigin(btnW / 2.0f, btnH / 2.0f);
    btnQuitter.setPosition(centerX, startBtnY + gapY * 2.0f);
    btnQuitter.setOutlineThickness(2.0f);
    btnQuitter.setOutlineColor(UI::Gold);

    sf::Text txtQuitter("QUITTER LE JEU", font, static_cast<unsigned int>(btnH * 0.38f));
    txtQuitter.setFillColor(UI::TextWhite);
    sf::FloatRect bQ = txtQuitter.getLocalBounds();
    txtQuitter.setOrigin(bQ.left + bQ.width / 2.0f, bQ.top + bQ.height / 2.0f);
    txtQuitter.setPosition(btnQuitter.getPosition());

    AudioJeu::jouer("assets/Music/Fantasy RPG title screen music _ OpenGameArt.org.ogg", 30.0f, true);

    bool programmeEnCours = true;

    while (window.isOpen() && programmeEnCours)
    {
        bool lancerJeu = false;
        bool partieChargee = false;

        bool sauvegardeExiste = fichierSauvegardeExiste();

        // BOUCLE DU MENU PRINCIPAL
        while (window.isOpen() && !lancerJeu)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    programmeEnCours = false;
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    if (btnJouer.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        lancerJeu = true;
                        partieChargee = false;
                    }
                    else if (sauvegardeExiste && btnContinuer.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        if (chargerPartie(joueur, zoneActuelle, territoiresConquis))
                        {
                            lancerJeu = true;
                            partieChargee = true;
                        }
                    }
                    else if (btnQuitter.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        window.close();
                        programmeEnCours = false;
                    }
                }
            }

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // Hover Nouvelle partie
            UI::drawButton(window, btnJouer, txtJouer, btnJouer.getGlobalBounds().contains(mousePos.x, mousePos.y));

            // Hover Continuer
            if (sauvegardeExiste)
            {
                UI::drawButton(window, btnContinuer, txtContinuer, btnContinuer.getGlobalBounds().contains(mousePos.x, mousePos.y));
            }
            else
            {
                btnContinuer.setFillColor(sf::Color(25, 25, 35, 180));
                btnContinuer.setOutlineColor(sf::Color(80, 80, 90));
                txtContinuer.setFillColor(sf::Color(100, 100, 110));
            }

            // Hover Quitter
            UI::drawButton(window, btnQuitter, txtQuitter, btnQuitter.getGlobalBounds().contains(mousePos.x, mousePos.y),
                           UI::DarkPanelLight, sf::Color(180, 40, 40), UI::TextWhite, UI::TextWhite);

            window.clear(sf::Color(12, 14, 20));

            if (fondCharge)
                window.draw(spriteFond);

            sf::RectangleShape overlay(sf::Vector2f(screenW, screenH));
            overlay.setFillColor(sf::Color(10, 12, 22, 160));
            window.draw(overlay);

            window.draw(titre);
            window.draw(sousTitre);

            window.draw(btnJouer);
            window.draw(txtJouer);

            window.draw(btnContinuer);
            window.draw(txtContinuer);

            window.draw(btnQuitter);
            window.draw(txtQuitter);

            // Mention de sauvegarde existante
            if (sauvegardeExiste)
            {
                sf::Text txtSaveInfo("Une sauvegarde d'aventure a ete detectee dans les parchemins.", font, 16);
                txtSaveInfo.setFillColor(UI::AmberXP);
                sf::FloatRect bSI = txtSaveInfo.getLocalBounds();
                txtSaveInfo.setOrigin(bSI.left + bSI.width / 2.0f, bSI.top + bSI.height / 2.0f);
                txtSaveInfo.setPosition(centerX, screenH * 0.88f);
                window.draw(txtSaveInfo);
            }

            window.display();
        }

        if (lancerJeu)
        {
            if (!partieChargee)
            {
                // Écran de chargement immersif
                ecranChargementSFML(window, font);

                // Création du personnage
                bool persoCree = creerPersonnageSFML(window, font, joueur);

                if (!persoCree)
                {
                    AudioJeu::jouer("assets/Music/Fantasy RPG title screen music _ OpenGameArt.org.ogg", 30.0f, true);
                    continue; // Retour au menu principal
                }

                zoneActuelle = 1;
                territoiresConquis = 0;
                sauvegarderPartie(joueur, zoneActuelle, territoiresConquis);
            }

            // Boucle principale dans la Cité / Bastion
            while (joueur.vie > 0 && window.isOpen())
            {
                bool dansLaCite = afficherMenuVilleSFML(window, font, joueur, zoneActuelle, territoiresConquis);
                if (!dansLaCite)
                {
                    break; // Retour au Menu Principal
                }
            }

            AudioJeu::jouer("assets/Music/Fantasy RPG title screen music _ OpenGameArt.org.ogg", 30.0f, true);
        }
    }

    AudioJeu::arreter();
    return 0;
}