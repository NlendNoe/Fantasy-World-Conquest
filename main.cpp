#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "structure.h"
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
    int gameOption = 0;

    cout << "\n====================================================\n";
    cout << "               FANTASY WORLD CONQUEST \n";
    cout << "====================================================\n";
    cout << "[1] Commencer la partie\n";
    cout << "[2] Quitter\n";
    cout << "Choix: ";
    cin >> gameOption;

    if (gameOption == 1)
    {
        creerPersonnage(joueur);
    }
    else if (gameOption == 2)
    {
        cout << "Le jeu se ferme!\n";
        delete[] joueur.inventaire.sac;
        return 0;
    }
    else
    {
        cout << "Choix non valide\n";
    }

    while (joueur.vie > 0 && territoiresConquis < 6)
    {
        explorerMonde(territoiresConquis, zoneActuelle, option, joueur);
    }

    if (joueur.vie <= 0)
    {
        cout << "\n [GAME OVER] VOUS ETES MORT SANS ECRIRE VOTRE LEGENDE. L'EMPIRE EST REDUI EN CENDRE...\n";
    }
    else
    {
        cout << "\n [VICTOIRE ABSOLUE] INCROYABLE ! L'EMPEREUR DES OMBRES EST VAINCU. VOUS ETES LE HERO QUI SAUVAT UTOPIA DE LA DESTRUCTION VOTRE LEGENDE NAIT !\n";
    }

    delete[] joueur.inventaire.sac;
    return 0;
}