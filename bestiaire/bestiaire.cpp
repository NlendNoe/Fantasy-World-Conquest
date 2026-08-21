#include <iostream>
#include <string>
#include "../structure.h"

using namespace std;

void genererMonstre(int zone, int numeroAleatoire, Monstre &monstre)
{
    switch (zone)
    {
    case 1: // FORÊT DES ANCIENS
        switch (numeroAleatoire)
        {
        case 0:  monstre = {"Gobelin", 30, 5, 5, 10}; break;
        case 1:  monstre = {"Loup Sauvage", 45, 8, 8, 15}; break;
        case 2:  monstre = {"Squelette", 60, 10, 12, 20}; break;
        case 3:  monstre = {"Araignee Geante", 50, 12, 10, 18}; break;
        case 4:  monstre = {" Roi Gobelin (BOSS)", 350, 40, 200, 250}; break;
        default: monstre = {"Gobelin", 30, 5, 5, 10}; break;
        }
        break;

    case 2: // MONTAGNES DU DESTIN
        switch (numeroAleatoire)
        {
        case 0:  monstre = {"Orc Guerrier", 90, 25, 20, 35}; break;
        case 1:  monstre = {"Troll des Cavernes", 180, 35, 35, 80}; break;
        case 2:  monstre = {"Harpie", 110, 25, 25, 40}; break;
        case 3:  monstre = {"Golem de Pierre", 250, 40, 60, 90}; break;
        case 4:  monstre = {" Dragon de Givre (BOSS)", 800, 70, 250, 600}; break;
        default: monstre = {"Orc Guerrier", 90, 20, 20, 35}; break;
        }
        break;

    case 3: // DÉSERT MAUDIT
        switch (numeroAleatoire)
        {
        case 0:  monstre = {"Scorpion Geant", 120, 30, 30, 55}; break;
        case 1:  monstre = {"Momie", 150, 25, 23, 65}; break;
        case 2:  monstre = {"Anubis Dechu", 300, 45, 90, 120}; break;
        case 4:  monstre = {" Seigneur des Sables (BOSS)", 1200, 70, 300, 650}; break;
        default: monstre = {"Scorpion Geant", 120, 22, 30, 55}; break;
        }
        break;

    case 4: // ROYAUME DES OMBRES
        switch (numeroAleatoire)
        {
        case 0:  monstre = {"Chevalier Noir", 250, 60, 60, 150}; break;
        case 1:  monstre = {"Mage Noir", 140, 65, 70, 100}; break;
        case 2:  monstre = {"Demon Mineur", 300, 70, 80, 180}; break;
        case 3:  monstre = {"Faucheur", 400, 80, 150, 220}; break;
        case 4:  monstre = {"Empereur des Ombres (BOSS FINAL)", 2000, 100, 450, 600}; break;
        default: monstre = {"Chevalier Noir", 250, 40, 80, 150}; break;
        }
        break;

    case 5: // LE DOMAINES VAMPIRIQUE
        switch (numeroAleatoire)
        {
        case 0:  monstre = {"Goule", 250, 40, 40, 150}; break;
        case 1:  monstre = {"Vampire bas rang", 340, 60, 70, 100}; break;
        case 2:  monstre = {"Soldat Vampire", 370, 75, 100, 180}; break;
        case 3:  monstre = {"Vampire de rang Sup.", 400, 95, 150, 220}; break;
        case 4:  monstre = {"CONTE VAMPIRE (BOSS)", 2700, 120, 500, 750}; break;
        default: monstre = {"Chauve-souris Niv2", 25, 10, 10, 20}; break;
        }
        break;

    case 6: // FORET MAUDITE
        switch (numeroAleatoire)
        {
        case 0:  monstre = {"Fee malefique", 270, 55, 50, 160}; break;
        case 1:  monstre = {"Plante carnivore Geante", 350, 65, 75, 150}; break;
        case 2:  monstre = {"Les enfants de la foret (DANCER!!!)", 670, 175, 120, 270}; break;
        case 3:  monstre = {"Vampire de rang Sup.", 400, 95, 150, 220}; break;
        case 4:  monstre = {"REINE DES FEES MALEFIQUE (BOSS)", 2900, 320, 6500, 750}; break;
        default: monstre = {"Fleur mortel", 25, 10, 10, 20}; break;
        }
        break;

    default:
        monstre = {"Gobelin Perdu", 30, 5, 5, 10};
        break;
    }
}