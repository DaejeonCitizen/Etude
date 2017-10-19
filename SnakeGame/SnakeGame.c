#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include "SnakeGame.h"

#define STR_VIEW_CENTER(str) ((TILE_SIZE_X / 2) - ((double)strlen(str) / 4))
#define RAND_POS(x, y) ((x) = (rand() % TILE_SIZE_X)), ((y) = (rand() % TILE_SIZE_Y))

void RemoveCursor(void)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0; // change member value
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo); // set change value
}

/*
	help function
*/

void gotoxy(double x, double y, char * s) // move cursor and print string
{
	COORD pos = { (int)(2 * x), (int)y }; // 2x = secure field width
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", s);
}

void ShowScore(Score * pscore)
{
	gotoxy(0, 20, "Score: ");
	printf("%-5d   Last Score: %-5d   Top Score: %-5d",
		pscore->cScore, pscore->lScore, pscore->tScore);
}

void gotoxyPos(Pos * pos, char * str)
{
	gotoxy(pos->x, pos->y, str);
}

void DrawWallForMap(int pMap[][TILE_SIZE_X])
{
	int x = 0, y = 0;

	for (y = 0; y < TILE_SIZE_Y; y++)
	{
		pMap[y][x] = WALL; // make left line
		pMap[y][TILE_SIZE_X - 1] = WALL; // make right line

		if (y == 0 || y == TILE_SIZE_Y - 1) // make top and bottom line
		{
			for (x = 0; x < TILE_SIZE_X; x++)
			{
				pMap[y][x] = WALL;
			}
			x = 0; // init x for make left line(=next while)
		}
	}
}

void DrawWall(void)
{
	int x = 0, y = 0;

	for (y = 0; y < TILE_SIZE_Y; y++)
	{
		gotoxy(x, y, "¡á"); // make left line
		gotoxy(x + TILE_SIZE_X - 1, y, "¡á"); // make right line

		if (y == 0 || y == TILE_SIZE_Y - 1) // make top and bottom line
		{
			for (x = 0; x < TILE_SIZE_X; x++)
			{
				gotoxy(x, y, "¡á");
			}
			x = 0; // init x for make left line(=next while)
		}
	}
}

void MapInit(int pMap[][TILE_SIZE_X])
{
	int i;

	for (i = 0; i < TILE_SIZE_Y; i++)
		memset(pMap[i], EMPTY, sizeof(int) * TILE_SIZE_X);
}

/*
	main function
*/

void ShowTitle(void)
{
	int key = 0;

	RemoveCursor();
	system("mode con cols=60 lines=21"); // resizing window

	while (_kbhit())
		_getch();

	char * title_str = "S   N   A   K   E";
	char * start_str = "< PRESS ANY KEY START >";
	char * move_str = "Move: UP, DOWN, LEFT, RIGHT KEY";
	char * pause_str = "PAUSE: PRESS \"P\" KEY";
	char * exit_str = "EXIT: PRESS ESC KEY";

	DrawWall();

	gotoxy(STR_VIEW_CENTER(title_str), 3, title_str);
	gotoxy(STR_VIEW_CENTER(start_str), 9, start_str);
	gotoxy(STR_VIEW_CENTER(move_str), 11, move_str);
	gotoxy(STR_VIEW_CENTER(pause_str), 12, pause_str);
	gotoxy(STR_VIEW_CENTER(exit_str), 13, exit_str);

	while (!_kbhit()) // game start and blink
	{
		Sleep(200);
		gotoxy(STR_VIEW_CENTER(start_str), 9, "                       ");
		Sleep(200);
		gotoxy(STR_VIEW_CENTER(start_str), 9, start_str);
	}

	key = _getch();
	if (key == ESC)
		exit(0);
	
}

