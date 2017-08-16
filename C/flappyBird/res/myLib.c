#include "myLib.h"
#include "text.h"
#include "../images/redBird.h"
#include <stdlib.h>

unsigned short *videoBuffer = (unsigned short *)0x6000000;




void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color)
{
	for (int x = 0; x < height; x++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET((row + x), col, 240)];
		DMA[3].cnt = DMA_SOURCE_FIXED | DMA_ON | width;
	}
}

void delay(int n)
{
	int volatile x = 0;
	for(int i =0; i<n*8000; i++)
	{
		x = x + 1;
	}
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER<159);
}

void drawImage3(int x, int y, int width, int height, const unsigned short *image) {
	
	for (int row = 0; row < height; row++) {
		DMA[3].src = image + ((row) * width);
		DMA[3].dst = videoBuffer + OFFSET((row + x), y, 240);
		DMA[3].cnt = DMA_ON | width | DMA_SOURCE_INCREMENT;
	}
}

void clearScreen() {
	volatile unsigned short s = 0;
	DMA[3].src = &s;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = DMA_SOURCE_FIXED | DMA_ON | DMA_SOURCE_INCREMENT | (240 * 160);
}

// method for converting Strings into ints
// i wrote this before the correction to allow us to use other methods for doing this

void tostring(char str[], int num) {

    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0) {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++) {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

// all the gameplay occurs in this method
// the bird creating and pillar creating as 
// well as the score displaying

int gamePlay() {
	FLAPPYBIRD bird;
	FLAPPYBIRD oldbird;
	REG_DISPCTL = MODE3 | BG2_ENABLE;


	int size = 15;
	int score = 0;	
	int tempScore = 0;	
	
	

	bird.row = 10;
	bird.col = 40;
	oldbird = bird;

	PILLAR enemy; 
	PILLAR oldEnemy; 
	enemy.row = 0;
	enemy.col = 200;
	enemy.height = 35;

	oldEnemy = enemy;

	PILLAR enemy2;
	PILLAR oldEnemy2;

	enemy2.col = 200;
	enemy2.height = 20;
	enemy2.row = 140;

	oldEnemy2 = enemy2;

	clearScreen();

	drawRect(enemy.row, enemy.col, enemy.height, 10, BLUE);


	while(1) // Game Loop
	{

		char points[500];


		// if you hit the bottom or the top game over

		if ((bird.row > 145) | (bird.row < 10) | (bird.col < 2) | (bird.col > 225)) {
			score = 0;
			tostring(points, score);
			return 2;
		}

		// if score over 900 you win

		if (score > 10) {
			score = 0;
			tostring(points, score);
			return 1;
		}

		// draws score

		drawString(10, 150, "Score: ", YELLOW);

		if (score == 0) {
			drawString(10, 190, "0", YELLOW);
		}


		if(KEY_DOWN_NOW(BUTTON_UP)) {
			bird.row = bird.row - 10;
			tempScore++;
			if (tempScore == 10) {
				score++;
				tostring(points, score);
				tempScore = 0;
				drawRect(10, 190, 10, 30, BLACK);
				drawString(10, 190, points, YELLOW);
			}
		}

		if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
			bird.col = bird.col + 1;
		}

		if(KEY_DOWN_NOW(BUTTON_LEFT)) {
			bird.col = bird.col - 1;
		}

		if(KEY_DOWN_NOW(BUTTON_DOWN)) {
			bird.row = bird.row + 1;
		}

		if(KEY_DOWN_NOW(BUTTON_SELECT)) {
			score = 0;
			tostring(points, score);
			return 0;
		}

		waitForVblank();

		// this erases the trails behind the bird and the pillars
			
		drawRect(oldbird.row, oldbird.col, size, size, BLACK);	

		drawRect(oldEnemy.row, oldEnemy.col, oldEnemy.height, 10, BLACK);

		oldEnemy = enemy;

		//draws top pillar
		drawRect(enemy.row, enemy.col, enemy.height, 10, BLUE);

		enemy.col--;	

		drawRect(oldEnemy2.row, oldEnemy2.col, oldEnemy2.height, 10, BLACK);

		oldEnemy2 = enemy2;

		// draw bottom pillar
		drawRect(enemy2.row, enemy2.col, enemy2.height, 10, BLUE);

		enemy2.col--;

		if (enemy.col < 0) {

			enemy.height = (rand() % 35) + 5;

			enemy2.height = (rand() % 35) + 5;
			enemy2.row = 160 - enemy2.height;

			enemy.col = 240;
			enemy2.col = 240;
		}
		
		drawImage3(bird.row, bird.col, REDBIRD_WIDTH, REDBIRD_HEIGHT, redBird);	
		oldbird = bird;
		bird.row = bird.row + 4;

		//colision logic

		if (((bird.col + 14) >= enemy.col) & (bird.row < enemy.height) & ((bird.col + 14) <= (enemy.col + 10))) {
			score = 0;
			tostring(points, score);
			return 2;
		}

		if (((bird.col + 14) >= enemy2.col) & ((bird.row + 14) >= (160 - enemy2.height)) & (bird.col <= (enemy.col + 10))) {
			score = 0;
			tostring(points, score);
			return 2;
		}

		
		drawString(10, 190, points, YELLOW);
		
	} // Game Loop
}
