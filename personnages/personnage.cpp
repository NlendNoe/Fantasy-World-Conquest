#include <iostream>
#include <string>
using namespace std;

void creerPersonnage(string &nom, int &vie, int &attaque, int &niveau)
{
    cout << "Quel est le nom de votre avatar ?: ";
    cin >> nom;
    vie = 100;
    attaque = 10;
    niveau = 1;
}

void afficherPersonnage(string nom, int vie, int attaque, int niveau)
{
    cout << "===================\n";
    cout << "\n JOUEUR \n";
    cout << "===================\n";

    cout << "Nom : " << nom << "\n";
    cout << "Vie : " << vie << "\n";
    cout << "Attaque : " << attaque << "\n";
    cout << "Niveau : " << niveau << "\n";
}