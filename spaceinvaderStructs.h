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
	GameObjectStruct(int16_t X, int16_t Y, ObjectStatus status)
	{
		this->X = X;
		this->Y = Y;
		this->Status = status;
	};

	GameObjectStruct(int16_t X, int16_t Y) // ACTIVE by default
	{
		this->X = X;
		this->Y = Y;
		this->Status = ACTIVE;
	};

	GameObjectStruct() //0,0 ACTIVE by default
	{
		this->X = 0;
		this->Y = 0;
		this->Status = ACTIVE;
	};

	ObjectStatus Status = ACTIVE;
	int16_t X = 0;
	int16_t Y = 0;
};

struct BaseObjectStruct : GameObjectStruct
{
public:
	BaseObjectStruct(int16_t X, int16_t Y, ObjectStatus status) : GameObjectStruct(X, Y, status){};
	BaseObjectStruct(int16_t X, int16_t Y) : GameObjectStruct(X, Y){}; // ACTIVE by default
	BaseObjectStruct() : GameObjectStruct(){};						   // (0,0) and ACTIVE by default
	uint8_t XBM[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

struct ExplosableObjectStruct : GameObjectStruct
{
	ExplosableObjectStruct(int16_t X, int16_t Y, ObjectStatus status) : GameObjectStruct(X, Y, status){};
	ExplosableObjectStruct(int16_t X, int16_t Y) : GameObjectStruct(X, Y){}; // ACTIVE by default
	ExplosableObjectStruct() : GameObjectStruct(){};						 // (0,0) and ACTIVE by default

	uint8_t ExplosionXBMCounter = 0; // how long we want the ExplosionXBM to last
};

struct AlienObjectStruct : ExplosableObjectStruct
{
	AlienObjectStruct(int16_t X, int16_t Y, ObjectStatus status) : ExplosableObjectStruct(X, Y, status){};
	AlienObjectStruct(int16_t X, int16_t Y) : ExplosableObjectStruct(X, Y){}; // ACTIVE by default
	AlienObjectStruct() : ExplosableObjectStruct(){};						  // (0,0) and ACTIVE by default
};

struct PlayerObjectStruct : ExplosableObjectStruct
{
	PlayerObjectStruct() : ExplosableObjectStruct(PLAYER_X_START, PLAYER_Y_START, ACTIVE) { reset(); };
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
	uint16_t AliensDestroyed = 0; // count of how many killed so far
	uint8_t AlienSpeed = 0;		  // higher the number slower they go, calculated when ever alien destroyed
								  // uint8_t ExplosionXBMCounter = 0; // how long we want the ExplosionXBM to last
};

typedef struct GameObjectStruct GameObject;
typedef struct BaseObjectStruct BaseObject;
typedef struct ExplosableObjectStruct ExplosableObject;
typedef struct AlienObjectStruct AlienObject;
typedef struct PlayerObjectStruct PlayerObject;