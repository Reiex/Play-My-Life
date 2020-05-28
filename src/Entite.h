#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Entite
{
    public:

        Entite();
        Entite(float x, float y, float w = 0, float h = 0);
		Entite(sf::View const& view);

        void setCoord(float x, float y, float w = 0, float h = 0);

        bool collision(Entite const& entite) const;
        bool collisionSouris(sf::RenderWindow const& window) const;

		int getX() const;
		int getY() const;
		int getW() const;
		int getH() const;

    protected:

		float m_x;
		float m_y;
		float m_w;
		float m_h;
};

