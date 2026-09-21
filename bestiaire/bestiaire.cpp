#ifndef BESTIAIRE_CPP
#define BESTIAIRE_CPP

#include <iostream>
#include <string>
#include "../structure.h"

inline std::string getNomZone(int zone)
{
    switch (zone)
    {
    case 1: return "La Forêt des Anciens";
    case 2: return "Les Montagnes du Destin";
    case 3: return "Le Désert Maudit";
    case 4: return "Le Royaume des Ombres";
    case 5: return "Le Domaine Vampirique";
    case 6: return "La Forêt Maudite & Sanctuaire Final";
    default: return "Les Terres Sauvages";
    }
}

inline void genererMonstre(int zone, int numeroAleatoire, Monstre &monstre)
{
    monstre.isBoss = false;

    switch (zone)
    {
    case 1: // FORÊT DES ANCIENS
        switch (numeroAleatoire)
        {
        case 0:  monstre = {"Gobelin", 35, 35, 8, 2, 10, 15, false, "Coup Sournois", 0}; break;
        case 1:  monstre = {"Loup Sauvage", 50, 50, 12, 3, 12, 20, false, "Morsure Rapide", 2}; break;
        case 2:  monstre = {"Squelette", 65, 65, 14, 5, 15, 25, false, "Coup d'Os", 1}; break;
        case 3:  monstre = {"Araignee Geante", 55, 55, 16, 3, 18, 22, false, "Morsure Venimeuse", 2}; break;
        case 4:  monstre = {"Roi Gobelin (BOSS)", 350, 350, 32, 10, 200, 250, true, "Ecrasement Royal", 0}; break;
        default: monstre = {"Gobelin", 35, 35, 8, 2, 10, 15, false, "Coup Sournois", 0}; break;
        }
        break;

    case 2: // MONTAGNES DU DESTIN
        switch (numeroAleatoire)
        {
        case 0:  monstre = {"Orc Guerrier", 90, 90, 22, 8, 25, 40, false, "Fendoir Brise-Crane", 0}; break;
        case 1:  monstre = {"Troll des Cavernes", 160, 160, 28, 12, 40, 75, false, "Frappe Sismique", 4}; break;
        case 2:  monstre = {"Harpie", 105, 105, 25, 6, 30, 50, false, "Cri Percant", 3}; break;
        case 3:  monstre = {"Golem de Pierre", 220, 220, 30, 20, 60, 90, false, "Peau de Granit", 4}; break;
        case 4:  monstre = {"Dragon de Givre (BOSS)", 750, 750, 55, 25, 450, 600, true, "Souffle Glacial", 5}; break;
        default: monstre = {"Orc Guerrier", 90, 90, 22, 8, 25, 40, false, "Rage", 0}; break;
        }
        break;

    case 3: // DÉSERT MAUDIT
        switch (numeroAleatoire)
        {
        case 0:  monstre = {"Scorpion Geant", 130, 130, 32, 12, 40, 60, false, "Dard Toxique", 2}; break;
        case 1:  monstre = {"Momie", 160, 160, 35, 10, 45, 70, false, "Malediction Ancienne", 1}; break;
        case 2:  monstre = {"Anubis Dechu", 280, 280, 42, 18, 85, 130, false, "Jugement des Sables", 3}; break;
        case 3:  monstre = {"Pillard des Sables", 180, 180, 38, 14, 60, 85, false, "Lame du Vent", 0}; break;
        case 4:  monstre = {"Seigneur des Sables (BOSS)", 1100, 1100, 68, 30, 700, 850, true, "Tempete de Sable", 4}; break;
        default: monstre = {"Scorpion Geant", 130, 130, 32, 12, 40, 60, false, "Dard", 2}; break;
        }
        break;

    case 4: // ROYAUME DES OMBRES
        switch (numeroAleatoire)
        {
        case 0:  monstre = {"Chevalier Noir", 240, 240, 48, 22, 75, 160, false, "Lame Noire", 5}; break;
        case 1:  monstre = {"Mage Noir", 180, 180, 55, 12, 80, 140, false, "Eclair du Neant", 3}; break;
        case 2:  monstre = {"Demon Mineur", 320, 320, 58, 20, 100, 190, false, "Feu de l'Abysse", 1}; break;
        case 3:  monstre = {"Faucheur", 380, 380, 65, 25, 140, 230, false, "Faux Fantomatique", 1}; break;
        case 4:  monstre = {"Empereur des Ombres (BOSS)", 1900, 1900, 92, 40, 1500, 2000, true, "Cataclysme des Ombres", 5}; break;
        default: monstre = {"Chevalier Noir", 240, 240, 48, 22, 75, 160, false, "Drain", 5}; break;
        }
        break;

    case 5: // LE DOMAINE VAMPIRIQUE
        switch (numeroAleatoire)
        {
        case 0:  monstre = {"Goule", 260, 260, 50, 15, 60, 160, false, "Griffes Pourries", 1}; break;
        case 1:  monstre = {"Vampire de Bas Rang", 320, 320, 62, 20, 90, 200, false, "Baiser Funeste", 3}; break;
        case 2:  monstre = {"Soldat Vampire", 380, 380, 72, 28, 120, 240, false, "Epee de Sang", 0}; break;
        case 3:  monstre = {"Vampire de Rang Sup.", 440, 440, 85, 32, 180, 300, false, "Festin Nocturne", 3}; break;
        case 4:  monstre = {"Comte Vampire (BOSS)", 2500, 2500, 115, 48, 2500, 3000, true, "Danse Macabre", 5}; break;
        default: monstre = {"Goule", 260, 260, 50, 15, 60, 160, false, "Attaque", 1}; break;
        }
        break;

    case 6: // FORÊT MAUDITE
        switch (numeroAleatoire)
        {
        case 0:  monstre = {"Fee Malefique", 300, 300, 75, 20, 90, 220, false, "Sortilefe Corrompu", 3}; break;
        case 1:  monstre = {"Plante Carnivore Geante", 400, 400, 85, 30, 120, 260, false, "Engourdissement", 2}; break;
        case 2:  monstre = {"Enfant de la Foret", 550, 550, 100, 35, 180, 350, false, "Malediction de Ronces", 0}; break;
        case 3:  monstre = {"Vampire Ancestral", 500, 500, 110, 40, 220, 400, false, "Regard Mortel", 3}; break;
        case 4:  monstre = {"Reine des Fees Malefique (BOSS)", 3200, 3200, 140, 55, 5000, 5000, true, "Jugement Feerique", 5}; break;
        default: monstre = {"Fee Malefique", 300, 300, 75, 20, 90, 220, false, "Magie", 3}; break;
        }
        break;

    default:
        monstre = {"Gobelin Sauvage", 35, 35, 8, 2, 10, 15, false, "Coup Sournois", 0};
        break;
    }
}

#endif