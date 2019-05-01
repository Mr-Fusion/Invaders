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

#define ALIEN1_WIDTH        36
#define ALIEN1_HEIGHT       24

#define ALIEN2_WIDTH        33
#define ALIEN2_HEIGHT       24

#define ALIEN3_WIDTH        24
#define ALIEN3_HEIGHT       24

#define BULLET_HEIGHT       16
#define BULLET_WIDTH        16
#define BULLET_VELOCITY     4

#define ALIEN_SPRITE_NUM	2

#define HORIZONTAL_MARGIN   16

LTexture iTexture_A, iTexture_B, iTexture_C;

bool loadAlienTextures() {

    bool success = true;
    
    if (!iTexture_A.hasTexture()){
        if( !iTexture_A.loadFromFile( "../assets/ss_alien2_x3.png") )
        {
            printf( "Failed to load alien1 sprite sheet texture!\n" );
            success = false;
        }
    }

    if (!iTexture_B.hasTexture()){
        if( !iTexture_B.loadFromFile( "../assets/ss_alien1_x3.png") )
        {
            printf( "Failed to load alien2 sprite sheet texture!\n" );
            success = false;
        }
    }

    if (!iTexture_C.hasTexture()){
        if( !iTexture_C.loadFromFile( "../assets/ss_alien3_x3.png") )
        {
            printf( "Failed to load alien3 sprite sheet texture!\n" );
            success = false;
        }
    }
}

class Alien 
{
    public:

    	SDL_Rect dim, bulDim;

        SDL_Point vel, bulVel;

        LTexture *ss_alien;
    	SDL_Rect SprClipsAlien1[ ALIEN_SPRITE_NUM ];

    	bool pose;
        int frame;

    ///Deconstructor
    ~Alien(){
        printf("Alien Object Deconstructing...\n");

        ss_alien = NULL;
        delete ss_alien;


    }

    void commonInit() {
        dim.h = ALIEN1_HEIGHT;
        dim.w = ALIEN1_WIDTH;
        dim.x = 0;
        dim.y = 0;

        vel.x = 0;
        vel.y = 0;

        bulDim.h = BULLET_HEIGHT;
        bulDim.w = BULLET_WIDTH;
        bulDim.x = 0;
        bulDim.y = 0;

        bulVel.x = 0;
        bulVel.y = BULLET_VELOCITY;

        setPos(SCREEN_WIDTH/8, SCREEN_HEIGHT/8);

        frame = 0;

        loadAlienTextures();
    }

    //TODO: Can we streamline the sprite sheet creation into a function?
    bool createSprites() {
        
        //Loading success flag
        bool success = true;

        //Load sprite sheet texture
        //if( !ss_alien.loadFromFile( "../assets/ss_alien_x2.png") )
        if (ss_alien == NULL)
        {
            printf( "Failed to load alien1 sprite sheet texture!\n" );
            success = false;
        }
        else
        {
            int n = 0;
            for (int i = 0; i < ALIEN_SS_COLS; i++){
                for (int j = 0; j < ALIEN_SS_ROWS; j++){
                    SprClipsAlien1[ n ].x = dim.w * i;
                    SprClipsAlien1[ n ].y = dim.h * j;
                    SprClipsAlien1[ n ].w = dim.w;
                    SprClipsAlien1[ n ].h = dim.h;
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

    void setVel(int x, int y){
        vel.x = x;
        vel.y = y;
    }

    SDL_Rect getDim() {
        return dim;
    }

    Bullet* shoot() {
        bulDim.x = dim.x + dim.w/2;
        bulDim.y = dim.y;
        return new Bullet(bulDim,bulVel);
        //play sound effect?
        //graphical effects?
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

    void move() {
        dim.x += vel.x;
        dim.y += vel.y;
    }

    void cycleFrame() {
        frame++;
        if (frame == ALIEN_SPRITE_NUM)
            frame = 0;
    }

    bool atRightEdge() {
        if (dim.x + dim.w > SCREEN_WIDTH -  HORIZONTAL_MARGIN )
            return true;
        else return false;
    }

    bool atLeftEdge(){
        if (dim.x < HORIZONTAL_MARGIN)
            return true;
        else return false;
    }

    //Shows alien sprite
    void render(){
        //Show current sprite
        ss_alien->render( dim.x, dim.y, &SprClipsAlien1[ frame ] );
    }

};

class InvaderA : public Alien
{
    public:

    ///Constructor Function
    InvaderA(){

        commonInit();

        dim.h = ALIEN1_HEIGHT;
        dim.w = ALIEN1_WIDTH;

        ss_alien = &iTexture_A;

        //Load media
        if( !createSprites() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //delayTimer.start();
        }

    }
};

class InvaderB : public Alien
{
    public:

    ///Constructor Function
    InvaderB(){

        commonInit();

        dim.h = ALIEN2_HEIGHT;
        dim.w = ALIEN2_WIDTH;

        ss_alien = &iTexture_B;

        //Load media
        if( !createSprites() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //delayTimer.start();
        }

    }
};

class InvaderC : public Alien
{
    public:

    ///Constructor Function
    InvaderC(/*LTexture *tex*/){

        commonInit();

        dim.h = ALIEN3_HEIGHT;
        dim.w = ALIEN3_WIDTH;

        ss_alien = &iTexture_C;

        //Load media
        if( !createSprites() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //delayTimer.start();
        }

    }
};
#endif // ALIEN_H_INCLUDED