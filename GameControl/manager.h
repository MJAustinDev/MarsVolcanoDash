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

#pragma once

#include <memory>
#include <array>
#include <list>

#include "gameModeSettings.h"
#include "gameManagement.h"

#include "animation.h"

namespace mvd {

namespace game_ctrl {

/**
 * TODO WORD UP
 */
enum class MenuOptions {
    easy_mode,
    normal_mode,
    hard_mode,
    two_player_mode,
    exit_game,
    resume_game,
    main_menu,

    // visual only // TODO LOOK INTO MOVING INTO OWN ENUM if needed...
    score,
    wins,
    back_board_menu,
    back_board_paused
};

/**
 * TODO WORD UP
 */
struct MenuButton {

    // MenuButton(int p_id) {m_id = p_id;};

    // TODO MOVE TO MANAGER ITSELF...
    // void setTexture(MenuTexture* ptr, unsigned int id, float* wCoords, float* tCoords);
    // void draw(Camera* camera, bool selected); // TODO CHANGE SCOPE...
    // LinkedList<MenuTexture> textures; //contains all the text based textures that the button draws

    MenuButton(MenuOptions p_id, std::array<float,4> p_highlight, std::array<float,4> p_backing) :
        m_id(p_id), m_highlight(p_highlight), m_backing(p_backing) {};

    MenuOptions m_id;
    std::array<float,4> m_highlight;
    std::array<float,4> m_backing;
    std::list<MenuTexture> m_textures; // TODO COME BACK TO TEXTURES
};



/* Main manager, controls drawing, menu and game options,
and uses the GameManager to set up and run the game */
class Manager{

public:

    Manager();
    ~Manager();

    void process(bool* keys);
    void draw(Camera &p_camera);

private:

    MenuOptions m_selectedMenuButton;
    std::array<MenuButton, 7> m_menuButtons;
    // std::array<MenuButton, 4> m_visualButtons; // TODO VERIFY THIS
    std::unique_ptr<GameManager> m_gameManager;
    bool m_isOnMenu;

    // MenuButton* getButton(int id); //returns position of menu button going of button identifier

    void processMenu(bool* keys); //run menu control events
    void processGame(bool* keys); //run game events

    bool isSelected(MenuButton* button); //returns if the button is currently selected or not
    void buttonSelection(bool* keys, int lower, int higher); //handles button selection within given ID range

    void setScoreButton(int points); //sets textures for score to be visualised
    void clearScoreButton(); //resets the score button so it is ready to be used again
    void setWinsButton(bool winPlay1); //sets the X in 'Player X Wins' to the winning player's number
    void clearWinsButton(); //resets the wins button so it is ready to be used again

    //bool onMenu = true;
    bool isTwoPlayer = false;
    // interactable menu buttons
    // store in an std::array have some access func???
  //  MenuButton butEasy;
  //  MenuButton butNorm;
  //  MenuButton butHard;
  //  MenuButton butTwoPlay;
  //  MenuButton butExit;
 //   MenuButton butResume;
 //   MenuButton butReturn;
    // visual only menu buttons
  //  MenuButton butScore;
  //  MenuButton butWins;
    bool scoreReady = false;
    bool winsReady = false;
//    MenuButton butBoardMenu; //main menu backing board
 //   MenuButton butBoardGame; //gameplay menu backing board

    bool paused = false; //game is paused
    bool dead = false; //game is over

    Animation animateBack; //main menu's background animation controller

    //vars to set the background colour
    float colBackMenu[4] = COLOUR_BACK_MENU;
    float colBackGame[4] = COLOUR_BACK_GAME;
    float* colBack = colBackMenu;

};

}; // end of namespace game_ctrl

}; // end of namespace mvd
