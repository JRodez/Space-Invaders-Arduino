/* **********************************************************
/*                      Libraries                           *
/* ******************************************************** */
#include <U8g2lib.h>
#include "sprites.h"
#include "SpaceInvaderTools.h"

#include "gameplay_variables.h"
// #include "GameObjects.h"

#define USE_EEPROM
#ifdef USE_EEPROM
#include <EEPROM.h>
#endif
#define PULL_UP true
#define U8G2_CONSTRUCTOR U8G2_SH1106_128X64_NONAME_F_HW_I2C

#define DISP_WIDTH 128
#define DISP_HEIGHT 64

#define FIRE_BUTTON 2
#define LEFT_BUTTON 3
#define RIGHT_BUTTON 4

/* **********************************************************
/*                      Global Classes                      *
/*                     Game structures                      *
/* ******************************************************** */

// struct GameObjectStruct
// {
// 	// base object which most other objects will include
// 	signed int X;
// 	signed int Y;
// 	uint8_t Status; //0 active, 1 exploding, 2 destroyed
// };

// struct BaseStruct
// {
// 	GameObjectStruct Ord;
// 	uint8_t XBM[16];
// };

// struct AlienStruct
// {
// 	GameObjectStruct Ord;
// 	uint8_t ExplosionXBMCounter; // how long we want the ExplosionXBM to last
// };

// struct PlayerStruct
// {
// 	GameObjectStruct Ord;
// 	unsigned int Score;
// 	uint8_t Lives;
// 	uint8_t Level;
// 	uint8_t AliensDestroyed;	 // count of how many killed so far
// 	uint8_t AlienSpeed;			 // higher the number slower they go, calculated when ever alien destroyed
// 	uint8_t ExplosionXBMCounter; // how long we want the ExplosionXBM to last
// };

/* **********************************************************
/*                    Global Variables                      *
/* ******************************************************** */
//                    Aliens & display
//          The array of aliens across the display
U8G2_CONSTRUCTOR display(U8G2_R0, U8X8_PIN_NONE, SCL, SDA);

AlienObject Alien[NUM_ALIEN_COLUMNS][NUM_ALIEN_ROWS];
AlienObject MotherShip;
GameObject AlienBomb[MAXBOMBS];
// BaseObject Base[NUM_BASES];

uint8_t AlienWidth[] = {8, 11, 12}; // top, middle ,bottom widths

static const uint8_t TOTAL_ALIENS PROGMEM = NUM_ALIEN_COLUMNS * NUM_ALIEN_ROWS; //NEW

// widths of aliens
// as aliens are the same type per row we do not need to store their graphic width per alien in the structure above
// that would take a uint8_t per alien rather than just three entries here, 1 per row, saving significant memory
// uint8_t AlienWidth[] = {8, 11, 12}; // top, middle ,bottom widths

int8_t AlienXMoveAmount = 2;
int8_t InvadersMoveCounter;	 // counts down, when 0 move invaders, set according to how many aliens on display
bool AnimationFrame = false; // two frames of animation, if true show one if false show the other

// Mothership
int8_t MotherShipSpeed;
unsigned int MotherShipBonus;
signed int MotherShipBonusXPos; // pos to display bonus at
uint8_t MotherShipBonusCounter; // how long bonus amount left on display
uint8_t MotherShipType;			// which mothership to display

// Player global variables
PlayerObject Player;
GameObject Missile;

// game variables
unsigned int HiScore;
bool Playing = false;
uint8_t FONT_Ascent;
uint8_t FONT_Descent;

/* **********************************************************
/*                       Void Setup                         *
/* ******************************************************** */
void setup()
{
	//OLED Diplay
	/* U8g2 Project: SSD1306 or SH1106 OLED SPI Board */
	display.begin();

	display.enableUTF8Print();
	display.clear();
	display.clearBuffer();
	display.setFont(u8g2_font_7x13B_tf); // choix d'une police de caractère
	display.setCursor(25, 40);
	display.print("Hello World!");
	display.sendBuffer();

	delay(2000);

	//reinit
	display.setBitmapMode(1);
	// InitPlayer();
	InitAliens(0);
	Player.reset();
	Missile.Status = DESTROYED;

	// Buttons init
#if PULL_UP == 1
	pinMode(FIRE_BUTTON, INPUT_PULLUP);
	pinMode(LEFT_BUTTON, INPUT_PULLUP);
	pinMode(RIGHT_BUTTON, INPUT_PULLUP);
#else
	pinMode(FIRE_BUTTON, INPUT);
	pinMode(LEFT_BUTTON, INPUT);
	pinMode(RIGHT_BUTTON, INPUT);
#endif
 

	display.setFont(u8g2_font_t0_11b_tf); //font size is ok - it is loaded in PROGMEM
	FONT_Ascent = display.getAscent();	  //getAscent returns the number of pixels above the baseline
	FONT_Descent = -display.getDescent(); //getDescent returns a negative value, a number of pixels below the baseline
	display.setDrawColor(1);			  //set the color

	EEPROM.get(0, HiScore);
	if (HiScore == 65535)
	{ // new unit never written to
		HiScore = 0;
		EEPROM.put(0, HiScore);
	}
}

