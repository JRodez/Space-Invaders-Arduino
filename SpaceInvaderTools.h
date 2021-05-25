#include <stdint.h>
#include "gameplay_variables.h"

typedef enum
{
	ACTIVE = 0,
	EXPLODING = 1,
	DESTROYED = 2
} ObjectStatus;

struct GameObject
{
	// public:
	GameObject(uint16_t X, uint16_t Y, ObjectStatus status)
	{
		this->X = X;
		this->Y = Y;
		this->Status = status;
	};

	GameObject(uint16_t X, uint16_t Y) // ACTIVE by default
	{
		this->X = X;
		this->Y = Y;
		this->Status = ACTIVE;
	};

	GameObject() // ACTIVE by default
	{
		this->X = 0;
		this->Y = 0;
		this->Status = ACTIVE;
	};

	ObjectStatus Status;
	int16_t X = 0;
	int16_t Y = 0;
};

struct BaseObject : GameObject
{
public:
	BaseObject(uint16_t posX, uint16_t posY, ObjectStatus status) : GameObject(posX, posY, status){};
	BaseObject(uint16_t posX, uint16_t posY) : GameObject(posX, posY){}; // ACTIVE by default
	BaseObject() : GameObject(){};										 // (0,0) and ACTIVE by default
	uint8_t XBM[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

struct AlienObject : GameObject
{
	AlienObject(uint16_t posX, uint16_t posY, ObjectStatus status) : GameObject(posX, posY, status){};
	AlienObject(uint16_t posX, uint16_t posY) : GameObject(posX, posY){}; // ACTIVE by default
	AlienObject() : GameObject(){};										  // (0,0) and ACTIVE by default

	uint8_t ExplosionXBMCounter = 0; // how long we want the ExplosionXBM to last
};

struct PlayerObject : GameObject
{
	PlayerObject() : GameObject(PLAYER_X_START, PLAYER_Y_START, ACTIVE) { reset(); };
	void reset()
	{
		this->Score = 0;
		this->Lives = LIVES;
		this->Level = 0;
		this->X = PLAYER_X_START;
		this->Y = PLAYER_Y_START;
		this->Status = ACTIVE;
	};

	uint16_t Score = 0;
	uint8_t Lives = LIVES;
	uint8_t Level = 0;
	uint8_t AliensDestroyed = 0;	 // count of how many killed so far
	uint8_t AlienSpeed = 0;			 // higher the number slower they go, calculated when ever alien destroyed
	uint8_t ExplosionXBMCounter = 0; // how long we want the ExplosionXBM to last
};