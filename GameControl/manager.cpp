/*

MIT License

Copyright (c) 2022 Matthew James Austin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "manager.h"

namespace mvd {

namespace game_ctrl {

const std::array<std::array<float, 4>, 7> kHighlights = {
    std::array<float, 4>({0.9f, -0.3f, 0.7f, 0.3f}), // easy
    std::array<float, 4>({0.5f, -0.3f, 0.3f, 0.3f}), // normal
    std::array<float, 4>({0.1f, -0.3f, -0.1f, 0.3f}), // hard
    std::array<float, 4>({-0.3f, -0.3f, -0.5f, 0.3f}), // two player
    std::array<float, 4>({-0.7f, -0.3f, -0.9f, 0.3f}), // exit
    std::array<float, 4>({0.5f, -0.5f, 0.3f, 0.5f}), // resume
    std::array<float, 4>({0.1f, -0.5f, -0.1f, 0.5f}) // return
};

const std::array<std::array<float, 4>, 7> kBackings = {
    std::array<float, 4>({0.88f, -0.28f, 0.72f, 0.28f}), // easy
    std::array<float, 4>({0.48f, -0.28f, 0.32f, 0.28f}), // normal
    std::array<float, 4>({0.08f, -0.28f, -0.08f, 0.28f}), // hard
    std::array<float, 4>({-0.32f, -0.28f, -0.48f, 0.28f}), // two player
    std::array<float, 4>({-0.72f, -0.28f, -0.88f, 0.28f}), // exit
    std::array<float, 4>({0.48f, -0.48f, 0.32f, 0.48f}), // resume
    std::array<float, 4>({0.08f, -0.48f, -0.08f, 0.48f}), // return
};

Manager::Manager() :
    m_selectedMenuButton(MenuOptions::easy_mode),
    m_menuButtons({
        MenuButton(MenuOptions::easy_mode, kHighlights.at(0), kBackings.at(0)),
        MenuButton(MenuOptions::normal_mode, kHighlights.at(1), kBackings.at(1)),
        MenuButton(MenuOptions::hard_mode, kHighlights.at(2), kBackings.at(2)),
        MenuButton(MenuOptions::two_player_mode, kHighlights.at(3), kBackings.at(3)),
        MenuButton(MenuOptions::exit_game, kHighlights.at(4), kBackings.at(4))
    }),
    m_gameManager(nullptr),
    m_isOnMenu(true),
    m_isTwoPlayer(false) {

    // TODO LAMBDA FOR SETTING BUTTON TEXTURES
    // TODO COME BACK TO TEXTURES
}


Manager :: ~Manager() {
}


void Manager :: process(bool* keys) {
    if (m_isOnMenu) {
        processMenu(keys);
    } else {
        processGame(keys);
    }
}

void Manager::processButtonSelection(bool* keys, MenuOptions p_lower, MenuOptions p_higher) {
    // TODO REWORD THIS SO IT'S CLEANER.. MAYBE OVERLOAD FOR MENUOPTIONS TYPE???
    auto selected = static_cast<int>(m_selectedMenuButton);
    auto lower = static_cast<int>(p_lower);
    auto higher = static_cast<int>(p_higher);

    if (keys[2]) { // W is pressed -- cycle upwards
        selected--;
        keys[2] = false; //prevent button selection from cycling uncontrollably
    }

    if (keys[3]) { // S is pressed -- cycle downwards
        selected++;
        keys[3] = false; //prevent button selection from cycling uncontrollably
    }

    //catch selected
    if (selected < lower) {
        selected = higher;
    }
    if (selected > higher) {
        selected = lower;
    }
    m_selectedMenuButton = static_cast<MenuOptions>(selected);
}

//process events when on main menu
void Manager::processMenu(bool* keys){

    processButtonSelection(keys, MenuOptions::easy_mode, MenuOptions::exit_game);

    if (keys[6]) { // ENTER is pressed -- user has selected an option

        GameModeSettings settings;
        GameMode gameMode;
        switch (m_selectedMenuButton) {
            case MenuOptions::easy_mode : {gameMode = GameMode::easy; break;}
            case MenuOptions::normal_mode : {gameMode = GameMode::normal; break;}
            case MenuOptions::hard_mode : {gameMode = GameMode::hard; break;}
            case MenuOptions::two_player_mode : {gameMode = GameMode::two_player; break;}
            case MenuOptions::exit_game : {return;} // TODO NEW EXIT METHOD
            default : {throw std::runtime_error("invalid menu selection");}
        }
        // TODO BACK GROUND COLOURING
        setGameMode(gameMode, settings);
        m_gameManager = std::make_unique<GameManager>(&settings);
        m_isOnMenu = false;
        m_isTwoPlayer = (m_selectedMenuButton == MenuOptions::two_player_mode);
    }
    animateBack.process(); //update background animation
}


//process events when game is running
void Manager :: processGame(bool* keys){
/*
    //game is not over and not paused
    if (!dead && !paused){
        dead = gameMan->process(keys);
        //ESCAPE is pressed -- user has paused game
         if (keys[7]){
            paused = true;
         }
    } else {
        //either dead or paused
        if (dead){
            buttonSelection(keys, 6, 6); //only return to menu is a valid option
        } else {
            buttonSelection(keys, 5, 6); //resume and return to menu are valid options
        }
        //ENTER is pressed -- user has selected an option
        if (keys[6]){
            switch (selected) {
                case 6 : { //return to main menu
                    dead = false;
                    selected = 0;
                    delete gameMan;
                    gameMan = nullptr;
                    onMenu = true;
                    colBack = colBackMenu; //set to menu background colour
                    camera->setCamPos(0.0f, 0.0f, 1.0f); //set camera position and zoom for the main menu
                    if (scoreReady){clearScoreButton();} //reset score button's extra textures
                    if (winsReady){clearWinsButton();} //reset wins button's extra textures
                    //don't break as want to execute case 5 code as well
                }
                case 5 : {paused = false; keys[6] = false; break;} //resume game
            }
        }
    }*/
}
/*
//return's a pointer to a button from its id
MenuButton* Manager :: getButton(int id){
    switch(id){
        case 0 : {return &butEasy;}
        case 1 : {return &butNorm;}
        case 2 : {return &butHard;}
        case 3 : {return &butTwoPlay;}
        case 4 : {return &butExit;}
        case 5 : {return &butResume;}
        case 6 : {return &butReturn;}
        case 7 : {return &butScore;}
        case 8 : {return &butWins;}
        case 9 : {return &butBoardMenu;}
        case 10 : {return &butBoardGame;}
        default : {return nullptr;}
    }
}


//returns if a button is selected or not, used on current graphics system
bool Manager :: isSelected(MenuButton* button){
    return (button->m_id == selected);
}
*/

