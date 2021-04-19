#ifndef _NEODISPLAY_H_
#define _NEODISPLAY_H_
#include <U8g2lib.h>
#include <Arduino.h>
#include <stdint.h>

#define SDA 21
#define SCL 22

// #ifdef __cplusplus
// extern "C"
// {
// #endif

class Neodisplay : public U8G2
{
public:
	Neodisplay(const u8g2_cb_t *, uint8_t, uint8_t, uint8_t);
	Neodisplay(void);

	boolean isConnected(void);

private:
	boolean connected = false;
	void init(const u8g2_cb_t *, uint8_t, uint8_t, uint8_t);
};
// #ifdef __cplusplus
// }
// #endif

#endif