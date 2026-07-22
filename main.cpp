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
    int orJoueur = 10;
    int xpJoueur = 0;
    int xpSeuil = 100;

    int nombreBouclier = 0;
    int potionsNormales = 1;
    int grandesPotions = 1;

    int territoiresConquis = 0;
    int zoneActuelle = 1;
    int option = 0;

    creerPersonnage(nomJoueur, vieJoueur, attaqueJoueur, niveauJoueur);
    vieMaxJoueur = vieJoueur;

    cout << "\n====================================================\n";
    cout << "    BIENVENUE DANS LE MONDE DE FANTASY WORLD CONQUEST \n";
    cout << "====================================================\n";

    while (vieJoueur > 0 && territoiresConquis < 6)
    {
        explorerMonde(territoiresConquis, zoneActuelle, option, nomJoueur, vieJoueur, vieMaxJoueur, attaqueJoueur, niveauJoueur, orJoueur, xpJoueur, xpSeuil, potionsNormales, grandesPotions, nombreBouclier);
    }

    if (vieJoueur <= 0)
    {
        cout << "\n [GAME OVER] VOUS ETES MORT SANS ECRIRE VOTRE LEGENDE. L'EMPIRE EST REDUI EN CENDRE...\n";
    }
    else
    {
        cout << "\n [VICTOIRE ABSOLUE] INCROYABLE ! L'EMPEREUR DES OMBRES EST VAINCU. VOUS ETES LE HERO QUI SAUVAT UTOPIA DU LA DESTRUCTION VOTRE LEGENDE NAIT !\n";
    }

    return 0;
}