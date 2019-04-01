#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdlib.h>
//#include <sstream>
#include "Const.h"
//#include "GameState.h"
#include "LTimer.h"
#include "LTexture.h"

/*
#define BULLET_HEIGHT       8
#define BULLET_WIDTH        2
#define VELOCITY            -10
*/
class Bullet
{
    public:

    	SDL_Rect dim;

        SDL_Point vel;

    ///Constructor Function
    Bullet(SDL_Rect iDim, SDL_Point iVel){

        dim = iDim;
        vel = iVel; //Placeholder

    }

    ///Deconstructor
    ~Bullet(){
        printf("Bullet Object Deconstructing...\n");


    }

    SDL_Rect getDim() {
        return dim;
    }

    void hit() {
        //play sound effect or trigger more logic here
    }

    bool offscreen() {
        if (dim.y < 0)
            return true;
        if (dim.y > SCREEN_HEIGHT)
            return true;
        if (dim.x < 0)
            return true;
        if (dim.x > SCREEN_WIDTH)
            return true;
        return false;
    }

    void logic(){

        dim.y += vel.y;
        dim.x += vel.x;

    }

    void render(){

    	SDL_RenderFillRect(gRenderer, &dim);
    }

};
#endif // BULLET_H_INCLUDED