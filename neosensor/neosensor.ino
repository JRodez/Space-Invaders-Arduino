#include <Arduino.h>
#include <U8g2lib.h>
#include <HardwareSerial.h>
#include "sprites/sprites.h"

#define SDA 21
#define SCL 22

U8G2_SH1106_128X64_NONAME_F_HW_I2C disp(U8G2_R0, U8X8_PIN_NONE, SCL, SDA); // OLEDs without Reset of the Display

void setup()
{
	Serial.begin(115200);

	while (!Serial.available())
	{
		Serial.print(".");
		delay(1000);
	}
	Serial.println();

	disp.begin();
	disp.enableUTF8Print();
	Serial.println("Début :");

	disp.clearBuffer();				  // on efface ce qui se trouve déjà dans le buffer
	disp.setFont(u8g2_font_7x13B_tf); // choix d'une police de caractère
	disp.setCursor(25, 40);
	disp.print("Hello World!");
	disp.sendBuffer();
	delay(5000);
}

uint8_t i = 0;
void loop()
{
	disp.clearBuffer(); // on efface ce qui se trouve déjà dans le buffer
	disp.drawXBMP(0, 0, alien_width, alien_height, alienA[i]);
	disp.sendBuffer();
	delay(500);
	i = (i + 1) % 2;
}