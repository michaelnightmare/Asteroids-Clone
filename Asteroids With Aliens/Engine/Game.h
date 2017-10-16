#pragma once
#include <SFML/Graphics.hpp>
#include "GameObjects.h"

class Game
{
public:
	Game();
	void Draw(sf::RenderWindow* window);
	void Update(sf::RenderWindow * window, float dt);
	void AddObject(GameObject* object);
	void RespawnPlayer();
	void GameOver() { m_gameOver = true;  }
	void updateScore(int s);
	void updateBomb(int b);
	void resetcontinue();
	int getWave();
	bool getcontinue();
	void updateWave(int w);
	sf::Vector2f returnplayerpos();
	int bomb;
	int m_healthRemaining;
	bool clone = false;
	
private:
	std::vector<GameObject*> m_gameObjects;
	bool m_gameOver;
	int m_livesRemaining;
	float m_timeUntilRespawn;
	int score;
	int wavelevel;
	bool continueon;
	int wave;
	

	sf::Font m_mainFont;
};

class Particle
{
public:
	Particle()
		: m_pos(0.0f, 0.0f)
		, m_vel(0.0f, 0.0f)
		, m_accel(0.0f, 0.0f)
	{

	}
	sf::Vector2f m_pos;
	sf::Vector2f m_vel;
	sf::Vector2f m_accel;
	sf::Color m_color;
	float m_size;
	float m_lifetime;
	float m_initialLifetime;
};
class ParticleSystem : public GameObject
{
public:
	ParticleSystem(float count, sf::Vector2f pos, sf::Color color, float size, int spread, float angle, float speed, int speedSpread);
	void Draw(sf::RenderWindow* window);
	void Update(sf::RenderWindow * window, float dt);
	std::vector<Particle> m_particles;

	sf::CircleShape m_rectShape;
};