/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"

static void updateShip(ship *player);
static void shipMotion(ship *player);
static void createRocks(struct rock *r);
static void updateRocks(struct rock *r);
static void updateMissile(struct missile *m);
static void wrapRock(struct rock *r);
static void deleteMissile(struct missile *missiles);
static void deleteRock(struct rock *r);
static void detectMissileCollision(struct missile *m, struct rock *r);
static void detectRockShipCollision(struct ship *player, struct rock *r);
static struct point rotatePoint(struct point point, struct point origin, float angle);

static int activeRocks = 0;
struct missile *missileLst = NULL;
struct rock *rockLst = NULL;

void physics(void){
	if (!paused)	{
		updateShip(&player);
		updateMissile(missileLst);
		if(activeRocks < 10) {createRocks(rockLst); }
		updateRocks(rockLst);
		deleteMissile(missileLst);
		detectMissileCollision(missileLst, rockLst);
		detectRockShipCollision(&player, rockLst); 
		deleteRock(rockLst);
	}
}

void updateShip(ship *player) {
	shipMotion(player);
	
	//New ship coordinates
	player->shape[0].x = player->p.x + 10;
	player->shape[0].y = player->p.y;
	player->shape[1].x = player->p.x - 5;
	player->shape[1].y = player->p.y + 5;
	player->shape[2].x = player->p.x - 5;
	player->shape[2].y = player->p.y - 5; 
	
	//Rotating the ship based on the angle
	player->shape[0] = rotatePoint(player->shape[0], player->p, player->angle);
	player->shape[1] = rotatePoint(player->shape[1], player->p, player->angle);
	player->shape[2] = rotatePoint(player->shape[2], player->p, player->angle);
}

//Increasing/decreasing the ship's rotation variable
void rotateShip(ship *player, int angle) {
	player->angle += angle;
}

//Dealing with the ship's motion
void shipMotion(ship *player) {
	float xmovement = cos(radians(player->angle));
	float ymovement = sin(radians(player->angle));
	
	player->v.x = xmovement * player->speed;
	player->v.y = ymovement * player->speed;
	
	//Wrapping the ship
	if(player->p.x + player->v.x > 400) { player->p.x -= 400; }
	else if (player->p.x + player->v.x < 0) { player->p.x = 390; }
	else if (player->p.y + player->v.y > 280) { player->p.y = 0;}
	else if (player->p.y + player->v.y < 0) { player->p.y = 280;}
	else {
		player->p.x += player->v.x; 
		player->p.y += player->v.y;
	}
	
	//apply drag to the ship
	player->v.x = player->v.x - (player->v.x * 0.01);
	player->v.y = player->v.y - (player->v.y * 0.01);
	
}

//Changes the ship's speed
void changeShipSpeed(ship *player, float speed) {
	player->speed = speed;
}

//Adds a new missile to the linked list
void createMissile(ship *player) {
	//Creating the missile and initialising its variables
	missile * newMissile = (struct missile*)malloc(sizeof(struct missile));
	newMissile->speed = 0.75;
	newMissile->timeToLive = 1;
	newMissile->projectile[0].x = player->shape[0].x;
	newMissile->projectile[0].y = player->shape[0].y;
	
	float xmovement = cos(radians(player->angle));
	float ymovement = sin(radians(player->angle));
	
	//Missile's velocity
	newMissile->v.x = xmovement * newMissile->speed;
	newMissile->v.y = ymovement * newMissile->speed;

	//Position of the missile's edge
	newMissile->projectile[1].x = newMissile->projectile[0].x + 10*newMissile->v.x;
	newMissile->projectile[1].y = newMissile->projectile[0].y + 10*newMissile->v.y;
	
	//Adding the missile to the linked list
	if(newMissile) {
		newMissile->next = missileLst;
		missileLst = newMissile;
	}
}

//Creates the rocks
void createRocks(struct rock *r) {
	rock * newRock = (struct rock*)malloc(sizeof(struct rock));
	
	int startingPos = randrange(0, 5);
	switch(startingPos) {
		//Rock coming from the left
		case 1:
			newRock->p.x = 0;
			newRock->p.y = randrange(0, 270);
			if(newRock->p.y > 135) {newRock->v.y = - 0.1; }
			else newRock->v.y = 0.1;
			newRock->v.x = 0.1;
			break;
		//Rock coming from the top
		case 2:
			newRock->p.x = randrange(10, 370);
			newRock->p.y = 0;
			if(newRock->p.x > 185) {newRock->v.x = -0.1; }
			else newRock->v.x = 0.1;
			newRock->v.y = 0.1;
			break;
		//Rock coming from the right
		case 3:
			newRock->p.x = 360;
			newRock->p.y = randrange(10, 270);
			if(newRock->p.y > 135) {newRock->v.y = - 0.1; }
			else newRock->v.y = 0.1;
			newRock->v.x = -0.1;
			break;
		//Rock coming from the bottom
		case 4:
			newRock->p.x = randrange(10, 370);
			newRock->p.y = 280;
			if(newRock->p.x > 185) {newRock->v.x = -0.1; }
			else newRock->v.x = 0.1;
			newRock->v.y = -0.1;
			break;
	}
	newRock->asteroid_width = 27;
  newRock->asteroid_height = 25;
	newRock->outOfGame = false;
	
	if(newRock) {
		newRock->next = rockLst;
		rockLst = newRock;
	}
	activeRocks++;
}

