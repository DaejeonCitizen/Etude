#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include "SnakeGame.h"

int g_speed = SPEED_INIT;
int g_status = GO_UP;
int g_foodStatus = 0;
int g_length = 1;
int g_body_val = 9999;

int main(void)
{
	FILE * fp;
	Score score = { 0, 0, 0 };
	Pos snakehead, snaketail;
	int map[TILE_SIZE_Y][TILE_SIZE_X] = { EMPTY, };
	int key = 0;

	fopen_s(&fp, "record.dat", "a");
	if (fp == NULL) {
		puts("failed open top score");
	}
	fclose(fp);

	fopen_s(&fp, "record.dat", "r");
	if (fp == NULL) {
		puts("failed open top score");
	}
	fread((void *)&(score.tScore), sizeof(int), 1, fp);
	fclose(fp);

	while (1)
	{
		ShowTitle();
		SetGame(map, &snakehead, &snaketail, &score);
		while (1)
		{
			if (g_foodStatus == FALSE)
				CreateFood(map);

			Sleep(g_speed);

			if (_kbhit())
			{
				do
				{
					key = _getch();
				} while (key == 224);

				switch (key)
				{
				case KEY_UP:
					if (g_status == GO_DOWN)
						break;
					g_status = GO_UP;
					break;

				case KEY_DOWN:
					if (g_status == GO_UP)
						break;
					g_status = GO_DOWN;
					break;

				case KEY_LEFT:
					if (g_status == GO_RIGHT)
						break;
					g_status = GO_LEFT;
					break;

				case KEY_RIGHT:
					if (g_status == GO_LEFT)
						break;
					g_status = GO_RIGHT;
					break;
				case PAUSE:
					PauseGame();
					break;
				}
			}
			if (IsTouchBodyOrWall(map, &snakehead))
			{
				GameOver(&score);
				break;
			}
			if (EatFood(map, &snakehead))
			{
				score.cScore += 10;
				ShowScore(&score);
			}
			else
			{
				MoveBody(map, &snakehead, &snaketail);
			}

		}
	}

	return 0;
}