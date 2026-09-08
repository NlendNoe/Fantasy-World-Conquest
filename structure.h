#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <string>
#include <vector>

struct Inventaire
{
    int potionsNormales = 1;
    int grandesPotions = 1;
    int nombreBouclier = 1;
    int capaciteSac = 3;
    int *sac = nullptr;
};

struct Joueur
{
    std::string nom;
    std::string competenceSpeciale;
    Inventaire inventaire;
    int vie = 100;
    int vieMax = 100;
    int attaque = 10;
    int niveau = 1;
    int orJoueur = 30;
    int xp = 0;
    int xpSeuil = 100;
    int mana; 
    int manaMax;
    int defense;    
    int chanceCrit; 
};

struct Monstre
{
    std::string nom;
    int vie = 0;
    int attaque = 0;
    int orRecompense = 0;
    int xpRecompense = 0;
};

enum TypeNoeud
{
    COMBAT_NORMAL,
    COMBAT_ELITE,
    TRESOR,
    EVENEMENT,
    BOSS,
    REPOS
};

struct Noeud
{
    int id = 0;
    TypeNoeud type = COMBAT_NORMAL;
    bool visite = false;
    bool accessible = false;
    float posX = 0.0f;
    float posY = 0.0f;
    std::vector<Noeud*> voisinsSuivants;
};

#endif