#include <Arduino.h>
#include <U8g2lib.h>
#include <HardwareSerial.h>
#include "sprites/sprites.h"
#define SDA 21
#define SCL 22
// #define U8G2_128X64_CONSTRUCTOR U8G2_SSD1306_128X64_NONAME_F_HW_I2C
#define U8G2_128X64_CONSTRUCTOR U8G2_SH1106_128X64_NONAME_F_HW_I2C

U8G2_128X64_CONSTRUCTOR disp(U8G2_R0, U8X8_PIN_NONE, SCL, SDA); // OLEDs without Reset of the Display

void setup()
{
	Serial.begin(115200);

	disp.begin();
	disp.enableUTF8Print();
	disp.setFont(u8g2_font_7x13B_tf); // choix d'une police de caractère
	disp.clearBuffer();				  // on efface ce qui se trouve déjà dans le buffer

	Serial.println("Début :");

	disp.setCursor(25, 30);
	disp.print("Hello World!");
	disp.sendBuffer();

	delay(5000);
	disp.clearDisplay();
}

void loop()
{
	// disp.clearBuffer(); // on efface ce qui se trouve déjà dans le buffer
	// disp.drawXBMP(0, 0, alien_width, alien_height, alienA[i]);
	// disp.sendBuffer();
	disp.clearBuffer(); // on efface ce qui se trouve déjà dans le buffer
	// disp.setCursor(random(0, 110), random(0, 56));
	disp.setCursor(10, 10);
	disp.print(random(0, 100));
	disp.sendBuffer();
	delay(500);
}