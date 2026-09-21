#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

namespace AudioJeu
{
    static sf::Music musiqueCourante;
    static std::string titreMusique = "";

    inline void jouer(const std::string &chemin, float volume = 30.0f, bool loop = true)
    {
        if (titreMusique == chemin && musiqueCourante.getStatus() == sf::Music::Playing)
            return;
        
        musiqueCourante.stop();
        if (musiqueCourante.openFromFile(chemin))
        {
            titreMusique = chemin;
            musiqueCourante.setLoop(loop);
            musiqueCourante.setVolume(volume);
            musiqueCourante.play();
        }
        else
        {
            std::cout << "[Audio] Impossible d'ouvrir le fichier audio: " << chemin << std::endl;
        }
    }

    inline void arreter()
    {
        musiqueCourante.stop();
        titreMusique = "";
    }
}

#endif
