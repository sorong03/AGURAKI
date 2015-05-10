#include "mylib.h"
#include "text.h"
#include <stdlib.h>
#include <stdio.h>

#define NUMOBJS 8
#define MAXHEIGHT 149
int splash();
int controls();
void win();
int game();
void lose();

enum {SPLASH, CONTROLS, GAME, WIN, LOSE};

int main(void)
{
	int seed = 0;
	int state = SPLASH;
	while(1)
	{
		switch(state)
		{
		case SPLASH:
			seed = splash();
			state = CONTROLS;
			break;
		case CONTROLS:
			seed = controls();
			state = GAME;
			break;
		case GAME:
			state = game(seed);
			break;
		case WIN:
			win();
			state = SPLASH;
			break;
		case LOSE:
			lose();
			state = SPLASH;
			break;
		}
	}
}

int splash()
{
	int seed = 0;
	u16 *ptr = videoBuffer;
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	int i;
	for(i=0; i<38400; i++)
	{
		*ptr++ = RED;
	}
	drawString(70, 100, "AGURAKI", CYAN);
	drawString(80, 90, "Press Start", CYAN);
	while(!KEY_DOWN_NOW(BUTTON_START))
	{
		seed++;
	}
	while(KEY_DOWN_NOW(BUTTON_START));
	return seed;
}

int controls()
{
	int seed = 0;
	u16 *ptr = videoBuffer;
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	int i;
	for(i=0; i<38400; i++)
	{
		*ptr++ = BLACK;
	}
	drawString(5, 85, "HOW TO PLAY", YELLOW);
	drawString(20, 5, "You take control of a Hollow Rectang-", YELLOW);
	drawString(30, 5, "ular Ship and are trying to harvest", YELLOW);
	drawString(40, 5, "the energy of flying meteorites. ", YELLOW);
	drawString(50, 5, "You can harvest energy by switching", YELLOW);
	drawString(60, 5, "to the same color of the meteorite ", YELLOW);
	drawString(70, 5, "and colliding with them. However, you", YELLOW);
	drawString(80, 5, "will lose if you collide with a meteo-", YELLOW);
	drawString(90, 5, "rite of a different color.", YELLOW);
	drawString(110, 90, "CONTROLS", YELLOW);
	drawString(120, 5, "Up, Down, Left, Right - Move ship", YELLOW);
	drawString(130, 5, "A - Switch to RED, B- Switch to BLUE", YELLOW);
	drawString(145, 60, "Press Start to Play", YELLOW);
	while(!KEY_DOWN_NOW(BUTTON_START))
	{
		seed++;
	}
	while(KEY_DOWN_NOW(BUTTON_START));
	return seed;
}
void win()
{
    	int i;
	u16 *ptr = videoBuffer;
	for(i=0; i<38400; i++)
	{
		*ptr++ = BLUE;
	}
	drawString(10, 90, "YOU WIN!", YELLOW);
	drawString(30, 5, "You are now the strongest and baddest", YELLOW);
	drawString(40, 4, "hollow rectangular ship there ever was", YELLOW);
	drawString(145, 40, "Press Start to play again!", YELLOW);
	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}
void lose()
{
	u16 *ptr = videoBuffer;
	int i;
	for(i=0; i<38400; i++)
	{
		*ptr++ = RED;
	}
	drawString(10, 90, "YOU LOST!", BLACK);
	drawString(30, 5, "You weren't able to complete your", BLACK);
	drawString(40, 13, "mission, try again next time.", BLACK);
	drawString(145, 40, "Press start to play again!", BLACK);
	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}

