#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace AudioJeu
{
    static sf::Music musiqueCourante;
    static std::string titreMusique = "";

    // Cache des effets sonores : chaque fichier n'est lu qu'une fois du disque.
    static std::map<std::string, sf::SoundBuffer> &tamponsSons()
    {
        static std::map<std::string, sf::SoundBuffer> tampons;
        return tampons;
    }

    // Pool de canaux : plusieurs effets peuvent se superposer (musique + coups...).
    static std::vector<sf::Sound> &poolSons()
    {
        static std::vector<sf::Sound> pool(10);
        return pool;
    }

    inline void jouerEffet(const std::string &nom, float volume = 60.0f)
    {
        std::string chemin = "assets/sounds/" + nom + ".wav";
        auto &tampons = tamponsSons();
        if (tampons.find(chemin) == tampons.end())
        {
            sf::SoundBuffer tampon;
            if (!tampon.loadFromFile(chemin))
            {
                std::cout << "[Audio] Effet introuvable: " << chemin << std::endl;
                return;
            }
            tampons[chemin] = tampon;
        }

        auto &pool = poolSons();
        for (auto &son : pool)
        {
            if (son.getStatus() != sf::Sound::Playing)
            {
                son.setBuffer(tampons[chemin]);
                son.setVolume(volume);
                son.play();
                return;
            }
        }
        // Tous les canaux sont occupes : on reutilise le premier.
        pool[0].setBuffer(tampons[chemin]);
        pool[0].setVolume(volume);
        pool[0].play();
    }

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
