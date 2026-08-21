#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <string>

struct Inventaire {
    int potionsNormales = 1;
    int grandesPotions = 1;
    int nombreBouclier = 1;
    int capaciteSac = 3;
    int* sac = nullptr;
};

struct Joueur {
    std::string nom;
    int vie = 100;
    int vieMax = 100;
    int attaque = 10;
    int niveau = 1;
    int orJoueur = 30;
    int xp = 0;
    int xpSeuil = 100;
    Inventaire inventaire;
};

struct Monstre {
    std::string nom;
    int vie = 0;
    int attaque = 0;
    int orRecompense = 0;
    int xpRecompense = 0;
};

#endif // STRUCTURE_H