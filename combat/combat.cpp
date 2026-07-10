#include <iostream>
#include <string>

using namespace std;

void lancerCombat(string nomJoueur, int &vieJoueur, int vieMaxJoueur, int attaqueJoueur, int &orJoueur, int zone, string nomMonstre, int vieMonstre, int attaqueMonstre, int orRecompense, int xpRecompense, int &potionsNormales, int &grandesPotions, int defenseBouclier)
{
    cout << "\n --- [! UN MONSTRE SURGIT DEVANT VOUS : " << nomMonstre << " (Zone " << zone << ") !] ---\n";
    cout << " Vos PV : " << vieJoueur << "/" << vieMaxJoueur << " | PV du Monstre : " << vieMonstre << "\n";

    while (vieJoueur > 0 && vieMonstre > 0)
    {
        int actionCombat = 0;
        bool actionValide = false;
        bool postureDefensive = false;

        while (!actionValide)
        {
            cout << "\n[ C'est votre tour ] :\n";
            cout << "[1] Attaquer avec votre arme\n";
            cout << "[2] Ouvrir l'inventaire (Potions / Bouclier)\n";
            cout << "Votre choix : ";
            cin >> actionCombat;

            if (actionCombat == 1)
            {
                vieMonstre -= attaqueJoueur;
                cout << "\n"
                     << nomJoueur << " attaque et inflige " << attaqueJoueur << " degats au " << nomMonstre << ".\n";
                actionValide = true;
            }
            else if (actionCombat == 2)
            {
                cout << "\n--- INVENTAIRE DE COMBAT ---\n";
                cout << "[1] Potion de soins (+30 PV) (Quantite: " << potionsNormales << ")\n";
                cout << "[2] Grande Potion (100% PV)  (Quantite: " << grandesPotions << ")\n";
                cout << "[3] Se proteger avec le bouclier (Defense x2 ce tour)\n";
                cout << "[4] Retour\n";
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
                else if (choixObjet == 3)
                {
                    postureDefensive = true;
                    cout << "Vous levez votre bouclier. Vous reduirez grandement le prochain coup.\n";
                    actionValide = true;
                }
                else if (choixObjet == 4)
                {
                    cout << "Retour au menu precedent...\n";
                }
                else
                {
                    cout << "[!] Choix invalide.\n";
                }
            }
            else
            {
                cout << "[!] Choix invalide.\n";
            }
        }

        if (vieMonstre <= 0)
            break;

        int degatSubis = 0;

        if (postureDefensive)
            degatSubis = attaqueMonstre - (defenseBouclier * 2);
        else
            degatSubis = attaqueMonstre - defenseBouclier;

        if (degatSubis < 1)
            degatSubis = 1;

        vieJoueur -= degatSubis;
        cout << "Le " << nomMonstre << " replique et vous inflige " << degatSubis << " degats.\n";

        if (vieJoueur < 0)
            vieJoueur = 0;
        cout << "PV restants : " << vieJoueur << "/" << vieMaxJoueur << "\n";
    }

    if (vieJoueur > 0)
    {
        cout << "\nVICTOIRE ! Vous avez terrasse le " << nomMonstre << " !\n";
        orJoueur += orRecompense;
        cout << "[+] Vous ramassez " << orRecompense << " pieces d'or.\n";
    }
    else
    {
        cout << "\nVous avez ete foudroye par le " << nomMonstre << "... GAME OVER.\n";
    }
}