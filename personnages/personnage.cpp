#include <iostream>
#include <string>
#include "../structure.h"

using namespace std;

void creerPersonnage(Joueur &joueur)
{
    cout << "Quel est le nom de votre avatar (Pas d'espace ni de caractere special) ?: ";
    cin >> joueur.nom;
    joueur.vie = 100;
    joueur.vieMax = 100;
    joueur.attaque = 10;
    joueur.niveau = 1;
}

void afficherPersonnage(const Joueur &joueur)
{
    cout << "===================\n";
    cout << "\n JOUEUR \n";
    cout << "===================\n";

    cout << "Nom : " << joueur.nom << "\n";
    cout << "Vie : " << joueur.vie << "/" << joueur.vieMax << "\n";
    cout << "Attaque : " << joueur.attaque << "\n";
    cout << "Niveau : " << joueur.niveau << "\n";
}