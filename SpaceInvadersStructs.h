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
	GameObject(int16_t X, int16_t Y, ObjectStatus status)
	{
		this->X = X;
		this->Y = Y;
		this->Status = status;
	};

	GameObject(int16_t X, int16_t Y) // ACTIVE by default
	{
		this->X = X;
		this->Y = Y;
		this->Status = ACTIVE;
	};

	GameObject() //0,0 ACTIVE by default
	{
		this->X = 0;
		this->Y = 0;
		this->Status = ACTIVE;
	};

	// virtual void control() = 0;

	ObjectStatus Status = ACTIVE;
	int16_t X = 0;
	int16_t Y = 0;
};

struct BaseObject : GameObject
{

	BaseObject(int16_t X, int16_t Y, ObjectStatus status) : GameObject(X, Y, status){};
	BaseObject(int16_t X, int16_t Y) : GameObject(X, Y){}; // ACTIVE by default
	BaseObject() : GameObject(){};						   // (0,0) and ACTIVE by default
	uint8_t XBM[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

struct ExplosableObject : GameObject
{
	ExplosableObject(int16_t X, int16_t Y, ObjectStatus status) : GameObject(X, Y, status){};
	ExplosableObject(int16_t X, int16_t Y) : GameObject(X, Y){}; // ACTIVE by default
	ExplosableObject() : GameObject(){};						 // (0,0) and ACTIVE by default

	uint8_t ExplosionXBMCounter = 0; // how long we want the ExplosionXBM to last
};

struct AlienObject : ExplosableObject
{
	AlienObject(int16_t X, int16_t Y, ObjectStatus status) : ExplosableObject(X, Y, status){};
	AlienObject(int16_t X, int16_t Y) : ExplosableObject(X, Y){}; // ACTIVE by default
	AlienObject() : ExplosableObject(){};						  // (0,0) and ACTIVE by default
};
struct MotherShip  : ExplosableObject
{
	MotherShip(int16_t X, int16_t Y, ObjectStatus status) : ExplosableObject(X, Y, status){};
	MotherShip(int16_t X, int16_t Y) : ExplosableObject(X, Y){}; // ACTIVE by default
	MotherShip() : ExplosableObject(){};						   // (0,0) and ACTIVE by default
};

struct PlayerObject : ExplosableObject
{
	PlayerObject() : ExplosableObject(PLAYER_X_START, PLAYER_Y_START, ACTIVE) { reset(); };
	void reset()
	{
		this->Score = 0;
		this->Lives = LIVES;
		this->Level = 0;
		this->X = PLAYER_X_START;
		this->Y = PLAYER_Y_START;
		this->Status = ACTIVE;
	};

	// void control();
	uint16_t Score = 0;
	uint8_t Lives = LIVES;
	uint8_t Level = 0;
	uint16_t AliensDestroyed = 0; // count of how many killed so far
	uint8_t AlienSpeed = 0;		  // higher the number slower they go, calculated when ever alien destroyed
								  // uint8_t ExplosionXBMCounter = 0; // how long we want the ExplosionXBM to last
};

// typedef struct GameObjectStruct GameObject;
// typedef struct BaseObjectStruct BaseObject;
// typedef struct ExplosableObjectStruct ExplosableObject;
// typedef struct AlienObjectStruct AlienObject;
// typedef struct PlayerObjectStruct PlayerObject;
