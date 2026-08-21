#include <iostream>
#include <string>
#include "../structure.h"

using namespace std;

void lancerCombat(Joueur &joueur, int zone, Monstre monstre)
{
    cout << "\n --- [! UN MONSTRE SURGIT DEVANT VOUS : " << monstre.nom << " (Zone " << zone << ") !] ---\n";
    cout << " Vos PV : " << joueur.vie << "/" << joueur.vieMax << " | PV du Monstre : " << monstre.vie << "\n";

    while (joueur.vie > 0 && monstre.vie > 0)
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
                monstre.vie -= joueur.attaque;
                cout << "\n" << joueur.nom << " attaque et inflige " << joueur.attaque << " degats au " << monstre.nom << ".\n";
                actionValide = true;
            }
            else if (actionCombat == 2)
            {
                cout << "\n--- INVENTAIRE DE COMBAT ---\n";
                cout << "[1] Potion de soins (+30 PV) (Quantite: " << joueur.inventaire.potionsNormales << ")\n";
                cout << "[2] Grande Potion (100% PV)  (Quantite: " << joueur.inventaire.grandesPotions << ")\n";
                cout << "[3] Utiliser un bouclier (Degats /2) (Quantite: " << joueur.inventaire.nombreBouclier << ")\n";
                cout << "[4] Retour\n";
                cout << "Choisissez une action : ";

                int choixObjet;
                cin >> choixObjet;

                if (choixObjet == 1)
                {
                    if (joueur.inventaire.potionsNormales > 0)
                    {
                        joueur.inventaire.potionsNormales--;
                        joueur.vie += 30;
                        if (joueur.vie > joueur.vieMax)
                            joueur.vie = joueur.vieMax;
                        cout << "Vous buvez une Potion. Vos PV : " << joueur.vie << "/" << joueur.vieMax << " !\n";
                    }
                    else cout << "[!] Vous n'avez pas de Potion de soins !\n";
                }
                else if (choixObjet == 2)
                {
                    if (joueur.inventaire.grandesPotions > 0)
                    {
                        joueur.inventaire.grandesPotions--;
                        joueur.vie = joueur.vieMax;
                        cout << "Grande Potion bue ! PV restaures a 100% (" << joueur.vie << ") !\n";
                    }
                    else cout << "[!] Vous n'avez pas de Grande Potion !\n";
                }
                else if (choixObjet == 3)
                {
                    if (joueur.inventaire.nombreBouclier > 0)
                    {
                        joueur.inventaire.nombreBouclier--;
                        postureDefensive = true;
                        cout << "Vous levez votre bouclier ! Il absorbera la moitie des degats de la prochaine attaque.\n";
                    }
                    else cout << "[!] Vous n'avez plus de bouclier dans votre sac !\n";
                }
                else if (choixObjet == 4)
                {
                    cout << "Retour au menu precedent...\n";
                }
                else cout << "[!] Choix invalide.\n";
            }
            else cout << "[!] Choix invalide.\n";
        }

        if (monstre.vie <= 0) break;

        int degatSubis = postureDefensive ? (monstre.attaque / 2) : monstre.attaque;
        if (postureDefensive)
            cout << "\n[BOUCLIER] Votre bouclier absorbe l'impact ! Degats reduits de moitie.\n";

        if (degatSubis < 1) degatSubis = 1;

        joueur.vie -= degatSubis;
        cout << "Le " << monstre.nom << " replique et vous inflige " << degatSubis << " degats.\n";

        if (joueur.vie < 0) joueur.vie = 0;
        cout << "PV restants : " << joueur.vie << "/" << joueur.vieMax << "\n";
    }

    if (joueur.vie > 0)
    {
        cout << "\nVICTOIRE ! Vous avez terrasse le " << monstre.nom << " !\n";
        joueur.orJoueur += monstre.orRecompense;
        cout << "[+] Vous ramassez " << monstre.orRecompense << " pieces d'or.\n";
    }
    else
    {
        cout << "\nVous avez ete foudroye par le " << monstre.nom << "... GAME OVER.\n";
    }
}