void SetGame(int pMap[][TILE_SIZE_X], Pos * pSnkhead, Pos * pSnktail, Score * pscore)
{
	Pos temp;
	// set score
	if (pscore->cScore > pscore->tScore)
		pscore->tScore = pscore->cScore;
	pscore->lScore = pscore->cScore;
	pscore->cScore = 0;

	// global set
	g_speed = SPEED_INIT;
	g_status = GO_UP;
	g_foodStatus = FALSE;
	g_body_val = 9999;

	system("cls");
	MapInit(pMap);
	DrawWall();
	ShowScore(pscore);
	DrawWallForMap(pMap); // screen and array sync

	// init head pos
	pSnkhead->x = TILE_SIZE_X / 2;
	pSnkhead->y = (TILE_SIZE_Y / 2);

	temp = *pSnkhead;

	pSnkhead->y += g_length;

	// draw body
	for (*pSnktail = *pSnkhead; pSnkhead->y != temp.y; (pSnkhead->y)--)
	{
		gotoxyPos(pSnkhead , "¡Ü");
		pMap[(pSnkhead->y)][pSnkhead->x] = g_body_val;
		g_body_val--;
	}
	(pSnkhead->y)++;
	g_body_val++;
}

void CreateFood(int pMap[][TILE_SIZE_X])
{
	srand((unsigned)time(NULL));
	int x = 0, y = 0;

	RAND_POS(x, y);

	while(pMap[y][x] != EMPTY)
		RAND_POS(x, y);
	
	pMap[y][x] = FOOD;
	gotoxy(x, y, "¡Û");

	g_foodStatus = TRUE;
}

int IsTouchBodyOrWall(int pMap[][TILE_SIZE_X], Pos * pSnkhead)
{
	switch (g_status)
	{
	case GO_UP:
		if ((pMap[(pSnkhead->y) - 1][pSnkhead->x] == WALL) || (pMap[(pSnkhead->y) - 1][pSnkhead->x] > BODY))
			return TRUE;
		break;
	case GO_DOWN:
		if ((pMap[(pSnkhead->y) + 1][pSnkhead->x] == WALL) || (pMap[(pSnkhead->y) + 1][pSnkhead->x] > BODY))
			return TRUE;
		break;
	case GO_LEFT:
		if ((pMap[pSnkhead->y][(pSnkhead->x) - 1] == WALL) || (pMap[pSnkhead->y][(pSnkhead->x) - 1] > BODY))
			return TRUE;
		break;
	case GO_RIGHT:
		if ((pMap[pSnkhead->y][(pSnkhead->x) + 1] == WALL) || (pMap[pSnkhead->y][(pSnkhead->x) + 1] > BODY))
			return TRUE;
		break;
	}

	return FALSE;
}

int EatFood(int pMap[][TILE_SIZE_X], Pos * pSnkhead)
{
	switch (g_status)
	{
	case GO_UP:
		if (pMap[(pSnkhead->y) - 1][pSnkhead->x] == FOOD)
		{
			g_body_val--;
			(pSnkhead->y)--;
			pMap[pSnkhead->y][pSnkhead->x] = g_body_val;
			gotoxyPos(pSnkhead, "¡Ü");
			g_foodStatus = FALSE;
			g_speed -= 3;
			return TRUE;
		}
		break;
	case GO_DOWN:
		if (pMap[(pSnkhead->y) + 1][pSnkhead->x] == FOOD)
		{
			g_body_val--;
			(pSnkhead->y)++;
			pMap[pSnkhead->y][pSnkhead->x] = g_body_val;
			gotoxyPos(pSnkhead, "¡Ü");
			g_foodStatus = FALSE;
			g_speed -= 3;
			return TRUE;
		}
		break;
	case GO_LEFT:
		if (pMap[pSnkhead->y][(pSnkhead->x) - 1] == FOOD)
		{
			g_body_val--;
			(pSnkhead->x)--;
			pMap[pSnkhead->y][pSnkhead->x] = g_body_val;
			gotoxyPos(pSnkhead, "¡Ü");
			g_foodStatus = FALSE;
			g_speed -= 3;
			return TRUE;
		}
		break;
	case GO_RIGHT:
		if (pMap[pSnkhead->y][(pSnkhead->x) + 1] == FOOD)
		{
			g_body_val--;
			(pSnkhead->x)++;
			pMap[pSnkhead->y][pSnkhead->x] = g_body_val;
			gotoxyPos(pSnkhead, "¡Ü");
			g_foodStatus = FALSE;
			g_speed -= 3;
			return TRUE;
			break;
		}
	}

	return FALSE;
}

