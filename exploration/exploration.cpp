#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../structure.h"

using namespace std;

void lancerCombat(Joueur &joueur, int zone, Monstre monstre);
void genererMonstre(int zone, int numeroAleatoire, Monstre &monstre);
void explorerMonde(int &territoiresConquis, int &zoneActuelle, int &option, Joueur &joueur)
{
    while (joueur.vie > 0)
    {
        cout << "\n=============================================\n";
        cout << "    MENU PRINCIPAL - ZONE ACTUELLE : " << zoneActuelle << "\n";
        cout << "=============================================\n";
        cout << "[1] Commencer a explorer la contree\n";
        cout << "[2] Afficher l'etat du heros\n";
        cout << "[3] LANCER LE RAID CONTRE LE BOSS DE LA ZONE\n";
        cout << "[4] Se reposer a l'Auberge (Soins complets) [20 Or]\n";
        cout << "[5] Aller a la boutique de la cite\n";
        cout << "=============================================\n";
        cout << "Votre choix : ";
        cin >> option;

        if (option == 1)
        {
            bool continuerExplo = true;

            while (continuerExplo && joueur.vie > 0)
            {
                int objetsUtilises = 0;
                for (int i = 0; i < joueur.inventaire.capaciteSac; i++)
                {
                    if (joueur.inventaire.sac[i] != 0)
                    {
                        objetsUtilises++;
                    }
                }

                cout << "\n-------------------------------------------------------\n";
                cout << "PV: " << joueur.vie << "/" << joueur.vieMax << " | Or: " << joueur.orJoueur << " | Niveau: " << joueur.niveau << " | ATQ: " << joueur.attaque << " | Boucliers: " << joueur.inventaire.nombreBouclier << " | Sac: " << objetsUtilises << "/" << joueur.inventaire.capaciteSac << "\n";
                cout << "\n-------------------------------------------------------\n";
                cout << "\n Vous marchez prudemment dans la zone " << zoneActuelle;
                for (int i = 0; i < 3; i++)
                {
                    cout << " .";
                    this_thread::sleep_for(chrono::seconds(3));
                }
                cout << "\n";

                int deEvenement = rand() % 100;

                if (deEvenement < 35)
                {
                    Monstre monstre;
                    int quelMonstre = rand() % 4;

                    genererMonstre(zoneActuelle, quelMonstre, monstre);
                    lancerCombat(joueur, zoneActuelle, monstre);

                    if (joueur.vie > 0)
                    {
                        joueur.xp += monstre.xpRecompense;
                        cout << "[+] Vous gagnez " << monstre.xpRecompense << " XP ! (" << joueur.xp << "/" << joueur.xpSeuil << " XP)\n";

                        if (joueur.xp >= joueur.xpSeuil)
                        {
                            joueur.niveau++;
                            joueur.xp -= joueur.xpSeuil;
                            joueur.xpSeuil += 50;
                            joueur.vieMax += 25;
                            joueur.attaque += 7;
                            joueur.vie = joueur.vieMax;
                            cout << "\nLEVEL UP ! Vous passez au Niveau " << joueur.niveau << " !\n";
                        }
                    }
                }
                else if (deEvenement >= 35 && deEvenement < 70)
                {
                    int typeTresor = rand() % 4;
                    cout << "\n [DECOUVERTE] Oh ! Votre regard est attire par un coffre abandonne...\n";

                    switch (typeTresor)
                    {
                    case 0:
                    {
                        int orTrouve = (rand() % 10) + 10;
                        joueur.orJoueur += orTrouve;
                        cout << " Vous ouvrez le coffre et trouvez une bourse de " << orTrouve << " pieces d'or !\n";
                        break;
                    }
                    case 1:
                        joueur.inventaire.potionsNormales++;
                        cout << "Merveilleux ! Une Potion de soins etait cachee a l'interieur.\n";
                        break;
                    case 2:
                        joueur.inventaire.grandesPotions++;
                        cout << " Incroyable ! Vous mettez la main sur une Grande Potion rare !\n";
                        break;
                    case 3:
                        joueur.inventaire.nombreBouclier++;
                        cout << " Genial ! Vous trouvez un bouclier en bon etat ! Boucliers en stock : " << joueur.inventaire.nombreBouclier << "\n";
                        break;
                    }
                }
                else if (deEvenement >= 70 && deEvenement < 80)
                {
                    int degatsPiege = (rand() % 11) + (zoneActuelle * 5);
                    joueur.vie -= degatsPiege;
                    if (joueur.vie < 0)
                        joueur.vie = 0;

                    cout << "\n[PIEGE] Clic... Un piege dissimule se declenche ! Vous perdez " << degatsPiege << " PV.\n";
                    cout << "Vos PV actuels : " << joueur.vie << "/" << joueur.vieMax << "\n";

                    if (joueur.vie <= 0)
                        break;

                    if (joueur.vie > 0)
                    {
                        cout << "Voulez-vous utiliser une Potion de votre sac ?\n";
                        cout << " [1] Potion (" << joueur.inventaire.potionsNormales << " restants)\n";
                        cout << " [2] Grande Potion (" << joueur.inventaire.grandesPotions << " restants)\n";
                        cout << " [3] Continuer sans se soigner\n";

                        int choixSoin;
                        cin >> choixSoin;
                        if (choixSoin == 1 && joueur.inventaire.potionsNormales > 0)
                        {
                            joueur.inventaire.potionsNormales--;
                            joueur.vie += 30;
                            if (joueur.vie > joueur.vieMax)
                                joueur.vie = joueur.vieMax;
                            cout << "Potion utilisee. PV : " << joueur.vie << "/" << joueur.vieMax << "\n";
                        }
                        else if (choixSoin == 2 && joueur.inventaire.grandesPotions > 0)
                        {
                            joueur.inventaire.grandesPotions--;
                            joueur.vie = joueur.vieMax;
                            cout << "Grande Potion utilisee. PV : " << joueur.vie << "/" << joueur.vieMax << "\n";
                        }
                    }
                }
                else if (deEvenement >= 80 && deEvenement < 90)
                {
                    int echoEpee = rand() % 3;
                    cout << "\n [DECOUVERTE!] Vous avez senti l'echo d'une epee qui est proche\n";

                    switch (echoEpee)
                    {
                    case 0:
                    {
                        int epee = (rand() % 11) + 10;
                        joueur.attaque += epee;
                        cout << " Vous ouvrez le coffre et trouvez une [Epee en fin de vie] [+" << epee << " ATQ]! Attaque : " << joueur.attaque << "\n";
                        break;
                    }
                    case 1:
                        joueur.attaque += 10;
                        cout << "Vous trouvez une simple epee! ATQ [+10] Votre attaque passe a : " << joueur.attaque << "!\n";
                        break;
                    case 2:
                        joueur.attaque += 20;
                        cout << " Incroyable ! Vous mettez la main sur une Grande Epee ATQ [+20] Votre attaque passe a : " << joueur.attaque << "!\n";
                        break;
                    }
                }
                else
                {
                    cout << "Apres avoir explore la zone pendant des heures, le paysage reste calme.\n";
                }

                if (joueur.vie > 0)
                {
                    bool gestionRoute = true;
                    while (gestionRoute)
                    {
                        cout << "\n-----------------------------------------\n";
                        cout << "[Poursuivre l'Aventure ?]\n";
                        cout << "[1] Enfoncer plus loin dans l'inconnu (Continuer)\n";
                        cout << "[2] Acceder a l'inventaire des soins\n";
                        cout << "[3] Rebrousser chemin (Retourner en ville en securite)\n";
                        cout << "Votre choix : ";
                        int choixRoute;
                        cin >> choixRoute;

                        if (choixRoute == 1)
                            gestionRoute = false;
                        else if (choixRoute == 2)
                        {
                            cout << "\n--- INVENTAIRE DE SOINS ---\n";
                            cout << "[1] Potion de soins (+30 PV) (Quantite: " << joueur.inventaire.potionsNormales << ")\n";
                            cout << "[2] Grande Potion (100% PV)  (Quantite: " << joueur.inventaire.grandesPotions << ")\n";
                            cout << "[3] Retour\n";
                            cout << "Choisissez une action : ";

                            int choixObjet;
                            cin >> choixObjet;

                            if (choixObjet == 1 && joueur.inventaire.potionsNormales > 0)
                            {
                                joueur.inventaire.potionsNormales--;
                                joueur.vie += 30;
                                if (joueur.vie > joueur.vieMax)
                                    joueur.vie = joueur.vieMax;
                                cout << "Vous buvez une Potion. PV : " << joueur.vie << "/" << joueur.vieMax << " !\n";
                            }
                            else if (choixObjet == 2 && joueur.inventaire.grandesPotions > 0)
                            {
                                joueur.inventaire.grandesPotions--;
                                joueur.vie = joueur.vieMax;
                                cout << "Grande Potion bue ! PV : " << joueur.vie << "/" << joueur.vieMax << " !\n";
                            }
                        }
                        else if (choixRoute == 3)
                        {
                            continuerExplo = false;
                            gestionRoute = false;
                            cout << "Vous quittez l'exploration et rentrez vous mettre a l'abri en ville.\n";
                        }
                    }
                }
            }
        }
        else if (option == 2)
        {
            cout << "\n--- FEUILLE DE PERSONNAGE ---\n";
            cout << " Nom de l'Avatar : " << joueur.nom << " | Niveau : " << joueur.niveau << "\n";
            cout << " PV : " << joueur.vie << "/" << joueur.vieMax << " | Attaque : " << joueur.attaque << "\n";
            cout << " Experience : " << joueur.xp << "/" << joueur.xpSeuil << " XP\n";
            cout << " Votre bourse : " << joueur.orJoueur << " pieces d'or\n";
            cout << " Sac a dos : [" << joueur.inventaire.potionsNormales << "] Potions | [" << joueur.inventaire.grandesPotions << "] Grandes Potions | [" << joueur.inventaire.nombreBouclier << "] Boucliers\n";
            cout << " Territoires conquis : " << territoiresConquis << "/6\n";
        }
        else if (option == 3)
        {
            cout << "\n ATTENTION ! Vous franchissez les portes de la forteresse ennemie...\n";
            Monstre boss;
            genererMonstre(zoneActuelle, 4, boss);

            cout << " Le boss " << boss.nom << " se dresse devant vous !\n";
            lancerCombat(joueur, zoneActuelle, boss);

            if (joueur.vie > 0)
            {
                territoiresConquis++;
                joueur.xp += boss.xpRecompense;
                joueur.orJoueur += boss.orRecompense;
                cout << "\n SPLENDIDE ! Le boss de la zone " << zoneActuelle << " est mort !\n";

                if (joueur.xp >= joueur.xpSeuil)
                {
                    joueur.niveau++;
                    joueur.xp -= joueur.xpSeuil;
                    joueur.xpSeuil += 50;
                    joueur.vieMax += 30;
                    joueur.attaque += 10;
                    joueur.vie = joueur.vieMax;
                    cout << "\n LEVEL UP ++ ! Vous passez au Niveau " << joueur.niveau << " !\n";
                }

                if (zoneActuelle < 6)
                {
                    zoneActuelle++;
                    cout << "\n[[ EVENEMENT : Vous penetrez dans la ZONE " << zoneActuelle << " ]]!\n";
                }
            }
        }
        else if (option == 4)
        {
            if (joueur.orJoueur >= 20)
            {
                joueur.orJoueur -= 20;
                joueur.vie = joueur.vieMax;
                cout << "Une bonne nuit a l'Auberge. Vos PV sont recharges au maximum (" << joueur.vie << ").\n";
            }
            else
                cout << "[!] Pas assez d'Or (Il vous faut 20 pieces).\n";
        }
        else if (option == 5)
        {
            bool dansBoutique = true;
            while (dansBoutique)
            {
                cout << "\n=======================================================\n";
                cout << "               GRANDE BOUTIQUE DE LA CITE             \n";
                cout << "=======================================================\n";
                cout << " Votre Bourse : " << joueur.orJoueur << " PO\n";
                cout << "-------------------- ARMES ----------------------------\n";
                cout << " [1]  Dague de Voleur (+5 ATQ)               - 15 Or\n";
                cout << " [2]  Hache de Pierre (+15 ATQ)              - 40 Or\n";
                cout << " [3]  Epee en Fer (+25 ATQ)                  - 75 Or\n";
                cout << " [4]  Epee d'Acier (+40 ATQ)                 - 130 Or\n";
                cout << " [5]  Epee Magique (+75 ATQ)                 - 250 Or\n";
                cout << " [6]  Epee du Heros (+370 ATQ)               - 1750 Or\n";
                cout << "------------------- ARMURES ---------------------------\n";
                cout << " [7]  Bouclier en Bois (+1 Bouclier)         - 35 Or\n";
                cout << " [8]  Bouclier en Acier (+2 Boucliers)       - 65 Or\n";
                cout << " [9]  Cotte de Mailles (+15 PV Max)          - 80 Or\n";
                cout << " [10] Armure de Plaque (+40 PV Max)          - 180 Or\n";
                cout << "------------------ CONSOMMABLES -----------------------\n";
                cout << " [11] Potion de soins (+30 PV)               - 20 Or\n";
                cout << " [12] Grande Potion (Soins 100%)             - 50 Or\n";
                cout << " [13] Elixir d'Experience (+50 XP)           - 90 Or\n";
                cout << "----------------- MAGIE & AMELIORATIONS ---------------\n";
                cout << " [14] Parchemin de Force (+5 ATQ permanent)  - 100 Or\n";
                cout << " [15] Pierre de Teleportation (Soins + Fuite)- 45 Or\n";
                cout << " [16] Agrandir Sac (+2 Emplacements)         - 70 Or\n";
                cout << " [17] Sac du Voyageur (+5 Emplacements)      - 150 Or\n";
                cout << "-------------------------------------------------------\n";
                cout << " [0] Quitter la boutique\n";
                cout << "=======================================================\n";
                cout << "Votre choix : ";
                int choixB;
                cin >> choixB;

                if (choixB == 1 && joueur.orJoueur >= 15)
                {
                    joueur.orJoueur -= 15;
                    joueur.attaque += 5;
                }
                else if (choixB == 2 && joueur.orJoueur >= 40)
                {
                    joueur.orJoueur -= 40;
                    joueur.attaque += 15;
                }
                else if (choixB == 3 && joueur.orJoueur >= 75)
                {
                    joueur.orJoueur -= 75;
                    joueur.attaque += 25;
                }
                else if (choixB == 4 && joueur.orJoueur >= 130)
                {
                    joueur.orJoueur -= 130;
                    joueur.attaque += 40;
                }
                else if (choixB == 5 && joueur.orJoueur >= 250)
                {
                    joueur.orJoueur -= 250;
                    joueur.attaque += 75;
                }
                else if (choixB == 6 && joueur.orJoueur >= 1750)
                {
                    joueur.orJoueur -= 1750;
                    joueur.attaque += 370;
                }
                else if (choixB == 7 && joueur.orJoueur >= 35)
                {
                    joueur.orJoueur -= 35;
                    joueur.inventaire.nombreBouclier++;
                }
                else if (choixB == 8 && joueur.orJoueur >= 65)
                {
                    joueur.orJoueur -= 65;
                    joueur.inventaire.nombreBouclier += 2;
                }
                else if (choixB == 9 && joueur.orJoueur >= 80)
                {
                    joueur.orJoueur -= 80;
                    joueur.vieMax += 15;
                    joueur.vie += 15;
                }
                else if (choixB == 10 && joueur.orJoueur >= 180)
                {
                    joueur.orJoueur -= 180;
                    joueur.vieMax += 40;
                    joueur.vie += 40;
                }
                else if (choixB == 11 && joueur.orJoueur >= 20)
                {
                    joueur.orJoueur -= 20;
                    joueur.inventaire.potionsNormales++;
                }
                else if (choixB == 12 && joueur.orJoueur >= 50)
                {
                    joueur.orJoueur -= 50;
                    joueur.inventaire.grandesPotions++;
                }
                else if (choixB == 13 && joueur.orJoueur >= 90)
                {
                    joueur.orJoueur -= 90;
                    joueur.xp += 50;
                    if (joueur.xp >= joueur.xpSeuil)
                    {
                        joueur.niveau++;
                        joueur.xp -= joueur.xpSeuil;
                        joueur.xpSeuil += 50;
                        joueur.vieMax += 25;
                        joueur.attaque += 7;
                        joueur.vie = joueur.vieMax;
                    }
                }
                else if (choixB == 14 && joueur.orJoueur >= 100)
                {
                    joueur.orJoueur -= 100;
                    joueur.attaque += 5;
                }
                else if (choixB == 15 && joueur.orJoueur >= 45)
                {
                    joueur.orJoueur -= 45;
                    joueur.vie = joueur.vieMax;
                }
                else if (choixB == 16 && joueur.orJoueur >= 70)
                {
                    int *nouveauSac = new int[joueur.inventaire.capaciteSac + 2]{0};
                    for (int i = 0; i < joueur.inventaire.capaciteSac; i++)
                        nouveauSac[i] = joueur.inventaire.sac[i];
                    delete[] joueur.inventaire.sac;
                    joueur.inventaire.sac = nouveauSac;
                    joueur.inventaire.capaciteSac += 2;
                    joueur.orJoueur -= 70;
                }
                else if (choixB == 17 && joueur.orJoueur >= 150)
                {
                    int *nouveauSac = new int[joueur.inventaire.capaciteSac + 5]{0};
                    for (int i = 0; i < joueur.inventaire.capaciteSac; i++)
                        nouveauSac[i] = joueur.inventaire.sac[i];
                    delete[] joueur.inventaire.sac;
                    joueur.inventaire.sac = nouveauSac;
                    joueur.inventaire.capaciteSac += 5;
                    joueur.orJoueur -= 150;
                }
                else if (choixB == 0)
                    dansBoutique = false;
                else
                    cout << "Action impossible.\n";
            }
        }
        else if (option == 6)
            break;
    }
}

