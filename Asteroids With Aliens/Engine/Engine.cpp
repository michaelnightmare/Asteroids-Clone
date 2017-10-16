// Engine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameObjects.h"
#include <string>
#include <iostream>
#include <vector>

bool fullscreen = false;
int main()
{
	srand(static_cast<unsigned int>(time(0)));
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Asteroids!");		
	Game game;


	sf::Music cautionmusic;
	cautionmusic.openFromFile("Sounds/Music.ogg");
	cautionmusic.play();
	cautionmusic.setLoop(true);

	sf::Texture texture2;
	if (!texture2.loadFromFile("Sprites/Backgrounds/Purple1.png"))
	{
		std::cout << "texture wont load";
	}
	sf::Sprite background;
	background.setTexture(texture2);
	
	
	background.setTextureRect(sf::IntRect(100, 150, 1280, 720));


	

	game.RespawnPlayer();
		
	

		for (int i = 0; i < 5; i++)
		{
			
			Asteroid* newAsteroid;
			newAsteroid = new LargeAsteroid(sf::Vector2f(rand() % 1000 + 100, rand() % 600 + 50));
			newAsteroid->SetAngle(rand() % 360);
			newAsteroid->SetVelocity(150);
			game.AddObject(newAsteroid);
			
		}
	
		
	
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Return &&
					event.key.alt)
				{					
					window.close();
					fullscreen = !fullscreen;
					window.create(fullscreen ? sf::VideoMode(1920,1080) : sf::VideoMode(1280, 720), "Welcome to SDVA 103!", fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar);
				}
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}
		


		
		game.Update(&window, dt.asSeconds());
		game.Draw(&window);
		

		if (game.getcontinue())
		{
			for (int i = 0; i < 5 + game.getWave(); i++)
			{

				Asteroid* newAsteroid;
				
				if (rand() % 2 == 0)
				{
					newAsteroid = new SmallAsteroid(sf::Vector2f(rand() % 1000 + 100, rand() % 600 + 50));
				}
				else
				{
					newAsteroid = new LargeAsteroid(sf::Vector2f(rand() % 1000 + 100, rand() % 600 + 50));
				}
				newAsteroid->SetAngle(rand() % 360);
				newAsteroid->SetVelocity(150);
				game.AddObject(newAsteroid);
			
				
			}
			if (game.getWave() >= 1)
			{
				//if (rand() % 2 == 0)
				//{
					HealthPowerUp* newhealthpowerup;
					newhealthpowerup = new HealthPowerUp(sf::Vector2f(rand() % 1000 + 100, rand() % 600 + 50));
					game.AddObject(newhealthpowerup);
				//}
			/*	else
				{
					ClonePowerUp* newclonepowerup;
					newclonepowerup = new ClonePowerUp(sf::Vector2f(rand() % 1000 + 100, rand() % 600 + 50));
					game.AddObject(newclonepowerup);
				}*/
				
			}

			game.resetcontinue();
		}
		
		window.display();
		window.draw(background);	
	}

	return 0;
}
