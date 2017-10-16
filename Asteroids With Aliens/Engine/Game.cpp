#include "Game.h"
#include "GameObjects.h"
#include <iostream>
#include <sstream>




Game::Game()
	: m_gameOver(false)
	, m_timeUntilRespawn(2.0f)
	, m_livesRemaining(4)
	, m_healthRemaining(3)
	, wavelevel(0)
	, continueon(false)
	, wave(1)
{
	m_mainFont.loadFromFile("Fonts/kenpixel_high_square.ttf");
}
void Game::Draw(sf::RenderWindow * window)
{	
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		current->Draw(window);
	}

	for (int i = 0; i < 1; i++)
	{
		sf::Color Transparent;
		Transparent.a = 150;
		Transparent.r = 255;
		Transparent.b = 255;
		Transparent.g = 255;

		sf::Texture texture2;
		if (!texture2.loadFromFile("Sprites/PNG/Bomb.png"))
		{
			std::cout << "texture wont load";
		}
		sf::Sprite Bomb;
		Bomb.setTexture(texture2);
		Bomb.setColor(Transparent);
		Bomb.setScale(.4, .4);
		Bomb.setOrigin(-20, -200);
		Bomb.setPosition(sf::Vector2f(i * 60, 5));
		window->draw(Bomb);
	}

	for (int i = 0; i < 1; i++)
	{
		
		sf::Color Transparent;
		Transparent.a = 150;
		Transparent.r = 255;
		Transparent.b = 255;
		Transparent.g = 255;
		
		
		
		sf::Texture texture;
		if (!texture.loadFromFile("Sprites/PNG/playerShip2_red.png"))
		{
			std::cout << "texture wont load";
		}
		sf::Sprite Ship;
		Ship.setTexture(texture);
		Ship.setScale(.4,.4);
		Ship.setColor(Transparent);
		Ship.setOrigin(-25, -80);
		Ship.setPosition(sf::Vector2f(i * 60, 5));
		window->draw(Ship);

	}

	for (int i = 0; i < m_healthRemaining; i++)
	{

		sf::Texture texture3;
		if (!texture3.loadFromFile("Sprites/PNG/RedHealth.png"))
		{
			std::cout << "texture wont load";
		}
		sf::Sprite Health;
		Health.setTexture(texture3);
		Health.setScale(.7, .7);
		
		Health.setOrigin(-175, -5);
		Health.setPosition(sf::Vector2f(i * 30, 4));
		window->draw(Health);

	}



	if (m_gameOver)
	{
		sf::Text gameOverText;
		gameOverText.setFont(m_mainFont);
		gameOverText.setString("GAME OVER!");
		gameOverText.setCharacterSize(110);
		gameOverText.setPosition(450, 150);
		window->draw(gameOverText);
	}

	sf::Text ScoreText;
	ScoreText.setFont(m_mainFont);
	ScoreText.setString("Score:");
	ScoreText.setCharacterSize(50);
	ScoreText.setPosition(1050, 0);
	window->draw(ScoreText);


	sf::Text Score;
	Score.setFont(m_mainFont);
	Score.setString(std::to_string(score));
	Score.setCharacterSize(50);
	Score.setPosition(1150, 0);
	window->draw(Score);

	sf::Text Wave;
	Wave.setFont(m_mainFont);
	Wave.setString("Wave ");
	Wave.setCharacterSize(50);
	Wave.setPosition(512, 5);
	window->draw(Wave);


	sf::Text Waveno;
	Waveno.setFont(m_mainFont);
	Waveno.setString(std::to_string(wave));
	Waveno.setCharacterSize(50);
	Waveno.setPosition(610, 5);
	window->draw(Waveno);
	
	sf::Text X;
	X.setFont(m_mainFont);
	X.setString("x");
	X.setCharacterSize(30);
	X.setPosition(60, 80);
	window->draw(X);

	sf::Text X2;
	X2.setFont(m_mainFont);
	X2.setString("x");
	X2.setCharacterSize(30);
	X2.setPosition(60, 35);
	window->draw(X2);


	sf::Text Bombno;
	Bombno.setFont(m_mainFont);
	Bombno.setString(std::to_string(bomb));
	Bombno.setCharacterSize(50);
	Bombno.setPosition(75, 65);
	window->draw(Bombno);

	sf::Text Livesno;
	Livesno.setFont(m_mainFont);
	Livesno.setString(std::to_string(m_livesRemaining));
	Livesno.setCharacterSize(50);
	Livesno.setPosition(75, 20);
	window->draw(Livesno);
	
	sf::Text HealthText;
	HealthText.setFont(m_mainFont);
	HealthText.setString("Health [                   ]");
	HealthText.setCharacterSize(40);
	HealthText.setPosition(10, -10);
	window->draw(HealthText);
}

