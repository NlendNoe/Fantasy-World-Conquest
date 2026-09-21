#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include <fstream>
#include <string>
#include "structure.h"

inline bool fichierSauvegardeExiste()
{
    std::ifstream fichier("sauvegarde.dat");
    bool existe = fichier.good();
    fichier.close();
    return existe;
}

inline bool sauvegarderPartie(const Joueur &joueur, int zoneActuelle, int territoiresConquis)
{
    std::ofstream fichier("sauvegarde.dat");
    if (!fichier.is_open()) return false;

    fichier << joueur.nom << "\n";
    fichier << joueur.classeNom << "\n";
    fichier << joueur.classeIndex << "\n";
    fichier << joueur.competenceSpeciale << "\n";
    fichier << joueur.vie << " " << joueur.vieMax << "\n";
    fichier << joueur.mana << " " << joueur.manaMax << "\n";
    fichier << joueur.attaque << " " << joueur.defense << " " << joueur.chanceCrit << "\n";
    fichier << joueur.niveau << " " << joueur.orJoueur << " " << joueur.xp << " " << joueur.xpSeuil << "\n";
    fichier << joueur.monstresVaincus << "\n";
    fichier << joueur.inventaire.potionsNormales << " " 
            << joueur.inventaire.grandesPotions << " " 
            << joueur.inventaire.nombreBouclier << " " 
            << joueur.inventaire.potionsMana << " " 
            << joueur.inventaire.capaciteSac << "\n";
    fichier << zoneActuelle << " " << territoiresConquis << "\n";

    fichier.close();
    return true;
}

inline bool chargerPartie(Joueur &joueur, int &zoneActuelle, int &territoiresConquis)
{
    std::ifstream fichier("sauvegarde.dat");
    if (!fichier.is_open()) return false;

    std::getline(fichier, joueur.nom);
    std::getline(fichier, joueur.classeNom);
    fichier >> joueur.classeIndex;
    fichier.ignore();
    std::getline(fichier, joueur.competenceSpeciale);
    fichier >> joueur.vie >> joueur.vieMax;
    fichier >> joueur.mana >> joueur.manaMax;
    fichier >> joueur.attaque >> joueur.defense >> joueur.chanceCrit;
    fichier >> joueur.niveau >> joueur.orJoueur >> joueur.xp >> joueur.xpSeuil;
    fichier >> joueur.monstresVaincus;
    fichier >> joueur.inventaire.potionsNormales 
            >> joueur.inventaire.grandesPotions 
            >> joueur.inventaire.nombreBouclier 
            >> joueur.inventaire.potionsMana 
            >> joueur.inventaire.capaciteSac;
    fichier >> zoneActuelle >> territoiresConquis;

    fichier.close();
    return true;
}

#endif