/* **********************************************************
/*                      Void Loop                           *
/* ******************************************************** */
void loop()
{
	if (Playing)
	{
		Physics();
		UpdateDisplay();
	}
	else
		AttractScreen();
}

/* **********************************************************
/*                 Functions and Subroutines                *
/* ******************************************************** */

void AttractScreen()
{
	uint8_t RowHeight;
	uint8_t ColPosition;

	ColPosition = display.getStrWidth("8");

	if (HiScore < 10)
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Hi Score ") - ColPosition) / 2.0);
	}
	else if (HiScore < 100)
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Hi Score ") - ColPosition * 2) / 2.0);
	}
	else if (HiScore < 1000)
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Hi Score ") - ColPosition * 3) / 2.0);
	}
	else if (HiScore < 10000)
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Hi Score ") - ColPosition * 4) / 2.0);
	}
	else
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Hi Score ") - ColPosition * 5) / 2.0);
		//65,535 max
	}

	display.clearBuffer();
	RowHeight = FONT_Ascent + (DISP_HEIGHT - 4 * (FONT_Ascent + FONT_Descent + 1)) / 2;
	CenterText("Play", RowHeight);
	//--> Next lines are for debugging <--
	//display.print(" ");display.print(FONT_Ascent); display.print(" ");display.print(FONT_Descent);
	//display.print(" ");display.print(ColPosition);
	RowHeight = RowHeight + FONT_Ascent + FONT_Descent + 1;
	CenterText("Space Invaders", RowHeight);
	RowHeight = RowHeight + FONT_Ascent + FONT_Descent + 1;
	CenterText("Press Fire to start", RowHeight);
	RowHeight = RowHeight + FONT_Ascent + FONT_Descent + 1;
	// Special Center Text ---->
	display.setCursor(ColPosition, RowHeight);
	display.print("Hi Score ");
	display.print(HiScore);
	//display.display();

	if ((digitalRead(LEFT_BUTTON) != PULL_UP) && (digitalRead(RIGHT_BUTTON) != PULL_UP))
	{
		HiScore = 0;
		EEPROM.put(0, HiScore);
	}

	display.sendBuffer();
	if (digitalRead(FIRE_BUTTON) != PULL_UP)
	{
		Playing = true;
		NewGame();
	}
}

void Physics()
{
	if (Player.Status == ACTIVE)
	{
		AlienControl();
		MotherShipPhysics();
		PlayerControl();
		MissileControl();
		CheckCollisions();
	}
}

uint8_t GetScoreForAlien(int RowNumber)
{
	// returns value for killing and alien at the row indicated
	switch (RowNumber)
	{
	case 0:
		return 30;
	case 1:
		return 20;
	case 2:
		return 10;
	default:
		return 0;
	}
}

void MotherShipPhysics()
{
	if (MotherShip.Status == ACTIVE)
	{ // spawned, move it
		//toneAC2( pin1, pin2, frequency [, length [, background ]] )
		//toneAC2(spkr_pos,spkr_neg,(MotherShip.X % 8)*500,200,true);
		if (MotherShipType)
		{
			//Smaller Mothership
			//   toneAC((MotherShip.X % 8)*800,6,200,true);
		}
		else
		{
			//Larger Mothership
			//   toneAC((MotherShip.X % 8)*500,6,200,true);
		}
		MotherShip.X += MotherShipSpeed;
		if (MotherShipSpeed > 0) // going left to right , check if off right hand side
		{
			if (MotherShip.X >= DISP_WIDTH)
			{
				MotherShip.Status = DESTROYED;
				//noToneAC2();
				// noToneAC();
			}
		}
		else // going right to left , check if off left hand side
		{
			if (MotherShip.X + MotherShipXBM.X() < 0)
			{
				MotherShip.Status = DESTROYED;
				//noToneAC2();
				// noToneAC();
			}
		}
	}
	else
	{
		// try to spawn mothership
		if (random(SHIP_SPAWN_RATE) == 1)
		{
			// Spawn a mother ship, starts just off  at top
			MotherShip.Status = ACTIVE;
			//randomSeed(analogRead(A1));
			MotherShipType = random(2); // a value of 0 or 1

			// need to set direction
			if (random(2) == 1) // values between 0 and 1
			{
				MotherShip.X = DISP_WIDTH;
				MotherShipSpeed = -SHIP_SPEED; // if we go in here swaps to right to left
			}
			else
			{
				MotherShip.X = -MotherShipXBM.X();
				MotherShipSpeed = SHIP_SPEED; // set to go left ot right
			}
		}
	}
}

