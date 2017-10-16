#include "stdafx.h"
#include "Game.h"
#include "GameObjects.h"
#include <iostream>

GameObject::GameObject(std::string texturePath, const sf::Vector2f & pos)
	: m_pos(pos)
	, m_destroyed(false)
	, m_collisionRadius(0)
	
{
	// Load the texture
	// Assign the sprite		
	if (texturePath != "")
	{
		m_texture.loadFromFile(texturePath);
		m_sprite.setTexture(m_texture);
		m_sprite.setOrigin(m_sprite.getTextureRect().width * 0.5f, m_sprite.getTextureRect().height * 0.5f);
	}
}

void GameObject::Destroy()
{
	m_destroyed = true;
}

bool GameObject::IsDestroyed()
{
	return m_destroyed;
}

bool GameObject::IsColliding(GameObject* other)
{
	bool result = false;
	sf::Vector2f vecToOther = other->m_pos - m_pos;
	float dist = sqrt(vecToOther.x * vecToOther.x + vecToOther.y  *vecToOther.y);
	if (this != other &&
		!IsDestroyed() &&
		!other->IsDestroyed() &&
		dist < (m_collisionRadius + other->m_collisionRadius))
	{
		result = true;
	}
	return result;
}

void GameObject::CollidedWith(GameObject * other)
{
}

void GameObject::Draw(sf::RenderWindow * window)
{
	
	sf::CircleShape collisionRadius;
	collisionRadius.setRadius(m_collisionRadius);
	collisionRadius.setOutlineThickness(2);
	collisionRadius.setFillColor(sf::Color::Transparent);
	collisionRadius.setPosition(m_pos);
	collisionRadius.setOrigin(m_collisionRadius, m_collisionRadius);
	collisionRadius.setOutlineColor(sf::Color::Red);
	//window->draw(collisionRadius);
	window->draw(m_sprite);
}

void GameObject::Update(sf::RenderWindow * window, float dt)
{
	float decelScalar = 0.5f;
	m_vel += m_accel * dt;
	
	LimitVelocity(dt);
	ApplyDrag(dt);

	m_pos += m_vel * dt;
	if (m_pos.x < 0)
	{
		m_pos.x = window->getSize().x;
	}
	else if (m_pos.x > window->getSize().x)
	{
		m_pos.x = 0;
	}

	if (m_pos.y < 0)
	{
		m_pos.y = window->getSize().y;
	}
	else if (m_pos.y > window->getSize().y)
	{
		m_pos.y = 0;
	}


	m_sprite.setRotation(m_angle);
	m_sprite.setPosition(m_pos);
}

void GameObject::LimitVelocity(float dt)
{
	float speed = sqrt(m_vel.x * m_vel.x + m_vel.y  *m_vel.y);	

	if (speed <= 0.1)
	{
		return;
	}

	sf::Vector2f normalizedVel = sf::Vector2f(m_vel.x / speed, m_vel.y / speed);

	if (speed > 500)
	{
		speed = 500;
	}

	m_vel.x = normalizedVel.x * speed;
	m_vel.y = normalizedVel.y * speed;
}

void GameObject::SetAngle(float angle)
{
	m_angle = angle;
}

float GameObject::GetAngle()
{
	return m_angle;
}

void GameObject::SetPos(const sf::Vector2f & pos)
{
	m_pos = pos;
}

sf::Vector2f GameObject::getPos()
{
	return m_pos;
}

void GameObject::SetAccel(float amount)
{	
	if (amount > 0)
	{
		float rotInRadians = DEG_TO_RAD * m_angle;
		m_accel = sf::Vector2f(amount * sin(rotInRadians), -amount * cos(rotInRadians));
	}
	else
	{
		m_accel = sf::Vector2f(0, 0);
	}
}

void GameObject::SetVelocity(float amount)
{
	if (amount > 0)
	{
		float rotInRadians = DEG_TO_RAD * m_angle;
		m_vel = sf::Vector2f(amount * sin(rotInRadians), -amount * cos(rotInRadians));
	}
	else
	{
		m_vel = sf::Vector2f(0, 0);
	}
}

void GameObject::SetOwner(Game * owner)
{
	m_owner = owner;
}

