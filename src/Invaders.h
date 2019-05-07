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
#include "Ufo.h"

#define NUM_INVADERS        55

//#define INVADE_DELAY    300
#define MIN_DELAY           20
#define CHEER_DELAY         300
#define UFO_DELAY           5000
#define UFO_FRAME_DELAY     100

#define ALIEN_SS_COLS       1
#define ALIEN_SS_ROWS       2

#define ALIEN_SPRITE_NUM    2

#define FORMATION_ROWS      5
#define FORMATION_COLS      11

#define FORMATION_XDELTA    48
#define FORMATION_YDELTA    48

#define PADDLE_HEIGHT       60
#define PADDLE_WIDTH        8
#define PADDLE_OFFSET       12

#define BALL_WIDTH          8
#define BALL_HEIGHT         8

#define SERVE_SPEED         5
#define MAX_SPEED           15

#define MAX_PADDLE_VEL      6

#define POINTS_TO_WIN       15

#define DEFAULT_LIVES       3

#define ALIEN_POINTS        10
#define UFO_POINTS          100

#define INVADER_VEL         8

typedef struct _Velocity_Vector{
  int yVel, xVel;
} VelocityVector;

enum FormationStates
{
    STATE_FORMATION_NULL,
    STATE_FORMATION_UP,
    STATE_FORMATION_RIGHT,
    STATE_FORMATION_DOWN,
    STATE_FORMATION_LEFT,
};

class Invaders : public GameState
{
    public:

    int colorCount;

    int aliensRemaining;
    int currentLev;
    int invadeDelay;
    int iXVel, iYVel;
    int lives;
    int score;

    int curFormationState, nextFormationState, prevFormationState;

    bool levelBegin;
    bool hitTaken;
    bool fReverse;
    bool showLvl;
    bool gameOver;

    SDL_Color textColor;

    // Background color
    int bgR, bgG, bgB;

    // Sprite color
    int spR, spG, spB;

    //In memory text stream
    std::stringstream msgText;
    std::stringstream lvlText;
    std::stringstream livesText;
    std::stringstream scoreText;

    //Scene textures
    LTexture livesTextTexture;
    LTexture lvlTextTexture;
    LTexture scoreTextTexture;

    LTimer delayTimer;
    LTimer invadeTimer;
    LTimer ufoTimer;
    LTimer ufoFrameTimer;

    //The sound effects that will be used
    
    Mix_Chunk *playerShot = NULL;
    Mix_Chunk *playerHit = NULL;

    SDL_Rect field;

    Defender player;

    Alien *invader[NUM_INVADERS];

    Bullet *bullet, *iBullet;

    Ufo *ufo;

    bool lInput, rInput, shInput;

    int shooter;



    ///Constructor Function
    Invaders(){

        colorCount = 0;

        currentLev = 0;

        aliensRemaining = NUM_INVADERS;
        invadeDelay = MIN_DELAY;

        hitTaken = false;
        fReverse = false;
        showLvl = false;
        gameOver = false;

        bgR = bgG = bgB = 0x00;

        spR = spG = spB = 0xFF;

        textColor = { spR, spG, spB, 0xFF};

        field.x = 0;
        field.y = 0;
        field.w = SCREEN_WIDTH;
        field.h = SCREEN_HEIGHT;

        bullet = NULL;
        iBullet = NULL;

        ufo = NULL;

        lInput = rInput = shInput = false;

        iXVel = 0;
        iYVel = 0;

        lives = 0;
        score = 0;

        shooter = 0;

        curFormationState = nextFormationState = prevFormationState = STATE_FORMATION_NULL;

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
        Mix_FreeChunk( playerShot );
        playerShot = NULL;
        Mix_FreeChunk( playerHit );
        playerHit = NULL;

        //Free loaded image
        livesTextTexture.free();
        lvlTextTexture.free();
        scoreTextTexture.free();

        for (int i = 0; i < NUM_INVADERS; i++) {
            if (invader[i] != NULL) {
                delete invader[i];
            }
        }

        if (ufo != NULL)
            delete ufo;

        clearBullets();
    }

    void startGame()
    {
        //Initialization goes here
        lives = DEFAULT_LIVES;
        score = 0;
        updateLivesText();
        updateScoreText();
        goNextLevel();
    }

