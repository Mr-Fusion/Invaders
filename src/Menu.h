#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdlib.h>
#include "Button.h"

class Menu : public GameState
{
    public:
    
    LButton mButton;

    std::stringstream credit;

    LTexture titleText;
    LTexture mText;
    LTexture creditText;

    SDL_Rect paddle;

    SDL_Color textColor;

    ///Constructor Function
    Menu(){

        paddle.x = PADDLE_OFFSET;
        paddle.y = 0;
        paddle.w = PADDLE_WIDTH;
        paddle.h = PADDLE_HEIGHT;

        textColor = { 0xFF, 0xFF, 0xFF };

        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
        	//Initialize Menu here
        	mButton = LButton(0, SCREEN_HEIGHT/2, SCREEN_WIDTH, mText.getHeight() );

        	SDL_SetWindowSize(gWindow,SCREEN_WIDTH, SCREEN_HEIGHT);
        }

    }

    ///Deconstructor
    ~Menu(){
        printf("Menu Object Deconstructing...\n");

		//Destroy Menu assets here

        //Free loaded image
        titleText.free();
        mText.free();
        creditText.free();

    }

    bool loadMedia()
    {
        //Loading success flag
        bool success = true;

        //Load sprite sheet texture



        //Open the font
        gFont = TTF_OpenFont( "../assets/SAMSRG__.TTF", 60 );
        if( gFont == NULL )
        {
            printf( "Failed to load Pong Title font! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
        else
        {
            //Render text
            if( !titleText.loadFromRenderedText( TITLE, textColor ) )
            {
                printf( "Failed to render title text!\n" );
                success = false;
            }
        }

        gFont = TTF_OpenFont( "../assets/PressStart2P.ttf", 14 );
        if( gFont == NULL )
        {
            printf( "Failed to load Minesweeper font! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
        else
        {
            //Render text
            //Set text to be rendered
            credit.str( "" );
            credit << "By Joel Turner | Version: " << MAJOR_REV << "." << MINOR_REV << "." << INCREMENTAL_REV;

            //Render text
            if( !creditText.loadFromRenderedText( credit.str().c_str(), textColor ) )
            {
                printf( "Failed to render credit text!\n" );
                success = false;
            }
        }

        gFont = TTF_OpenFont( "../assets/SAMST___.TTF", 40 );
        if( gFont == NULL )
        {
            printf( "Failed to load Pong font! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
        else
        {
            //Render text
            if( !mText.loadFromRenderedText( "BEGIN", textColor ) )
            {
                printf( "Failed to render mText!\n" );
                success = false;
            }
        }
        return success;
    }
    
    ///Handles mouse event
    void handleEvent( SDL_Event* e){

        int x, y;

        //Get mouse position
        if( e->type == SDL_MOUSEMOTION ){
            SDL_GetMouseState( &x, &y );

        paddle.y = y - (paddle.h/2);
            if ( y > SCREEN_HEIGHT - paddle.h/2 )
                paddle.y = SCREEN_HEIGHT - paddle.h;
            if ( y < paddle.h/2 )
                paddle.y = 0;
        }

        //Handle button events first
        mButton.handleEvent(e);

        if (mButton.isClicked)
            set_next_state(STATE_GAME_MED);
    }

    void logic(){

        if (mButton.inside)
            mText.setColor(0xFF, 0x00, 0x00);
        else
            mText.setColor(0xFF, 0xFF, 0xFF);

        mButton.logic();

    }

    void render(){

        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderFillRect(gRenderer, &paddle);

        titleText.render(SCREEN_WIDTH/2 - titleText.getWidth()/2, 30);
        mText.render(SCREEN_WIDTH/2 - mText.getWidth()/2, SCREEN_HEIGHT/2);
        creditText.render(SCREEN_WIDTH/2 - creditText.getWidth()/2, SCREEN_HEIGHT - 60);
    }

};

#endif // MENU_H_INCLUDED