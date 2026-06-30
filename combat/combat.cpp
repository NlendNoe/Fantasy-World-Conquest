#include <iostream>
#include <string>

using namespace std;

void lancerCombat(string nomJoueur, int &vieJoueur, int vieMaxJoueur, int attaqueJoueur, int &orJoueur, int zone, string nomMonstre, int vieMonstre, int attaqueMonstre, int orRecompense, int xpRecompense, int &potionsNormales, int &grandesPotions)
{
    cout << "\n--- UN COMBAT COMMENCE CONTRE: " << nomMonstre << " (Zone " << zone << ") ! ---\n";
    cout << " Vos PV : " << vieJoueur << "/" << vieMaxJoueur << " | PV du Monstre : " << vieMonstre << "\n";

    while (vieJoueur > 0 && vieMonstre > 0)
    {
        int actionCombat = 0;
        bool actionValide = false;

        while (!actionValide)
        {
            cout << "\n[ C'est votre tour ] :\n";
            cout << "[1] Attaquer avec votre arme\n";
            cout << "[2] Ouvrir l'inventaire (Potions)\n";
            cout << "Votre choix : ";
            cin >> actionCombat;

            if (actionCombat == 1)
            {
                vieMonstre -= attaqueJoueur;
                cout << "\n " << nomJoueur << " attaque et inflige " << attaqueJoueur << " degats au " << nomMonstre << ".\n";
                actionValide = true;
            }
            else if (actionCombat == 2)
            {
                cout << "\n--- INVENTAIRE DE COMBAT ---\n";
                cout << "[1] Potion de soins (+30 PV) (Quantite: " << potionsNormales << ")\n";
                cout << "[2] Grande Potion (100% PV)  (Quantite: " << grandesPotions << ")\n";
                cout << "[3] Retour\n";
                cout << "Choisissez un objet : ";
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
                        cout << "Vous buvez une Potion. Vos PV remontent a " << vieJoueur << "/" << vieMaxJoueur << " !\n";
                        actionValide = true;
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
                        cout << "Vous buvez une Grande Potion ! Vos PV sont restaures a 100% (" << vieJoueur << ") !\n";
                        actionValide = true;
                    }
                    else
                        cout << "[!] Vous n'avez pas de Grande Potion !\n";
                }
                else
                    cout << "Retour au choix de combat...\n";
            }
            else
                cout << "[!] Choix invalide.\n";
        }

        if (vieMonstre <= 0)
            break;

        vieJoueur -= attaqueMonstre;
        cout << " Le " << nomMonstre << " replique et inflige " << attaqueMonstre << " degats a " << nomJoueur << ".\n";
        if (vieJoueur < 0)
            vieJoueur = 0;
        cout << " PV restants : " << vieJoueur << "/" << vieMaxJoueur << "\n";
    }

    if (vieJoueur > 0)
    {
        cout << "\nVICTOIRE ! Vous avez terrasse le " << nomMonstre << " !\n";
        orJoueur += orRecompense;
        cout << " [+] Vous ramassez " << orRecompense << " pieces d'or.\n";
    }
    else
    {
        cout << "\n Vous avez ete foudroye par le " << nomMonstre << "... GAME OVER.\n";
    }
}