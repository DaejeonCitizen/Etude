#ifndef __SNAKE_GAME_H__
#define __SNAKE_GAME_H__

#define TRUE	1
#define FALSE	0
#define TILE_SIZE_X 30
#define TILE_SIZE_Y 20

#define EXT_KEY   0xffffffe0
#define KEY_LEFT  0x4b
#define KEY_RIGHT  0x4d
#define KEY_UP   0x48
#define KEY_DOWN  0x50
#define ESC 27
#define PAUSE 112

#define SPEED_INIT 200
typedef enum _status { 
	GO_UP,
	GO_DOWN,
	GO_LEFT,
	GO_RIGHT
} Status;

typedef enum _mapstatus {
	EMPTY,
	WALL,
	FOOD,
	BODY
} MapStatus;

typedef struct _score
{
	int cScore;
	int lScore;
	int tScore;
} Score;

typedef struct _pos
{
	int x;
	int y;
} Pos;

extern int g_speed;
extern int g_status;
extern int g_foodStatus;
extern int g_length;
extern int g_body_val;

void gotoxy(double x, double y, char * s);

void DrawWall(void);

void ShowTitle(void);

void ShowScore(Score * pscore);

void SetGame(int pMap[][TILE_SIZE_X], Pos * pSnkhead, Pos * pSnktail, Score * pscore);

void CreateFood(int pMap[][TILE_SIZE_X]);

int IsTouchBodyOrWall(int pMap[][TILE_SIZE_X], Pos * pSnkhead);

int EatFood(int pMap[][TILE_SIZE_X], Pos * pSnkhead);

void MoveBody(int pMap[][TILE_SIZE_X], Pos * pSnkhead, Pos * pSnktail);

void PauseGame(void);

void GameOver(Score * pscore);
#endif