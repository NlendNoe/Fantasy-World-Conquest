#ifndef UI_UTILS_H
#define UI_UTILS_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

namespace UI
{
    const sf::Color Silver(235, 242, 255);
    const sf::Color Azure(90, 200, 255);
    const sf::Color DarkPanel(14, 18, 28, 235);
    const sf::Color DarkPanelLight(24, 32, 50, 225);
    const sf::Color BorderMuted(65, 80, 115);
    const sf::Color Crimson(215, 50, 50);
    const sf::Color GreenHP(45, 195, 95);
    const sf::Color BlueMana(45, 140, 245);
    const sf::Color AmberGold(245, 185, 55);
    const sf::Color TextWhite(250, 250, 255);
    const sf::Color TextMuted(180, 190, 215);

    inline void drawProgressBar(sf::RenderWindow &window, float x, float y, float width, float height,
                                int current, int maxVal, sf::Color fillColor, sf::Color emptyColor,
                                const std::string &label, sf::Font &font, unsigned int textSize = 16)
    {
        if (maxVal <= 0) maxVal = 1;
        float ratio = std::clamp(static_cast<float>(current) / static_cast<float>(maxVal), 0.0f, 1.0f);

        sf::RectangleShape background(sf::Vector2f(width, height));
        background.setPosition(x, y);
        background.setFillColor(emptyColor);
        background.setOutlineThickness(1.0f);
        background.setOutlineColor(BorderMuted);
        window.draw(background);

        if (ratio > 0.0f)
        {
            sf::RectangleShape fill(sf::Vector2f(width * ratio, height));
            fill.setPosition(x, y);
            fill.setFillColor(fillColor);
            window.draw(fill);
        }

        if (!label.empty())
        {
            sf::Text txt(label, font, textSize);
            txt.setFillColor(TextWhite);
            txt.setOutlineThickness(1.5f);
            txt.setOutlineColor(sf::Color::Black);
            sf::FloatRect bounds = txt.getLocalBounds();
            txt.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
            txt.setPosition(x + width / 2.0f, y + height / 2.0f);
            window.draw(txt);
        }
    }

    inline void drawButton(sf::RenderWindow &window, sf::RectangleShape &btn, sf::Text &txt, bool isHovered,
                           sf::Color normalFill = DarkPanelLight,
                           sf::Color hoverFill = sf::Color(45, 62, 95, 240),
                           sf::Color normalText = TextWhite,
                           sf::Color hoverText = Azure)
    {
        btn.setOutlineThickness(0.0f);
        if (isHovered)
        {
            btn.setFillColor(hoverFill);
            txt.setFillColor(hoverText);
        }
        else
        {
            btn.setFillColor(normalFill);
            txt.setFillColor(normalText);
        }

        window.draw(btn);
        window.draw(txt);
    }

    inline void drawPanel(sf::RenderWindow &window, float x, float y, float width, float height,
                          const std::string &title = "", sf::Font *font = nullptr, unsigned int titleSize = 22)
    {
        sf::RectangleShape panel(sf::Vector2f(width, height));
        panel.setPosition(x, y);
        panel.setFillColor(DarkPanel);
        panel.setOutlineThickness(1.5f);
        panel.setOutlineColor(BorderMuted);
        window.draw(panel);

        if (!title.empty() && font != nullptr)
        {
            sf::RectangleShape banner(sf::Vector2f(width - 4.0f, 38.0f));
            banner.setPosition(x + 2.0f, y + 2.0f);
            banner.setFillColor(sf::Color(26, 34, 52, 220));
            window.draw(banner);

            sf::Text txtTitle(title, *font, titleSize);
            txtTitle.setFillColor(Silver);
            sf::FloatRect b = txtTitle.getLocalBounds();
            txtTitle.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
            txtTitle.setPosition(x + width / 2.0f, y + 21.0f);
            window.draw(txtTitle);

            sf::RectangleShape line(sf::Vector2f(width - 20.0f, 1.0f));
            line.setPosition(x + 10.0f, y + 42.0f);
            line.setFillColor(BorderMuted);
            window.draw(line);
        }
    }

    inline std::string wrapText(const std::string &texte, const sf::Font &font, unsigned int taille, float largeurMax)
    {
        std::string resultat;
        std::string ligne;

        size_t i = 0;
        while (i < texte.size())
        {
            size_t j = i;
            while (j < texte.size() && texte[j] != ' ' && texte[j] != '\n') ++j;
            std::string mot = texte.substr(i, j - i);

            std::string essai = ligne.empty() ? mot : ligne + " " + mot;
            float largeur = sf::Text(essai, font, taille).getLocalBounds().width;

            if (largeur > largeurMax && !ligne.empty())
            {
                resultat += ligne + "\n";
                ligne = mot;
            }
            else
            {
                ligne = essai;
            }

            if (j < texte.size() && texte[j] == '\n')
            {
                resultat += ligne + "\n";
                ligne.clear();
            }
            i = j + 1;
        }
        resultat += ligne;
        return resultat;
    }

    struct FloatingText
    {
        std::string text;
        sf::Vector2f pos;
        sf::Color color;
        float timer = 1.0f;
        float maxTimer = 1.0f;
        unsigned int taille = 30; // taille du texte flottant
    };

    inline void updateAndDrawFloatingTexts(sf::RenderWindow &window, sf::Font &font,
                                          std::vector<FloatingText> &floatingTexts, float dt)
    {
        for (auto it = floatingTexts.begin(); it != floatingTexts.end();)
        {
            it->timer -= dt;
            if (it->timer <= 0.0f)
            {
                it = floatingTexts.erase(it);
            }
            else
            {
                it->pos.y -= 40.0f * dt;
                float alpha = (it->timer / it->maxTimer) * 255.0f;
                sf::Color c = it->color;
                c.a = static_cast<sf::Uint8>(std::clamp(alpha, 0.0f, 255.0f));

                sf::Text txt(it->text, font, it->taille);
                txt.setFillColor(c);
                txt.setOutlineThickness(2.0f);
                txt.setOutlineColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));
                sf::FloatRect b = txt.getLocalBounds();
                txt.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
                txt.setPosition(it->pos);

                window.draw(txt);
                ++it;
            }
        }
    }
}

#endif
