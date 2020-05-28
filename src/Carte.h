#pragma once

#include "Entite.h"
#include <unordered_map>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <iostream>


class Joueur : public Entite
{
	public:

		Joueur();

		void deplacer(int manette, int fps);
};


class Bonus : public Entite
{
	public:

		void avancer(float vitesse, int fps);
};


class Obstacle : public Entite
{
	public:

		void avancer(float vitesse, int fps);
};


class Carte : public Entite
{
	public:

		static int const nbEtapes;
		static int const nbImages;
		static std::string const nomsImages[];

		Carte(int numero, int manette);

		void etapeSuivante();
		void chargerTexturesCourantes();

		void afficher(sf::RenderWindow& window, sf::Shader* shader);
		void afficherScore(sf::RenderWindow& window);
		void afficherScoreFinal(sf::RenderWindow& window);

		void faireAvancer(int fps);
		void detecterCollisions();
		void genererDecors();
		void faireDefilerDecors(int fps);

	private:

		int m_etape;
		int m_numero;
		int m_manette;

		float m_score;

		float m_vitesse;
		sf::Clock m_generatrice;

		std::unordered_map<std::string, sf::Texture> m_textures;
		std::unordered_map<std::string, sf::Texture*> m_texturesCourantes;
		sf::Font m_font;

		Joueur m_joueur;
		std::vector<Bonus> m_bonus;
		std::vector<Obstacle> m_obstacles;
};
