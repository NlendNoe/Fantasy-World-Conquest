#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "personnages/personnage.cpp"
#include "bestiaire/bestiaire.cpp"
#include "combat/combat.cpp"
#include "exploration/exploration.cpp"

using namespace std;

int main()
{
    srand(time(0));

    string nomJoueur;
    int vieJoueur = 0;
    int attaqueJoueur = 0;
    int niveauJoueur = 0;

    int vieMaxJoueur = 100;
    int orJoueur = 30;
    int xpJoueur = 0;
    int xpSeuil = 100;

    int nombreBouclier = 1;
    int potionsNormales = 1;
    int grandesPotions = 1;
    int capaciteSac = 3;
    int* sac = new int[capaciteSac];

    int territoiresConquis = 0;
    int zoneActuelle = 1;
    int option = 0;

    int gameOption;

    cout << "\n====================================================\n";
    cout << "               FANTASY WORLD CONQUEST \n";
    cout << "====================================================\n";
    cout << "[1] Commencer la partie\n";
    cout << "[2] Quitter\n";
    cout << "Choix: ";
    cin >> gameOption;

    if (gameOption == 1)
    {
        creerPersonnage(nomJoueur, vieJoueur, attaqueJoueur, niveauJoueur);
        vieMaxJoueur = vieJoueur;
    }
    else if (gameOption == 2)
    {
        cout << "Le jeu se ferme!\n";
        return 0;
    }
    else
        cout << "Choix non valide\n";

        
    while (vieJoueur > 0 && territoiresConquis < 6)
    {
        explorerMonde(territoiresConquis, zoneActuelle, option, nomJoueur, vieJoueur, vieMaxJoueur, attaqueJoueur, niveauJoueur, orJoueur, xpJoueur, xpSeuil, potionsNormales, grandesPotions, nombreBouclier, sac, capaciteSac);
    }

    if (vieJoueur <= 0)
    {
        cout << "\n [GAME OVER] VOUS ETES MORT SANS ECRIRE VOTRE LEGENDE. L'EMPIRE EST REDUI EN CENDRE...\n";
    }
    else
    {
        cout << "\n [VICTOIRE ABSOLUE] INCROYABLE ! L'EMPEREUR DES OMBRES EST VAINCU. VOUS ETES LE HERO QUI SAUVAT UTOPIA DU LA DESTRUCTION VOTRE LEGENDE NAIT !\n";
    }

    delete[] sac;
    return 0;
}