void PlayerControl()
{
	// user input checks
	if ((digitalRead(RIGHT_BUTTON) != PULL_UP) && (Player.X + TANKXBM_WIDTH < DISP_WIDTH))
		Player.X += PLAYER_X_MOVE_AMOUNT;
	if ((digitalRead(LEFT_BUTTON) != PULL_UP) && (Player.X > 0))
		Player.X -= PLAYER_X_MOVE_AMOUNT;
	if ((digitalRead(FIRE_BUTTON) != PULL_UP) && (Missile.Status != ACTIVE))
	{
		Missile.X = Player.X + (TANKXBM_WIDTH / 2);
		Missile.Y = PLAYER_Y_START;
		Missile.Status = ACTIVE;
	}
}

void MissileControl()
{
	if (Missile.Status == ACTIVE)
	{
		Missile.Y -= MISSILE_SPEED;
		if (Missile.Y + MISSILE_HEIGHT < 0) // If off top of display destroy so can be used again
			Missile.Status = DESTROYED;
	}
}

void AlienControl()
{
	if ((InvadersMoveCounter--) < 0)
	{
		bool Dropped = false;
		if ((RightMostPos() + AlienXMoveAmount >= DISP_WIDTH) | (LeftMostPos() + AlienXMoveAmount < 0)) // at edge of display
		{
			AlienXMoveAmount = -AlienXMoveAmount; // reverse direction
			Dropped = true;						  // and indicate we are dropping
		}

		// update the alien postions
		for (int Across = 0; Across < NUM_ALIEN_COLUMNS; Across++)
		{
			for (int Down = 0; Down < 3; Down++)
			{
				if (Alien[Across][Down].Status == ACTIVE)
				{
					if (Dropped == false)
						Alien[Across][Down].X += AlienXMoveAmount;
					else
						Alien[Across][Down].Y += INVADERS_DROP_BY;
				}
			}
		}
		InvadersMoveCounter = Player.AlienSpeed;
		AnimationFrame = !AnimationFrame; //swap to other frame
	}
	// should the alien drop a bomb
	if (random(CHANCEOFBOMBDROPPING) == 1)
		DropBomb();
	MoveBombs();
}

void MoveBombs()
{
	for (int i = 0; i < MAXBOMBS; i++)
	{
		if (AlienBomb[i].Status == ACTIVE)
			AlienBomb[i].Y += 2;
	}
}

void DropBomb()
{
	// if there is a free bomb slot then drop a bomb else nothing happens
	bool Free = false;
	uint8_t ActiveCols[NUM_ALIEN_COLUMNS];
	uint8_t BombIdx = 0;
	// find a free bomb slot
	while ((Free == false) & (BombIdx < MAXBOMBS))
	{
		if (AlienBomb[BombIdx].Status == DESTROYED)
			Free = true;
		else
			BombIdx++;
	}
	if (Free)
	{
		uint8_t Columns = 0;
		// now pick and alien at random to drop the bomb
		// we first pick a column, obviously some columns may not exist, so we count number of remaining cols
		// first, this adds time but then also picking columns randomly that don't exist may take time also
		uint8_t ActiveColCount = 0;
		signed char Row;
		uint8_t ChosenColumn;

		while (Columns < NUM_ALIEN_COLUMNS)
		{
			Row = 2;
			while (Row >= 0)
			{
				if (Alien[Columns][Row].Status == ACTIVE)
				{
					ActiveCols[ActiveColCount] = Columns;
					ActiveColCount++;
					break;
				}
				--Row;
			}
			Columns++;
		}
		// we have ActiveCols array filled with the column numbers of the active cols and we have how many
		// in ActiveColCount, now choose a column at random
		ChosenColumn = random(ActiveColCount); // random number between 0 and the amount of columns
		// we now find the first available alien in this column to drop the bomb from
		Row = 2;
		while (Row >= 0)
		{
			if (Alien[ActiveCols[ChosenColumn]][Row].Status == ACTIVE)
			{
				// Set the bomb from this alien
				AlienBomb[BombIdx].Status = ACTIVE;
				AlienBomb[BombIdx].X = Alien[ActiveCols[ChosenColumn]][Row].X + int(AlienWidth[Row] / 2);
				// above sets bomb to drop around invaders center, here we add a little randomness around this pos
				AlienBomb[BombIdx].X = (AlienBomb[BombIdx].X - 2) + random(0, 4);
				AlienBomb[BombIdx].Y = Alien[ActiveCols[ChosenColumn]][Row].Y + 4;
				break;
			}
			--Row;
		}
	}
}

