#ifndef ALIEN_H_INCLUDED
#define ALIEN_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdlib.h>
//#include <sstream>
#include "Const.h"
//#include "GameState.h"
#include "LTimer.h"
#include "LTexture.h"

#define ALIEN_SS_COLS		1
#define ALIEN_SS_ROWS		2

#define ALIEN1_WIDTH		36
#define ALIEN1_HEIGHT		28

#define ALIEN_SPRITE_NUM	2


class Alien 
{
    public:

    	SDL_Rect dim;

    	LTexture ss_alien1;
    	SDL_Rect SprClipsAlien1[ ALIEN_SPRITE_NUM ];

    	bool pose;
        bool isHit;

   		int crawlSpeed;
   		int dir;

   		LTimer delayTimer;


    ///Constructor Function
    Alien(){

    	dim.h = ALIEN1_HEIGHT;
    	dim.w = ALIEN1_WIDTH;

        setPos(SCREEN_WIDTH/8, SCREEN_HEIGHT/8);


		pose = 0;
        isHit = false;

		crawlSpeed = 10;
		dir = 1;


        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
        	delayTimer.start();
        }

    }

    ///Deconstructor
    ~Alien(){
        printf("Gamestate Object Deconstructing...\n");

        ss_alien1.free();

    }


    //TODO: Can we streamline the sprite sheet creation into a function?
    bool loadMedia() {
        
        //Loading success flag
        bool success = true;

        //Load sprite sheet texture
        if( !ss_alien1.loadFromFile( "../assets/ss_alien1_x2.png") )
        {
            printf( "Failed to load alien1 sprite sheet texture!\n" );
            success = false;
        }
        else
        {
            int n = 0;
            for (int i = 0; i < ALIEN_SS_COLS; i++){
                for (int j = 0; j < ALIEN_SS_ROWS; j++){
                    SprClipsAlien1[ n ].x = ALIEN1_WIDTH * i;
                    SprClipsAlien1[ n ].y = ALIEN1_HEIGHT * j;
                    SprClipsAlien1[ n ].w = ALIEN1_WIDTH;
                    SprClipsAlien1[ n ].h = ALIEN1_HEIGHT;
                    n++;
                }
            }
        }

        return success;
    }

    void setPos(int x, int y){
        dim.x = x;
        dim.y = y;
    }

    void getHit() {
        die();
        isHit = false;
    }

    void die(){
        int x = rand() % (SCREEN_WIDTH - ALIEN1_WIDTH - 10);
        int y = rand() % (SCREEN_HEIGHT - ALIEN1_HEIGHT - 40);
        setPos(x,y);
    }

    ///Handles input event
    void handleEvent( SDL_Event* e){

    }

    void logic(){

        if (delayTimer.getTicks() > 200){
            delayTimer.stop();
            dim.x += crawlSpeed * dir;
            pose = !pose;
            if (dim.x > SCREEN_WIDTH - ALIEN1_WIDTH ){
            	dim.x = SCREEN_WIDTH - ALIEN1_WIDTH;
            	dim.y += crawlSpeed;
            	dir *= -1;
            }
            if (dim.x < 0){
            	dim.x = 0;
            	dim.y += crawlSpeed;
            	dir *= -1;
            }
            delayTimer.start();
        }

        if(isHit){
            delayTimer.stop();
            die();
        }

    }

    //Shows alien sprite
    void render(){
        //Show current sprite
        ss_alien1.render( dim.x, dim.y, &SprClipsAlien1[ pose ] );
    }

};
#endif // ALIEN_H_INCLUDED