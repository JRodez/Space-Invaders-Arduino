#include <Arduino.h>
#include <U8g2lib.h>
#include <HardwareSerial.h>
#include "sprites/sprites.h"
#include "neodisplay/neodisplay.hpp"
#include "neocampus_i2c.h"
#include <Wire.h>
#define SDA 21
#define SCL 22
// #define U8G2_128X64_CONSTRUCTOR U8G2_SH1106_128X64_NONAME_F_HW_I2C

U8G2_SH1106_128X64_NONAME_F_HW_I2C disp(U8G2_R0, U8X8_PIN_NONE, SCL, SDA); // OLEDs without Reset of the Display
uint8_t i = 0;

void setup()
{
	Serial.begin(115200);

	while (!Serial.available())
	{
		Serial.print(".");
		delay(1000);
	}

	Neodisplay neodisp;

	neodisp.begin();
	neodisp.enableUTF8Print();

	Serial.println("Début :");

	neodisp.clearBuffer();				 // on efface ce qui se trouve déjà dans le buffer
	neodisp.setFont(u8g2_font_7x13B_tf); // choix d'une police de caractère
	neodisp.setCursor(25, 40);
	neodisp.print("Hello World  !");
	neodisp.sendBuffer();
	// delay(5000);
}

void loop()
{
	// disp.clearBuffer(); // on efface ce qui se trouve déjà dans le buffer
	// disp.drawXBMP(0, 0, alien_width, alien_height, alienA[i]);
	// disp.sendBuffer();
	delay(500);
	i = (i + 1) % 2;
}