    //TODO: Can we streamline the sprite sheet creation into a function?
    bool loadMedia()
    {
        //Loading success flag
        bool success = true;

        //Load sound effects
        playerShot = Mix_LoadWAV( "../assets/sfx_player_shot.wav" );
        if( playerShot == NULL )
        {
            printf( "Failed to load player shot sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }

        playerHit = Mix_LoadWAV( "../assets/sfx_player_hit.wav" );
        if( playerHit == NULL )
        {
            printf( "Failed to load player hit sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }

        //Set text to be rendered
        msgText.str( " " );
        livesText.str( " " );

        //Render text
        livesTextTexture.setText(livesText.str().c_str(), textColor);

        lvlTextTexture.setText(msgText.str().c_str(), textColor);

        return success;
    }

    void updateLvlText(int num){
        //Set text to be rendered
        lvlText.str( "" );
        lvlText << "Mission: " << num;

        //Render text
        lvlTextTexture.setText(lvlText.str().c_str(), textColor);

    }

    void updateLivesText() {
        //Set text to be rendered
        livesText.str( "" );
        livesText << "Lives: " << lives;

        //Render text
        livesTextTexture.setText(livesText.str().c_str(), textColor);
    }

    void updateScoreText() {
        //Set text to be rendered
        scoreText.str( "" );
        scoreText << "Score: " << score;

        //Render text
        scoreTextTexture.setText(scoreText.str().c_str(), textColor);
    }

    void goNextLevel(){

        for (int i = 0; i < NUM_INVADERS; i++) {
            if (i % FORMATION_ROWS == 0)
                invader[i] = new InvaderC();
            else if (i % FORMATION_ROWS == 1)
                invader[i] = new InvaderB();
            else if (i % FORMATION_ROWS == 2)
                invader[i] = new InvaderB();
            else
                invader[i] = new InvaderA();
        }

        aliensRemaining = NUM_INVADERS;
        updateInvadeDelay();

        currentLev++;

        initLevel();

    }

    void initLevel() {

        invadeTimer.stop();

        iXVel = INVADER_VEL;
        iYVel = 0;
        fReverse = false;
        nextFormationState = STATE_FORMATION_RIGHT;

        updateLvlText(currentLev);

        showLvl = true;   

        int i = 0;
        for (int j = 0; j < FORMATION_COLS; j++){
            for (int k = 0; k < FORMATION_ROWS; k++){
                if (invader[i] != NULL){
                    invader[i]->setPos( FORMATION_XDELTA*j + ( FORMATION_XDELTA - invader[i]->getDim().w )/2 + SCREEN_WIDTH/8 , ( FORMATION_YDELTA * k ) + 80 );
                    invader[i]->setVel(iXVel,iYVel);
                }
                i++;
            }
        }

        shooter = findNextShooter();

        delayTimer.start();
        invadeTimer.start();
        ufoTimer.start();

        levelBegin = true;

    }

    void updateInvadeDelay() {
        invadeDelay = aliensRemaining * 10;
        if (invadeDelay < MIN_DELAY)
                invadeDelay = MIN_DELAY; 
    }

    void clearBullets() {
        if (bullet != NULL){
            delete bullet;
            bullet = NULL;
        }
        if (iBullet != NULL){
            delete iBullet;
            iBullet = NULL;
        }
    }

    void setVelUnanimous(){
        for (int i = 0; i < NUM_INVADERS; i++) {
            if (invader[i] != NULL){
                invader[i]->setVel(iXVel,iYVel);
            }                        
        }
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

        if (!gameOver){
            // Player Input/Control Logic
            if (lInput)
                player.moveLeft();

            if (rInput)
                player.moveRight();

            if (shInput && bullet == NULL){
                bullet = player.shoot();
                Mix_PlayChannel( -1, playerShot, 0 );
            }
        }


        // Bullet Movement Logic
        if (bullet != NULL){
            bullet->move();
            if (bullet->offScreen()){
                delete bullet;
                bullet = NULL;
            }
        }

        // Bullet Hit Detection Logic
        if (iBullet != NULL){
            iBullet->move();
            if (player.checkCollision(iBullet->getDim()))
                hitTaken = true;
            if (iBullet->offScreen()){
                delete iBullet;
                iBullet = NULL;
            }
        }

        if (bullet != NULL){
            if (iBullet != NULL){
                if ( iBullet->checkCollision( bullet->getDim() ) ){
                    delete iBullet;
                    iBullet = NULL;
                    delete bullet;
                    bullet = NULL;
                }
            }
        }

        // Invader Navigation Logic
        for (int i = 0; i < NUM_INVADERS; i++) {

            if (invader[i] != NULL) {

                switch (curFormationState){

                    case STATE_FORMATION_RIGHT:
                        if ( invader[i]->atRightEdge() ){
                            iXVel = 0;
                            iYVel = INVADER_VEL * 2;
                            nextFormationState = STATE_FORMATION_DOWN;
                        }
                    break;

                    case STATE_FORMATION_DOWN:

                        if ( prevFormationState == STATE_FORMATION_RIGHT ){
                            iXVel = -INVADER_VEL;
                            iYVel = 0;
                            nextFormationState = STATE_FORMATION_LEFT;
                        }
                        if ( prevFormationState == STATE_FORMATION_LEFT ){
                            iXVel = INVADER_VEL;
                            iYVel = 0;
                            nextFormationState = STATE_FORMATION_RIGHT;
                        }
                    break;

                    case STATE_FORMATION_LEFT:
                        if ( invader[i]->atLeftEdge() ){
                            iXVel = 0;
                            iYVel = INVADER_VEL * 2;
                            nextFormationState = STATE_FORMATION_DOWN;
                        }
                    break;

                    default:
                    break;
                }
            }
        }

        // Invader Hit Detection Logic
        for (int i = 0; i < NUM_INVADERS; i++) {

            if (invader[i] != NULL) {
                if ( invader[i]->checkCollision(player.getDim()) )
                    hitTaken = true;

                if (bullet != NULL){
                    if ( invader[i]->checkCollision( bullet->getDim() ) ){
                        delete invader[i];
                        delete bullet;
                        aliensRemaining--;
                        score += ALIEN_POINTS;
                        updateScoreText();
                        updateInvadeDelay();
                        invader[i] = NULL;
                        bullet = NULL;
                    }
                }
            }
        }

        // Ufo Hit Detection Logic
        if (ufo != NULL){
            if (bullet != NULL){
                if (ufo->checkCollision( bullet->getDim() ) ){
                    delete ufo;
                    delete bullet;
                    score += UFO_POINTS;
                    updateScoreText();
                    ufo = NULL;
                    bullet = NULL;
                }
            }
        }
/*
        for (int i = 0; i < NUM_INVADERS; i++) {
            if (invader[i] != NULL)
                invader[i]->move();
        }
*/
        // Invader Action Logic
        if (invadeTimer.getTicks() > invadeDelay){

            invadeTimer.stop();

            setVelUnanimous();
            prevFormationState = curFormationState;
            curFormationState = nextFormationState;

            for (int i = 0; i < NUM_INVADERS; i++) {
                if (invader[i] != NULL){
                    invader[i]->cycleFrame();
                    if (!gameOver){
                        invader[i]->move();
                        if (iBullet == NULL && i == shooter) {
                            iBullet = invader[i]->shoot();
                        }
                    }
                }
            shooter = findNextShooter();
            }
            invadeTimer.start();
        }

        // UFO Spawn Logic
        if (ufoTimer.getTicks() > UFO_DELAY){
            ufoTimer.stop();
            if (ufo == NULL){
                ufo = new Ufo(rand() % 10 - 5);
                ufoFrameTimer.start();
            }
            ufoTimer.start();
        }

        // UFO Movement/Action Logic
        if (ufo != NULL){
            ufo->move();
            if ( ufoFrameTimer.getTicks() > UFO_FRAME_DELAY){
                ufoFrameTimer.stop();
                ufo->cycleFrame();
                ufoFrameTimer.start();
            }
            
            if (ufo->offScreen()){
                delete ufo;
                ufo = NULL;
            }
        }



        // Level/Game flow logic
        if (aliensRemaining == 0) {
            goNextLevel();
        }

        if (!gameOver){
            if (hitTaken) {
                Mix_PlayChannel( -1, playerHit, 0 );
                SDL_Delay(3000);
                lives--;

                if (lives < 0){
                    gameOver = true;
                    invadeDelay = CHEER_DELAY;
                    //Set text to be rendered
                    lvlText.str( "Game Over" );
                    //Render text
                    lvlTextTexture.setText(lvlText.str().c_str(), textColor);
                    showLvl = true;
                }
                else {
                    updateLivesText();
                    clearBullets();
                    initLevel();
                }
                hitTaken = false;
            }
        }

        if (levelBegin){
            if (delayTimer.getTicks() > 3000){
                delayTimer.stop();
                showLvl = false;
                levelBegin = false;
            }
        }
    }

    int findNextShooter() {
        int i = FORMATION_ROWS * ( ( rand() % FORMATION_COLS ) + 1) - 1;
        while (invader[i] == NULL && i > 0)
            i--;
        return i;
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

        if (ufo != NULL)
            ufo->render();

        livesTextTexture.setColor(spR, spG, spB);
        livesTextTexture.render(SCREEN_WIDTH - livesTextTexture.getWidth(), 1 );

        scoreTextTexture.setColor(spR, spG, spB);
        scoreTextTexture.render(5, 1 );

        if (showLvl) {
            lvlTextTexture.setColor(spR, spG, spB);
            lvlTextTexture.render(SCREEN_WIDTH/2 - lvlTextTexture.getWidth()/2, SCREEN_HEIGHT - lvlTextTexture.getHeight() * 2 );
        }

    }

};

#endif // INVADERS_H_INCLUDED