/*
    License (OLC-3)
    ~~~~~~~~~~~~~~~

    Copyright 2018 - 2020 OneLoneCoder.com

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:

    1. Redistributions or derivations of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    2. Redistributions or derivative works in binary form must reproduce the above
    copyright notice. This list of conditions and the following    disclaimer must be
    reproduced in the documentation and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors may
    be used to endorse or promote products derived from this software without specific
    prior written permission.
*/


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <istream>
#include <sstream>

class Game : public olc::PixelGameEngine
{
    //int world[100*100];
    int *world;
    int *world_next;
    int X;
    int Y;
    bool is_active = false;
    olc::Pixel pix = olc::Pixel(33,70,100);
    bool startup = true;
    float kA;
    float kB;

public:
    Game(int inX, int inY, float ikA, float ikB)
    {
        sAppName = "Example";
        X = inX;
        Y = inY;
        world = new int[X*Y];
        world_next = new int[X*Y];
        kA = ikA;
        kB = ikB;

    }

private:
    int A(int x, int y)
    {
        return x+y*X;
    }

    int map255(float x, int u, int m){
        int r = (int) 255 * x/(u-m);
        if (r < 0 ) return 0;
        return r;
    }

    //void next_gen()
    //{
        //for (int x=1; x<X-1; x++)
            //for(int y=1; y<Y-1; y++)
            //{
                //int ls = sum(x,y);

                //if (world[A(x,y)] == 1)
                //{
                    //if (ls < 2)
                        //world_next[A(x,y)] = 0;
                    //else if(ls < 4)
                        //world_next[A(x,y)] = 1;
                    //else
                        //world_next[A(x,y)] = 0;
                //}
                //else
                    //if (ls == 3)
                        //world_next[A(x,y)] = 1;
            //}

        //for (int x=1; x<X-1; x++)
            //for(int y=1; y<Y-1; y++)
            //{
                //world[A(x,y)] += world_next[A(x,y)];
            //}
    //}

    ///////////////////////////////////////////////////////////////////////////
    void next_gen()
    {
        for (int x=1; x<X-1; x++)
            for(int y=1; y<Y-1; y++)
            {
                int t = world[A(x,y)];
                int l = world[A(x-1,y)];
                int p = world[A(x+1,y)];
                int u = world[A(x,y-1)];
                int d = world[A(x,y+1)];

                // przewodzenie
                float r = kB*((l-t)+(p-t)+(u-t)+(d-t));
                //float r = kB*(l+p+u+d)/(4*kB);

                // unoszenie
                r += kA * d;
                world_next[A(x,y)] = r;
                //world_next[A(x,y)] = 0.25*(l-t)+0.25*(p-t) +0.8*(u-t);
            }

        for (int x=1; x<X-1; x++)
            for(int y=1; y<Y-1; y++)
            {
                world[A(x,y)] = world_next[A(x,y)];
            }

        // fixed bottom elements
        for (int x = (X/2)-5; x < (X/2)+5; x++){
            for (int y = Y-20; y < Y-16; y++){
                world[A(x,y)] = 200;
                //world[A(x,y)] = rand() % 200;
            }
        }

    }
    ///////////////////////////////////////////////////////////////////////////