void BombCollisions()
{
	//check bombs collisions
	for (int i = 0; i < MAXBOMBS; i++)
	{
		if (AlienBomb[i].Status == ACTIVE)
		{
			if (AlienBomb[i].Y > 64) // gone off bottom of display
				AlienBomb[i].Status = DESTROYED;
			else
			{
				// HAS IT HIT PLAYERS missile
				if (Collision(AlienBomb[i], BOMB_WIDTH, BOMB_HEIGHT, Missile, MISSILE_WIDTH, MISSILE_HEIGHT))
				{
					// destroy missile and bomb
					AlienBomb[i].Status = EXPLODING;
					Missile.Status = DESTROYED;
				}
				else
				{
					// has it hit players ship
					if (Collision(AlienBomb[i], BOMB_WIDTH, BOMB_HEIGHT, Player, TANKXBM_WIDTH, TANKXBM_HEIGHT))
					{
						PlayerHit();
						AlienBomb[i].Status = DESTROYED;
					}
					// else
					// 	BombAndBasesCollision(&AlienBomb[i]);
				}
			}
		}
	}
}

void PlayerHit()
{
	Player.Status = EXPLODING;
	Player.ExplosionXBMCounter = PLAYER_EXPLOSION_TIME;
	Missile.Status = DESTROYED;
	// noiseAC(500,10,&PlayerExplosionNoiseCompleted);
}

void CheckCollisions()
{
	MissileAndAlienCollisions();
	MotherShipCollisions();
	BombCollisions();
}

void MotherShipCollisions()
{
	if ((Missile.Status == ACTIVE) & (MotherShip.Status == ACTIVE))
	{
		if (Collision(Missile, MISSILE_WIDTH, MISSILE_HEIGHT, MotherShip, MotherShipXBM.X(), MotherShipXBM.Y()))
		{
			MotherShip.Status = EXPLODING;
			MotherShip.ExplosionXBMCounter = EXPLOSION_XBM_TIME;
			Missile.Status = DESTROYED;

			MotherShipBonus = random(4); // a random number between 0 and 3
			switch (MotherShipBonus)
			{
			case 0:
				MotherShipBonus = 50;
				break;
			case 1:
				MotherShipBonus = 100;
				break;
			case 2:
				MotherShipBonus = 150;
				break;
			case 3:
				MotherShipBonus = 300;
				break;
			}
			Player.Score += MotherShipBonus + MotherShipType * 100;
			MotherShipBonusXPos = MotherShip.X;
			if (MotherShipBonusXPos > 100) // to ensure isn't half off right hand side of display
				MotherShipBonusXPos = 100;
			if (MotherShipBonusXPos < 0) // to ensure isn't half off right hand side of display
				MotherShipBonusXPos = 0;
			MotherShipBonusCounter = DISPLAY_MOTHERSHIP_BONUS_TIME;
		}
	}
}

