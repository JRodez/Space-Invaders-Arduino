#include <U8g2lib.h>
#include <Arduino.h>
#include <stdint.h>
#include "neodisplay.hpp"
#include <Wire.h>

#define I2C_ADDR_START 0x01
#define I2C_ADDR_STOP 0x7F

#define SH1106_N 2
const uint8_t SH1106_ADDRS[SH1106_N] = {0x3c, 0x3d};

Neodisplay::Neodisplay(const u8g2_cb_t *rotation, uint8_t scl, uint8_t sda, uint8_t reset = U8X8_PIN_NONE) : U8G2()
{
	// i2c_addr = I2C_ADDR_START;
	Serial.println("i2c scanner : ");
	Wire.begin();

	connected = false;
	for (uint8_t i2c_addr = I2C_ADDR_START; i2c_addr <= I2C_ADDR_STOP; i2c_addr++)
	{
		delay(20);

		Wire.beginTransmission(i2c_addr);
		delay(10);
		if (Wire.endTransmission() == 0)
		{
			Serial.print("Found address: ");
			Serial.print(i2c_addr, DEC);
			Serial.print(" (0x");
			Serial.print(i2c_addr, HEX);
			Serial.println(")");

			for (uint8_t i = 0; i < SH1106_N; i++)
			{
				if (i2c_addr == SH1106_ADDRS[i])
				{
					u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
					u8x8_SetPin_HW_I2C(getU8x8(), reset, scl, sda);
					connected = true;
				}
				if (connected)
					break;
			}
		}
		if (connected)
			break;
	}

	if (!connected)
	{
		Serial.println("No HardWare screen found");
		u8g2_Setup_null(&u8g2, rotation, u8x8_byte_empty, u8x8_dummy_cb);
	}
}

Neodisplay::Neodisplay() : U8G2()
{
	// i2c_addr = I2C_ADDR_START;
	Serial.println("i2c scanner : ");
	Wire.begin();

	connected = false;
	for (uint8_t i2c_addr = I2C_ADDR_START; i2c_addr <= I2C_ADDR_STOP; i2c_addr++)
	{
		Wire.beginTransmission(i2c_addr);
		if (Wire.endTransmission() == 0)
		{
			Serial.print("Found address: ");
			Serial.print(i2c_addr, DEC);
			Serial.print(" (0x");
			Serial.print(i2c_addr, HEX);
			Serial.println(")");

			for (uint8_t i = 0; i < SH1106_N; i++)
			{
				if (i2c_addr == SH1106_ADDRS[i])
				{
					u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
					u8x8_SetPin_HW_I2C(getU8x8(), U8X8_PIN_NONE, SCL, SDA);
					connected = true;
				}
				if (connected)
					break;
			}

			delay(20);
		}
		if (connected)
			break;
	}

	if (!connected)
	{
		Serial.println("No HardWare screen found");
		u8g2_Setup_null(&u8g2, U8G2_R0, u8x8_byte_empty, u8x8_dummy_cb);
	}
}

boolean Neodisplay::isConnected()
{
	return connected;
}

// void i2c_finder()
// {

// 	Serial.println("Searching for screen : ");

// 	for (uint8_t i2c_addr = I2C_ADDR_START; i2c_addr <= I2C_ADDR_STOP; i2c_addr++)
// 	{
// 		Wire.beginTransmission(i);
// 		if (Wire.endTransmission() == 0)
// 		{
// 			Serial.print("Found address: ");
// 			Serial.print(i, DEC);
// 			Serial.print(" (0x");
// 			Serial.print(i, HEX);
// 			Serial.println(")");
// 			delay(20); //  unneeded?
// 		}
// 	}
// }
