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

//#define INVADE_DELAY    300
#define MIN_DELAY       20

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

#define BULLET_HEIGHT       8
#define BULLET_WIDTH        2
#define VELOCITY            -10

typedef struct _Velocity_Vector{
  int yVel, xVel;
} VelocityVector;

class Invaders : public GameState
{
    public:

    int colorCount;

    int aliensRemaining;
    int currentLev;
    int invadeDelay;

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

    Bullet *bullet, *iBullet;

    bool lInput, rInput, shInput;



    ///Constructor Function
    Invaders(){

        colorCount = 0;

        currentLev = 0;

        aliensRemaining = NUM_INVADERS;
        invadeDelay = MIN_DELAY;

        victory = false;

        bgR = bgG = bgB = 0x00;

        spR = spG = spB = 0xFF;

        textColor = { spR, spG, spB, 0xFF};

        field.x = 0;
        field.y = 0;
        field.w = SCREEN_WIDTH;
        field.h = SCREEN_HEIGHT;

        iTexture = new LTexture;

        bullet = NULL;
        iBullet = NULL;

        lInput = rInput = shInput = false;

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

        if (bullet != NULL)
            delete bullet;

        if (iBullet != NULL)
            delete iBullet;

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
        updateInvadeDelay();

        delayTimer.start();
        invadeTimer.start();

        newLevel = true;

    }

    void updateInvadeDelay() {
        invadeDelay = aliensRemaining * 10;
        if (invadeDelay < MIN_DELAY)
                invadeDelay = MIN_DELAY; 
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
        
    }

    void logic(){

        if (lInput)
            player.moveLeft();

        if (rInput)
            player.moveRight();

        if (shInput && bullet == NULL)
            bullet = player.shoot();

        if (bullet != NULL){
            bullet->logic();
            if (bullet->offScreen()){
                delete bullet;
                bullet = NULL;
            }
        }

        if (iBullet != NULL){
            iBullet->logic();
            if (iBullet->offScreen()){
                delete iBullet;
                iBullet = NULL;
            }
        }

        for (int i = 0; i < NUM_INVADERS; i++) {

            if (invader[i] != NULL) {


                if (invader[i]->checkReverse()) {
                    //printf("reversing...\n");
                    for (int j = 0; j < NUM_INVADERS; j++)
                        if (invader[j] != NULL)
                            invader[j]->setReverse(true);
                    //break;
                }

                player.checkCollision(invader[i]->dim);

                if (bullet != NULL){
                    if ( invader[i]->checkCollision( bullet->getDim() ) ){
                        delete invader[i];
                        delete bullet;
                        aliensRemaining--;
                        updateInvadeDelay();
                        invader[i] = NULL;
                        bullet = NULL;
                    }
                }


            }

        }

        if (invadeTimer.getTicks() > invadeDelay){

            invadeTimer.stop();

            for (int i = 0; i < NUM_INVADERS; i++) {
                if (invader[i] != NULL){
                    invader[i]->logic();
                    if (iBullet == NULL)
                        iBullet = invader[i]->shoot();
                }
            




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

        if (bullet != NULL)
            bullet->render();

        if (iBullet != NULL)
            iBullet->render();

        msgTextTexture.setColor(spR, spG, spB);
        msgTextTexture2.setColor(spR, spG, spB);
        msgTextTexture.render(SCREEN_WIDTH/2 - msgTextTexture.getWidth()/2, msgTextTexture.getHeight() / 2 );
        msgTextTexture2.render(SCREEN_WIDTH/2 - msgTextTexture2.getWidth()/2, SCREEN_HEIGHT - msgTextTexture2.getHeight() * 2 );

    }

};

#endif // INVADERS_H_INCLUDED