void MissileAndAlienCollisions()
{
	for (int across = 0; across < NUM_ALIEN_COLUMNS; across++)
	{
		for (int down = 0; down < NUM_ALIEN_ROWS; down++)
		{
			if (Alien[across][down].Status == ACTIVE)
			{
				if (Missile.Status == ACTIVE)
				{
					if (Collision(Missile, MISSILE_WIDTH, MISSILE_HEIGHT, Alien[across][down], AlienWidth[down], INVADER_HEIGHT))
					{
						// missile hit
						Alien[across][down].Status = EXPLODING;
						//toneAC2(spkr_pos,spkr_neg,700,100,true);
						//   toneAC(700,10,100,true);
						Missile.Status = DESTROYED;
						Player.Score += GetScoreForAlien(down);
						Player.AliensDestroyed++;
						// calc new speed of aliens, note (float) must be before TOTAL_ALIENS to force float calc else
						// you will get an incorrect result
						Player.AlienSpeed = ((1 - (Player.AliensDestroyed / (float)TOTAL_ALIENS)) * INVADERS_SPEED);
						// for very last alien make to  fast!
						if (Player.AliensDestroyed == TOTAL_ALIENS - 2)
							if (AlienXMoveAmount > 0)
								AlienXMoveAmount = ALIEN_X_MOVE_AMOUNT * 2;
							else
								AlienXMoveAmount = -(ALIEN_X_MOVE_AMOUNT * 2);
						// for very last alien make to super fast!
						if (Player.AliensDestroyed == TOTAL_ALIENS - 1)
							if (AlienXMoveAmount > 0)
								AlienXMoveAmount = ALIEN_X_MOVE_AMOUNT * 4;
							else
								AlienXMoveAmount = -(ALIEN_X_MOVE_AMOUNT * 4);

						if (Player.AliensDestroyed == TOTAL_ALIENS)
							NextLevel();
					}
				}
				if (Alien[across][down].Status == ACTIVE) // double check still active afer above code
				{
					// check if this alien is in contact with TankXBM
					if (Collision(Player, TankXBM.X(), TankXBM.Y(), Alien[across][down], AlienWidth[down], INVADER_HEIGHT))
						PlayerHit();
					else
					{
						// check if alien is below bottom of display
						if (Alien[across][down].Y + 8 > DISP_HEIGHT)
							PlayerHit();
					}
				}
			}
		}
	}
}

bool Collision(GameObject Obj1, uint8_t Width1, uint8_t Height1, GameObject Obj2, uint8_t Width2, uint8_t Height2)
{
	return ((Obj1.X + Width1 > Obj2.X) & (Obj1.X < Obj2.X + Width2) & (Obj1.Y + Height1 > Obj2.Y) & (Obj1.Y < Obj2.Y + Height2));
}

int RightMostPos()
{
	//returns x pos of right most alien
	int Across = NUM_ALIEN_COLUMNS - 1;
	int Down;
	int Largest = 0;
	int RightPos;
	while (Across >= 0)
	{
		Down = 0;
		while (Down < NUM_ALIEN_ROWS)
		{
			if (Alien[Across][Down].Status == ACTIVE)
			{
				// different aliens have different widths, add to x pos to get rightpos
				RightPos = Alien[Across][Down].X + AlienWidth[Down];
				if (RightPos > Largest)
					Largest = RightPos;
			}
			Down++;
		}
		if (Largest > 0) // we have found largest for this column
			return Largest;
		Across--;
	}
	return 0; // should never get this far
}

int LeftMostPos()
{
	//returns x pos of left most alien
	int Across = 0;
	int Down;
	int Smallest = DISP_WIDTH * 2;
	while (Across < NUM_ALIEN_COLUMNS)
	{
		Down = 0;
		while (Down < 3)
		{
			if (Alien[Across][Down].Status == ACTIVE)
				if (Alien[Across][Down].X < Smallest)
					Smallest = Alien[Across][Down].X;
			Down++;
		}
		if (Smallest < DISP_WIDTH * 2) // we have found smallest for this column
			return Smallest;
		Across++;
	}
	return 0; // should nevr get this far
}