void MoveBody(int pMap[][TILE_SIZE_X], Pos * pSnkhead, Pos * pSnktail)
{
	g_body_val--;

	switch (g_status)
	{
	case GO_UP:
		(pSnkhead->y)--;
		pMap[pSnkhead->y][pSnkhead->x] = g_body_val;
		break;
	case GO_DOWN:
		(pSnkhead->y)++;
		pMap[pSnkhead->y][pSnkhead->x] = g_body_val;
		break;
	case GO_LEFT:
		(pSnkhead->x)--;
		pMap[pSnkhead->y][pSnkhead->x] = g_body_val;
		break;
	case GO_RIGHT:
		(pSnkhead->x)++;
		pMap[pSnkhead->y][pSnkhead->x] = g_body_val;
		break;
	}
	gotoxyPos(pSnkhead, "¡Ü");

	if (pMap[(pSnktail->y) - 1][pSnktail->x] == (pMap[pSnktail->y][pSnktail->x] - 1))
	{
		pMap[pSnktail->y][pSnktail->x] = EMPTY;
		gotoxyPos(pSnktail, "  ");
		(pSnktail->y)--;
	}
	else if (pMap[(pSnktail->y) + 1][pSnktail->x] == (pMap[pSnktail->y][pSnktail->x] - 1))
	{
		pMap[pSnktail->y][pSnktail->x] = EMPTY;
		gotoxyPos(pSnktail, "  ");
		(pSnktail->y)++;
	}
	else if (pMap[pSnktail->y][(pSnktail->x) - 1] == (pMap[pSnktail->y][pSnktail->x] - 1))
	{
		pMap[pSnktail->y][pSnktail->x] = EMPTY;
		gotoxyPos(pSnktail, "  ");
		(pSnktail->x)--;
	}
	else if(pMap[pSnktail->y][pSnktail->x + 1] == (pMap[pSnktail->y][pSnktail->x] - 1))
	{
		pMap[pSnktail->y][pSnktail->x] = EMPTY;
		gotoxyPos(pSnktail, "  ");
		(pSnktail->x)++;
	}
}

void PauseGame(void)
{
	while (_kbhit())
		_getch();
	
	int i;

	gotoxy(9, 0, "                       ");
	gotoxy(11, 0, "P   A   U   S   E");

	while (!_kbhit())
	{
		Sleep(200);
		gotoxy(9, 0, "                       ");
		Sleep(200);
		gotoxy(11, 0, "P   A   U   S   E");
	}
	
	for (i = 0; i < TILE_SIZE_X; i++)
		gotoxy(i, 0, "¡á");
	while (_kbhit())
		_getch();
}

void GameOver(Score * pscore)
{
	while (_kbhit())
		_getch();

	FILE * fp_go;
	if (pscore->cScore > pscore->tScore)
	{
		pscore->tScore = pscore->cScore;
		fopen_s(&fp_go, "record.dat", "w");
		if (fp_go == NULL) {
			puts("failed open top score");
		}
		fwrite((void *)&(pscore->tScore), sizeof(int), 1, fp_go);
		fclose(fp_go);
	}

	ShowScore(pscore);

	char * gameover = "Game Over!";
	char * yourscore = "Your Score: ";
	char * topscore = "Top Score: ";
	char * backtitle = "PRESS ANY KEY BACK TO TITLE";

	gotoxy(STR_VIEW_CENTER("--------------------"), 7, "--------------------");
	gotoxy(STR_VIEW_CENTER("|                  |"), 8, "|                  |");
	gotoxy(STR_VIEW_CENTER("|                  |"), 9, "|                  |");
	gotoxy(STR_VIEW_CENTER("|                  |"), 10, "|                  |");
	gotoxy(STR_VIEW_CENTER("--------------------"), 11, "--------------------");

	gotoxy(STR_VIEW_CENTER(gameover), 8, gameover);
	gotoxy(11, 9, yourscore); printf("%d", pscore->cScore);
	gotoxy(11.5, 10, topscore); printf("%d", pscore->tScore);

	while (!_kbhit())
	{
		gotoxy(STR_VIEW_CENTER(backtitle), 13, backtitle);
		Sleep(200);
		gotoxy(STR_VIEW_CENTER(backtitle), 13, "                           ");
		Sleep(200);
	}
}