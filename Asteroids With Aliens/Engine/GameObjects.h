#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#define DEG_TO_RAD (0.0174532925f)

class Game;

class GameObject
{
public:
	GameObject(std::string texturePath, const sf::Vector2f& pos);
	virtual void Draw(sf::RenderWindow* window);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void LimitVelocity(float dt);
	virtual void ApplyDrag(float dt);
	void SetAngle(float angle);
	float GetAngle();
	void SetPos(const sf::Vector2f& pos);
	void SetAccel(float amount);
	void SetVelocity(float amount);
	void SetOwner(Game* owner);
	sf::Vector2f getPos();
	virtual void Destroy();
	bool IsDestroyed();
	void SetCollisionRadius(float val) { m_collisionRadius = val;  }
	float GetCollisionRadius() { return m_collisionRadius; }
	bool IsColliding(GameObject* other);
	virtual void CollidedWith(GameObject* other);
	sf::Sprite m_sprite;
		

protected:
	sf::Vector2f m_pos;
	sf::Vector2f m_vel;
	sf::Vector2f m_alien;
	sf::Vector2f m_accel;
	sf::Texture m_texture;

	Game* m_owner;
	float m_angle;
	float m_collisionRadius;
	bool m_destroyed;
};

class Bullet : public GameObject
{
public:
	Bullet(const sf::Vector2f& pos);	
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void ApplyDrag(float dt) {};
	virtual void CollidedWith(GameObject* other);
	float m_timeAlive;
	sf::SoundBuffer AExplosionBuffer;
	sf::Sound AExplosionSound;
};

class Bomb : public GameObject
{
public:
	Bomb(const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void ApplyDrag(float dt) {};
	virtual void CollidedWith(GameObject* other);
	float m_timeAlive;
	sf::SoundBuffer AExplosionBuffer;
	sf::Sound AExplosionSound;
};

class Player : public GameObject
{
public:
	Player(std::string texturePath, const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void CollidedWith(GameObject* other);
	void Draw(sf::RenderWindow* window);
	sf::SoundBuffer ShotBuffer;
	sf::Sound ShotSound;
	sf::SoundBuffer ExplosionBuffer;
	sf::Sound ExplosionSound;
	sf::SoundBuffer RespawnBuffer;
	sf::Sound RespawnSound;
	sf::SoundBuffer HurtBuffer;
	sf::Sound HurtSound;
	sf::SoundBuffer BombBuffer;
	sf::Sound BombSound;
	
private:
	bool m_firing;
	float m_fireCooldown;
	float m_invulnerable;
	
	
};


class Asteroid : public GameObject
{
public:
	Asteroid(std::string texturePath, const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void ApplyDrag(float dt) {};
private:
	float m_rotationRate;
	
	
};

class SmallAsteroid : public Asteroid
{
public:
	SmallAsteroid(const sf::Vector2f& pos);
	virtual void Destroy();
};

class MediumAsteroid : public Asteroid
{
public:
	
	MediumAsteroid(const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void Destroy();
	sf::SoundBuffer BatBuffer;
	sf::Sound BatSound;
	
};

class LargeAsteroid : public Asteroid
{
public:
	LargeAsteroid(const sf::Vector2f& pos);
	virtual void Destroy();
};

class Alien :public GameObject
{
public:
		Alien( const sf::Vector2f& pos);
		virtual void Update(sf::RenderWindow * window, float dt);
		virtual void Destroy();
		
private:
};

class Powerup : public GameObject
{
public:
	Powerup(std::string texturePath, const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void CollidedWith(GameObject* other);
private:
	float m_rotationRate;
	float m_timeAlive;
};

class BombPowerUp : public Powerup
{
public:
	BombPowerUp(const sf::Vector2f& pos);
	virtual void Destroy();
	sf::SoundBuffer PowerUpBuffer;
	sf::Sound PowerUpSound;
};

class HealthPowerUp : public Powerup
{
public:
	HealthPowerUp(const sf::Vector2f& pos);
	virtual void Destroy();
	sf::SoundBuffer PowerUpBuffer;
	sf::Sound PowerUpSound;
};

class ClonePowerUp : public Powerup
{
public:
	ClonePowerUp(const sf::Vector2f& pos);
	virtual void Destroy();
	sf::SoundBuffer PowerUpBuffer;
	sf::Sound PowerUpSound;
	
	
};