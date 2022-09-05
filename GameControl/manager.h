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


#ifndef MANAGER_H
#define MANAGER_H

#include "gameConfig.h"
#include "gameManagement.h"

#include "animation.h"

/*
Buttons allow a user to visually interact with the system,
displaying menu options or information like score.
The class is entirely scoped to the Manager which exclusively uses it
*/
class Button {

friend class Manager;

private:

    Button(int id);
    ~Button();

    void setCoords(float high[4], float bck[4]); //sets the coordinates for the highlight and the main backing panel of the button

    void setTexture(MenuTexture* ptr, unsigned int id, float* wCoords, float* tCoords); //used to configure textures that are drawn on the button

    void draw(Camera* camera, bool selected);

    //coordinates follow pattern of top (+y), left (-x), bottom (-y), right (+x)
    float highlight[4]; //coordinates of highlighted border of button
    float backing[4]; //coordinates of backing panel of button

    LinkedList<MenuTexture> textures; //contains all the text based textures that the button draws

    int buttonID; //button identifier, used by Manager

};



/*
Main manager, controls drawing, menu and game options,
and uses the GameManager to set up and run the game
*/
class Manager{


public:

    Manager(GLFWwindow* w, Camera* cam);
    ~Manager();

    void process(bool** keys); //called to process the game/menu events
    void draw();

private:

    Button* getButton(int id); //returns position of menu button going of button identifier

    //configure the game modes
    void configEasy();
    void configNormal();
    void configArmageddon();
    void configTwoPlayer();

    void processMenu(bool** keys); //run menu control events
    void processGame(bool** keys); //run game events

    bool isSelected(Button* button); //returns if the button is currently selected or not
    void buttonSelection(bool** keys, int lower, int higher); //handles button selection within given ID range

    void setScoreButton(int points); //sets textures for score to be visualised
    void clearScoreButton(); //resets the score button so it is ready to be used again
    void setWinsButton(bool winPlay1); //sets the X in 'Player X Wins' to the winning player's number
    void clearWinsButton(); //resets the wins button so it is ready to be used again

    int selected = 0; //button identifier of the currently selected button

    GameConfig config; //handles game mode configuration (enemy size, etc)
    GameManager* gameMan = nullptr; //sets up the game
    GLFWwindow* window; //pointer to the window

    bool onMenu = true;
    //interactable menu buttons
    Button butEasy;
    Button butNorm;
    Button butHard;
    Button butTwoPlay;
    Button butExit;
    Button butResume;
    Button butReturn;
    //visual only menu buttons
    Button butScore;
    Button butWins;
    bool scoreReady = false;
    bool winsReady = false;
    Button butBoardMenu; //main menu backing board
    Button butBoardGame; //gameplay menu backing board

    bool paused = false; //game is paused
    bool dead = false; //game is over

    Animation animateBack; //main menu's background animation controller

    //game timers
    double timerGame = 0.0; //process game events
    double timerMenu = 0.0; //process menu events
    double timerDraw = 0.0; //draw to screen
    Camera* camera = nullptr; //points to main camera

    //vars to set the background colour
    float colBackMenu[4] = COLOUR_BACK_MENU;
    float colBackGame[4] = COLOUR_BACK_GAME;
    float* colBack = colBackMenu;

};


#endif