void UpdateDisplay()
{
	int i;
	uint8_t RowHeight;

	//display.clearDisplay();
	display.clearBuffer();
	RowHeight = FONT_Ascent;
	// Mothership bonus display if required
	if (MotherShipBonusCounter > 0)
	{
		// mothership bonus
		display.setCursor(MotherShipBonusXPos, RowHeight);
		display.print(MotherShipBonus);
		MotherShipBonusCounter--;
	}
	else
	{
		// draw score and lives, anything else can go above them
		display.setCursor(0, RowHeight);
		display.print(Player.Score);
		display.setCursor(DISP_WIDTH - 7, RowHeight);
		display.print(Player.Lives);
	}

	//BOMBS
	// draw bombs next as aliens have priority of overlapping them
	for (i = 0; i < MAXBOMBS; i++)
	{
		if (AlienBomb[i].Status == ACTIVE)
			//old code:
			//display.drawBitmap(AlienBomb[i].X, AlienBomb[i].Y,  AlienBombXBM, 2, 4,WHITE);
			//New code:
			display.drawXBMP(AlienBomb[i].X, AlienBomb[i].Y, AlienBombXBM.X(), AlienBombXBM.Y(), AlienBombXBM.XBM());
		//drawXBMP(x, y, w, h, *bitmap)
		else
		{ // must be destroyed
			if (AlienBomb[i].Status == EXPLODING)
				//olde code:
				//display.drawBitmap(AlienBomb[i].X-4, AlienBomb[i].Y,  ExplosionXBM, 4, 8,WHITE);
				//New code:
				display.drawXBMP(AlienBomb[i].X - 4, AlienBomb[i].Y, ExplosionXBM.X(), ExplosionXBM.Y(), ExplosionXBM.XBM());
			//drawXBMP(x, y, w, h, *bitmap)

			// Ensure on next draw that ExplosionXBM dissapears
			AlienBomb[i].Status = DESTROYED;
		}
	}

	//Invaders
	for (int across = 0; across < NUM_ALIEN_COLUMNS; across++)
	{
		for (int down = 0; down < NUM_ALIEN_ROWS; down++)
		{
			if (Alien[across][down].Status == ACTIVE)
			{
				int j;
				if (AnimationFrame)
				{
					j = 0;
				}
				else
				{
					j = 1;
				}
				switch (down)
				{
				case 0:
					//if(AnimationFrame)
					//  display.drawBitmap(Alien[across][down].X, Alien[across][down].Y,  InvaderTopXBM, AlienWidth[down],INVADER_HEIGHT,WHITE);
					//else
					//  display.drawBitmap(Alien[across][down].X, Alien[across][down].Y,  InvaderTopXBM2, AlienWidth[down],INVADER_HEIGHT,WHITE);
					display.drawXBMP(Alien[across][down].X, Alien[across][down].Y, InvaderTopXBM.X(), InvaderTopXBM.Y(), InvaderTopXBM.XBM(j));
					break;
				case 1:
					//if(AnimationFrame)
					//  display.drawBitmap(Alien[across][down].X, Alien[across][down].Y,  InvaderMiddleXBM, AlienWidth[down],INVADER_HEIGHT,WHITE);
					//else
					//  display.drawBitmap(Alien[across][down].X, Alien[across][down].Y,  InvaderMiddleXBM2, AlienWidth[down],INVADER_HEIGHT,WHITE);
					display.drawXBMP(Alien[across][down].X, Alien[across][down].Y, InvaderMiddleXBM.X(), InvaderMiddleXBM.Y(), InvaderMiddleXBM.XBM(j));
					break;
				default:
					//if(AnimationFrame)
					//  display.drawBitmap(Alien[across][down].X, Alien[across][down].Y,  InvaderBottomXBM, AlienWidth[down],INVADER_HEIGHT,WHITE);
					//else
					//  display.drawBitmap(Alien[across][down].X, Alien[across][down].Y,  InvaderBottomXBM2, AlienWidth[down],INVADER_HEIGHT,WHITE);
					display.drawXBMP(Alien[across][down].X, Alien[across][down].Y, InvaderBottomXBM.X(), InvaderBottomXBM.Y(), InvaderBottomXBM.XBM(j));
				} //end switch
			}
			else
			{
				if (Alien[across][down].Status == EXPLODING)
				{
					Alien[across][down].ExplosionXBMCounter--;
					if (Alien[across][down].ExplosionXBMCounter > 0)
					{
						//toneAC2(spkr_pos,spkr_neg,Alien[across][down].ExplosionXBMCounter*100,100,true);
						// toneAC(Alien[across][down].ExplosionXBMCounter*100,10,100,true);
						//display.drawBitmap(Alien[across][down].X, Alien[across][down].Y,  ExplosionXBM, 13, 8,WHITE);
						display.drawXBMP(Alien[across][down].X, Alien[across][down].Y, 13, 8, ExplosionXBM.XBM());
					}
					else
						Alien[across][down].Status = DESTROYED;
				}
			} //end if
		}	  //end for
	}		  // end for

	// player
	if (Player.Status == ACTIVE)
		//display.drawBitmap(Player.X, Player.Y,  TankXBM, TANKXBM_WIDTH, TANKXBM_HEIGHT,WHITE);
		display.drawXBMP(Player.X, Player.Y, TankXBM.X(), TankXBM.Y(), TankXBM.XBM());
	else
	{
		if (Player.Status == EXPLODING)
		{
			for (i = 0; i < TANKXBM_WIDTH; i += 2)
			{
				//display.drawBitmap(Player.X+i, Player.Y,  ExplosionXBM, random(4)+2, 8,WHITE);
				display.drawXBMP(Player.X + i, Player.Y, random(4) + 2, ExplosionXBM.Y(), ExplosionXBM.XBM());
			}
			Player.ExplosionXBMCounter--;
			if (Player.ExplosionXBMCounter == 0)
			{
				Player.Status = DESTROYED;
				delay(500); // small delay after tank explodes and player status display
				LoseLife();
			}
		}
	}
	//missile
	if (Missile.Status == ACTIVE)
		//display.drawBitmap(Missile.X, Missile.Y,  MissileXBM, MISSILE_WIDTH, MISSILE_HEIGHT,WHITE);
		// display.drawXBMP(Missile.X, Missile.Y, MissileXBM.X(), MissileXBM.Y(), MissileXBM.XBM());
		display.drawXBMP(Missile.X, Missile.Y, MissileXBM.X(), MissileXBM.Y(), MissileXBM.XBM());

	// mothership (not bonus if hit)
	if (MotherShip.Status == ACTIVE)
	{
		//display.drawBitmap(MotherShip.X, MotherShip.Y,  MotherShipXBM, MotherShipXBM.X(), MotherShipXBM.Y(),WHITE);
		display.drawXBMP(MotherShip.X, MotherShip.Y, MotherShipXBM.X(), MotherShipXBM.Y(), MotherShipXBM.XBM(MotherShipType));
	}
	else
	{
		if (MotherShip.Status == EXPLODING)
		{
			for (i = 0; i < MotherShipXBM.X(); i += 2)
			{
				//display.drawBitmap(MotherShip.X+i, MotherShip.Y,  ExplosionXBM, random(4)+2, MotherShipXBM.Y(),WHITE);
				display.drawXBMP(MotherShip.X + i, MotherShip.Y, random(4) + 2, MotherShipXBM.Y(), ExplosionXBM.XBM());
			}
			//toneAC2( pin1, pin2, frequency [, length [, background ]] )
			//toneAC2(spkr_pos,spkr_neg,MotherShip.ExplosionXBMCounter*50,100,true);
			//   toneAC(MotherShip.ExplosionXBMCounter*50,10,100,true);
			MotherShip.ExplosionXBMCounter--;
			if (MotherShip.ExplosionXBMCounter == 0)
			{
				MotherShip.Status = DESTROYED;
			}
		}
	}

	// plot bases

	// for (i = 0; i < NUM_BASES; i++)
	// {
	// 	if (Base[i].Status == ACTIVE)
	// 		//display.drawBitmap(Base[i].X, Base[i].Y,  Base[i].XBM, BASE_WIDTH, BASE_HEIGHT,WHITE,true);
	// 		display.drawXBM(Base[i].X, Base[i].Y, BASE_WIDTH, BASE_HEIGHT, Base[i].XBM);
	// }
	//display.display();
	display.sendBuffer();
}