void GameObject::ApplyDrag(float dt)
{
	if (m_accel.x == 0 && m_accel.y == 0)
	{
		float dragAmount = dt * 0.9f;
		m_vel.x -= dragAmount* m_vel.x;
		m_vel.y -= dragAmount * m_vel.y;
	}
}

Bullet::Bullet (const sf::Vector2f & pos)
	: GameObject("Sprites/PNG/Lasers/laserBlue08.png", pos)
	, m_timeAlive(0.0f)
{
	m_sprite.setScale(0.5, 0.5);
	SetCollisionRadius(20);
	AExplosionSound.setBuffer(AExplosionBuffer);
	if (!AExplosionBuffer.loadFromFile("Sounds/AExplosion.wav"))
	{
		std::cout << "error";
	}
	AExplosionSound.setBuffer(AExplosionBuffer);
}

void Bullet::Update(sf::RenderWindow * window, float dt)
{
	GameObject::Update(window, dt);
	m_angle += dt * 360;
	m_timeAlive += dt;

	if (m_timeAlive > 1.0f)
	{
		Destroy();
	}
}

void Bullet::CollidedWith(GameObject * other)
{
	Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
	if (asteroid != nullptr)
	{
		if (dynamic_cast<LargeAsteroid*>(other) != NULL)
		{
			m_owner->updateScore(100);
			
		}
		if (dynamic_cast<MediumAsteroid*>(other) != NULL)
		{
			m_owner->updateScore(50);
		}
		if (dynamic_cast<SmallAsteroid*>(other) != NULL)
		{
			m_owner->updateScore(20);
		}

		Destroy();
		AExplosionSound.play();
		other->Destroy();
		
		
	}	

	Alien* alien = dynamic_cast<Alien*>(other);
	if (alien != nullptr)
	{
		if (dynamic_cast<Alien*>(other) != NULL)
		{
			m_owner->updateScore(100);
		}


		Destroy();
		AExplosionSound.play();
		other->Destroy();


	}
}

Player::Player(std::string texturePath, const sf::Vector2f & pos)
	: GameObject(texturePath, pos)
	, m_firing(false)
	, m_fireCooldown(0.0f)
	, m_invulnerable(3.0f)
	
	
	
	

{
	SetCollisionRadius(30);
	ShotSound.setBuffer(ShotBuffer);
	if (!ShotBuffer.loadFromFile("Sounds/Shot.wav"))
	{
	std::cout << "error";
	}
	ExplosionSound.setBuffer(ExplosionBuffer);
	if (!ExplosionBuffer.loadFromFile("Sounds/Explosion.wav"))
	{
		std::cout << "error";
	}
	RespawnSound.setBuffer(RespawnBuffer);
	if (!RespawnBuffer.loadFromFile("Sounds/Respawn.wav"))
	{
		std::cout << "error";
	}
	HurtSound.setBuffer(HurtBuffer);
	if (!HurtBuffer.loadFromFile("Sounds/Hurt.wav"))
	{
		std::cout << "error";
	}
	BombSound.setBuffer(BombBuffer);
	if (!BombBuffer.loadFromFile("Sounds/Bomb.wav"))
	{
		std::cout << "error";
	}
	
	
	RespawnSound.play();
}

