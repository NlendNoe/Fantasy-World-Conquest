#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <string>
#include <vector>

struct Inventaire
{
    int potionsNormales = 2;
    int grandesPotions = 1;
    int nombreBouclier = 1;
    int potionsMana = 2;
    int capaciteSac = 5;
    int *sac = nullptr;
};

struct Joueur
{
    std::string nom = "Héros";
    std::string classeNom = "Guerrier";
    int classeIndex = 0;
    std::string competenceSpeciale = "Coup puissant";
    Inventaire inventaire;
    int vie = 120;
    int vieMax = 120;
    int mana = 20;
    int manaMax = 20;
    int attaque = 15;
    int defense = 15;
    int chanceCrit = 15;
    int niveau = 1;
    int orJoueur = 50;
    int xp = 0;
    int xpSeuil = 100;
    int monstresVaincus = 0;
};

struct Monstre
{
    std::string nom = "Gobelin";
    int vie = 35;
    int vieMax = 35;
    int attaque = 8;
    int defense = 2;
    int orRecompense = 10;
    int xpRecompense = 15;
    bool isBoss = false;
    std::string capaciteSpeciale = "Coup Sournois";
    int spriteType = 0;
};

#endif