void LoseLife()
{
	Player.Lives--;
	if (Player.Lives > 0)
	{
		DisplayPlayerAndLives();
		// clear alien missiles
		for (int i = 0; i < MAXBOMBS; i++)
		{
			AlienBomb[i].Status = DESTROYED;
			AlienBomb[i].Y = 0;
		}
		// ENABLE PLAYER
		Player.Status = ACTIVE;
		Player.X = 0;
	}
	else
	{
		GameOver();
	}
}

void GameOver()
{
	uint8_t RowHeight;
	uint8_t ColPosition;
	Playing = false;
	//display.clearDisplay();
	display.clearBuffer();
	if (Player.Score > HiScore)
	{
		RowHeight = FONT_Ascent;
	}
	else
	{
		RowHeight = FONT_Ascent + (DISP_HEIGHT - 4 * (FONT_Ascent + FONT_Descent + 1)) / 2;
	}
	CenterText("Player 1", RowHeight);
	RowHeight = RowHeight + FONT_Ascent + FONT_Descent + 1;
	CenterText("Game Over", RowHeight);
	RowHeight = RowHeight + FONT_Ascent + FONT_Descent + 1;
	// Special Center Text ---->
	ColPosition = display.getStrWidth("8");
	if (HiScore < 10)
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Score ") - ColPosition) / 2.0);
	}
	else if (HiScore < 100)
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Score ") - ColPosition * 2) / 2.0);
	}
	else if (HiScore < 1000)
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Score ") - ColPosition * 3) / 2.0);
	}
	else if (HiScore < 10000)
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Score ") - ColPosition * 4) / 2.0);
	}
	else
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Score ") - ColPosition * 5) / 2.0);
	}
	display.setCursor(ColPosition, RowHeight);
	display.print("Score ");
	display.print(Player.Score);
	if (Player.Score > HiScore)
	{
		RowHeight = RowHeight + FONT_Ascent + FONT_Descent + 1;
		CenterText("NEW HIGH SCORE!!!", RowHeight);
		RowHeight = RowHeight + FONT_Ascent + FONT_Descent + 1;
		CenterText("**CONGRATULATIONS**", RowHeight);
	}
	//display.display();
	display.sendBuffer();
	if (Player.Score > HiScore)
	{
		HiScore = Player.Score;
		EEPROM.put(0, HiScore);
	}
	delay(3000);
}

