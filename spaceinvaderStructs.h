#include <stdint.h>
#include "gameplay_variables.h"

typedef enum
{
	ACTIVE = 0,
	EXPLODING = 1,
	DESTROYED = 2
} ObjectStatus;

struct GameObjectStruct
{
	// public:
	GameObjectStruct(uint16_t X, uint16_t Y, ObjectStatus status)
	{
		this->X = X;
		this->Y = Y;
		this->Status = status;
	};

	GameObjectStruct(uint16_t X, uint16_t Y) // ACTIVE by default
	{
		this->X = X;
		this->Y = Y;
		this->Status = ACTIVE;
	};

	GameObjectStruct() // ACTIVE by default
	{
		this->X = 0;
		this->Y = 0;
		this->Status = ACTIVE;
	};

	ObjectStatus Status;
	int16_t X = 0;
	int16_t Y = 0;
};

struct BaseObjectStruct : GameObjectStruct
{
public:
	BaseObjectStruct(uint16_t posX, uint16_t posY, ObjectStatus status) : GameObjectStruct(posX, posY, status){};
	BaseObjectStruct(uint16_t posX, uint16_t posY) : GameObjectStruct(posX, posY){}; // ACTIVE by default
	BaseObjectStruct() : GameObjectStruct(){};										 // (0,0) and ACTIVE by default
	uint8_t XBM[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

struct AlienObjectStruct : GameObjectStruct
{
	AlienObjectStruct(uint16_t posX, uint16_t posY, ObjectStatus status) : GameObjectStruct(posX, posY, status){};
	AlienObjectStruct(uint16_t posX, uint16_t posY) : GameObjectStruct(posX, posY){}; // ACTIVE by default
	AlienObjectStruct() : GameObjectStruct(){};										  // (0,0) and ACTIVE by default

	uint8_t ExplosionXBMCounter = 0; // how long we want the ExplosionXBM to last
};

struct PlayerObjectStruct : GameObjectStruct
{
	PlayerObjectStruct() : GameObjectStruct(PLAYER_X_START, PLAYER_Y_START, ACTIVE) { reset(); };
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

typedef struct GameObjectStruct GameObject;
typedef struct BaseObjectStruct BaseObject;
typedef struct AlienObjectStruct AlienObject;
typedef struct PlayerObjectStruct PlayerObject;