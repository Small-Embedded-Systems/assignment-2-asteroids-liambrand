/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"

/* Joystick 5-way switch variables and functions
*/
typedef enum {
JLT = 0, JRT, JUP, JDN, JCR
} btnId_t;
static DigitalIn jsBtns[] = {P5_0, P5_4, P5_2, P5_1, P5_3}; // LFT, RGHT, UP, DWN, CTR

void shipControls(struct ship *player);
void rotate(ship *player, int angle);
bool jsPrsd(btnId_t b);
bool jsPrsdAndRlsd(btnId_t b);


void controls(void) {	
	shipControls(&player);
}

void shipControls(struct ship *player) {
	if (!paused)	{
		if(jsPrsd(JUP)) {changeShipSpeed(player, 0.5);}
		else {changeShipSpeed(player, 0); }
		if(jsPrsd(JLT)) {rotateShip(player, 10); }
		if(jsPrsd(JRT)) {rotateShip(player, -10); }
		if(jsPrsdAndRlsd(JCR)) {createMissile(player); }
	}
}

//Test if joystick button is being pressed
bool jsPrsd(btnId_t b) {
	bool result = false;
	uint32_t state;
	state = jsBtns[b].read();
  if (state == 0) {
		result = true;
	}
	return result;
}

//Test if joystick button has been pressed and released
bool jsPrsdAndRlsd(btnId_t b) {
	bool result = false;
	uint32_t state;
	static uint32_t savedState[5] = {1,1,1,1,1};
        //initially all 1s: nothing pressed
	state = jsBtns[b].read();
  if ((savedState[b] == 0) && (state == 1)) {
		result = true;
	}
	savedState[b] = state;
	return result;
	
}
