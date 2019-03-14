#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdlib.h>
//#include <sstream>
#include "Const.h"
//#include "GameState.h"
#include "LTimer.h"
#include "LTexture.h"

#define BULLET_HEIGHT		8
#define BULLET_WIDTH		2
#define VELOCITY            -10


class Bullet
{
    public:

    	SDL_Rect dim;

        int yVel;

        bool active;

    ///Constructor Function
    Bullet(){

    	dim.h = BULLET_HEIGHT;
    	dim.w = BULLET_WIDTH;
    	dim.x = SCREEN_WIDTH + 10;
    	dim.y = SCREEN_HEIGHT + 10;

        yVel = 0;
        active = false;

        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {


        }

    }

    ///Deconstructor
    ~Bullet(){
        printf("Gamestate Object Deconstructing...\n");


    }


    //TODO: Can we streamline the sprite sheet creation into a function?
    bool loadMedia() {
        
        //Loading success flag
        bool success = true;

        return success;
    }

    void fire(int x, int y) {
        dim.x = x;
        dim.y = y;
        yVel = VELOCITY;
        active = true;
    }


    ///Handles mouse event
    void handleEvent( SDL_Event* e){


    }

    void logic(){

        dim.y += yVel;

        if (dim.y < -10){
            yVel = 0;
            active = false;
        }

    }

    void render(){

    	SDL_RenderFillRect(gRenderer, &dim);
    }

};
#endif // BULLET_H_INCLUDED