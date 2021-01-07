#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>

class Game : public olc::PixelGameEngine
{
    //int world[100*100];
    int *world;
    int *world_next;
    int X;
    int Y;
    
public:
	Game(int inX, int inY)
	{
		sAppName = "Example";
        X = inX;
        Y = inY;
        world = new int[X*Y];
        world_next = new int[X*Y];
	}

private:
    int A(int x, int y)
    {
        return x+y*X;
    }

    void next_gen()
    {
        for (int x=1; x<X-1; x++)
            for(int y=1; y<Y-1; y++)
            {
                int ls = sum(x,y);

                if (world[A(x,y)] == 1)
                {
                    if (ls < 2)
                        world_next[A(x,y)] = 0;
                    else if(ls < 4) 
                        world_next[A(x,y)] = 1;
                    else
                        world_next[A(x,y)] = 0;
                }
                else
                    if (ls == 3)
                        world_next[A(x,y)] = 1;
            }

        for (int x=1; x<X-1; x++)
            for(int y=1; y<Y-1; y++)
            {
                world[A(x,y)] = world_next[A(x,y)];
            }
    }

    int sum(int x, int y)
    {
        int local = world[A(x,y)];
        int summ = 0;
        
        for (int i=x-1; i < x+2; i++)
            for (int j=y-1; j < y+2; j++)
                summ += world[A(i,j)];
        
        summ -= local;
        return summ;
    }



public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

        for (int x = 0; x < ScreenWidth(); x++){
            for (int y = 0; y < ScreenHeight(); y++){
                world[A(x,y)] = 0; 
                world_next[A(x,y)] = 0; 
            }
        }

        for (int x = 1; x < X-1; x++){
            for (int y = 1; y < Y-1; y++){
                world[A(x,y)] = rand() % 2;
            }
        }
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
        for (int x = 0; x < X; x++)
            for (int y = 0; y < Y; y++)
            {
                if ( world[A(x,y)] == 0)
                    Draw(x, y, olc::Pixel(33,70,100));
                else
                    Draw(x, y, olc::Pixel(255,255,255));
            }

        next_gen();

		return true;
	}
};


int main()
{
    const int d = 4;
    const int iW = 1920 / d;
    const int iH = 1080 / d;

    int size = iW * iH;
    std::cout << size;

	Game life(iW,iH);
	if (life.Construct(iW, iH, d, d))
		life.Start();

	return 0;
}