bool explorerMondeSFML(sf::RenderWindow &window, sf::Font &font, Joueur &joueur, int &zoneActuelle, int &territoiresConquis)
{
    float screenW = static_cast<float>(window.getSize().x);
    float screenH = static_cast<float>(window.getSize().y);

    // 1. Zone du HAUT (70%)
    float hauteurHaut = screenH * 0.70f;
    sf::RectangleShape zoneHaut(sf::Vector2f(screenW, hauteurHaut));
    zoneHaut.setFillColor(sf::Color(15, 15, 25));

    // 2. Zone du BAS (30%)
    float hauteurBas = screenH * 0.30f;
    sf::RectangleShape zoneBas(sf::Vector2f(screenW, hauteurBas));
    zoneBas.setPosition(0.0f, hauteurHaut);
    zoneBas.setFillColor(sf::Color(20, 24, 35));

    // 3. Ligne de séparation dorée
    sf::RectangleShape ligneSeparation(sf::Vector2f(screenW, 4.0f));
    ligneSeparation.setPosition(0.0f, hauteurHaut - 2.0f);
    ligneSeparation.setFillColor(sf::Color(212, 175, 55));

    sf::Text txtStatut("", font, static_cast<unsigned int>(screenH * 0.025f));
    txtStatut.setFillColor(sf::Color::White);
    txtStatut.setPosition(25.0f, 20.0f);

    sf::Text txtNarration("Vous vous enfoncez prudemment dans les terres sauvages...", font, static_cast<unsigned int>(screenH * 0.024f));
    txtNarration.setFillColor(sf::Color(220, 220, 240));
    txtNarration.setPosition(35.0f, hauteurHaut + 30.0f);

    float btnW = screenW * 0.16f;
    float btnH = screenH * 0.06f;

    sf::RectangleShape btnAvancer(sf::Vector2f(btnW, btnH));
    btnAvancer.setOrigin(btnW / 2.0f, btnH / 2.0f);
    btnAvancer.setPosition(screenW * 0.88f, hauteurHaut + (hauteurBas * 0.30f));
    btnAvancer.setFillColor(sf::Color(40, 140, 40));

    sf::Text txtAvancer("AVANCER", font, static_cast<unsigned int>(btnH * 0.40f));
    sf::FloatRect boundsAv = txtAvancer.getLocalBounds();
    txtAvancer.setOrigin(boundsAv.left + boundsAv.width / 2.0f, boundsAv.top + boundsAv.height / 2.0f);
    txtAvancer.setPosition(btnAvancer.getPosition());


    sf::RectangleShape btnVille(sf::Vector2f(btnW, btnH));
    btnVille.setOrigin(btnW / 2.0f, btnH / 2.0f);
    btnVille.setPosition(screenW * 0.88f, hauteurHaut + (hauteurBas * 0.70f));
    btnVille.setFillColor(sf::Color(60, 60, 120));

    sf::Text txtVille("RETOUR VILLE", font, static_cast<unsigned int>(btnH * 0.35f));
    sf::FloatRect boundsVi = txtVille.getLocalBounds();
    txtVille.setOrigin(boundsVi.left + boundsVi.width / 2.0f, boundsVi.top + boundsVi.height / 2.0f);
    txtVille.setPosition(btnVille.getPosition());

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
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (btnAvancer.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    txtNarration.setString("Un bruit feutre retentit dans les buissons...\nQu'allez-vous faire ?");
                }
                else if (btnVille.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    return true; 
                }
            }
        }

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (btnAvancer.getGlobalBounds().contains(mousePos.x, mousePos.y))
            btnAvancer.setFillColor(sf::Color(60, 180, 60));
        else
            btnAvancer.setFillColor(sf::Color(40, 140, 40));

        if (btnVille.getGlobalBounds().contains(mousePos.x, mousePos.y))
            btnVille.setFillColor(sf::Color(90, 90, 170));
        else
            btnVille.setFillColor(sf::Color(60, 60, 120));

        window.clear();
        window.draw(zoneHaut);
        window.draw(zoneBas);
        window.draw(ligneSeparation);

        std::string infoHeros = joueur.nom + " (Niv. " + std::to_string(joueur.niveau) + ")"
            + " | PV: " + std::to_string(joueur.vie) + "/" + std::to_string(joueur.vieMax)
            + " | Or: " + std::to_string(joueur.orJoueur) + " PO";
        txtStatut.setString(infoHeros);
        window.draw(txtStatut);

        window.draw(txtNarration);
        window.draw(btnAvancer);
        window.draw(txtAvancer);
        window.draw(btnVille);
        window.draw(txtVille);

        window.display();
    }

    return true;
}

