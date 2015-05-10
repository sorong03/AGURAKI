#include "mylib.h"
u16 *videoBuffer = (u16*)0x6000000;


void setPixel(int row, int col, u16 color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}


/*void drawRect(int row, int col, int height, int width, u16 color)
{
    int r, c;
    for(r = 0; r < height; r++)
    {
        for(c = 0; c < width; c++)
        {
            setPixel(row+r, col+c, color);
        }
    }
}*/
void drawRect(int row, int col, int height, int width, volatile u16 color)
{
	int r;
	for(r=0; r<height; r++)
	{
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row+r, col, 240)];
		DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;	
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

void fillScreen(volatile u16 color)
{
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240*160) | DMA_SOURCE_FIXED | DMA_ON;
}

void drawPicture(const u16 arr[])
{
	DMA[3].src = arr;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240*160) | DMA_ON;
}

void drawImage3(int row, int col, int width, int height, const u16* image)
{
	int r;
	for(r=0; r<height; r++)
	{
		DMA[3].src = &image[OFFSET(row+r,col,240)];
		DMA[3].dst = &videoBuffer[OFFSET(row+r, col, 240)];
		DMA[3].cnt = width | DMA_ON;	
	}
}