// void DisplayPlayerAndLives(PlayerStruct *Player)F
void DisplayPlayerAndLives()
{
	uint8_t RowHeight;
	//display.clearDisplay();
	display.clearBuffer();
	RowHeight = FONT_Ascent + (DISP_HEIGHT - 4 * (FONT_Ascent + FONT_Descent + 1)) / 2;
	CenterText("Player 1", RowHeight);
	RowHeight = RowHeight + FONT_Ascent + FONT_Descent + 1;
	CenterText("Score ", RowHeight);
	display.print(Player.Score);
	RowHeight = RowHeight + FONT_Ascent + FONT_Descent + 1;
	CenterText("Lives ", RowHeight);
	display.print(Player.Lives);
	RowHeight = RowHeight + FONT_Ascent + FONT_Descent + 1;
	CenterText("Level ", RowHeight);
	display.print(Player.Level);
	//display.display();
	display.sendBuffer();
	delay(2000);
	Player.X = PLAYER_X_START;
}

void CenterText(const char *Text, uint8_t RowValue)
{
	// center text on display
	display.setCursor(int((DISP_WIDTH - display.getStrWidth(Text)) / 2.0), RowValue);
	display.print(Text);
}

void InitPlayer()
{
	Player.reset();
	Missile.Status = DESTROYED;
	// Player.Y = PLAYER_Y_START;
	// Player.X = PLAYER_X_START;
	// Player.Status = ACTIVE;
	// Player.Lives = LIVES;
	// Player.Level = 0;
	// Player.Score = 0;
}

// void NextLevel(PlayerStruct *Player)
void NextLevel()

{
	// reset any dropping bombs
	int YStart;
	for (int i = 0; i < MAXBOMBS; i++)
		AlienBomb[i].Status = DESTROYED;
	AnimationFrame = false;
	Player.Level++;
	YStart = ((Player.Level - 1) % LEVEL_TO_RESET_TO_START_HEIGHT) * AMOUNT_TO_DROP_BY_PER_LEVEL;
	InitAliens(YStart);
	AlienXMoveAmount = ALIEN_X_MOVE_AMOUNT;
	Player.AlienSpeed = INVADERS_SPEED;
	Player.AliensDestroyed = 0;
	MotherShip.X = -MotherShipXBM.X();
	MotherShip.Status = DESTROYED;
	Missile.Status = DESTROYED;
	randomSeed(100);
	// InitBases();
	DisplayPlayerAndLives();
}

void NewGame()
{
	Player.reset();
	Missile.Status = DESTROYED;
	// InitPlayer();
	NextLevel();
}

void InitAliens(int YStart)
{
	for (int across = 0; across < NUM_ALIEN_COLUMNS; across++)
	{
		for (int down = 0; down < 3; down++)
		{
			// we add down to centralize the aliens, just happens to be the right value we need per row!
			// we need to adjust a little as row zero should be 2, row 1 should be 1 and bottom row 0
			Alien[across][down].X = X_START_OFFSET + (across * (LARGEST_ALIEN_WIDTH + SPACE_BETWEEN_ALIEN_COLUMNS)) - (AlienWidth[down] / 2);
			Alien[across][down].Y = YStart + (down * SPACE_BETWEEN_ROWS);
			Alien[across][down].Status = ACTIVE;
			Alien[across][down].ExplosionXBMCounter = EXPLOSION_XBM_TIME;
		}
	}
	MotherShip.Y = 0;
	MotherShip.X = -MotherShipXBM.X();
	MotherShip.Status = DESTROYED;
}
