#ifndef INVADERS_H_INCLUDED
#define INVADERS_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <sstream>
#include "Const.h"
#include "GameState.h"
#include "LTimer.h"
#include "LTexture.h"
#include "Defender.h"
#include "Alien.h"

#define NUM_INVADERS    40

#define INVADE_DELAY    300

#define ALIEN_SS_COLS       1
#define ALIEN_SS_ROWS       2

#define ALIEN_SPRITE_NUM    2

#define FORMATION_ROWS  5
#define FORMATION_COLS  8

#define PADDLE_HEIGHT   60
#define PADDLE_WIDTH    8
#define PADDLE_OFFSET   12

#define BALL_WIDTH      8
#define BALL_HEIGHT     8

#define SERVE_SPEED     5
#define MAX_SPEED       15

#define MAX_PADDLE_VEL  6

#define POINTS_TO_WIN   15

typedef struct _Velocity_Vector{
  int yVel, xVel;
} VelocityVector;

class Invaders : public GameState
{
    public:

    int colorCount;

    int aliensRemaining;
    int currentLev;

    bool newLevel;
    bool victory;

    SDL_Color textColor;

    // Background color
    int bgR, bgG, bgB;

    // Sprite color
    int spR, spG, spB;

    //In memory text stream
    std::stringstream msgText;
    std::stringstream msgText2;

    //Scene textures
    LTexture msgTextTexture;
    LTexture msgTextTexture2;

    LTimer delayTimer;
    LTimer invadeTimer;

    //The sound effects that will be used
    Mix_Chunk *gHigh = NULL;
    Mix_Chunk *gLow = NULL;

    SDL_Rect field;

    Defender player;

    Alien *invader[NUM_INVADERS];

    LTexture *iTexture;



