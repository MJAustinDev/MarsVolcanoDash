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

Manager :: Manager(GLFWwindow* w, Camera* cam) :
    butEasy(0), butNorm(1), butHard(2), butTwoPlay(3),
    butExit(4), butResume(5), butReturn(6), butScore(7),
    butWins(8), butBoardMenu(9), butBoardGame(10) {

    camera = cam;
    window = w;

    ButtonDefaults butConfig; //access button configuration values
    float textCoords[4] = {1, 0, 0, 1}; //texture coordinate bounds

    MenuButton* but;
    MenuTexture* textPtr;
    //set background board positions and sizes
    for (int i=9;i<11;i++){
        but = getButton(i);
        but->setCoords(butConfig.highlights[i], butConfig.backings[i]);
    }

    //set all 9 menu buttons positions, sizes and primary textures
    for (int i=0;i<9;i++){
        but = getButton(i);
        but->setCoords(butConfig.highlights[i], butConfig.backings[i]);
        textPtr = new MenuTexture;
        but->setTexture(textPtr, camera->getTexture(i+10), butConfig.textWorld[i], textCoords);
        but->textures.addEnd(textPtr);
    }
    //add 'Player' to Player X Wins button
    textPtr = new MenuTexture;
    butWins.setTexture(textPtr, camera->getTexture(13), butConfig.textWorld[9], butConfig.onlyPlayer);
    butWins.textures.addEnd(textPtr);

    but = nullptr;
    textPtr = nullptr;
}


Manager :: ~Manager(){
    if (gameMan != nullptr){
        delete gameMan;
    }
}


//process events
void Manager :: process(bool* keys){

    if (onMenu && (timerMenu < glfwGetTime())) {
        processMenu(keys); //process main menu activity
        timerMenu = glfwGetTime() + 0.01;
    } else if (!onMenu && timerGame < glfwGetTime()){
        processGame(keys); //process game activity
        timerGame = glfwGetTime() + 0.01;
    }

}

//handle menu button selection
void Manager :: buttonSelection(bool* keys, int lower, int higher){

    //W is pressed -- cycle upwards
    if (keys[2]){
        selected--;
        keys[2] = false; //prevent button selection from cycling uncontrollably
    }
    //S is pressed -- cycle downwards
    if (keys[3]){
        selected++;
        keys[3] = false; //prevent button selection from cycling uncontrollably
    }

    //catch selected
    if (selected<lower){
        selected = higher;
    }
    if (selected > higher){
        selected = lower;
    }
}

//process events when on main menu
void Manager :: processMenu(bool* keys){

    buttonSelection(keys, 0, 4); // let user scan up/down the options

    //ENTER is pressed -- user has selected an option
    if (keys[6]){

        GameModeSettings settings;
        GameMode gameMode;
        switch (selected) { // TODO ENUM FOR BUTTONS
            case 0 : {gameMode = GameMode::easy; break;}
            case 1 : {gameMode = GameMode::normal; break;}
            case 2 : {gameMode = GameMode::hard; break;}
            case 3 : {gameMode = GameMode::two_player; break;}
            case 4 : {glfwSetWindowShouldClose(window, GL_TRUE); return;}
        }
        setGameMode(gameMode, settings);

        selected = 5; //set selected for in game resume option
        colBack = colBackGame; //set background colour to game background colour
        camera->setCamPos(0.0f, 0.0f, 0.05f); //focus camera with default in game zoom
        gameMan = new GameManager(&settings); // TODO REMOVE NEW/DELETE... USE SMART POINTERS
        onMenu = false;
        isTwoPlayer = (selected == 3); // TODO ENUM FOR BUTTONS
    }
    animateBack.process(); //update background animation

}


//process events when game is running
void Manager :: processGame(bool* keys){

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
    }
}

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

/* sets the texture on the score button to display the players score
does so by repeatedly adding the final digit to the back of the button space
and then knocking the position of the next digit along */
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


//draws either the menu or the running game
void Manager :: draw(){

    if (timerDraw < glfwGetTime()) {

        //clear screen and viewport
        glClear(GL_COLOR_BUFFER_BIT); //clear total screen creating black space behind the view port
        float viewport[4] = {1.0f,-1.0f,-1.0f,1.0f};
        camera->drawPureRect(colBack, viewport); //clears the view port to the given background colour

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
        glfwSwapBuffers(window);

        timerDraw = (1.0/60.0) + glfwGetTime(); //aiming for 60 fps
        camera->updateGlow(); //update glow value
    }
}

}; // end of namespace game_ctrl

}; // end of namespace mvd
