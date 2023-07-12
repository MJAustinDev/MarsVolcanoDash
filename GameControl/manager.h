/*

MIT License

Copyright (c) 2022-23 Matthew James Austin

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
 * Defines the different options for the menu buttons
 */
enum class MenuOptions {
    easy_mode = 0,
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
 * Structure containing all values needed to draw a menu button
 */
struct MenuButton {
    MenuButton(MenuOptions p_option, std::array<float, 4> p_highlight, std::array<float,4> p_backing) :
        m_option(p_option), m_highlight(p_highlight), m_backing(p_backing) {};

    MenuOptions m_option;
    std::array<float,4> m_highlight;
    std::array<float,4> m_backing;
    std::list<MenuTexture> m_textures; // TODO COME BACK TO TEXTURES
};

/**
 * Defines the possible states that the game can be in.
 */
enum class GameState {
    on_main_menu,
    in_game_alive,
    in_game_paused,
    in_game_dead,
    exitting_game
};

/**
 * Manager class.
 * Controls the logic of menus, gameplay, and drawing.
 */
class Manager {

private:

    MenuOptions m_selectedMenuButton;
    const std::array<MenuButton, 5> m_menuButtons;
    const std::array<MenuButton, 2> m_pauseButtons;
    // std::array<MenuButton, 4> m_visualButtons; // TODO look into this later on
    std::unique_ptr<GameManager> m_gameManager;
    GameState m_state;
    bool m_isTwoPlayer;

    /**
     * Processes events on the main menu
     * @param keys ptr to key booleans, TODO REPLACE THIS SYSTEM
     */
    void processMainMenu(bool* keys);

    /**
     * Processes events during gameplay
     * @param keys ptr to key booleans, TODO REPLACE THIS SYSTEM
     */
    void processGameplay(bool* keys);

    /**
     * Processes events on the pause and death menus
     * @param keys ptr to key booleans, TODO REPLACE THIS SYSTEM
     */
    void processGameMenu(bool* keys); //run game events

    /**
     * Processes events on the pause and death menus
     * @param keys ptr to key booleans, TODO REPLACE THIS SYSTEM
     */
    void processButtonSelection(bool* keys, MenuOptions p_lower, MenuOptions p_higher);

    Animation animateBack; // TODO REFACTOR THIS and then correctly name and place here

public:

    Manager();
    ~Manager() = default;

    /**
     * Processes all game events
     * @param keys ptr to key booleans, TODO REPLACE THIS SYSTEM
     */
    void process(bool* keys);

    /**
     * Draws the game to the screen
     * @param p_camera camera used to draw the world
     */
    void draw(Camera &p_camera);

    /**
     * Getter for games current state
     * @returns The game manger's current game state
     */
    GameState getGameState();

}; // end of Manager

}; // end of namespace game_ctrl

}; // end of namespace mvd