    ///Constructor Function
    Invaders(){

        colorCount = 0;

        currentLev = 0;

        aliensRemaining = NUM_INVADERS;

        victory = false;

        bgR = bgG = bgB = 0x00;

        spR = spG = spB = 0xFF;

        textColor = { spR, spG, spB, 0xFF};

        field.x = 0;
        field.y = 0;
        field.w = SCREEN_WIDTH;
        field.h = SCREEN_HEIGHT;

        iTexture = new LTexture;

        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Initialize playing field dimensions, difficulty, and appearance
            startGame();

            //Initialize and display graphical interface
            SDL_SetWindowSize(gWindow,SCREEN_WIDTH,SCREEN_HEIGHT);
        }

    }

    ///Deconstructor
    ~Invaders(){
        printf("Gamestate Object Deconstructing...\n");

        //Free the sound effects
        Mix_FreeChunk( gLow );
        gLow = NULL;
        Mix_FreeChunk( gHigh );
        gHigh = NULL;

        //Free loaded image
        msgTextTexture.free();
        msgTextTexture2.free();

        for (int i = 0; i < NUM_INVADERS; i++) {
            if (invader[i] != NULL) {
                delete invader[i];
            }
        }

    }

    void startGame()
    {

        //Initialization goes here

        nextLevel();


    }

    //TODO: Can we streamline the sprite sheet creation into a function?
    bool loadMedia()
    {
        //Loading success flag
        bool success = true;

        //Load sound effects
        gLow = Mix_LoadWAV( "../assets/sfx_sounds_Blip7.wav" );
        if( gLow == NULL )
        {
            printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }

        gHigh = Mix_LoadWAV( "../assets/sfx_sounds_Blip9.wav" );
        if( gHigh == NULL )
        {
            printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }

        //Set text to be rendered
        msgText.str( "" );
        //msgText << "" << POINTS_TO_WIN << " Points to Win!";

        //Render text
        if( !msgTextTexture.loadFromRenderedText( msgText.str().c_str(), textColor ) )
        {
            printf( "Unable to render message texture!\n" );
        }

        //Set text to be rendered
        setMessage2(currentLev);

        //Load sprite sheet texture
        if( !iTexture->loadFromFile( "../assets/ss_alien1_x2.png") )
        {
            printf( "Failed to load alien1 sprite sheet texture!\n" );
            success = false;
        }

        return success;
    }

    void setMessage(char* text){
        //Set text to be rendered
        msgText.str( "" );
        msgText << text;

        //Render text
        if( !msgTextTexture.loadFromRenderedText( msgText.str().c_str(), textColor ) )
        {
            printf( "Unable to render message texture!\n" );
        }

    }

    void setMessage2(int num){
        //Set text to be rendered
        msgText2.str( "" );
        msgText2 << "Mission: " << num;

        //Render text
        if( !msgTextTexture2.loadFromRenderedText( msgText2.str().c_str(), textColor ) )
        {
            printf( "Unable to render message texture 2!\n" );
        }

    }

    void clearMessage2(){
        //Set text to be rendered
        msgText2.str( "" );
        msgText2 << "";

        //Render text
        if( !msgTextTexture2.loadFromRenderedText( msgText2.str().c_str(), textColor ) )
        {
            printf( "Unable to render message texture 2!\n" );
        }

    }

    void nextLevel(){

        invadeTimer.stop();

        currentLev++;

        setMessage2(currentLev);

        for (int i = 0; i < NUM_INVADERS; i++) {
            invader[i] = new Alien( iTexture );
        }

        int i = 0;
        for (int j = 0; j < FORMATION_COLS; j++){
            for (int k = 0; k < FORMATION_ROWS; k++){
                invader[i]->setPos( (48 * j ) + SCREEN_WIDTH/5 , ( 48 * k ) + 48 );
                i++;
            }
        }

        aliensRemaining = NUM_INVADERS;

        delayTimer.start();
        invadeTimer.start();

        newLevel = true;

    }


    ///Routine for winning the game
    void gameOver(){

        delayTimer.stop();

        setMessage("Final Score");

    }

    ///Handles mouse event
    void handleEvent( SDL_Event* e){

        int x, y;

        //Get mouse position
        if( e->type == SDL_MOUSEMOTION ){
            SDL_GetMouseState( &x, &y );
        }


        if( e->button.button == SDL_BUTTON_RIGHT && e->type == SDL_MOUSEBUTTONUP ){
            set_next_state(STATE_MENU);
        }

        player.handleEvent(e);

        for (int i = 0; i < NUM_INVADERS; i++) {
            if (invader[i] != NULL)
                invader[i]->handleEvent(e);
        }
        
    }

    void logic(){

        player.logic();

        for (int i = 0; i < NUM_INVADERS; i++) {

            if (invader[i] != NULL) {


                if (invader[i]->checkReverse()) {
                    //printf("reversing...\n");
                    for (int j = 0; j < NUM_INVADERS; j++)
                        if (invader[j] != NULL)
                            invader[j]->setReverse();
                    //break;
                }

                if (player.peaShot.dim.y < ( invader[i]->dim.y + invader[i]->dim.h ) ) {
                    if (player.peaShot.dim.y + player.peaShot.dim.h > invader[i]->dim.y) {
                        if (player.peaShot.dim.x > invader[i]->dim.x) {
                            if (player.peaShot.dim.x < invader[i]->dim.x + invader[i]->dim.w) {
                                player.peaShot.hit();
                                invader[i]->getHit();
                                delete invader[i];
                                aliensRemaining--;
                                invader[i] = NULL;
                            }
                        }
                    }
                }
            }

        }

        if (invadeTimer.getTicks() > INVADE_DELAY){

            invadeTimer.stop();

            for (int i = 0; i < NUM_INVADERS; i++) {
                if (invader[i] != NULL)
                    invader[i]->logic();
            
        
                //printf("Invader %d tick count: %d\n",i,invader[i].timeDbg);


            }

            invadeTimer.start();
        }

        if (aliensRemaining == 0) {
            nextLevel();
        }

        if (victory) {
            colorCount++;
            spR = ( colorCount % 192 ) + 64;
            spG = ( ( colorCount + 64) % 192 ) + 64;
            spB = ( (colorCount + 128 ) % 192 ) + 64;
            if (colorCount > 192)
                colorCount = 0;
        }

        if (newLevel){
            if (delayTimer.getTicks() > 3000){
                delayTimer.stop();
                setMessage("");
                clearMessage2();

                newLevel = false;
            }
        }
    }

    void render(){


        SDL_SetRenderDrawColor( gRenderer, bgR, bgG, bgB, 0xFF );
        SDL_RenderFillRect(gRenderer, &field);

        SDL_SetRenderDrawColor( gRenderer, spR, spG, spB, 0xFF );
        player.render();

        for (int i = 0; i < NUM_INVADERS; i++) {
            if (invader[i] != NULL) {
                invader[i]->render();
            }
        }

        msgTextTexture.setColor(spR, spG, spB);
        msgTextTexture2.setColor(spR, spG, spB);
        msgTextTexture.render(SCREEN_WIDTH/2 - msgTextTexture.getWidth()/2, msgTextTexture.getHeight() / 2 );
        msgTextTexture2.render(SCREEN_WIDTH/2 - msgTextTexture2.getWidth()/2, SCREEN_HEIGHT - msgTextTexture2.getHeight() * 2 );

    }

};

#endif // INVADERS_H_INCLUDED