bool afficherMenuVilleSFML(sf::RenderWindow &window, sf::Font &font, Joueur &joueur, int &zoneActuelle, int &territoiresConquis)
{
    float screenW = static_cast<float>(window.getSize().x);
    float screenH = static_cast<float>(window.getSize().y);
    float centerX = screenW / 2.0f;

    // Titre de la Ville
    sf::Text titreVille("LA CITE DE RIVENOIR", font, static_cast<unsigned int>(screenH * 0.06f));
    titreVille.setFillColor(sf::Color(212, 175, 55));
    sf::FloatRect boundsTitre = titreVille.getLocalBounds();
    titreVille.setOrigin(boundsTitre.left + boundsTitre.width / 2.0f, boundsTitre.top + boundsTitre.height / 2.0f);
    titreVille.setPosition(centerX, screenH * 0.08f);

    sf::Text sousTitre("Capitale & Bastion de la Zone " + std::to_string(zoneActuelle), font, static_cast<unsigned int>(screenH * 0.025f));
    sousTitre.setFillColor(sf::Color(180, 180, 200));
    sf::FloatRect boundsSous = sousTitre.getLocalBounds();
    sousTitre.setOrigin(boundsSous.left + boundsSous.width / 2.0f, boundsSous.top + boundsSous.height / 2.0f);
    sousTitre.setPosition(centerX, screenH * 0.13f);

    // Panneau de Fiche du Héros (Gauche)
    sf::RectangleShape panneauHeros(sf::Vector2f(screenW * 0.28f, screenH * 0.65f));
    panneauHeros.setPosition(screenW * 0.05f, screenH * 0.20f);
    panneauHeros.setFillColor(sf::Color(20, 24, 38, 220));
    panneauHeros.setOutlineThickness(2.0f);
    panneauHeros.setOutlineColor(sf::Color(212, 175, 55));

    sf::Text txtFiche("STATISTIQUES DU HEROS", font, static_cast<unsigned int>(screenH * 0.028f));
    txtFiche.setFillColor(sf::Color(212, 175, 55));
    txtFiche.setPosition(screenW * 0.07f, screenH * 0.22f);

    // Boutons de la Ville (Droite)
    float btnW = screenW * 0.45f;
    float btnH = screenH * 0.08f;
    float startY = screenH * 0.22f;
    float gapY = screenH * 0.11f;

    // 1. Explorer
    sf::RectangleShape btnExplo(sf::Vector2f(btnW, btnH));
    btnExplo.setPosition(screenW * 0.40f, startY);
    btnExplo.setFillColor(sf::Color(40, 130, 50));

    sf::Text txtExplo("EXPLORER LA CONTREE", font, static_cast<unsigned int>(btnH * 0.38f));
    sf::FloatRect bExp = txtExplo.getLocalBounds();
    txtExplo.setOrigin(bExp.left + bExp.width / 2.0f, bExp.top + bExp.height / 2.0f);
    txtExplo.setPosition(btnExplo.getPosition().x + (btnW / 2.0f), btnExplo.getPosition().y + (btnH / 2.0f));

    // 2. Auberge (Soin)
    sf::RectangleShape btnAuberge(sf::Vector2f(btnW, btnH));
    btnAuberge.setPosition(screenW * 0.40f, startY + gapY);
    btnAuberge.setFillColor(sf::Color(160, 100, 30));

    sf::Text txtAuberge("SE REPOSER A L'AUBERGE (20 PO)", font, static_cast<unsigned int>(btnH * 0.32f));
    sf::FloatRect bAub = txtAuberge.getLocalBounds();
    txtAuberge.setOrigin(bAub.left + bAub.width / 2.0f, bAub.top + bAub.height / 2.0f);
    txtAuberge.setPosition(btnAuberge.getPosition().x + (btnW / 2.0f), btnAuberge.getPosition().y + (btnH / 2.0f));

    // 3. Raid Boss
    sf::RectangleShape btnBoss(sf::Vector2f(btnW, btnH));
    btnBoss.setPosition(screenW * 0.40f, startY + (gapY * 2.0f));
    btnBoss.setFillColor(sf::Color(160, 40, 40));

    sf::Text txtBoss("LANCER LE RAID CONTRE LE BOSS", font, static_cast<unsigned int>(btnH * 0.34f));
    sf::FloatRect bBos = txtBoss.getLocalBounds();
    txtBoss.setOrigin(bBos.left + bBos.width / 2.0f, bBos.top + bBos.height / 2.0f);
    txtBoss.setPosition(btnBoss.getPosition().x + (btnW / 2.0f), btnBoss.getPosition().y + (btnH / 2.0f));

    // 4. Menu Principal
    sf::RectangleShape btnQuitter(sf::Vector2f(btnW, btnH));
    btnQuitter.setPosition(screenW * 0.40f, startY + (gapY * 3.5f));
    btnQuitter.setFillColor(sf::Color(80, 30, 40));

    sf::Text txtQuitter("MENU PRINCIPAL", font, static_cast<unsigned int>(btnH * 0.35f));
    sf::FloatRect bQui = txtQuitter.getLocalBounds();
    txtQuitter.setOrigin(bQui.left + bQui.width / 2.0f, bQui.top + bQui.height / 2.0f);
    txtQuitter.setPosition(btnQuitter.getPosition().x + (btnW / 2.0f), btnQuitter.getPosition().y + (btnH / 2.0f));

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
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (btnExplo.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    explorerMondeSFML(window, font, joueur, zoneActuelle, territoiresConquis);
                }
                else if (btnAuberge.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    if (joueur.orJoueur >= 20)
                    {
                        joueur.orJoueur -= 20;
                        joueur.vie = joueur.vieMax;
                    }
                }
                else if (btnQuitter.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    return false; // Retour au Menu Titre principal
                }
            }
        }

        // Hover Effect
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        btnExplo.setFillColor(btnExplo.getGlobalBounds().contains(mousePos.x, mousePos.y) ? sf::Color(60, 170, 70) : sf::Color(40, 130, 50));
        btnAuberge.setFillColor(btnAuberge.getGlobalBounds().contains(mousePos.x, mousePos.y) ? sf::Color(190, 130, 40) : sf::Color(160, 100, 30));
        btnBoss.setFillColor(btnBoss.getGlobalBounds().contains(mousePos.x, mousePos.y) ? sf::Color(200, 60, 60) : sf::Color(160, 40, 40));
        btnQuitter.setFillColor(btnQuitter.getGlobalBounds().contains(mousePos.x, mousePos.y) ? sf::Color(120, 40, 50) : sf::Color(80, 30, 40));

        window.clear(sf::Color(12, 14, 22));

        window.draw(titreVille);
        window.draw(sousTitre);

        // Dessiner le Panneau Héros
        window.draw(panneauHeros);
        window.draw(txtFiche);

        std::string statsStr = "Nom : " + joueur.nom + "\n\n"
            + "Niveau : " + std::to_string(joueur.niveau) + "\n\n"
            + "PV : " + std::to_string(joueur.vie) + " / " + std::to_string(joueur.vieMax) + "\n\n"
            + "Attaque : " + std::to_string(joueur.attaque) + "\n\n"
            + "Bourse : " + std::to_string(joueur.orJoueur) + " PO\n\n"
            + "Boucliers : " + std::to_string(joueur.inventaire.nombreBouclier) + "\n\n"
            + "Potions : " + std::to_string(joueur.inventaire.potionsNormales);

        sf::Text txtStatsContent(statsStr, font, static_cast<unsigned int>(screenH * 0.022f));
        txtStatsContent.setFillColor(sf::Color(220, 220, 240));
        txtStatsContent.setPosition(screenW * 0.07f, screenH * 0.28f);
        window.draw(txtStatsContent);

        // Dessiner les boutons
        window.draw(btnExplo); window.draw(txtExplo);
        window.draw(btnAuberge); window.draw(txtAuberge);
        window.draw(btnBoss); window.draw(txtBoss);
        window.draw(btnQuitter); window.draw(txtQuitter);

        window.display();
    }

    return true;
}