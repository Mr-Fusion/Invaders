#ifndef DEFENDER_H_INCLUDED
#define DEFENDER_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdlib.h>
//#include <sstream>
#include "Const.h"
//#include "GameState.h"
#include "LTimer.h"
#include "LTexture.h"
#include "Bullet.h"

#define SHIP_HEIGHT		8
#define SHIP_WIDTH		60
#define VELOCITY		5

class Defender
{
    public:

    	SDL_Rect dim;
    	bool lInput, rInput, shInput;

    	Bullet peaShot;

    ///Constructor Function
    Defender(){

    	dim.h = SHIP_HEIGHT;
    	dim.w = SHIP_WIDTH;
    	dim.x = SCREEN_WIDTH/2 - SHIP_WIDTH/2;
    	dim.y = SCREEN_HEIGHT - SHIP_HEIGHT * 2;

    	lInput = false;
    	rInput = false;

    	shInput = false;


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
    ~Defender(){
        printf("Gamestate Object Deconstructing...\n");


    }


    //TODO: Can we streamline the sprite sheet creation into a function?
    bool loadMedia() {
        
        //Loading success flag
        bool success = true;

        return success;
    }


    ///Handles mouse event
    void handleEvent( SDL_Event* e){

        if (e->type == SDL_KEYDOWN) {
        	switch (e->key.keysym.sym) {
        		case SDLK_a:
        			lInput = true;
        		break;

        		case SDLK_d:
        			rInput = true;
        		break;

        		case SDLK_h:
        			shInput = true;
        		break;
        	}
        }


        if (e->type == SDL_KEYUP) {
        	switch (e->key.keysym.sym) {
        		case SDLK_a:
        			lInput = false;
        		break;

        		case SDLK_d:
        			rInput = false;
        		break;

        		case SDLK_h:
        			shInput = false;
        		break;
        	}
        }

        peaShot.handleEvent(e);
    }

    void logic(){

    	if (lInput)
    		dim.x -= VELOCITY;
    	if (rInput)
    		dim.x += VELOCITY;

    	if (dim.x < 0)
    		dim.x = 0;
    	if (dim.x > SCREEN_WIDTH - dim.w)
    		dim.x = SCREEN_WIDTH - dim.w;

    	if (shInput && !peaShot.active)
    		peaShot.fire(dim.x+dim.w/2,dim.y);

    	peaShot.logic();
    }

    void render(){

    	SDL_RenderFillRect(gRenderer, &dim);
    	peaShot.render();
    }

};
#endif // DEFENDER_H_INCLUDED