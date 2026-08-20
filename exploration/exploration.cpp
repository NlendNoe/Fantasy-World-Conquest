#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace std;

void lancerCombat(string nomJoueur, int &vieJoueur, int vieMaxJoueur, int attaqueJoueur, int &orJoueur, int zone, string nomMonstre, int vieMonstre, int attaqueMonstre, int orRecompense, int xpRecompense, int &potionsNormales, int &grandesPotions, int &nombreBouclier);
void genererMonstre(int zone, int numeroAleatoire, string &nomMonstre, int &vieMonstre, int &attaqueMonstre, int &orRecompense, int &xpRecompense);

void explorerMonde(int &territoiresConquis, int &zoneActuelle, int &option, string nomJoueur, int &vieJoueur, int &vieMaxJoueur, int &attaqueJoueur, int &niveauJoueur, int &orJoueur, int &xpJoueur, int &xpSeuil, int &potionsNormales, int &grandesPotions, int &nombreBouclier, int *&sac, int &capaciteSac)
{
    while (vieJoueur > 0)
    {
        cout << "\n=============================================\n";
        cout << "    MENU PRINCIPAL - ZONE ACTUELLE : " << zoneActuelle << "\n";
        cout << "=============================================\n";
        cout << "[1] Commencer a explorer la contree\n";
        cout << "[2] Afficher l'etat du heros\n";
        cout << "[3] LANCER LE RAID CONTRE LE BOSS DE LA ZONE\n";
        cout << "[4] Se reposer a l'Auberge (Soins complets) [20 Or]\n";
        cout << "[5] Aller a la boutique de la cite\n";
        cout << "[6] Retourner a l'ecran titre\n";
        cout << "=============================================\n";
        cout << "Votre choix : ";
        cin >> option;

        if (option == 1)
        {
            bool continuerExplo = true;

            while (continuerExplo && vieJoueur > 0)
            {
                int objetsUtilises = 0;
                for (int i = 0; i < capaciteSac; i++) {
                    if (sac[i] != 0) {
                        objetsUtilises++;
                    }
                }

                cout << "\n-------------------------------------------------------\n";
                cout << "PV: " << vieJoueur << "/" << vieMaxJoueur << " | Or: " << orJoueur << " | Niveau: " << niveauJoueur << " | ATQ: " << attaqueJoueur << " | Boucliers: " << nombreBouclier << " | Sac: " << objetsUtilises << "/" << capaciteSac << "\n";
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
                    string nomM;
                    int vieM = 0, atkM = 0, orM = 0, xpM = 0;
                    int quelMonstre = rand() % 4;

                    genererMonstre(zoneActuelle, quelMonstre, nomM, vieM, atkM, orM, xpM);
                    lancerCombat(nomJoueur, vieJoueur, vieMaxJoueur, attaqueJoueur, orJoueur, zoneActuelle, nomM, vieM, atkM, orM, xpM, potionsNormales, grandesPotions, nombreBouclier);

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
                else if (deEvenement >= 35 && deEvenement < 70)
                {
                    int typeTresor = rand() % 4;
                    cout << "\n [DECOUVERTE] Oh ! Votre regard est attire par un coffre abandonne...\n";

                    switch (typeTresor)
                    {
                    case 0:
                    {
                        int orTrouve = (rand() % 10) + 10;
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
                    case 3:
                        nombreBouclier++;
                        cout << " Genial ! Vous trouvez un bouclier en bon etat ! Boucliers en stock : " << nombreBouclier << "\n";
                        break;
                    }
                }
                else if (deEvenement >= 70 && deEvenement < 80)
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

                    if (vieJoueur > 0)
                    {
                        cout << "Voulez-vous utiliser une Potion de votre sac ?\n";
                        cout << " [1] Potion (" << potionsNormales << " restants)\n";
                        cout << " [2] Grande Potion (" << grandesPotions << " restants)\n";
                        cout << " [3] Continuer sans se soigner : \n";

                        int choixSoin;
                        cin >> choixSoin;
                        if (choixSoin == 1 && potionsNormales > 0)
                        {
                            potionsNormales--;
                            vieJoueur += 30;
                            if (vieJoueur > vieMaxJoueur)
                                vieJoueur = vieMaxJoueur;
                            cout << "Potion utilisee. PV : " << vieJoueur << "/" << vieMaxJoueur << "\n";
                        }
                        else if (choixSoin == 2 && grandesPotions > 0)
                        {
                            grandesPotions--;
                            vieJoueur = vieMaxJoueur;
                            cout << "Grande Potion utilisee. PV : " << vieJoueur << "/" << vieMaxJoueur << "\n";
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
                        attaqueJoueur += epee;
                        cout << " Vous ouvrez le coffre et trouvez une [Epee en fin de vie] qui vous transmet ses dernieres forces [+" << epee << " ATQ]! Votre attaque passe a : " << attaqueJoueur << "\n";
                        break;
                    }
                    case 1:
                        attaqueJoueur += 10;
                        cout << "Vous trouvez une simple epee! Pas mal. ATQ [+10] votre attaque passe a : " << attaqueJoueur << "!\n";
                        break;
                    case 2:
                        attaqueJoueur += 20;
                        cout << " Incroyable ! Vous mettez la main sur une Grande Epee ATQ [+20] votre attaque passe a : " << attaqueJoueur << "!\n";
                        break;
                    }
                }
                else
                {
                    cout << "Apres avoir explore la zone pendant des heures, le paysage reste calme.\n";
                }

                if (vieJoueur > 0)
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
                        {
                            gestionRoute = false;
                        }
                        else if (choixRoute == 2)
                        {
                            cout << "\n--- INVENTAIRE DE SOINS ---\n";
                            cout << "[1] Potion de soins (+30 PV) (Quantite: " << potionsNormales << ")\n";
                            cout << "[2] Grande Potion (100% PV)  (Quantite: " << grandesPotions << ")\n";
                            cout << "[3] Retour\n";
                            cout << "Choisissez une action : ";

                            int choixObjet;
                            cin >> choixObjet;

                            if (choixObjet == 1)
                            {
                                if (potionsNormales > 0)
                                {
                                    potionsNormales--;
                                    vieJoueur += 30;
                                    if (vieJoueur > vieMaxJoueur)
                                        vieJoueur = vieMaxJoueur;
                                    cout << "Vous buvez une Potion. Vos PV : " << vieJoueur << "/" << vieMaxJoueur << " !\n";
                                }
                                else
                                    cout << "[!] Vous n'avez pas de Potion de soins !\n";
                            }
                            else if (choixObjet == 2)
                            {
                                if (grandesPotions > 0)
                                {
                                    grandesPotions--;
                                    vieJoueur = vieMaxJoueur;
                                    cout << "Grande Potion bue ! PV restaures a 100% (" << vieJoueur << ") !\n";
                                }
                                else
                                    cout << "[!] Vous n'avez pas de Grande Potion !\n";
                            }
                        }
                        else if (choixRoute == 3)
                        {
                            continuerExplo = false;
                            gestionRoute = false;
                            cout << "Vous quittez l'exploration et rentrez vous mettre a l me abri en ville.\n";
                        }
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
            cout << " Sac a dos : [" << potionsNormales << "] Potions | [" << grandesPotions << "] Grandes Potions | [" << nombreBouclier << "] Boucliers\n";
            cout << " Territoires conquis : " << territoiresConquis << "/6\n";
        }
        else if (option == 3)
        {
            cout << "\n ATTENTION ! Vous franchissez les portes de la forteresse ennemie...\n";
            string nomBoss;
            int vieBoss = 0, atkBoss = 0, orBoss = 0, xpBoss = 0;

            genererMonstre(zoneActuelle, 4, nomBoss, vieBoss, atkBoss, orBoss, xpBoss);

            cout << " Le boss " << nomBoss << " se dresse devant vous !\n";
            lancerCombat(nomJoueur, vieJoueur, vieMaxJoueur, attaqueJoueur, orJoueur, zoneActuelle, nomBoss, vieBoss, atkBoss, orBoss, xpBoss, potionsNormales, grandesPotions, nombreBouclier);

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

                if (zoneActuelle < 6)
                {
                    zoneActuelle++;
                    cout << "\n[[ EVENEMENT : Vous penetrez dans la ZONE " << zoneActuelle << " ]]!\n";
                }
                else
                    cout << "\n[[ INCROYABLE ! Vous avez conquis LA FORET MAUDITE ]]!\n";
            }
        }
        else if (option == 4)
        {
            if (orJoueur >= 20)
            {
                orJoueur -= 20;
                vieJoueur = vieMaxJoueur;
                cout << "Une bonne nuit a l'Auberge. Vos PV sont recharges au maximum (" << vieJoueur << ").\n";
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
                cout << " Votre Bourse : " << orJoueur << " PO\n";
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
                cout << " [11] Potion de soins (+30 PV)               - 20 Or (Stock: " << potionsNormales << ")\n";
                cout << " [12] Grande Potion (Soins 100%)             - 50 Or (Stock: " << grandesPotions << ")\n";
                cout << " [13] Elixir d'Experience (+50 XP)           - 90 Or\n";
                cout << "----------------- MAGIE & AMELIORATIONS ---------------\n";
                cout << " [14] Parchemin de Force (+5 ATQ permanent)  - 100 Or\n";
                cout << " [15] Pierre de Teleportation (Soins + Fuite)- 45 Or\n";
                cout << " [16] Agrandir Sac (+2 Emplacements)         - 70 Or (Max: " << capaciteSac << ")\n";
                cout << " [17] Sac du Voyageur (+5 Emplacements)      - 150 Or\n";
                cout << "-------------------------------------------------------\n";
                cout << " [s] Quitter la boutique\n";
                cout << "=======================================================\n";
                cout << "Votre choix : ";
                int choixB;
                cin >> choixB;

                if (choixB == 1 && orJoueur >= 15)
                {
                    orJoueur -= 15;
                    attaqueJoueur += 5;
                    cout << "Vous achetez une Dague de Voleur ! (+5 ATQ)\n";
                }
                else if (choixB == 2 && orJoueur >= 40)
                {
                    orJoueur -= 40;
                    attaqueJoueur += 15;
                    cout << "Vous achetez une Hache de Pierre ! (+15 ATQ)\n";
                }
                else if (choixB == 3 && orJoueur >= 75)
                {
                    orJoueur -= 75;
                    attaqueJoueur += 25;
                    cout << "Vous equipez une Epee en Fer ! (+25 ATQ)\n";
                }
                else if (choixB == 4 && orJoueur >= 130)
                {
                    orJoueur -= 130;
                    attaqueJoueur += 40;
                    cout << "Vous equipez une super Epee d'Acier ! (+40 ATQ)\n";
                }
                else if (choixB == 5 && orJoueur >= 250)
                {
                    orJoueur -= 250;
                    attaqueJoueur += 75;
                    cout << "L'Epee Magique brille entre vos mains ! (+75 ATQ)\n";
                }
                else if (choixB == 6 && orJoueur >= 1750)
                {
                    orJoueur -= 1750;
                    attaqueJoueur += 370;
                    cout << "[! EPEE DU HEROS EQUIPEE ! VOTRE PUISSANCE AUGMENTE DE MANIERE LEGENDAIRE !]\n";
                }
                else if (choixB == 7 && orJoueur >= 35)
                {
                    orJoueur -= 35;
                    nombreBouclier++;
                    cout << "Vous achetez un Bouclier en Bois (+1 Bouclier en stock).\n";
                }
                else if (choixB == 8 && orJoueur >= 65)
                {
                    orJoueur -= 65;
                    nombreBouclier += 2;
                    cout << "Vous achetez un solide Bouclier en Acier (+2 Boucliers en stock).\n";
                }
                else if (choixB == 9 && orJoueur >= 80)
                {
                    orJoueur -= 80;
                    vieMaxJoueur += 15;
                    vieJoueur += 15;
                    cout << "Vous enfilez une Cotte de Mailles ! (+15 PV Max)\n";
                }
                else if (choixB == 10 && orJoueur >= 180)
                {
                    orJoueur -= 180;
                    vieMaxJoueur += 40;
                    vieJoueur += 40;
                    cout << "Vous vous equipez d'une lourde Armure de Plaque ! (+40 PV Max)\n";
                }
                else if (choixB == 11 && orJoueur >= 20)
                {
                    orJoueur -= 20;
                    potionsNormales++;
                    cout << "Potion de soins ajoutee a votre inventaire.\n";
                }
                else if (choixB == 12 && orJoueur >= 50)
                {
                    orJoueur -= 50;
                    grandesPotions++;
                    cout << "Grande Potion ajoutee a votre inventaire.\n";
                }
                else if (choixB == 13 && orJoueur >= 90)
                {
                    orJoueur -= 90;
                    xpJoueur += 50;
                    cout << "Vous buvez l'Elixir d'Experience ! (+50 XP)\n";
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
                else if (choixB == 14 && orJoueur >= 100)
                {
                    orJoueur -= 100;
                    attaqueJoueur += 5;
                    cout << "Vous lisez le Parchemin de Force ! Votre attaque augmente de +5 de facon permanente.\n";
                }
                else if (choixB == 15 && orJoueur >= 45)
                {
                    orJoueur -= 45;
                    vieJoueur = vieMaxJoueur;
                    cout << "La Pierre de Teleportation brille ! Vos PV sont restaurez a 100%.\n";
                }
                else if (choixB == 16 && orJoueur >= 70)
                {
                    int *nouveauSac = new int[capaciteSac + 2];
                    for (int i = 0; i < capaciteSac; i++)
                        nouveauSac[i] = sac[i];

                    nouveauSac[capaciteSac] = 0;
                    nouveauSac[capaciteSac + 1] = 0;

                    delete[] sac;
                    sac = nouveauSac;
                    capaciteSac += 2;
                    orJoueur -= 70;
                    cout << "Vous achetez +2 emplacements de sac ! Nouvelle capacite : " << capaciteSac << "\n";
                }
                else if (choixB == 17 && orJoueur >= 150)
                {
                    int *nouveauSac = new int[capaciteSac + 5];
                    for (int i = 0; i < capaciteSac; i++)
                        nouveauSac[i] = sac[i];

                    for (int i = 0; i < 5; i++)
                        nouveauSac[capaciteSac + i] = 0;

                    delete[] sac;
                    sac = nouveauSac;
                    capaciteSac += 5;
                    orJoueur -= 150;
                    cout << "Vous achetez le Sac du Voyageur (+5 emplacements) ! Nouvelle capacite : " << capaciteSac << "\n";
                }
                else if (choixB == 's' || choixB == 'S')
                {
                    dansBoutique = false;
                    cout << "Vous quittez la boutique.\n";
                }
                else
                {
                    cout << "Action impossible (Or insuffisant ou choix invalide).\n";
                }
            }
        }
        else if (option == 6)
        {
            cout << "Retour a la boucle principale du jeu.\n";
            break;
        }
        else
        {
            cout << "Choix invalide. Veuillez recommencer.\n";
        }
    }
}