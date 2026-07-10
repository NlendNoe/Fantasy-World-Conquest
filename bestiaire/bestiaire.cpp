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
            attaqueMonstre = 40;
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
            attaqueMonstre = 25;
            orRecompense = 20;
            xpRecompense = 35;
            break;
        case 1:
            nomMonstre = "Troll des Cavernes";
            vieMonstre = 180;
            attaqueMonstre = 35;
            orRecompense = 35;
            xpRecompense = 80;
            break;
        case 2:
            nomMonstre = "Harpie";
            vieMonstre = 110;
            attaqueMonstre = 25;
            orRecompense = 25;
            xpRecompense = 40;
            break;
        case 3:
            nomMonstre = "Golem de Pierre";
            vieMonstre = 250;
            attaqueMonstre = 40;
            orRecompense = 60;
            xpRecompense = 90;
            break;
        case 4:
            nomMonstre = " Dragon de Givre (BOSS)";
            vieMonstre = 800;
            attaqueMonstre = 70;
            orRecompense = 250;
            xpRecompense = 600;
            break;
        default:
            nomMonstre = "Orc Guerrier";
            vieMonstre = 90;
            attaqueMonstre = 20;
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
            attaqueMonstre = 30;
            orRecompense = 30;
            xpRecompense = 55;
            break;
        case 1:
            nomMonstre = "Momie";
            vieMonstre = 150;
            attaqueMonstre = 25;
            orRecompense = 23;
            xpRecompense = 65;
            break;
        case 2:
            nomMonstre = "Anubis Dechu";
            vieMonstre = 300;
            attaqueMonstre = 45;
            orRecompense = 90;
            xpRecompense = 120;
            break;
        case 4:
            nomMonstre = " Seigneur des Sables (BOSS)";
            vieMonstre = 1200;
            attaqueMonstre = 70;
            orRecompense = 300;
            xpRecompense = 650;
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
            attaqueMonstre = 60;
            orRecompense = 60;
            xpRecompense = 150;
            break;
        case 1:
            nomMonstre = "Mage Noir";
            vieMonstre = 140;
            attaqueMonstre = 65;
            orRecompense = 70;
            xpRecompense = 100;
            break;
        case 2:
            nomMonstre = "Demon Mineur";
            vieMonstre = 300;
            attaqueMonstre = 70;
            orRecompense = 80;
            xpRecompense = 180;
            break;
        case 3:
            nomMonstre = "Faucheur";
            vieMonstre = 400;
            attaqueMonstre = 80;
            orRecompense = 150;
            xpRecompense = 220;
            break;
        case 4:
            nomMonstre = "Empereur des Ombres (BOSS FINAL)";
            vieMonstre = 2000;
            attaqueMonstre = 100;
            orRecompense = 450;
            xpRecompense = 600;
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

    case 5: // LE DOMAINES VAMPIRIQUE
        switch (numeroAleatoire)
        {
        case 0:
            nomMonstre = "Goule";
            vieMonstre = 250;
            attaqueMonstre = 40;
            orRecompense = 40;
            xpRecompense = 150;
            break;
        case 1:
            nomMonstre = "Vampire bas rang";
            vieMonstre = 340;
            attaqueMonstre = 60;
            orRecompense = 70;
            xpRecompense = 100;
            break;
        case 2:
            nomMonstre = "Soldat Vampire";
            vieMonstre = 370;
            attaqueMonstre = 75;
            orRecompense = 100;
            xpRecompense = 180;
            break;
        case 3:
            nomMonstre = "Vampire de rang Sup.";
            vieMonstre = 400;
            attaqueMonstre = 95;
            orRecompense = 150;
            xpRecompense = 220;
            break;
        case 4:
            nomMonstre = "CONTE VAMPIRE (BOSS)";
            vieMonstre = 2700;
            attaqueMonstre = 120;
            orRecompense = 500;
            xpRecompense = 750;
            break;
        default:
            nomMonstre = "Chauve-souris Niv2";
            vieMonstre = 25;
            attaqueMonstre = 10;
            orRecompense = 10;
            xpRecompense = 20;
            break;
        }
        break;
        case 6: // FORET MAUDITE
        switch (numeroAleatoire)
        {
        case 0:
            nomMonstre = "Fee malefique";
            vieMonstre = 270;
            attaqueMonstre = 55;
            orRecompense = 50;
            xpRecompense = 160;
            break;
        case 1:
            nomMonstre = "Plante carnivore Geante";
            vieMonstre = 350;
            attaqueMonstre = 65;
            orRecompense = 75;
            xpRecompense = 150;
            break;
        case 2:
            nomMonstre = "Les enfants de la foret (DANCER!!!)";
            vieMonstre = 670;
            attaqueMonstre = 175;
            orRecompense = 120;
            xpRecompense = 270;
            break;
        case 3:
            nomMonstre = "Vampire de rang Sup.";
            vieMonstre = 400;
            attaqueMonstre = 95;
            orRecompense = 150;
            xpRecompense = 220;
            break;
        case 4:
            nomMonstre = "REINE DES FEES MALEFIQUE (BOSS)";
            vieMonstre = 2900;
            attaqueMonstre = 320;
            orRecompense = 6500;
            xpRecompense = 750;
            break;
        default:
            nomMonstre = "Fleur mortel";
            vieMonstre = 25;
            attaqueMonstre = 10;
            orRecompense = 10;
            xpRecompense = 20;
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