void Player::Update(sf::RenderWindow * window, float dt)
{
	m_fireCooldown -= dt;
	m_invulnerable -= dt;

	GameObject::Update(window, dt);

	SetAccel(0.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
	{
		SetAngle(GetAngle() + 180 * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
	{
		SetAngle(GetAngle() - 180 * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
	{
		SetAccel(400.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)|| 
		sf::Keyboard::isKeyPressed(sf::Keyboard::S)||
		sf::Mouse::isButtonPressed(sf::Mouse::Left)||
		sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (m_invulnerable < 0.0f)
		{
			m_firing = true;

		}
	}
	else
	{
		m_firing = false;
	}

	if (m_firing && m_fireCooldown <= 0.0f)
	{
		// Wants to do a multishot here
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			m_fireCooldown = 10.0f;
			for (int i = 0; i < 3; i++)
			{
				Bullet* bullet = new Bullet(m_pos);
				bullet->SetAngle(m_angle + i * 15 - 15);
				bullet->SetVelocity(500);
				m_owner->AddObject(bullet);
				ShotSound.play();
				ShotSound.setVolume(25);
				
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if(m_owner->bomb >= 1)
			{ 
		m_fireCooldown = 0.5f;
		m_owner->updateBomb(-1);
		BombSound.play();
		for (int i = 0; i < 24; i++)
		{
			Bomb* newbomb = new Bomb(m_pos);
			newbomb->SetAngle(m_angle + i * 15);
			newbomb->SetVelocity(1000);
			m_owner->AddObject(newbomb);
			
			
		}
			}
		
		}
		else
		{
			m_fireCooldown = .7f;
			Bullet* bullet = new Bullet(m_pos);
			bullet->SetAngle(m_angle);
			bullet->SetVelocity(500);
			m_owner->AddObject(bullet);
			ShotSound.play();
			
		}

		
	}

	


	
}

void Player::CollidedWith(GameObject * other)
{
	Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
	if (asteroid)
	{
		if (m_invulnerable < 0.0f && m_owner->m_healthRemaining >= 1)
		{
			m_owner->m_healthRemaining --;
			m_invulnerable = 1.0f;
			HurtSound.play();
		}
		if(m_invulnerable <0.0f && m_owner->m_healthRemaining == 0)
		{
			Destroy();

			sf::Color Orange;
			Orange.a = 150;
			Orange.r = 220;
			Orange.b = 40;
			Orange.g = 110;

			m_owner->AddObject(new ParticleSystem(30, m_pos, Orange, 5, 360, 0, 25, 100));
			m_owner->AddObject(new ParticleSystem(30, m_pos, sf::Color::Yellow, 5, 360, 0, 25, 100));
			ExplosionSound.play();
			m_owner->RespawnPlayer();
			m_owner->m_healthRemaining += 3;
		}
		
	}
	Alien* alien = dynamic_cast<Alien*>(other);
	if (alien)
	{
		if (m_invulnerable < 0.0f && m_owner->m_healthRemaining >= 1)
		{
			m_owner->m_healthRemaining--;
			m_invulnerable = 1.0f;
			HurtSound.play();
		}
		if (m_invulnerable <0.0f && m_owner->m_healthRemaining == 0)
		{
			Destroy();
			sf::Color Orange;
			Orange.a = 150;
			Orange.r = 220;
			Orange.b = 40;
			Orange.g = 110;

			m_owner->AddObject(new ParticleSystem(30, m_pos, Orange, 5, 360, 0, 25, 100));
			m_owner->AddObject(new ParticleSystem(30, m_pos, sf::Color::Yellow, 5, 360, 0, 25, 100));
			ExplosionSound.play();
			m_owner->RespawnPlayer();
			m_owner->m_healthRemaining += 3;
		}
	}
	
}



Asteroid::Asteroid(std::string texturePath, const sf::Vector2f& pos)
	: GameObject(texturePath, pos)
{
	m_rotationRate = rand()% 45 + 45; // between 45 - 90
	m_rotationRate *= rand() % 2 == 0 ? 1 : -1;		
	SetCollisionRadius(50);
	
	
		
	
}

void Asteroid::Update(sf::RenderWindow * window, float dt)
{
	m_angle += dt * m_rotationRate;

	GameObject::Update(window, dt);
}


SmallAsteroid::SmallAsteroid(const sf::Vector2f & pos)
	: Asteroid("Sprites/PNG/Meteors/MeteorBrown_small1.png", pos)
{
	
}

MediumAsteroid::MediumAsteroid(const sf::Vector2f & pos)
	: Asteroid("Sprites/PNG/Meteors/MeteorBrown_med1.png", pos)
{
	if (!BatBuffer.loadFromFile("Sounds/Bat.wav"))
	{
		std::cout << "error";
	}
	BatSound.setBuffer(BatBuffer);
}

void MediumAsteroid::Update(sf::RenderWindow * window, float dt)
{
	GameObject::Update(window, dt);



}

void MediumAsteroid::Destroy()
{
	m_owner->AddObject(new ParticleSystem(30, m_pos, sf::Color::White, 5, 360, 0, 25, 100));
	GameObject::Destroy();
	if (rand() % 20 >= 10)
	{
		for (int i = 0; i < 3; i++)
		{
			SmallAsteroid* smallAsteroid = new SmallAsteroid(m_pos);
			smallAsteroid->SetAngle(rand() % 360);
			smallAsteroid->SetVelocity(100);
			m_owner->AddObject(smallAsteroid);
			

		}
	}
	else if(rand() % 50 <= 2  )
	{
		for (int i = 0; i < 1; i++)
		{
			Powerup* bombpowerup = new BombPowerUp(sf::Vector2f(m_pos));
			m_owner->AddObject(bombpowerup);
		}
	}

	else if(rand() % 20 <= 3)
	{	
		BatSound.play();
		for (int i = 0; i < 5; i++)			
		{
				
			Alien* alien = new Alien(m_pos);
			m_owner->AddObject(alien);
			/*m_accel += m_alien;*/
			alien->SetVelocity(150);
			m_pos.x += rand() % 100 - 50;
			m_pos.y += rand() % 100 - 50;
			
		}		
			
			
		

	}
}


LargeAsteroid::LargeAsteroid(const sf::Vector2f & pos)
	: Asteroid("Sprites/PNG/Meteors/MeteorBrown_big1.png", pos)
{
}

void LargeAsteroid::Destroy()
{
	GameObject::Destroy();
	for (int i = 0; i < 3; i++)
	{
		MediumAsteroid* medAsteroid = new MediumAsteroid(m_pos);
		medAsteroid->SetAngle(rand() % 360);
		medAsteroid->SetVelocity(100);
		m_owner->AddObject(medAsteroid);
		m_owner->AddObject(new ParticleSystem(30, m_pos, sf::Color::White, 5, 360, 0, 25, 100));
		
	}
	
}

void SmallAsteroid::Destroy()
{
	GameObject::Destroy();
	
	{
		
		m_owner->AddObject(new ParticleSystem(30, m_pos, sf::Color::White, 5, 360, 0, 25, 100));

	}

}

void Alien::Destroy()
{
	GameObject::Destroy();

	{

		m_owner->AddObject(new ParticleSystem(30, m_pos, sf::Color::Red, 5, 360, 0, 25, 100));

	}

}

void Player::Draw(sf::RenderWindow* window)
{
	GameObject::Draw(window);


	if (m_invulnerable > 0.0f)
	{
		sf::CircleShape shape;
		shape.setRadius(50);
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineThickness(10 * m_invulnerable / 3.0f);
		shape.setOutlineColor(sf::Color::Red);
		shape.setOrigin(50, 50);
		shape.setPosition(m_pos.x, m_pos.y);
		window->draw(shape);
	}

	if (m_invulnerable > 0)
	{
		float sinValue = sin(m_invulnerable * 25); // Between -1 and 1
		sinValue += 1; // Between 0-2
		sinValue *= 0.5; // Between 0-1	
		float alpha = sinValue * 255; // 0 -255
		sf::Color shipColor = sf::Color::White;
		shipColor.a = alpha;
		m_sprite.setColor(shipColor);
		//std::cout<< "Sin is " << sinValue << std::endl;
	}
	else
	{
		m_sprite.setColor(sf::Color::White);
	}
}

void Alien::Update(sf::RenderWindow * window, float dt)
{
	
	GameObject::Update(window, dt);

	
		
	m_accel.x = m_owner->returnplayerpos().x - m_pos.x;
	m_accel.y = m_owner->returnplayerpos().y - m_pos.y;

	/*m_accel.x = m_owner->returnplayerpos().x - m_pos.x;
	m_accel.y = m_owner->returnplayerpos().y - m_pos.y;

	float aliendist = sqrt(m_alien.x *m_alien.x + m_alien.y * m_alien.y);

		m_alien.x = m_alien.x / aliendist;
		m_alien.y = m_alien.y / aliendist;
		m_alien.x = m_alien.x * (50 / aliendist) * 0.5;
		m_alien.y = m_alien.y * (50 / aliendist) * 0.5;*/

		
	
	//std::cout << m_owner->returnplayerpos().x << std::endl;
	//std::cout << m_owner->returnplayerpos().y << std::endl;

}


Alien::Alien(const sf::Vector2f& pos)
	: GameObject("Sprites/PNG/alien.png", pos)
	
{
	m_sprite.setScale(0.1, 0.1);
	SetCollisionRadius(50);
	
}


Bomb::Bomb(const sf::Vector2f & pos)
	: GameObject("Sprites/PNG/Lasers/Orange.png", pos)
	, m_timeAlive(0.0f)
{
	m_sprite.setScale(0.5, 0.5);
	SetCollisionRadius(20);
	AExplosionSound.setBuffer(AExplosionBuffer);
	if (!AExplosionBuffer.loadFromFile("Sounds/AExplosion.wav"))
	{
		std::cout << "error";
	}
	AExplosionSound.setBuffer(AExplosionBuffer);
}

void Bomb::Update(sf::RenderWindow * window, float dt)
{
	GameObject::Update(window, dt);
	m_angle += dt * 360;
	m_timeAlive += dt;

	if (m_timeAlive > 1.0f)
	{
		Destroy();
	}
}

void Bomb::CollidedWith(GameObject * other)
{
	Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
	if (asteroid != nullptr)
	{
		if (dynamic_cast<LargeAsteroid*>(other) != NULL)
		{
			m_owner->updateScore(100);

		}
		if (dynamic_cast<MediumAsteroid*>(other) != NULL)
		{
			m_owner->updateScore(50);
		}
		if (dynamic_cast<SmallAsteroid*>(other) != NULL)
		{
			m_owner->updateScore(20);
		}

		Destroy();
		AExplosionSound.play();
		other->Destroy();


	}

	Alien* alien = dynamic_cast<Alien*>(other);
	if (alien != nullptr)
	{
		if (dynamic_cast<Alien*>(other) != NULL)
		{
			m_owner->updateScore(100);
		}

		
		Destroy();
		AExplosionSound.play();
		other->Destroy();
		

	}
}

Powerup::Powerup(std::string texturePath,const sf::Vector2f & pos)
	: GameObject(texturePath, pos)
	
{
	m_sprite.setScale(0.5, 0.5);
	SetCollisionRadius(20);
	
	
}

void Powerup::Update(sf::RenderWindow * window, float dt)
{
	GameObject::Update(window, dt);
	m_angle += dt * 90;


	
}

void Powerup::CollidedWith(GameObject * other)
{
	Player* player = dynamic_cast<Player*>(other);
	if (player != nullptr)
	{
		Destroy();

	}

	
}

BombPowerUp::BombPowerUp(const sf::Vector2f & pos)
	: Powerup("Sprites/PNG/Bomb.png", pos)
{
	PowerUpSound.setBuffer(PowerUpBuffer);
	if (!PowerUpBuffer.loadFromFile("Sounds/Powerup.wav"))
	{
		std::cout << "error";
	}

}

void BombPowerUp::Destroy()
{   

	m_owner->updateBomb(1);
	PowerUpSound.play();
	GameObject::Destroy();
}

HealthPowerUp::HealthPowerUp(const sf::Vector2f & pos)
	: Powerup("Sprites/HealthPill.png", pos)
{
	PowerUpSound.setBuffer(PowerUpBuffer);
	if (!PowerUpBuffer.loadFromFile("Sounds/Powerup.wav"))
	{
		std::cout << "error";
	}
	m_sprite.setScale(1.5, 1.5);
}

void HealthPowerUp::Destroy()
{

	
	m_owner->m_healthRemaining++;
	if (m_owner->m_healthRemaining >= 4)
	{
		m_owner->m_healthRemaining = 3;
	}
	PowerUpSound.play();
	GameObject::Destroy();
}

ClonePowerUp::ClonePowerUp(const sf::Vector2f & pos)
	: Powerup("Sprites/Star.png", pos)
	
{

	PowerUpSound.setBuffer(PowerUpBuffer);
	if (!PowerUpBuffer.loadFromFile("Sounds/Powerup.wav"))
	{
		std::cout << "error";
	}
	m_sprite.setScale(1.5, 1.5);
	
}


void ClonePowerUp::Destroy()
{
	m_owner->clone = true;
	PowerUpSound.play();
	GameObject::Destroy();
}