int game(int seed)
{
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	int i;
	u16 *ptr = videoBuffer;
	for(i=0; i<38400; i++)
	{
		*ptr++ = BLACK;
	}

	char buffer[41];
	MAINRECT hero;
	MAINRECT oldhero;
	MAINRECT *herocur;
	MOVOBJ redMeteors[NUMOBJS];
	MOVOBJ oldRedMeteors[NUMOBJS];
	MOVOBJ blueMeteors[NUMOBJS];
	MOVOBJ oldBlueMeteors[NUMOBJS];
	MOVOBJ *redCur;
	MOVOBJ *blueCur;
	int score = 0;
	int herosize = 4;
	int size = 5;
	hero.size = herosize;
	hero.row = 140;
	hero.col = 120;
	hero.color = RED;
	oldhero = hero;
	int speeds[] = {-2, -1, 1, 2};
	srand(seed);
	for(i=0; i<NUMOBJS; i++)
	{
		int randRd = rand()%4;
		int randCd = rand()%4;
		redMeteors[i].row = rand()%100 + 1;
		redMeteors[i].col = rand()%239 + 1;
		redMeteors[i].rd = speeds[randRd];
		redMeteors[i].cd = speeds[randCd];
		redMeteors[i].size = size;
		redMeteors[i].color = RED;
		redMeteors[i].alive = 1;
		oldRedMeteors[i] = redMeteors[i];
	}
	for(i=0; i<NUMOBJS; i++)
	{
		int randRd = rand()%4;
		int randCd = rand()%4;
		blueMeteors[i].row = rand()%100 + 1;
		blueMeteors[i].col = rand()%239 + 1;
		blueMeteors[i].rd = speeds[randRd];
		blueMeteors[i].cd = speeds[randCd];
		blueMeteors[i].size = size;
		blueMeteors[i].color = BLUE;
		blueMeteors[i].alive = 1;
		oldBlueMeteors[i] = blueMeteors[i];
	}
	while(1)
	{
		if(KEY_DOWN_NOW(BUTTON_UP))
		{
			herocur-> row -= 2;
		}
		if(KEY_DOWN_NOW(BUTTON_DOWN))
		{
			herocur-> row += 2;
		}
		if(KEY_DOWN_NOW(BUTTON_LEFT))
		{
			herocur-> col -= 2;
		}
		if(KEY_DOWN_NOW(BUTTON_RIGHT))
		{
			herocur-> col += 2;
		}
		if(KEY_DOWN_NOW(BUTTON_A))
		{
			herocur-> color = RED;
		}
		if(KEY_DOWN_NOW(BUTTON_B))
		{
			herocur-> color = BLUE;
		}
		if(herocur-> row < 0)
		{
			herocur->row = 0;
		}
		if(herocur-> row > MAXHEIGHT-herocur->size+1)
		{
			herocur->row = MAXHEIGHT - herocur->size+1;
		}
		if(herocur-> col < 0)
		{
			herocur->col = 0;
		}
		if(herocur-> col > 240 - herocur->size)
		{
			herocur->col = 240 - herocur->size;
		}

		for(i=0; i<NUMOBJS; i++) //update position for red meteors
		{
			redCur = redMeteors + i;
			redCur->row = redCur->row + redCur->rd;
			redCur->col += redCur->cd;
			if(redCur->row<0)
			{
				redCur->row = 0;
				redCur->rd = -redCur->rd;
			}
			if(redCur->row>MAXHEIGHT-size+1)
			{
				redCur->row = MAXHEIGHT-size+1;
				redCur->rd = -redCur->rd;
			}
			if(redCur->col<0)
			{
				redCur->col = 0;
				redCur->cd = -redCur->cd;
			}
			if(redCur->col>239-size+1)
			{
				redCur->col = 239-size+1;
				redCur->cd = -redCur->cd;
			}
			if(!(redCur->row > hero.row + hero.size || redCur->row + redCur->size < hero.row ||
			     redCur-> col > hero.col + hero.size || redCur -> col + redCur->size < hero.col)
			     && redCur->color == hero.color && redCur->alive == 1)
			{
				score++;
				redCur->alive = 0;
				hero.size++;
			}
			else if(!(redCur->row > hero.row + hero.size || redCur->row + redCur->size < hero.row ||
			     redCur-> col > hero.col + hero.size || redCur -> col + redCur->size < hero.col)
			     && redCur->color != hero.color && redCur-> alive == 1)
			{
				return LOSE;
			}
	
		}

		for(i=0; i<NUMOBJS; i++) //update position for blue meteors
		{
			blueCur = blueMeteors + i;
			blueCur->row = blueCur->row + blueCur->rd;
			blueCur->col += blueCur->cd;
			if(blueCur->row<0)
			{
				blueCur->row = 0;
				blueCur->rd = -blueCur->rd;
			}
			if(blueCur->row>MAXHEIGHT-size+1)
			{
				blueCur->row = MAXHEIGHT-size+1;
				blueCur->rd = -blueCur->rd;
			}
			if(blueCur->col<0)
			{
				blueCur->col = 0;
				blueCur->cd = -blueCur->cd;
			}
			if(blueCur->col>239-size+1)
			{
				blueCur->col = 239-size+1;
				blueCur->cd = -blueCur->cd;
			}
			if(!(blueCur->row > hero.row + hero.size || blueCur->row + blueCur->size < hero.row ||
			     blueCur-> col > hero.col + hero.size || blueCur -> col + blueCur->size <hero.col)
			     && blueCur->color == hero.color && blueCur->alive == 1)
			{
				score++;
				blueCur->alive = 0;
				hero.size++;
			}
			else if(!(blueCur->row > hero.row + hero.size || blueCur->row + blueCur->size < hero.row ||
			     blueCur-> col > hero.col + hero.size || blueCur -> col + blueCur->size < hero.col)
			     && blueCur->color != hero.color && blueCur->alive == 1)
			{
				return LOSE;
			}
		}
		sprintf(buffer, "Score: %d", score);
		waitForVblank();
		drawRect(150, 5, 10, 72, BLACK);
		drawString(150, 5, buffer, MAGENTA);
		herocur = &hero;
		drawHollowRect(oldhero.row, oldhero.col, oldhero.size, oldhero.size, BLACK);
		drawHollowRect(herocur->row, herocur->col, herocur->size, herocur->size, herocur->color);
		oldhero = hero;
		for(i=0; i<NUMOBJS; i++)
		{	
			if(oldRedMeteors[i].alive == 1)
			drawRect(oldRedMeteors[i].row, oldRedMeteors[i].col, size, size, BLACK);
		}
		for(i=0; i<NUMOBJS; i++)
		{
			redCur = redMeteors + i;
			if(redCur->alive == 1)
			drawRect(redCur->row, redCur->col, size, size, redCur->color);
			oldRedMeteors[i] = redMeteors[i];
		}

		for(i=0; i<NUMOBJS; i++)
		{
			if(oldBlueMeteors[i].alive == 1)
			drawRect(oldBlueMeteors[i].row, oldBlueMeteors[i].col, size, size, BLACK);
		}
		for(i=0; i<NUMOBJS; i++)
		{
			blueCur = blueMeteors + i;
			if(blueCur->alive == 1)
			drawRect(blueCur->row, blueCur->col, size, size, blueCur->color);
			oldBlueMeteors[i] = blueMeteors[i];
		}
		if(score == NUMOBJS*2)
		{
			return WIN;
		}
	}
}



