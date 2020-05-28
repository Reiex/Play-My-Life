#include "Carte.h"


void gererFPS(sf::Clock& clock, int fps)
{
	while (clock.getElapsedTime().asSeconds() < 1.0 / fps)
	{
		continue;
	}

	clock.restart();
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Play my Life", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);

	Carte carte[3] = {
		Carte(1, 0),
		Carte(0, 1),
		Carte(2, 2)
	};
	int etape(0);
	int fps(60);

	sf::Shader shaderNormal, shaderAvance;
	shaderNormal.loadFromFile("ressources/normal.frag", sf::Shader::Fragment);
	shaderAvance.loadFromFile("ressources/avance.frag", sf::Shader::Fragment);

	sf::Event event;
	sf::Clock clock;
	bool continuerJeu(true);

	while (continuerJeu)
	{
		window.display();
		gererFPS(clock, fps);
		window.clear(sf::Color(0, 0, 0));

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::KeyReleased:

					if (event.key.code == sf::Keyboard::Escape)
					{
						return 0;
					}
					else if (event.key.code == sf::Keyboard::Space)
					{
						etape += 1;
						if (etape < Carte::nbEtapes)
						{
							for (int i(0); i < 3; i++)
							{
								carte[i].etapeSuivante();
							}
						}
						else
						{
							continuerJeu = false;
						}

						if (etape == 2)
						{
							fps = 10;
						}
						else
						{
							fps = 60;
						}
					}

					break;
			}
		}

		for (int i(0); i < 3; i++)
		{
			if (sf::Joystick::isConnected(i))
			{
				if (etape <= 4)
				{
					carte[i].afficher(window, &shaderNormal);
				}
				else
				{
					carte[i].afficher(window, &shaderAvance);
				}
				carte[i].faireAvancer(fps);
			}
		}
	}

	window.clear(sf::Color(0, 0, 0));
	for (int i(0); i < 3; i++)
	{
		carte[i].afficherScoreFinal(window);
	}
	window.display();

	bool continuerPause(true);
	while (continuerPause)
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:

				if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Space)
				{
					continuerPause = false;
				}
			}
		}
	}

	return 0;
}