// Name: Michael Goodrum

#include "myLib.h"
#include "student.h"
#include "math.h"
#include "bill1.h"
#include "bill2.h"

u16 *videoBuffer = (u16 *)0x6000000;

void rotateImageAndDraw(int width, int height, const unsigned short *image)
{
	int height2 = height;
	for (int x = 0; x <= height; x++) {
		DMA[3].src = image + ((height2) * width);
		DMA[3].dst = videoBuffer + OFFSET((x), 0, 240);
		DMA[3].cnt = DMA_ON | width | DMA_SOURCE_DECREMENT;
		height2--;
	}
}

void flipHorizontallyAndDraw(int width, int height, const unsigned short *image)
{
    for (int x = 0; x < height; x++) {
		DMA[3].src = image + ((x) * width) + 119;
		DMA[3].dst = videoBuffer + OFFSET((x), width, 240);
		DMA[3].cnt = DMA_ON | width | DMA_SOURCE_DECREMENT;
	}

}

void drawImage3(int row, int col, int width, int height, const unsigned short *image)
{
    for (int x = 0; x <= height; x++) {
		DMA[3].src = image + ((x) * width);
		DMA[3].dst = videoBuffer + OFFSET((row + x), col, 240);
		DMA[3].cnt = DMA_ON | DMA_SOURCE_INCREMENT | width;
	}
}


int main(void)
{
    // 4. TODO: You should probably do something here
    // so that whatever you want to draw actually gets drawn on the screen.

   REG_DISPCNT = MODE3 | BG2_ENABLE;

    
    waitForVblank();

    // 5. TODO: Call your functions appropriately here.
    // You want to draw the student image with the manipulation in the top left quadrant,
    // the math image with the manipulation in the top right quadrant,
    // the first Bill image in the bottom left quadrant,
    // and the second Bill image in the bottom right quadrant of the GBA screen.

    rotateImageAndDraw(STUDENT_WIDTH, STUDENT_HEIGHT, student_data);
    flipHorizontallyAndDraw(MATH_WIDTH, MATH_HEIGHT, math_data);
    drawImage3(80, 0, BILL1_WIDTH, BILL1_HEIGHT, bill1_data);
    drawImage3(80, 120, BILL2_WIDTH, BILL2_HEIGHT, bill2_data);
    
	




    // Wait after drawing
    while (1);
}


void waitForVblank()
{
    while(SCANLINECOUNTER > 160);
    while(SCANLINECOUNTER < 160);
}
