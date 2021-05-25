/* **************** Game Constants **************** */
// SHIP
#define SHIP_SPEED 2
#define SHIP_SPAWN_RATE 250		 //HIGHER IS LESS CHANCE OF SPAWN
#define DISPLAY_MOTHERSHIP_BONUS_TIME 20 // how long bonus stays on screen for displaying mothership

// Alien Settings
// #define ALIEN_HEIGHT 8
#define NUM_ALIEN_COLUMNS 7
#define NUM_ALIEN_ROWS 3
#define X_START_OFFSET 6
#define SPACE_BETWEEN_ALIEN_COLUMNS 5
#define LARGEST_ALIEN_WIDTH 11
#define SPACE_BETWEEN_ROWS 9
#define INVADERS_DROP_BY 5 // pixel amount that invaders move down by
#define INVADERS_SPEED 30  // speed of movement, lower=faster.
#define INVADER_HEIGHT 8
#define EXPLOSION_XBM_TIME 7 // How long an ExplosionXBM remains on screen before dissapearing
#define INVADERS_Y_START MotherShipXBM.Y() - 1
#define AMOUNT_TO_DROP_BY_PER_LEVEL 4	 //NEW How much farther down aliens start per new level
#define LEVEL_TO_RESET_TO_START_HEIGHT 4 // EVERY MULTIPLE OF THIS LEVEL THE ALIEN y START POSITION WILL RESET TO TOP
#define ALIEN_X_MOVE_AMOUNT 1			 //number of pixels moved at start of wave
#define CHANCEOFBOMBDROPPING 80			 // Higher the number the rarer the bomb drop,
#define BOMB_HEIGHT 4
#define BOMB_WIDTH 2
#define MAXBOMBS 3 // Max number of bombs allowed to drop at a time
// These two lines are for when bombs collide with the bases
#define CHANCE_OF_BOMB_DAMAGE_TO_LEFT_OR_RIGHT 20 // higher more chance
#define CHANCE_OF_BOMB_PENETRATING_DOWN 1		  // higher more chance

// Player settings
#define TANKXBM_WIDTH 13
#define TANKXBM_HEIGHT 8
#define PLAYER_X_MOVE_AMOUNT 2
#define LIVES 3					 // NEW
#define PLAYER_EXPLOSION_TIME 10 // How long an ExplosionXBM remains on screen before dissapearing
#define PLAYER_Y_START 56
#define PLAYER_X_START 0

// #define BASE_WIDTH 16
// #define BASE_WIDTH_IN_BYTES 2
// #define BASE_HEIGHT 8
// #define BASE_Y 46
// #define NUM_BASES 3
// #define BASE_MARGINS 10

#define MISSILE_HEIGHT 4
#define MISSILE_WIDTH 1
#define MISSILE_SPEED 5

// Status of a game object constants
// #define ACTIVE 0
// #define EXPLODING 1
// #define DESTROYED 2
