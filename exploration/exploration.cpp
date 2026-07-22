#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace std;

void lancerCombat(string nomJoueur, int &vieJoueur, int vieMaxJoueur, int attaqueJoueur, int &orJoueur, int zone, string nomMonstre, int vieMonstre, int attaqueMonstre, int orRecompense, int xpRecompense, int &potionsNormales, int &grandesPotions, int &nombreBouclier);
void genererMonstre(int zone, int numeroAleatoire, string &nomMonstre, int &vieMonstre, int &attaqueMonstre, int &orRecompense, int &xpRecompense);

void explorerMonde(int &territoiresConquis, int &zoneActuelle, int &option, string nomJoueur, int &vieJoueur, int &vieMaxJoueur, int &attaqueJoueur, int &niveauJoueur, int &orJoueur, int &xpJoueur, int &xpSeuil, int &potionsNormales, int &grandesPotions, int &nombreBouclier)
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
                cout << "\n-------------------------------------------------------\n";
                cout << "PV: " << vieJoueur << "/" << vieMaxJoueur << " | Or: " << orJoueur << " | Niveau: " << niveauJoueur << " | ATQ: " << attaqueJoueur << " | Boucliers: " << nombreBouclier << "\n";
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
                            cout << "Vous quittez l'exploration et rentrez vous mettre a l'abri en ville.\n";
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
            cout << " Territoires conquis : " << territoiresConquis << "/4\n";
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
                cout << "\n=================================\n";
                cout << "         BOUTIQUE DE LA CITE\n";
                cout << "=================================\n";
                cout << " Or disponible : " << orJoueur << " PO\n";
                cout << "[1] Epee en Fer (+10 ATQ)        - 30 Or\n";
                cout << "[2] Epee d'acier (+20 ATQ)       - 60 Or\n";
                cout << "[3] Epee magique (+40 ATQ)       - 120 Or\n";
                cout << "[4] Acheter Bouclier             - 60 Or (Stock actuel: " << nombreBouclier << ")\n";
                cout << "[5] Epee du Hero (+370 ATQ)      - 1750 Or\n";
                cout << "[6] Acheter Potion (+30 PV)      - 20 Or (Stock actuel: " << potionsNormales << ")\n";
                cout << "[7] Acheter Grande Potion (100%) - 50 Or (Stock actuel: " << grandesPotions << ")\n";
                cout << "[8] Agrandissement du sac        - 170 Or\n";
                cout << "[9] Retour\n";
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
                else if (choixB == 2 && orJoueur >= 60)
                {
                    orJoueur -= 60;
                    attaqueJoueur += 20;
                    cout << "Epee d'acier equipee !\n";
                }
                else if (choixB == 3 && orJoueur >= 120)
                {
                    orJoueur -= 120;
                    attaqueJoueur += 40;
                    cout << "Epee Magique equipee !\n";
                }
                else if (choixB == 4 && orJoueur >= 60)
                {
                    orJoueur -= 60;
                    nombreBouclier++;
                    cout << "Bouclier ajoute a l'inventaire !\n";
                }
                else if (choixB == 5 && orJoueur >= 1750)
                {
                    orJoueur -= 1750;
                    attaqueJoueur += 370;
                    cout << "[!EPEE DU HERO EQUIPEE VOTRE PUISSANCE AUGMENTE GRANDEMENT] !\n";
                }
                else if (choixB == 6 && orJoueur >= 20)
                {
                    orJoueur -= 20;
                    potionsNormales++;
                    cout << "Potion ajoutee a l'inventaire.\n";
                }
                else if (choixB == 7 && orJoueur >= 50)
                {
                    orJoueur -= 50;
                    grandesPotions++;
                    cout << "Grande Potion ajoutee a l'inventaire.\n";
                }
                else if (choixB == 8 && orJoueur >= 170)
                {
                    orJoueur -= 170;
                    cout << "Sac agrandi ! (Fonctionnalite visuelle uniquement pour le moment).\n";
                }
                else if (choixB == 9)
                {
                    dansBoutique = false;
                    cout << "Retour.\n";
                }
                else
                {
                    cout << "Action impossible (Or insuffisant ou mauvais choix).\n";
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