    void restart(){

        for (int x = 0; x < ScreenWidth(); x++){
            for (int y = 0; y < ScreenHeight(); y++){
                world[A(x,y)] = 0;
                world_next[A(x,y)] = 0;
            }
        }

        for (int x = (X/2)-5; x < (X/2)+5; x++){
            for (int y = Y-2; y < Y-1; y++){
                world[A(x,y)] = 200;
                //world[A(x,y)] = rand() % 200;
            }
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

    int aMax(int *ar, int els){
        int result=0;
        for(int i=0; i < els; i++){
            result = std::max(result, ar[i]);
        }
        return result;
    }



public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        restart();
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {

        if ( startup ){
            if (GetKey(olc::Key::SPACE).bPressed) startup = false;

                std::vector <std::string> main_text;

                main_text.push_back("Game of Life [C++]");
                main_text.push_back("by Tymancjo");
                main_text.push_back("powered by:");
                main_text.push_back("olcPixelGameEngine");
                main_text.push_back("keys:");
                main_text.push_back("SPACE - pause/resume");
                main_text.push_back("S - single generation");
                main_text.push_back("R - restart");
                main_text.push_back("C - change back color");
                main_text.push_back(" ");
                main_text.push_back(" ");
                main_text.push_back("World size: ");
                main_text.push_back(std::to_string( X * Y ));
                main_text.push_back("Press SPACE...");


                uint32_t text_size = 1 + X / 300;
                uint32_t text_sizeY = 1 + Y / (main_text.size() * 50 );
                text_size = std::min(text_size, text_sizeY);

            for (int k = 0; k < main_text.size(); k++){
                std::string this_line = main_text[k];
                int txtX = (X - 7 * text_size * this_line.length()) / 2;

                DrawString(txtX, 20 + k * 12 * text_size, this_line, olc::WHITE, text_size);
            }

        } else {
            bool step = false;
            // called once per frame

            //int maxOne=0;
            //for(int i=0; i < X*Y; i++){
                //maxOne = std::max(maxOne, world[i]);
            //}
            //maxOne = std::min(maxOne, 500);

            for (int x = 0; x < X; x++)
                for (int y = 0; y < Y; y++)
                {
                    int thisOne = world[A(x,y)];
                    if ( thisOne < 0)
                        Draw(x, y, pix);
                    else{
                        float pxC;
                        pxC = map255(thisOne,500,0);
                        //pxC = map255(thisOne,maxOne,0);
                        //std::cout << thisOne << std::endl;
                        olc::Pixel kolor =  olc::Pixel(pxC,0,0);
                        Draw(x, y, kolor);
                    }
                }

            // checking for user input
            if (GetKey(olc::Key::SPACE).bPressed) is_active = !is_active;
            if (GetKey(olc::Key::R).bPressed) restart();
            if (GetKey(olc::Key::S).bPressed) {
                step = true;
                is_active = false;
            }
            if (GetKey(olc::Key::C).bPressed)
            {
                int rc1 = rand() % 150;
                int rc2 = rand() % 150;
                int rc3 = rand() % 150;
                pix = olc::Pixel(rc1 / 3, rc2 * 2/3, rc3);
            }

            if (is_active || step){
                next_gen();
                step = false;
        }}

        return true;
    }
};


int main(int argc, char *argv[])
{
    int d = 3;
    int iW = 1920 / d;
    int iH = 1080 / d;
    float kA = 0.7f;
    float kB = 0.3f;

    if (argc > 3){
        // reading the line parameters and converting to int
        // based on the: https://stackoverflow.com/a/35206575/14836163
        std::istringstream iss( argv[1] );
        int val;
        if ((iss >> val) && iss.eof()){
            iW = val;

            std::istringstream iss2( argv[2] );
            if ((iss2 >> val) && iss2.eof()){
                iH = val;

                std::istringstream iss3( argv[3] );
                if ((iss3 >> val) && iss3.eof()){
                    d = val;
                    iW = iW / d;
                    iH = iH / d;
                    float fval;
                    std::istringstream iss3( argv[4] );
                    if ((iss3 >> fval) && iss3.eof()){
                       kA = fval; 

                        std::istringstream iss3( argv[5] );
                        if ((iss3 >> fval) && iss3.eof()){
                           kB = fval; 
                        }
                    }
                }
            }
        }
    }


    int size = iW * iH;
    std::cout << "[INFO] World size: " << size << std::endl;
    std::cout << "[INFO] kA: " << kA << std::endl;
    std::cout << "[INFO] kB: " << kB << std::endl;

    Game life(iW,iH, kA, kB);
    if (life.Construct(iW, iH, d, d))
        life.Start();

    return 0;
}
