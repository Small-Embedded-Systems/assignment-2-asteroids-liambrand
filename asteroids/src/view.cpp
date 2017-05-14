/* Asteroids view
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

#include "asteroids.h"
#include "model.h"
#include "utils.h"
#include "bitmaps.h"
#include "view.h"

extern "C" uint8_t bitflip(uint8_t);
static void initialiseScreen(void);
static void drawGameInfo(int score, int lives);
static void drawShip(ship player);
static void drawMissile(struct missile *m);
static void drawRocks(struct rock *r);
static void flipAsteroid();
void init_DBuffer(void);
void swap_DBuffer(void);
uint8_t bitflip(uint8_t);

static Display *graphics = Display::theDisplay();
static const colour_t background = rgb(0, 0, 0); /* Black */
static unsigned char flippedAsteroid[200];


//Draws the screen
void draw(void) {
	initialiseScreen();
	if(paused) {
		drawMenu();
	}
	else {
		drawGameInfo(score, lives); 
		drawShip(player);
		drawMissile(missileLst);
		drawRocks(rockLst);
	}
	swap_DBuffer();
}

//Sets up the screen
void initialiseScreen(void) {
	graphics->fillScreen(background);
	graphics->setTextColor(WHITE);
	graphics->setTextSize(1);
}

//Draws the main menu
void drawMenu(void) {
	graphics->setTextSize(5);
	graphics->setCursor(110, 30);
	graphics->printf("ASTEROIDS");
	
	graphics->setTextSize(2);
	graphics->setCursor(160, 80);
	graphics->printf("By Liam Brand");
	
	
	graphics->setTextSize(5);
	graphics->setCursor(50, 120);
	graphics->setTextSize(3);
	graphics->printf("Press USERBTN to start         a new game");
}

//Draws the in-game screen
void drawGameInfo(int score, int lives) {
	graphics->setTextSize(2);
	graphics->setCursor(410, 10);
	graphics->printf("SCORE");
	graphics->setCursor(435, 30);
	graphics->printf("%d", score);
	
	graphics->setCursor(410, 60);
	graphics->printf("LIVES");
	graphics->setCursor(435, 80);
	graphics->printf("%d", lives);
	
	graphics->drawLine(400, 0, 400, 300, WHITE);
}

//Draws the game's ship
void drawShip(ship player) {
	graphics->drawLine(player.shape[0].x, player.shape[0].y, player.shape[1].x, player.shape[1].y, WHITE);
	graphics->drawLine(player.shape[1].x, player.shape[1].y, player.shape[2].x, player.shape[2].y, WHITE);
	graphics->drawLine(player.shape[2].x, player.shape[2].y, player.shape[0].x, player.shape[0].y, WHITE);
	if(player.shieldHealth == 300) {graphics->drawCircle(player.p.x, player.p.y, 10, GREEN); }
	else if(player.shieldHealth == 200) {graphics->drawCircle(player.p.x, player.p.y, 10, YELLOW); }
	else if(player.shieldHealth == 100) {graphics->drawCircle(player.p.x, player.p.y, 10, RED); }
}

//Drawing missiles
void drawMissile(struct missile *m) {
	for( ; m ; m = m->next) {
			{graphics->drawLine(m->projectile[0].x, m->projectile[0].y, m->projectile[1].x, m->projectile[1].y, WHITE); }
	}
}

//Drawing the asteroids
void drawRocks(struct rock *r) {
	flipAsteroid();
	for( ; r ; r = r->next) {
		graphics->drawBitmap(r->p.x, r->p.y, flippedAsteroid, r->asteroid_width, r->asteroid_height, WHITE);
	}
}

//Draw the game over screen
void displayGameOver() {
	graphics->fillScreen(background);
	graphics->setTextSize(5);
	graphics->setCursor(90, 30);
	graphics->printf("GAME OVER");
	graphics->setTextSize(3);
	graphics->setCursor(50, 70);
	graphics->printf("Your score was ", "%d", score);
	
	graphics->setCursor(50, 120);
	graphics->printf("Press USERBTN to start         a new game");
}

//Flips the asteroid bits
void flipAsteroid() {
	int entry;
	for (entry = 0; entry < 200; entry++){
		flippedAsteroid[entry] = bitflip(asteroid_bits[entry]);
	}
}

/* double buffering functions */
void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}
