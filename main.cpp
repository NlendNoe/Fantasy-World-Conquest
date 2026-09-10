#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "structure.h"
#include "chargement/chargement.cpp"
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

    // Plein écran
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "FANTASY WORLD CONQUEST", sf::Style::Fullscreen);

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/GamePocket-Regular.ttf"))
    {
        cout << "Erreur: Impossible de charger Gameday.otf" << endl;
        if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
            cout << "Impossible de charger la police systeme." << endl;
    }

    sf::Texture textureFond;
    bool fondCharge = textureFond.loadFromFile("assets/backgrounds/font.jpg");
    sf::Sprite spriteFond;
    if (fondCharge)
    {
        spriteFond.setTexture(textureFond);
        sf::Vector2u tailleImage = textureFond.getSize();
        spriteFond.setScale((float)window.getSize().x / tailleImage.x, (float)window.getSize().y / tailleImage.y);
    }

    float screenW = static_cast<float>(window.getSize().x);
    float screenH = static_cast<float>(window.getSize().y);
    float centerX = screenW / 2.0f;
    float centerY = screenH / 2.0f;

    float btnWidth = screenW * 0.22f;          
    float btnHeight = screenH * 0.07f;         
    float margeTitreBoutons = screenH * 0.08f; 
    float margeEntreBoutons = screenH * 0.03f; 

    unsigned int taillePoliceTitre = static_cast<unsigned int>(screenH * 0.07f);
    unsigned int taillePoliceBouton = static_cast<unsigned int>(btnHeight * 0.45f);

    float yPremierBouton = centerY;
    float ySecondBouton = yPremierBouton + btnHeight + margeEntreBoutons;

    sf::Text titre("FANTASY WORLD CONQUEST", font, taillePoliceTitre);
    titre.setFillColor(sf::Color::White);
    sf::FloatRect boundsTitre = titre.getLocalBounds();
    titre.setOrigin(boundsTitre.left + boundsTitre.width / 2.0f, boundsTitre.top + boundsTitre.height / 2.0f);
    titre.setPosition(centerX, yPremierBouton - (btnHeight / 2.0f) - margeTitreBoutons - (boundsTitre.height / 2.0f));

    sf::RectangleShape btnJouer(sf::Vector2f(btnWidth, btnHeight));
    btnJouer.setOrigin(btnWidth / 2.0f, btnHeight / 2.0f);
    btnJouer.setPosition(centerX, yPremierBouton);
    btnJouer.setFillColor(sf::Color::Transparent);
    btnJouer.setOutlineThickness(2.0f);
    btnJouer.setOutlineColor(sf::Color::White);

    sf::Text txtJouer("Commencer", font, taillePoliceBouton);
    txtJouer.setFillColor(sf::Color::White);
    sf::FloatRect boundsJouer = txtJouer.getLocalBounds();
    txtJouer.setOrigin(boundsJouer.left + boundsJouer.width / 2.0f, boundsJouer.top + boundsJouer.height / 2.0f);
    txtJouer.setPosition(btnJouer.getPosition());

    sf::RectangleShape btnQuitter(sf::Vector2f(btnWidth, btnHeight));
    btnQuitter.setOrigin(btnWidth / 2.0f, btnHeight / 2.0f);
    btnQuitter.setPosition(centerX, ySecondBouton);
    btnQuitter.setFillColor(sf::Color::Transparent);
    btnQuitter.setOutlineThickness(2.0f);
    btnQuitter.setOutlineColor(sf::Color::White);

    sf::Text txtQuitter("Quitter", font, taillePoliceBouton);
    txtQuitter.setFillColor(sf::Color::White);
    sf::FloatRect boundsQuitter = txtQuitter.getLocalBounds();
    txtQuitter.setOrigin(boundsQuitter.left + boundsQuitter.width / 2.0f, boundsQuitter.top + boundsQuitter.height / 2.0f);
    txtQuitter.setPosition(btnQuitter.getPosition());

    sf::Music musicFond;
    if (!musicFond.openFromFile("assets/Music/Fantasy RPG title screen music _ OpenGameArt.org.ogg"))
    {
        std::cout << "Erreur : Impossible de charger la musique de fond !" << std::endl;
    }
    musicFond.setLoop(true);
    musicFond.setVolume(30.0f);
    musicFond.play();


    bool programmeEnCours = true;

    while (window.isOpen() && programmeEnCours)
    {
        bool jeuDemarre = false;

        // BOUCLE DU MENU PRINCIPAL
        while (window.isOpen() && !jeuDemarre)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    if (btnJouer.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        jeuDemarre = true;
                    }
                    else if (btnQuitter.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        window.close();
                    }
                }
            }

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (btnJouer.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                btnJouer.setFillColor(sf::Color::White);
                txtJouer.setFillColor(sf::Color::Black);
            } else {
                btnJouer.setFillColor(sf::Color::Transparent);
                txtJouer.setFillColor(sf::Color::White);
            }

            if (btnQuitter.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                btnQuitter.setFillColor(sf::Color::White);
                txtQuitter.setFillColor(sf::Color::Black);
            } else {
                btnQuitter.setFillColor(sf::Color::Transparent);
                txtQuitter.setFillColor(sf::Color::White);
            }

            window.clear(sf::Color(15, 15, 20));

            if (fondCharge)
                window.draw(spriteFond);

            window.draw(titre);
            window.draw(btnJouer);
            window.draw(txtJouer);
            window.draw(btnQuitter);
            window.draw(txtQuitter);
            window.display();
        }

        if (jeuDemarre)
        {
            // Petit ecran immersif avant la creation de l'avatar
            ecranChargementSFML(window, font);

            bool persoCree = creerPersonnageSFML(window, font, joueur);

            if (!persoCree)
            {
                // L'utilisateur a cliqué sur RETOUR -> On réaffiche le menu principal !
                continue;
            }

            while (joueur.vie > 0 && territoiresConquis < 6)
            {
                bool dansLaVille = afficherMenuVilleSFML(window, font, joueur, zoneActuelle, territoiresConquis);
                if (!dansLaVille) break; // Retourne au Menu Titre Principal
            }

            if (joueur.vie <= 0)
            {
                cout << "\n [GAME OVER] VOUS ETES MORT SANS ECRIRE VOTRE LEGENDE...\n";
            }
            else
            {
                cout << "\n [VICTOIRE ABSOLUE] L'EMPEREUR DES OMBRES EST VAINCU !\n";
            }
            programmeEnCours = false;
        }
        else
        {
            programmeEnCours = false;
        }
    }

    delete[] joueur.inventaire.sac;
    return 0;
}