#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

void explorerMonde(int &territoiresConquis, int &zoneActuelle, int &option, string nomJoueur, int &vieJoueur, int &vieMaxJoueur, int &attaqueJoueur, int &niveauJoueur, int &orJoueur, int &xpJoueur, int &xpSeuil, int &potionsNormales, int &grandesPotions)
{
    //1- Au niveau du combat quand on utilise un potion le tour du joeur fini et c'est a celui du monstre de lancer une attaque je dois gere ça
    //2- l'orthographe
    //3-ajouter une option qui demande au jouer s'il veux revenir en ville ou continuer a explorer
    bool choixInvalide = false;

    do
    {
        cout << "\n=============================================\n";
        cout << "   MENU PRINCIPAL - ZONE ACTUELLE : " << zoneActuelle << "\n";
        cout << "=============================================\n";
        cout << "[1] Commencer a explorer la contree\n";
        cout << "[2] Afficher l'etat du heros & de l'Empire\n";
        cout << "[3] LANCER LE RAID CONTRE LE BOSS DE LA ZONE\n";
        cout << "[4] Se reposer a l'Auberge (Soins complets) [10 Or]\n";
        cout << "[5] Aller a la boutique de la cite\n";
        cout << "=============================================\n";
        cout << "Votre choix : ";
        cin >> option;

        choixInvalide = false;

        if (option == 1)
        {
            cout << "\n Vous marchez prudemment dans la zone " << zoneActuelle;
            for (int i = 0; i < 3; i++)
            {
                cout << ". ";
                this_thread::sleep_for(chrono::seconds(5));
            }
            cout << "\n";

            int chanceRencontre = rand() % 2;

            if (chanceRencontre == 1)
            {
                string nomM;
                int vieM = 0, atkM = 0, orM = 0, xpM = 0;

                int quelMonstre = rand() % 4;

                genererMonstre(zoneActuelle, quelMonstre, nomM, vieM, atkM, orM, xpM);
                lancerCombat(nomJoueur, vieJoueur, vieMaxJoueur, attaqueJoueur, orJoueur, zoneActuelle, nomM, vieM, atkM, orM, xpM, potionsNormales, grandesPotions);

                if (vieJoueur > 0)
                {
                    xpJoueur += xpM;
                    cout << " [+] Vous gagnez " << xpM << " XP ! (" << xpJoueur << "/" << xpSeuil << " XP)\n";

                    if (xpJoueur >= xpSeuil)
                    {
                        niveauJoueur++;
                        xpJoueur -= xpSeuil;
                        xpSeuil += 50;
                        vieMaxJoueur += 25;
                        attaqueJoueur += 7;
                        vieJoueur = vieMaxJoueur;
                        cout << "\n LEVEL UP ! Vous passez au Niveau " << niveauJoueur << " !\n";
                    }
                }
            }
            else
                cout << " Apres avoir explorer des heures rien ne s'est passer!.\n";
        }
        else if (option == 2)
        {
            cout << "\n --- FEUILLE DE PERSONNAGE & EMPIRE ---\n";
            cout << " Nom de l'Avatar : " << nomJoueur << " | Niveau : " << niveauJoueur << "\n";
            cout << " PV : " << vieJoueur << "/" << vieMaxJoueur << " | Attaque : " << attaqueJoueur << "\n";
            cout << " Experience : " << xpJoueur << "/" << xpSeuil << " XP\n";
            cout << " Bourse d'or : " << orJoueur << " pieces d'or\n";
            cout << " Sac a dos : [" << potionsNormales << "] Potions | [" << grandesPotions << "] Grandes Potions\n";
            cout << " Territoires liberes : " << territoiresConquis << "/4\n";
        }
        else if (option == 3)
        {
            cout << "\nATTENTION ! Vous franchissez les portes de la forteresse ennemie...\n";
            string nomBoss;
            int vieBoss = 0, atkBoss = 0, orBoss = 0, xpBoss = 0;

            // Le numéro 4 correspond STRICTEMENT au Boss de la zone dans notre bestiaire
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
                cout << " [+] Recompense = " << orBoss << " !\n";

                if (xpJoueur >= xpSeuil)
                {
                    niveauJoueur++;
                    xpJoueur -= xpSeuil;
                    xpSeuil += 50;
                    vieMaxJoueur += 30;
                    attaqueJoueur += 10;
                    vieJoueur = vieMaxJoueur;
                    cout << " LEVEL UP ! Vous passez au Niveau " << niveauJoueur << " !\n";
                    cout << " [+] Vie Max = " << vieMaxJoueur << " !\n";
                    cout << " [+] Attaque = " << attaqueJoueur << " !\n";
                }

                if (zoneActuelle < 4)
                {
                    zoneActuelle++;
                    cout << "\n [[ EVENEMENT : Vous penetrez dans la ZONE " << zoneActuelle << " ]]!\n";
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
                cout << " Une bonne nuit al'Auberge. Vos PV sont recharges au maximum (" << vieJoueur << ").\n";
            }
            else
                cout << " [!] Pas assez d'Or (Il vous faut 10 pièces).\n";
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
                cout << "Epée achetée !\n";
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
                cout << " Potion ajoutee a l'inventaire.\n";
            }
            else if (choixB == 4 && orJoueur >= 50)
            {
                orJoueur -= 50;
                grandesPotions++;
                cout << " Grande Potion ajoutee a l'inventaire.\n";
            }
            else if (choixB == 5)
                cout << "Retour.\n";
            else
                cout << "Action impossible (Or insuffisant ou mauvais choix).\n";
        }
        else
        {
            cout << "Choix invalide. Veuillez recommencer.\n";
            choixInvalide = true;
        }

    } while (choixInvalide == true);
}