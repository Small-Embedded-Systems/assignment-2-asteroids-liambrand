/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"

void initialiseGame(void);
void initialiseShip(void);
void gameOver(void);
void scoreTimer(void);


/* Game state */
float elapsed_time; 
int   score;
int   lives;
bool 	paused = false;
struct ship player;

float Dt = 0.01f;

Ticker model, view, controller, scoreTracker;

/* The single user button needs to have the PullUp resistor enabled */
DigitalIn userbutton(P2_10,PullUp);


int main() {
    init_DBuffer();
    view.attach(draw, 0.025);
    model.attach(physics, Dt);
    controller.attach(controls, 0.1);
		scoreTracker.attach(&scoreTimer, 1);
		while(userbutton.read()) {
					paused = true;
					wait_ms(100);
		}
		paused = false;
		initialiseGame();
		initialiseShip();
		while(!(lives < 0)) {
			wait_ms(200);
		}
		gameOver();
}

//Initialising game related variables
void initialiseGame() {
	score = 0;
  lives = 3;
}

//Initialising ship position and shield at the game start
void initialiseShip() {
	player.p.x = 200;
	player.p.y = 120;
	player.shieldHealth = 300;
}

//Resets the stats upon a game over
void gameOver() {
	displayGameOver();
	score = 0;
	lives = 3;
}

//Increments the score for each second the player's ship is alive
void scoreTimer() {
	if(!paused) {
		score++;
	}
}