//Updates the position of the asteroids
void updateRocks(struct rock *r) {
	for( ; r!=NULL ; r = r->next) {
		wrapRock(r);
		r->p.x += r->v.x;
		r->p.y += r->v.y;
	}
}

//Updates the missile position
void updateMissile(struct missile *m) {
	//Iterates through the missile list and updates the variables
	for( ; m!=NULL ; m = m->next) {
		if(m->projectile[0].x > 400 ||
			 m->projectile[0].x < 0 ||
			 m->projectile[0].y < 0 ||
			 m->projectile[0].y > 270) { m->timeToLive = 0; }
		else {
		m->projectile[0].x += m->v.x;
		m->projectile[0].y += m->v.y;
		m->projectile[1].x += m->v.x;
		m->projectile[1].y += m->v.y;
		}
	}
}

//Looks for collisions between missiles and rocks
void detectMissileCollision(struct missile *m, struct rock *r) {
	for( ; m!=NULL ; m = m->next) {
		for( ; r!=NULL ; r = r->next) {
			float distX = m->projectile[1].x - r->p.x;
			float distY = m->projectile[1].y - r->p.y;
			float dist = sqrt((distX * distX) + (distY * distY));
			
			
			//float collisionArea = ((m->projectile[0].x - r->p.x)*2) + ((m->projectile[0].y - r->p.y)*2) <= ((0.5+0.5)*2)
			if(dist <= (20)) {
				m->timeToLive = 0;
				r->outOfGame = true;
			}
		}
	}
}

//Detect collisions between rocks and the player ship
void detectRockShipCollision(struct ship *player, struct rock *r) {
	for( ; r!=NULL ; r = r->next) {
		//Calculating collision circles for the ship and the rocks
		float distX = player->p.x - r->p.x;
		float distY = player->p.y - r->p.y;
		float dist = sqrt((distX * distX) + (distY * distY));
		
		if(dist <= 20) {
			if(player->shieldHealth > 0) {
				player->shieldHealth -= 100; 
				r->outOfGame = true;
			}
			else {
				lives--;
				player->p.x = 200;
				player->p.y = 120;
				player->shieldHealth = 300;
				r->outOfGame = true;
			}
		}
	}
}

//Wraps the to appear at the other side of the screen and move appropriately
void wrapRock(struct rock *r) {
	if(r->p.x > 380) {
		r->p.x = 0;
	}
	else if (r->p.x < 0) {
		r->p.x = 380;
	}
	else if (r->p.y > 280) {
		r->p.y = 0;
	}
	else if (r->p.y < 0) {
		r->p.y = 270;
	}
}

//Deletes missiles that have expires or collided from the missile list
void deleteMissile(struct missile *m) {
	for( ; m!=NULL ; m = m->next) {
		//If the current node needs deleting, the next node's data is copied to it
		//and the next node is freed
		if(m->timeToLive == 0) {
			struct missile *toDelete;
			toDelete = m->next;
			m->projectile[0] = m->next->projectile[0];
			m->projectile[1] = m->next->projectile[1];
			m->speed = m->next->speed;
			m->timeToLive = m->next->timeToLive;
			m->v = m->next->v;
			m->next = m->next->next;
			free(toDelete);
		}
	}
}

//Deletes rocks that have either went out of bounds or been shot
void deleteRock(struct rock *r) {
	for( ; r!=NULL ; r = r->next) {
		if(r->next && r->next->outOfGame == true) {
			struct rock *toDelete;
			toDelete = r->next;
			r->next = toDelete->next;
			free(toDelete);
			activeRocks--;
		}
	}
}


//Rotates a point by a specific origin by a specified angle
struct point rotatePoint(struct point point, struct point origin, float angle) {
	float degrees = radians(angle);
	
	float sinFloat = sin(degrees);
	float cosFloat = cos(degrees);
	
	point.x -= origin.x;
	point.y -= origin.y;
	
	float xnew = point.x * cosFloat - point.y * sinFloat;
	float ynew = point.x * sinFloat + point.y * cosFloat;
	
	point.x = xnew + origin.x;
	point.y = ynew + origin.y;

	return point;
}