void Game::Update(sf::RenderWindow * window, float dt)
{
	if (m_timeUntilRespawn > 0)
	{
		m_timeUntilRespawn -= dt;
		if (m_timeUntilRespawn <= 0)
		{

			Player* player = new Player("Sprites/PNG/playerShip2_red.png", sf::Vector2f(600, 300));
			AddObject(player);

	
		}
	}

	/*if (clone == true)
	{
		
		for (int i = 0; i <= 1; i++)
		{
			Player* player2 = new Player("Sprites/PNG/playerShip3_blue.png", sf::Vector2f(600, 300));
			AddObject(player2);
		}
		clone = false;
	}
*/
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		current->Update(window, dt);
	}
	// See if any collisions occurred
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		for (int j = 0; j < m_gameObjects.size(); j++)
		{
			GameObject* other = m_gameObjects[j];
			if (current->IsColliding(other))
			{
				current->CollidedWith(other);
			}
		}
	}

	for (int i = m_gameObjects.size() - 1; i >= 0; i--)
	{
		GameObject* current = m_gameObjects[i];
		if (current->IsDestroyed())
		{
			delete current;
			m_gameObjects.erase(m_gameObjects.begin() + i);
		}
	}

	if (!continueon)
	{
		for (int i = m_gameObjects.size() - 1; i >= 0; i--)
		{
			GameObject * current = m_gameObjects[i];
		
			if (dynamic_cast<Player*>(current) != NULL && m_gameObjects.size() == 1)
			{
				
				std::cout << "no more Asteroids" << std::endl;
				wavelevel += 1;
				wave += 1;
				continueon = true;
				

			}
		}
	}
}

void Game::resetcontinue()
{
	continueon = false;
}

bool Game::getcontinue()
{
	return continueon;
}

int Game::getWave()
{
	return wavelevel;
}
void Game::AddObject(GameObject * object)
{
	object->SetOwner(this);
	m_gameObjects.push_back(object);
}

void Game::RespawnPlayer()
{
	if (m_livesRemaining > 0)
	{
		m_livesRemaining--;
		m_timeUntilRespawn = 2.0f;
		
	}
	
	else 
	{
		m_gameOver = true;
	}

}

void Game::updateScore(int s)
{
	score += s; 
}

void Game::updateWave(int w)
{
	wave += w;
}

void Game::updateBomb(int b)
{
	bomb += b;
}

sf::Vector2f Game::returnplayerpos()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
	
		GameObject* current = m_gameObjects[i];
		if (dynamic_cast<Player*>(current) != NULL)
		{
			return current->getPos();
		}
		
	}

	return sf::Vector2f(0,0);
}

ParticleSystem::ParticleSystem(float count, sf::Vector2f pos, sf::Color color, float size, int spread, float angle, float speed, int speedSpread)
	: GameObject("", pos)
{
	for (int i = 0; i < count; i++)
	{
		Particle newParticle;
		newParticle.m_pos = pos;
		newParticle.m_color = color;
		newParticle.m_size = size + (rand() % 10 - 5);
		newParticle.m_lifetime = 1 + (rand() % 5 / 10.0f);
		newParticle.m_initialLifetime = newParticle.m_lifetime;
		float randAngle = angle + (spread > 0 ? (rand() % spread) : 0);
		randAngle *= DEG_TO_RAD;
		float particleSpeed = speed + (speedSpread > 0 ? (rand() % speedSpread) : 0);
		newParticle.m_vel = sf::Vector2f(sin(randAngle) * particleSpeed, cos(randAngle) * particleSpeed);
		newParticle.m_accel = sf::Vector2f(0.5f * -sin(randAngle) * particleSpeed, -0.5f * cos(randAngle) * particleSpeed);

		m_particles.push_back(newParticle);
	}
}

void ParticleSystem::Draw(sf::RenderWindow * window)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		Particle& particle = m_particles[i];

		m_rectShape.setPosition(particle.m_pos);
		m_rectShape.setFillColor(particle.m_color);
		m_rectShape.setRadius(particle.m_size);
		m_rectShape.setPointCount(6);
		window->draw(m_rectShape);
	}
}

void ParticleSystem::Update(sf::RenderWindow * window, float dt)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		Particle& particle = m_particles[i];
		particle.m_vel += particle.m_accel * dt;
		particle.m_pos += particle.m_vel * dt;
		particle.m_lifetime -= dt;
		particle.m_color.a = 255 * particle.m_lifetime / particle.m_initialLifetime;

		if (particle.m_lifetime <= 0.0f)
		{
			m_particles.erase(m_particles.begin() + i);
			i--;
		}
	}

	if (m_particles.size() <= 0)
	{
		Destroy();
	}
}



