#include "Carte.h"


// METHODES DE LA CLASSE JOUEUR


Joueur::Joueur()
{
	m_x = 319;
	m_y = 1000;
	m_w = 2;
	m_h = 2;
}


void Joueur::deplacer(int manette, int fps)
{
	float vitesse(sf::Joystick::getAxisPosition(manette, sf::Joystick::Axis::X));
	if (vitesse > 10 || vitesse < -10)
	{
		m_x += 10 * vitesse / fps;
	}
	vitesse = sf::Joystick::getAxisPosition(manette, sf::Joystick::Axis::Y);

	if (vitesse > 10 || vitesse < -10)
	{
		m_y += 10 * vitesse / fps;
	}

	if (m_x < 31)
	{
		m_x = 31;
	}
	else if (m_x > 640-31)
	{
		m_x = 640-31;
	}

	if (m_y < 31)
	{
		m_y = 31;
	}
	else if (m_y > 1080 - 31)
	{
		m_y = 1080 - 31;
	}
}


// METHODES DE LA CLASSE BONUS


void Bonus::avancer(float vitesse, int fps)
{
	m_y += vitesse / fps;
}


// METHODES DE LA CLASSE OBSTACLE


void Obstacle::avancer(float vitesse, int fps)
{
	m_y += vitesse / fps;
}


// METHODES DE LA CLASSE CARTE


int const Carte::nbEtapes(6);
int const Carte::nbImages(4);
std::string const Carte::nomsImages[Carte::nbImages] = {
	"/Joueur/Joueur.png",
	"/Obstacle/Obstacle.png",
	"/Bonus/Bonus.png",
	"/fond.png"
};


Carte::Carte(int numero, int manette)
{
	m_numero = numero;
	m_manette = manette;

	m_x = 640*numero;
	m_y = 0;

	m_score = 0;
	m_vitesse = 500;
	m_etape = 0;

	for (int i(0); i < nbEtapes; i++)
	{
		std::string nomEtape("images/etape ");
		nomEtape += std::to_string(i);

		for (int j(0); j < nbImages; j++)
		{
			std::string chemin(nomEtape + nomsImages[j]);
			m_textures[chemin] = sf::Texture();
			m_textures[chemin].loadFromFile(chemin);
		}
	}

	chargerTexturesCourantes();

	m_font.loadFromFile("ressources/abel.ttf");
}


void Carte::etapeSuivante()
{
	m_etape += 1;
	chargerTexturesCourantes();
	if (m_etape == 4)
	{
		m_font.loadFromFile("ressources/space age.ttf");
	}
}


void Carte::chargerTexturesCourantes()
{
	std::string nomEtape("images/etape ");
	nomEtape += std::to_string(m_etape);

	for (int i(0); i < nbImages; i++)
	{
		std::string chemin(nomEtape + nomsImages[i]);
		m_texturesCourantes[nomsImages[i]] = &m_textures[chemin];
	}
}


void Carte::afficher(sf::RenderWindow& window, sf::Shader* shader)
{
	sf::Sprite sprite;

	// Afficher le fond

	sprite = sf::Sprite(*m_texturesCourantes["/fond.png"]);
	sprite.setPosition(m_x, m_y);
	window.draw(sprite);

	// Afficher le joueur

	sprite = sf::Sprite(*m_texturesCourantes["/Joueur/Joueur.png"]);
	sprite.setPosition(640 * m_numero + m_joueur.getX()-31, m_joueur.getY()-31);
	window.draw(sprite);

	// Afficher les obstacles et les bonus

	sf::RenderTexture textureObjets;
	textureObjets.create(640, 1080);
	textureObjets.clear(sf::Color(0, 0, 0));

	sprite = sf::Sprite(*m_texturesCourantes["/Obstacle/Obstacle.png"]);
	for (int i(0); i < m_obstacles.size(); i++)
	{
		sprite.setPosition(m_obstacles[i].getX(), m_obstacles[i].getY());
		textureObjets.draw(sprite);
	}

	sprite = sf::Sprite(*m_texturesCourantes["/Bonus/Bonus.png"]);
	for (int i(0); i < m_bonus.size(); i++)
	{
		sprite.setPosition(m_bonus[i].getX(), m_bonus[i].getY());
		textureObjets.draw(sprite);
	}

	textureObjets.display();
	sf::Texture texture(textureObjets.getTexture());
	sprite = sf::Sprite(texture);
	sprite.setPosition(m_numero * 640, 0);

	if (m_etape == 5)
	{
		int taille(m_obstacles.size());
		sf::Glsl::Vec2* obstacles = (sf::Glsl::Vec2*) malloc(sizeof(sf::Glsl::Vec2)*taille);
		for (int i(0); i < taille; i++)
		{
			obstacles[i] = sf::Glsl::Vec2(float(m_obstacles[i].getX())/640, float(m_obstacles[i].getY())/1080);
		}

		shader->setUniform("nbObstacles", taille);
		shader->setUniformArray("obstacles", obstacles, taille);

		taille = m_bonus.size();
		sf::Glsl::Vec2* bonus = (sf::Glsl::Vec2*) malloc(sizeof(sf::Glsl::Vec2)*taille);
		for (int i(0); i < taille; i++)
		{
			bonus[i] = sf::Glsl::Vec2(float(m_bonus[i].getX())/640, float(m_bonus[i].getY())/1080);
		}

		shader->setUniform("nbBonus", taille);
		shader->setUniformArray("bonus", obstacles, taille);

		free(obstacles);
		free(bonus);
	}

	window.draw(sprite, shader);

	// Afficher le score

	afficherScore(window);
}


