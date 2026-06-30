#include <iostream>
#include <string>

using namespace std;

void genererMonstre(int zone, int numeroAleatoire, string &nomMonstre, int &vieMonstre, int &attaqueMonstre, int &orRecompense, int &xpRecompense)
{
    switch (zone)
    {
    case 1: // FORÊT DES ANCIENS
        switch (numeroAleatoire)
        {
        case 0:
            nomMonstre = "Gobelin";
            vieMonstre = 30;
            attaqueMonstre = 5;
            orRecompense = 5;
            xpRecompense = 10;
            break;
        case 1:
            nomMonstre = "Loup Sauvage";
            vieMonstre = 45;
            attaqueMonstre = 8;
            orRecompense = 8;
            xpRecompense = 15;
            break;
        case 2:
            nomMonstre = "Squelette";
            vieMonstre = 60;
            attaqueMonstre = 10;
            orRecompense = 12;
            xpRecompense = 20;
            break;
        case 3:
            nomMonstre = "Araignee Geante";
            vieMonstre = 50;
            attaqueMonstre = 12;
            orRecompense = 10;
            xpRecompense = 18;
            break;
        case 4:
            nomMonstre = " Roi Gobelin (BOSS)";
            vieMonstre = 350;
            attaqueMonstre = 30;
            orRecompense = 200;
            xpRecompense = 250;
            break;
        default:
            nomMonstre = "Gobelin";
            vieMonstre = 30;
            attaqueMonstre = 5;
            orRecompense = 5;
            xpRecompense = 10;
            break;
        }
        break;

    case 2: // MONTAGNES DU DESTIN
        switch (numeroAleatoire)
        {
        case 0:
            nomMonstre = "Orc Guerrier";
            vieMonstre = 90;
            attaqueMonstre = 15;
            orRecompense = 20;
            xpRecompense = 35;
            break;
        case 1:
            nomMonstre = "Troll des Cavernes";
            vieMonstre = 180;
            attaqueMonstre = 25;
            orRecompense = 50;
            xpRecompense = 80;
            break;
        case 2:
            nomMonstre = "Harpie";
            vieMonstre = 110;
            attaqueMonstre = 18;
            orRecompense = 25;
            xpRecompense = 40;
            break;
        case 3:
            nomMonstre = "Golem de Pierre";
            vieMonstre = 250;
            attaqueMonstre = 20;
            orRecompense = 60;
            xpRecompense = 90;
            break;
        case 4:
            nomMonstre = " Dragon de Givre (BOSS)";
            vieMonstre = 800;
            attaqueMonstre = 55;
            orRecompense = 500;
            xpRecompense = 600;
            break;
        default:
            nomMonstre = "Orc Guerrier";
            vieMonstre = 90;
            attaqueMonstre = 15;
            orRecompense = 20;
            xpRecompense = 35;
            break;
        }
        break;

    case 3: // DÉSERT MAUDIT
        switch (numeroAleatoire)
        {
        case 0:
            nomMonstre = "Scorpion Geant";
            vieMonstre = 120;
            attaqueMonstre = 22;
            orRecompense = 30;
            xpRecompense = 55;
            break;
        case 1:
            nomMonstre = "Momie";
            vieMonstre = 150;
            attaqueMonstre = 25;
            orRecompense = 40;
            xpRecompense = 65;
            break;
        case 2:
            nomMonstre = "Anubis Dechu";
            vieMonstre = 300;
            attaqueMonstre = 35;
            orRecompense = 90;
            xpRecompense = 120;
            break;
        case 4:
            nomMonstre = " Seigneur des Sables (BOSS)";
            vieMonstre = 1200;
            attaqueMonstre = 70;
            orRecompense = 700;
            xpRecompense = 900;
            break;
        default:
            nomMonstre = "Scorpion Geant";
            vieMonstre = 120;
            attaqueMonstre = 22;
            orRecompense = 30;
            xpRecompense = 55;
            break;
        }
        break;

    case 4: // ROYAUME DES OMBRES
        switch (numeroAleatoire)
        {
        case 0:
            nomMonstre = "Chevalier Noir";
            vieMonstre = 250;
            attaqueMonstre = 40;
            orRecompense = 80;
            xpRecompense = 150;
            break;
        case 1:
            nomMonstre = "Mage Noir";
            vieMonstre = 140;
            attaqueMonstre = 30;
            orRecompense = 70;
            xpRecompense = 100;
            break;
        case 2:
            nomMonstre = "Demon Mineur";
            vieMonstre = 300;
            attaqueMonstre = 45;
            orRecompense = 100;
            xpRecompense = 180;
            break;
        case 3:
            nomMonstre = "Faucheur";
            vieMonstre = 400;
            attaqueMonstre = 55;
            orRecompense = 150;
            xpRecompense = 220;
            break;
        case 4:
            nomMonstre = "Empereur des Ombres (BOSS FINAL)";
            vieMonstre = 2000;
            attaqueMonstre = 90;
            orRecompense = 5000;
            xpRecompense = 2000;
            break;
        default:
            nomMonstre = "Chevalier Noir";
            vieMonstre = 250;
            attaqueMonstre = 40;
            orRecompense = 80;
            xpRecompense = 150;
            break;
        }
        break;

    default:
        nomMonstre = "Gobelin Perdu";
        vieMonstre = 30;
        attaqueMonstre = 5;
        orRecompense = 5;
        xpRecompense = 10;
        break;
    }
}