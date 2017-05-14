/* Asteroids Model */
typedef struct point coordinate_t;
typedef struct point vector_t;

struct point {
    float x,y;
};

/* Some insitial struct types if you want to usd them */
struct ship {
	float angle;
	coordinate_t p;
	coordinate_t shape[3];
	
	float acceleration;
	float drag;
	
	float speed;
	int shieldHealth;
  vector_t     v;
};

/* initial struts for building linked lists */
struct missile {
  point projectile[2];
	float speed;
	int timeToLive;
	point v;
  struct missile *next;
};

struct rock {
    coordinate_t p;
		int asteroid_width;
		int asteroid_height;
		vector_t v;
	  bool outOfGame;
    struct rock *next;
};

extern void createMissile(ship *player);
extern void rotateShip(ship *player, int angle);
extern void changeShipSpeed(ship *player, float speed);
extern missile *missileLst;
extern rock *rockLst;

void physics(void);