void Carte::afficherScore(sf::RenderWindow& window)
{
	sf::Text ligne(sf::String(std::to_string(int(m_score))), m_font, 100);
	if (m_etape >= 4)
	{
		ligne.setPosition(m_numero * 640 + 10, -20);
		ligne.setFillColor(sf::Color(255, 255, 0));
		ligne.setOutlineColor(sf::Color(50, 50, 50));
		ligne.setOutlineThickness(10);
	}
	else
	{
		ligne.setPosition(m_numero * 640 + 10, -10);
		ligne.setFillColor(sf::Color(0, 0, 0));
	}
	window.draw(ligne);
}


void Carte::afficherScoreFinal(sf::RenderWindow& window)
{
	sf::Text ligne(sf::String(std::to_string(int(m_score))), m_font, 100);
	ligne.setPosition(m_numero * 640 + (640 - ligne.getGlobalBounds().width)/2, 500);
	ligne.setFillColor(sf::Color(255, 255, 0));
	ligne.setOutlineColor(sf::Color(50, 50, 50));
	ligne.setOutlineThickness(10);
	window.draw(ligne);
}


void Carte::faireAvancer(int fps)
{
	if (m_etape >= 1)
	{
		m_vitesse += 1;
		m_score += m_vitesse / (100*fps);
		m_joueur.deplacer(m_manette, fps);
		detecterCollisions();
		genererDecors();
		faireDefilerDecors(fps);
	}
	else
	{
		m_joueur.deplacer(m_manette, fps);
	}
}


void Carte::detecterCollisions()
{
	for (int i(0); i < m_obstacles.size(); i++)
	{
		if (m_joueur.collision(m_obstacles[i]))
		{
			m_obstacles.erase(m_obstacles.begin() + i);
			m_vitesse /= 2;
			i--;
		}
	}

	for (int i(0); i < m_bonus.size(); i++)
	{
		if (m_joueur.collision(m_bonus[i]))
		{
			m_bonus.erase(m_bonus.begin() + i);
			m_score += 30;
			i--;
		}
	}
}


void Carte::genererDecors()
{
	float p(erf(m_generatrice.getElapsedTime().asSeconds() - pow(500.0/m_vitesse, 2)));
	float r(rand());

	if (r / RAND_MAX < p)
	{
		if (rand() % 5 == 0)
		{
			m_bonus.push_back(Bonus());
			m_bonus[m_bonus.size() - 1].setCoord((rand() % 5) * 128, -128, 128, 128);
		}
		else
		{
			m_obstacles.push_back(Obstacle());
			m_obstacles[m_obstacles.size() - 1].setCoord((rand() % 5) * 128, -128, 128, 128);
		}

		m_generatrice.restart();
	}
}


void Carte::faireDefilerDecors(int fps)
{
	for (int i(0); i < m_obstacles.size(); i++)
	{
		m_obstacles[i].avancer(m_vitesse, fps);

		if (m_obstacles[i].getY() >= 1080)
		{
			m_obstacles.erase(m_obstacles.begin() + i);
			i--;
		}
	}

	for (int i(0); i < m_bonus.size(); i++)
	{
		m_bonus[i].avancer(m_vitesse, fps);

		if (m_bonus[i].getY() >= 1080)
		{
			m_bonus.erase(m_bonus.begin() + i);
			i--;
		}
	}
}
