#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace std;

void lancerCombat(string nomJoueur, int &vieJoueur, int vieMaxJoueur, int attaqueJoueur, int &orJoueur, int zone, string nomMonstre, int vieMonstre, int attaqueMonstre, int orRecompense, int xpRecompense, int &potionsNormales, int &grandesPotions);
void genererMonstre(int zone, int numeroAleatoire, string &nomMonstre, int &vieMonstre, int &attaqueMonstre, int &orRecompense, int &xpRecompense);

void explorerMonde(int &territoiresConquis, int &zoneActuelle, int &option, string nomJoueur, int &vieJoueur, int &vieMaxJoueur, int &attaqueJoueur, int &niveauJoueur, int &orJoueur, int &xpJoueur, int &xpSeuil, int &potionsNormales, int &grandesPotions)
{
    bool choixInvalide = false;

    do
    {
        cout << "\n=============================================\n";
        cout << "    MENU PRINCIPAL - ZONE ACTUELLE : " << zoneActuelle << "\n";
        cout << "=============================================\n";
        cout << "[1] Commencer a explorer la contree\n";
        cout << "[2] Afficher l'etat du heros\n";
        cout << "[3] LANCER LE RAID CONTRE LE BOSS DE LA ZONE\n";
        cout << "[4] Se reposer a l'Auberge (Soins complets) [10 Or]\n";
        cout << "[5] Aller a la boutique de la cite\n";
        cout << "=============================================\n";
        cout << "Votre choix : ";
        cin >> option;

        choixInvalide = false;

        if (option == 1)
        {
            bool continuerExplo = true;

            while (continuerExplo && vieJoueur > 0)
            {
                cout << "\n Vous marchez prudemment dans la zone " << zoneActuelle;
                for (int i = 0; i < 3; i++)
                {
                    cout << " .";
                    this_thread::sleep_for(chrono::seconds(3));
                }
                cout << "\n";

                int deEvenement = rand() % 100;

                if (deEvenement < 45)
                {
                    string nomM;
                    int vieM = 0, atkM = 0, orM = 0, xpM = 0;
                    int quelMonstre = rand() % 4;

                    genererMonstre(zoneActuelle, quelMonstre, nomM, vieM, atkM, orM, xpM);
                    lancerCombat(nomJoueur, vieJoueur, vieMaxJoueur, attaqueJoueur, orJoueur, zoneActuelle, nomM, vieM, atkM, orM, xpM, potionsNormales, grandesPotions);

                    if (vieJoueur > 0)
                    {
                        xpJoueur += xpM;
                        cout << "[+] Vous gagnez " << xpM << " XP ! (" << xpJoueur << "/" << xpSeuil << " XP)\n";

                        if (xpJoueur >= xpSeuil)
                        {
                            niveauJoueur++;
                            xpJoueur -= xpSeuil;
                            xpSeuil += 50;
                            vieMaxJoueur += 25;
                            attaqueJoueur += 7;
                            vieJoueur = vieMaxJoueur;
                            cout << "\nLEVEL UP ! Vous passez au Niveau " << niveauJoueur << " !\n";
                        }
                    }
                }
                else if (deEvenement >= 45 && deEvenement < 70)
                {
                    int typeTresor = rand() % 3;
                    cout << "\n [DECOUVERTE] Oh ! Votre regard est attire par un coffre abandonne...\n";

                    switch (typeTresor)
                    {
                    case 0:
                    {
                        int orTrouve = (rand() % 21) + 10;
                        orJoueur += orTrouve;
                        cout << " Vous ouvrez le coffre et trouvez une bourse de " << orTrouve << " pieces d'or !\n";
                        break;
                    }
                    case 1:
                        potionsNormales++;
                        cout << "Merveilleux ! Une Potion de soins etait cachee a l'interieur.\n";
                        break;
                    case 2:
                        grandesPotions++;
                        cout << " Incroyable ! Vous mettez la main sur une Grande Potion rare !\n";
                        break;
                    }
                }
                else if (deEvenement >= 70 && deEvenement < 85)
                {
                    int degatsPiege = (rand() % 11) + (zoneActuelle * 5);
                    vieJoueur -= degatsPiege;
                    if (vieJoueur < 0)
                        vieJoueur = 0;

                    cout << "\n[PIEGE] Clic... Un piege dissimule se declenche ! Vous perdez " << degatsPiege << " PV.\n";
                    cout << "Vos PV actuels : " << vieJoueur << "/" << vieMaxJoueur << "\n";

                    if (vieJoueur <= 0)
                    {
                        cout << "Le piege vous a ete fatal...\n";
                        break;
                    }
                }
                else
                    cout << "Apres avoir explore la zone pendant des heures, le paysage reste calme.\n";

                if (vieJoueur > 0)
                {
                    cout << "\n-----------------------------------------\n";
                    cout << "[Poursuivre l'Aventure ?]\n";
                    cout << "[1] Enfoncer plus loin dans l'inconnu (Continuer)\n";
                    cout << "[2] Rebrousser chemin (Retourner en ville en securite)\n";
                    cout << "Votre choix : ";
                    int choixRoute;
                    cin >> choixRoute;

                    if (choixRoute == 2)
                    {
                        continuerExplo = false;
                        cout << "Vous quittez l'exploration et rentrez vous mettre a l'abri en ville.\n";
                    }
                }
            }
        }
        else if (option == 2)
        {
            cout << "\n--- FEUILLE DE PERSONNAGE ---\n";
            cout << " Nom de l'Avatar : " << nomJoueur << " | Niveau : " << niveauJoueur << "\n";
            cout << " PV : " << vieJoueur << "/" << vieMaxJoueur << " | Attaque : " << attaqueJoueur << "\n";
            cout << " Experience : " << xpJoueur << "/" << xpSeuil << " XP\n";
            cout << " Votre bourse : " << orJoueur << " pieces d'or\n";
            cout << " Sac a dos : [" << potionsNormales << "] Potions | [" << grandesPotions << "] Grandes Potions\n";
            cout << " Territoires conquis : " << territoiresConquis << "/4\n";
        }
        else if (option == 3)
        {
            cout << "\n ATTENTION ! Vous franchissez les portes de la forteresse ennemie...\n";
            string nomBoss;
            int vieBoss = 0, atkBoss = 0, orBoss = 0, xpBoss = 0;

            genererMonstre(zoneActuelle, 4, nomBoss, vieBoss, atkBoss, orBoss, xpBoss);

            cout << " Le boss " << nomBoss << " se dresse devant vous !\n";
            lancerCombat(nomJoueur, vieJoueur, vieMaxJoueur, attaqueJoueur, orJoueur, zoneActuelle, nomBoss, vieBoss, atkBoss, orBoss, xpBoss, potionsNormales, grandesPotions);

            if (vieJoueur > 0)
            {
                territoiresConquis++;
                xpJoueur += xpBoss;
                orJoueur += orBoss;
                cout << "\n SPLENDIDE ! Le boss de la zone " << zoneActuelle << " est mort !\n";
                cout << " [+] Vous gagnez " << xpBoss << " XP !\n";
                cout << " [+] Recompense Royale de " << orBoss << " Or !\n";

                if (xpJoueur >= xpSeuil)
                {
                    niveauJoueur++;
                    xpJoueur -= xpSeuil;
                    xpSeuil += 50;
                    vieMaxJoueur += 30;
                    attaqueJoueur += 10;
                    vieJoueur = vieMaxJoueur;
                    cout << "\n LEVEL UP ++ ! Vous passez au Niveau " << niveauJoueur << " !\n";
                }

                if (zoneActuelle < 4)
                {
                    zoneActuelle++;
                    cout << "\n[[ EVENEMENT : Vous penetrez dans la ZONE " << zoneActuelle << " ]]!\n";
                }
                else
                    cout << "\n[[ INCROYABLE ! Vous avez conquis le Royaume des Ombres ]]!\n";
            }
        }
        else if (option == 4)
        {
            if (orJoueur >= 10)
            {
                orJoueur -= 10;
                vieJoueur = vieMaxJoueur;
                cout << "Une bonne nuit à l'Auberge. Vos PV sont recharges au maximum (" << vieJoueur << ").\n";
            }
            else
                cout << "[!] Pas assez d'Or (Il vous faut 10 pièces).\n";
        }
        else if (option == 5)
        {
            cout << "\n==== BOUTIQUE D'ARTEFACTES ==== \n";
            cout << "Or disponible : " << orJoueur << " PO\n";
            cout << "[1] Epee d'Acier (+10 ATQ)       - 30 Or\n";
            cout << "[2] Lame Mythique (+50 ATQ)      - 120 Or\n";
            cout << "[3] Acheter Potion (+30 PV)      - 20 Or (Stock actuel: " << potionsNormales << ")\n";
            cout << "[4] Acheter Grande Potion (100%) - 50 Or (Stock actuel: " << grandesPotions << ")\n";
            cout << "[5] Retour\n";
            cout << "=================================\n";
            cout << "Votre choix : ";
            int choixB;
            cin >> choixB;

            if (choixB == 1 && orJoueur >= 30)
            {
                orJoueur -= 30;
                attaqueJoueur += 10;
                cout << "Epee achetee !\n";
            }
            else if (choixB == 2 && orJoueur >= 120)
            {
                orJoueur -= 120;
                attaqueJoueur += 50;
                cout << "Lame Mythique equipee !\n";
            }
            else if (choixB == 3 && orJoueur >= 20)
            {
                orJoueur -= 20;
                potionsNormales++;
                cout << "Potion ajoutee a l'inventaire.\n";
            }
            else if (choixB == 4 && orJoueur >= 50)
            {
                orJoueur -= 50;
                grandesPotions++;
                cout << "Grande Potion ajoutee a l'inventaire.\n";
            }
            else if (choixB == 5)
            {
                cout << "Retour.\n";
            }
            else
            {
                cout << "Action impossible (Or insuffisant ou mauvais choix).\n";
            }
        }
        else
        {
            cout << "Choix invalide. Veuillez recommencer.\n";
            choixInvalide = true;
        }

    } while (choixInvalide == true);
}