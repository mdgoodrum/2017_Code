#include <stdlib.h>
#include "res/myLib.h"
#include "res/text.h"
//#include "redBird.h"
#include "images/flappyBird.h"
#include "images/gameOver.h"
enum GBAState {
	START,
	START_NODRAW,
	STATE_PLAY,
	STATE_GAMEOVER,
	STATE_GAMEOVER_NODRAW,
	STATE_WON,
	STATE_WON_NODRAW
};



int main()
{

	REG_DISPCTL = MODE3 | BG2_ENABLE;

	enum GBAState state = START;

	int counter = 0;

	unsigned short color = WHITE;

	int colorCounter = 0;

	while(1) {
		waitForVblank();
		switch(state) {
			case START:
				drawImage3(0, 0, FLAPPYBIRD_WIDTH, FLAPPYBIRD_HEIGHT, flappyBird);
				drawString(140, 85, "PRESS START", color);
				state = START_NODRAW;
				break;

			case START_NODRAW:
			// in no draw state the press start changes colors
				if (KEY_DOWN_NOW(BUTTON_START) && (counter == 0)) {
					counter = 1;
					state = STATE_PLAY;
					break;
				} else {
					colorCounter++;
					if ((colorCounter > 50) & (colorCounter < 100)) {
						color = YELLOW;
						drawString(140, 85, "PRESS START", color);
					} else if ((colorCounter > 100) & (colorCounter < 150)) {
						color = WHITE;
						drawString(140, 85, "PRESS START", color);
					} else if (colorCounter > 150) {
						colorCounter = 0;
					}
					state = START_NODRAW;
				}
				break;

			case STATE_PLAY:
			//game play returns different values
			//based on if you won lost or pressed shift
				counter = gamePlay();
				if (counter == 2) {
					state = STATE_GAMEOVER;
				} else if (counter == 1) {
					state = STATE_WON;
				} else if (counter == 0) {
					state = START;
				}
				break;

			case STATE_WON:
				clearScreen();
				drawString(20, 70, "CONGRATULATIONS!!!", WHITE);
				drawString(30, 45, "YOUR SCORE WAS OVER 100!!!", WHITE);
				drawString(120, 75, "PRESS SELECT TO", WHITE);
				drawString(130, 45, "TO RETURN TO TITLE SCREEN", WHITE);
				state = STATE_WON_NODRAW;
				break;

			case STATE_WON_NODRAW:
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					counter = 0;
					state = START;
					break;
				} else {
					state = STATE_WON_NODRAW;
				}
				break;



			case STATE_GAMEOVER:
				drawImage3(0, 0, GAMEOVER_WIDTH, GAMEOVER_HEIGHT, gameOver);
				drawString(120, 85, "PRESS SELECT", color);
				drawString(130, 45, "TO RETURN TO TITLE SCREEN", color);
				state = STATE_GAMEOVER_NODRAW;
				break;

			case STATE_GAMEOVER_NODRAW:
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					counter = 0;
					state = START;
					break;
				} else {
					colorCounter++;
					if ((colorCounter > 50) & (colorCounter < 100)) {
						color = YELLOW;
						drawString(120, 85, "PRESS SELECT", color);
						drawString(130, 45, "TO RETURN TO TITLE SCREEN", color);
					} else if ((colorCounter > 100) & (colorCounter < 150)) {
						color = WHITE;
						drawString(120, 85, "PRESS SELECT", color);
						drawString(130, 45, "TO RETURN TO TITLE SCREEN", color);
					} else if (colorCounter > 150) {
						colorCounter = 0;
					}
					state = STATE_GAMEOVER_NODRAW;
				}
				break;
		}
	}
}

