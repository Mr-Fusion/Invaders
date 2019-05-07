#ifndef UFO_H_INCLUDED
#define UFO_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
//#include <sstream>
#include "Const.h"
//#include "GameState.h"
#include "LTimer.h"
#include "LTexture.h"

#define UFO_SS_COLS		1
#define UFO_SS_ROWS		6

#define UFO_WIDTH		64
#define UFO_HEIGHT		28

#define UFO_ALTITUDE    50

#define UFO_SPRITE_NUM	6


class Ufo 
{
    public:

    	SDL_Rect dim;

        SDL_Point vel;

        LTexture ss_ufo;
    	SDL_Rect SprClipsUfo[ UFO_SPRITE_NUM ];

    	bool pose;
        int frame;

        bool fReverse;

   		int crawlSpeed;

        Mix_Chunk *siren = NULL;
        Mix_Chunk *alienHitB = NULL;

        int channel;

        //int timeDbg;

   		//LTimer delayTimer;


    ///Constructor Function
    Ufo(int speed){

        vel.x = speed;
        vel.y = 0;

    	dim.h = UFO_HEIGHT;
    	dim.w = UFO_WIDTH;

        if (vel.x == 0)
            vel.x = 1;

        if (vel.x > 0){
            dim.x = -dim.w + 1;
        }
        else if (vel.x < 0){
            dim.x = SCREEN_WIDTH - 1;
        }

        dim.y = UFO_ALTITUDE;

        frame = 0;
        //timeDbg = 0;


        //Load media
        if( !loadMedia() )
            printf( "Failed to load media!\n" );
        else
        	channel = Mix_PlayChannel( -1, siren, -1 );

    }

    ///Deconstructor
    ~Ufo(){
        printf("Alien Object Deconstructing...\n");

        //Play sound when destroyed
        Mix_PlayChannel( -1, alienHitB, 0 );

        ss_ufo.free();

        Mix_HaltChannel(channel);
        Mix_FreeChunk( siren );
        siren = NULL;
        Mix_FreeChunk( alienHitB );
        alienHitB = NULL;

    }


    //TODO: Can we streamline the sprite sheet creation into a function?
    bool loadMedia() {
        
        //Loading success flag
        bool success = true;

        //Load sprite sheet texture
        if( !ss_ufo.loadFromFile( "../assets/ss_ufo_x4.png") )
        {
            printf( "Failed to load ufo sprite sheet texture!\n" );
            success = false;
        }
        else
        {
            int n = 0;
            for (int i = 0; i < UFO_SS_COLS; i++){
                for (int j = 0; j < UFO_SS_ROWS; j++){
                    SprClipsUfo[ n ].x = UFO_WIDTH * i;
                    SprClipsUfo[ n ].y = UFO_HEIGHT * j;
                    SprClipsUfo[ n ].w = UFO_WIDTH;
                    SprClipsUfo[ n ].h = UFO_HEIGHT;
                    n++;
                }
            }
        }

        siren = Mix_LoadWAV( "../assets/sfx_ufo.wav" );
        if( siren == NULL )
        {
            printf( "Failed to load ufo sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }

        alienHitB = Mix_LoadWAV( "../assets/sfx_alien_hitB.wav" );
        if( alienHitB == NULL )
        {
            printf( "Failed to load alien hitB sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }

        return success;
    }

    void setPos(int x, int y){
        dim.x = x;
        dim.y = y;
    }

    void setVel(int x, int y){
        vel.x = x;
        vel.y = y;
    }

    bool checkCollision( SDL_Rect foreignObj){
        if (dim.y > ( foreignObj.y + foreignObj.h ) )
            return false;

        if (dim.y + dim.h < foreignObj.y)
            return false;
                
        if (dim.x + dim.w < foreignObj.x)
            return false;
                    
        if (dim.x > foreignObj.x + foreignObj.w)
            return false;

        return true;

    }

    bool offScreen() {
        if (dim.y + dim.h < 0)
            return true;
        if (dim.y > SCREEN_HEIGHT)
            return true;
        if (dim.x + dim.w < 0)
            return true;
        if (dim.x > SCREEN_WIDTH)
            return true;
        return false;
    }

    void move() {
        dim.x += vel.x;
        dim.y += vel.y;
    }

    void cycleFrame() {
        frame++;
        if (frame == UFO_SPRITE_NUM)
            frame = 0;
    }

    //Shows alien sprite
    void render(){
        //Show current sprite
        ss_ufo.render( dim.x, dim.y, &SprClipsUfo[ frame ] );
    }

};
#endif // UFO_H_INCLUDED