/* sets the texture on the score button to display the players score
does so by repeatedly adding the final digit to the back of the button space
and then knocking the position of the next digit along */
/*
void Manager :: setScoreButton(int points){

    //catch to prevent less than zero scores or the screen being spammed
    if (points < 0){
        points = 0;
    } else if (points > 9999999){
        points = 9999999;
    }

    ButtonDefaults butConfig;
    float textCoords[4] = {1, 0, 0, 1}; //texture coordinate bounds
    float* worldCoords = butConfig.scoreStart;
    MenuTexture* textPtr = nullptr;

    do { //do while as at least 1 digit will be printed (0)
        textPtr = new MenuTexture();
        //points%10 knocks off all bar the last digit as the rest can be divided by 10
        butScore.setTexture(textPtr, camera->getTexture((points%10)), worldCoords, textCoords);
        butScore.textures.addFront(textPtr); //add at front so can track the 'score:' texture by always keeping it at the end
        points /= 10; //shift score down a decimal place
        worldCoords[1] -= butConfig.scoreSpace;
        worldCoords[3] -= butConfig.scoreSpace;
    } while (points!=0);

    textPtr = nullptr;
    scoreReady = true;
}

//removes all digit textures from the score button
void Manager :: clearScoreButton(){
    while (butScore.textures.first!=butScore.textures.last){ //while other textures than 'Scores: ' exist in the linked list
        butScore.textures.remFront();
    }
    scoreReady = false;
}

//adds the winning players number in the middle of the texture panel
void Manager :: setWinsButton(bool winPlay1){
    int winner = 1;
    if (!winPlay1){
        winner = 2;
    }
    ButtonDefaults butConfig;
    float textCoords[4] = {1, 0, 0, 1}; //texture coordinate bounds

    MenuTexture* textPtr = new MenuTexture;
    butWins.setTexture(textPtr, camera->getTexture(winner), butConfig.textWorldNum, textCoords);
    butWins.textures.addEnd(textPtr);

    textPtr = nullptr;
    winsReady = true;
}

//removes the number texture from the win button
void Manager :: clearWinsButton(){
    butWins.textures.remEnd();
    winsReady = false;
}

*/
// draws either the menu or the running game
void Manager :: draw(Camera &p_camera) {

    if (m_isOnMenu) {
        animateBack.draw(p_camera);
        // TODO draw back menu

        // TODO wrap this into a function
        for (auto menuButton : m_menuButtons) {
            //set button's draw colours
            float colour[4] = COLOUR_PURPLE;
            float backColour[4] = COLOUR_PURPLE_BUTTON;
            if (menuButton.m_option != m_selectedMenuButton) {
                for (int i=0;i<3;i++){
                    colour[i] *= 0.5f; //dull main colour if not selected
                }
            }

            //draw highlight then backing panel over it
            p_camera.drawPureRect(colour, menuButton.m_highlight);
            p_camera.drawPureRect(backColour, menuButton.m_backing);
            // TODO TEXTURES...
        }
    } else {
        // TODO game drawing
    }

    /*
    if (timerDraw < glfwGetTime()) {

        if (onMenu){ //drawing main menu
            animateBack.draw(camera); //draw back ground splash animation
            butBoardMenu.draw(camera, true); //true so will glow
            //draw all 5 main menu buttons
            for (int i=0;i<5;i++){
                MenuButton* but = getButton(i);
                but->draw(camera, isSelected(but));
            }
        } else { //drawing gameplay
            if (!dead && !paused){
                camera->processInput(); //let camera move if player is not dead and if game is not paused
            }
            gameMan->draw(camera); //draw game world
            if (dead || paused){
                butBoardGame.draw(camera, true); //draw backing panel of pause/death menu
                butReturn.draw(camera, isSelected(&butReturn));
                if (dead) { //game is over show death screen
                    if (isTwoPlayer){ //2 players so display winning player's number
                        if (!winsReady){
                            setWinsButton(gameMan->getWinnerP1()); //set up win button's player number
                        }
                        butWins.draw(camera, true); //true so will glow
                    } else { //single player so show score
                        if (!scoreReady){
                            setScoreButton((int)(gameMan->getScore()/100.0f)); //set up score button's textures (every 100 meters scores 1 point)
                        }
                        butScore.draw(camera, true); //true so will glow
                    }
                } else { //game is just paused
                    butResume.draw(camera, isSelected(&butResume)); //display option to resume the game
                }
            }
        }
    }*/
}

}; // end of namespace game_ctrl

}; // end of namespace mvd
