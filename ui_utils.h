#ifndef UI_UTILS_H
#define UI_UTILS_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

namespace UI
{
    // Palette de couleurs
    const sf::Color Gold(212, 175, 55);
    const sf::Color GoldBright(255, 225, 115);
    const sf::Color DarkPanel(14, 18, 30, 235);
    const sf::Color DarkPanelLight(24, 30, 48, 220);
    const sf::Color Crimson(215, 45, 45);
    const sf::Color GreenHP(45, 185, 85);
    const sf::Color BlueMana(45, 135, 240);
    const sf::Color AmberXP(235, 165, 30);
    const sf::Color TextWhite(245, 245, 250);
    const sf::Color TextMuted(175, 180, 200);

    // Dessin d'une barre de progression (Vie, Mana, XP)
    inline void drawProgressBar(sf::RenderWindow &window, float x, float y, float width, float height,
                                int current, int maxVal, sf::Color fillColor, sf::Color emptyColor,
                                const std::string &label, sf::Font &font, unsigned int textSize = 16)
    {
        if (maxVal <= 0) maxVal = 1;
        float ratio = std::clamp(static_cast<float>(current) / static_cast<float>(maxVal), 0.0f, 1.0f);

        // Fond sombre
        sf::RectangleShape background(sf::Vector2f(width, height));
        background.setPosition(x, y);
        background.setFillColor(emptyColor);
        background.setOutlineThickness(2.0f);
        background.setOutlineColor(Gold);
        window.draw(background);

        // Remplissage
        if (ratio > 0.0f)
        {
            sf::RectangleShape fill(sf::Vector2f(width * ratio, height));
            fill.setPosition(x, y);
            fill.setFillColor(fillColor);
            window.draw(fill);
        }

        // Texte au centre
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

    // Dessin d'un bouton stylisé
    inline void drawButton(sf::RenderWindow &window, sf::RectangleShape &btn, sf::Text &txt, bool isHovered,
                           sf::Color normalFill = DarkPanelLight,
                           sf::Color hoverFill = Gold,
                           sf::Color normalText = TextWhite,
                           sf::Color hoverText = sf::Color::Black)
    {
        if (isHovered)
        {
            btn.setFillColor(hoverFill);
            btn.setOutlineColor(GoldBright);
            txt.setFillColor(hoverText);
        }
        else
        {
            btn.setFillColor(normalFill);
            btn.setOutlineColor(Gold);
            txt.setFillColor(normalText);
        }

        window.draw(btn);
        window.draw(txt);
    }

    // Dessin d'un panneau fantasy
    inline void drawPanel(sf::RenderWindow &window, float x, float y, float width, float height,
                          const std::string &title = "", sf::Font *font = nullptr, unsigned int titleSize = 22)
    {
        sf::RectangleShape panel(sf::Vector2f(width, height));
        panel.setPosition(x, y);
        panel.setFillColor(DarkPanel);
        panel.setOutlineThickness(2.0f);
        panel.setOutlineColor(Gold);
        window.draw(panel);

        if (!title.empty() && font != nullptr)
        {
            // Bannière de titre
            sf::RectangleShape banner(sf::Vector2f(width - 4.0f, 38.0f));
            banner.setPosition(x + 2.0f, y + 2.0f);
            banner.setFillColor(sf::Color(30, 36, 56, 200));
            window.draw(banner);

            sf::Text txtTitle(title, *font, titleSize);
            txtTitle.setFillColor(GoldBright);
            sf::FloatRect b = txtTitle.getLocalBounds();
            txtTitle.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
            txtTitle.setPosition(x + width / 2.0f, y + 21.0f);
            window.draw(txtTitle);

            // Ligne séparatrice
            sf::RectangleShape line(sf::Vector2f(width - 20.0f, 1.5f));
            line.setPosition(x + 10.0f, y + 42.0f);
            line.setFillColor(Gold);
            window.draw(line);
        }
    }

    // Système de textes flottants (dégâts, soins, alertes)
    struct FloatingText
    {
        std::string text;
        sf::Vector2f pos;
        sf::Color color;
        float timer = 1.0f; // secondes
        float maxTimer = 1.0f;
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
                it->pos.y -= 40.0f * dt; // monte lentement
                float alpha = (it->timer / it->maxTimer) * 255.0f;
                sf::Color c = it->color;
                c.a = static_cast<sf::Uint8>(std::clamp(alpha, 0.0f, 255.0f));

                sf::Text txt(it->text, font, 24);
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
