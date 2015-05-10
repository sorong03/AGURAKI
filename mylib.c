#include "mylib.h"
u16 *videoBuffer = (u16*)0x6000000;


void setPixel(int row, int col, u16 color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}


void drawRect(int row, int col, int height, int width, u16 color)
{
    int r, c;
    for(r = 0; r < height; r++)
    {
        for(c = 0; c < width; c++)
        {
            setPixel(row+r, col+c, color);
        }
    }
}

void drawHollowRect(int row, int col, int height, int width, u16 color)
{
    int r, c;
    for(r = 0; r < height; r++)
    {
        for(c = 0; c < width; c++)
        {
            if(r == 0|| r == height-1 || c == 0 || c == width-1){
            setPixel(row+r, col+c, color);
            }
        }
    }

}

void waitForVblank()
{
	while(SCANLINECOUNTER >160);
	while(SCANLINECOUNTER < 160);
}
