/*

MIT License

Copyright (c) 2022-2023 Matthew James Austin

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
    m_pauseButtons({
        MenuButton(MenuOptions::resume_game, kHighlights.at(5), kBackings.at(5)),
        MenuButton(MenuOptions::main_menu, kHighlights.at(6), kBackings.at(6)),
    }),
    m_gameManager(nullptr),
    m_state(GameState::on_main_menu),
    m_isTwoPlayer(false) {

    // TODO COME BACK TO TEXTURES
}

GameState Manager::getGameState() {
    return m_state;
}

void Manager::process(bool* keys) {

    switch (m_state) {
        case GameState::on_main_menu : {
            processButtonSelection(keys, MenuOptions::easy_mode, MenuOptions::exit_game);
            processMainMenu(keys);
            break;
        }
        case GameState::in_game_alive : {
            processGameplay(keys);
            break;
        }
        case GameState::in_game_paused : {
            processButtonSelection(keys, MenuOptions::resume_game, MenuOptions::main_menu);
            processGameMenu(keys);
            break;
        }
        case GameState::in_game_dead : {
            processButtonSelection(keys, MenuOptions::main_menu, MenuOptions::main_menu);
            processGameMenu(keys);
            break;
        }

        default : {throw std::runtime_error("Invalid game state");}
    }
}

void Manager::processMainMenu(bool* keys){
    if (keys[6]) { // ENTER is pressed -- user has selected an option
        GameModeSettings settings;
        GameMode gameMode; // TODO SOME KIND OF CAST BETWEEN THE ENUM TYPES INSTEAD...
        switch (m_selectedMenuButton) {
            case MenuOptions::easy_mode : {gameMode = GameMode::easy; break;}
            case MenuOptions::normal_mode : {gameMode = GameMode::normal; break;}
            case MenuOptions::hard_mode : {gameMode = GameMode::hard; break;}
            case MenuOptions::two_player_mode : {gameMode = GameMode::two_player; break;}
            case MenuOptions::exit_game : {m_state = GameState::exitting_game; return;}
            default : {throw std::runtime_error("Invalid menu selection");}
        }
        setGameMode(gameMode, settings);
        m_gameManager = std::make_unique<GameManager>(&settings);
        m_state = GameState::in_game_alive;
        m_isTwoPlayer = (m_selectedMenuButton == MenuOptions::two_player_mode);
    }
    animateBack.process(); //update background animation
}

void Manager::processGameMenu(bool* keys) {
    if (keys[6]) { // ENTER PRESSED TODO PROPER NAMING
        switch (m_selectedMenuButton) {
            case MenuOptions::main_menu : {
                m_state = GameState::on_main_menu;
                m_gameManager.reset();
                m_selectedMenuButton = MenuOptions::easy_mode;
                break;
            }
            case MenuOptions::resume_game : {
                m_state = GameState::in_game_alive;
                break;
            }
            default : {throw std::runtime_error("invalid menu selection");} // should never be hit
        }
        keys[6] = false;
    }
}

void Manager :: processGameplay(bool* keys){
    if (keys[7]){ // ESCAPE is pressed -- user has paused game
        m_state = GameState::in_game_paused;
        m_selectedMenuButton = MenuOptions::resume_game;
    } else {
        if (m_gameManager->process(keys)) {
            m_state = GameState::in_game_dead;
        }
    }
}

void Manager::processButtonSelection(bool* keys, MenuOptions p_lower, MenuOptions p_higher) {
    // TODO REWORD THIS SO IT'S CLEANER.. MAYBE OVERLOAD FOR MENUOPTIONS TYPE???
    auto selected = static_cast<int>(m_selectedMenuButton);
    auto lower = static_cast<int>(p_lower);
    auto higher = static_cast<int>(p_higher);

    assert(lower <= higher);

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

// TODO THIS UP PROPERLY
void drawButton(Camera &p_camera, const MenuButton &p_button, bool p_highlight) {
    float colour[4] = COLOUR_PURPLE;
    float backColour[4] = COLOUR_PURPLE_BUTTON;
    if (p_highlight) {
        for (int i=0;i<3;i++){
            colour[i] *= 0.5f; //dull main colour if not selected
        }
    }
    //draw highlight then backing panel over it
    p_camera.drawPureRect(colour, p_button.m_highlight);
    p_camera.drawPureRect(backColour, p_button.m_backing);
}

// draws either the menu or the running game
void Manager :: draw(Camera &p_camera) {

    switch (m_state) {
        case GameState::on_main_menu : {
            animateBack.draw(p_camera);
            for (auto menuButton : m_menuButtons) {
                drawButton(p_camera, menuButton, (menuButton.m_option != m_selectedMenuButton));
            }
            break;
        }
        case GameState::in_game_alive : {
            p_camera.processInput();
            m_gameManager->draw(&p_camera);
            break;
        }
        case GameState::in_game_paused : {
            m_gameManager->draw(&p_camera); // TODO CHANGE TO JUST PASS CAMERA... BASE GAME MANAGER WILL CHANGE!!!
            for (auto pauseButton : m_pauseButtons) {
                drawButton(p_camera, pauseButton, (pauseButton.m_option != m_selectedMenuButton));
            }
            break;
        }
        case GameState::in_game_dead : {
            m_gameManager->draw(&p_camera);
            // TODO DRAW SCORE/PLAYER X WINS 'BUTTON'
            drawButton(p_camera, m_pauseButtons.at(1), true); // TODO NAMED INDICATOR OF 1...
            break;
        }
        default : {throw std::runtime_error("Invalid game state");}
    }
}

}; // end of namespace game_ctrl

}; // end of namespace mvd


// OLD FUNCS TO LOOK INTO RE-IMPLEMENTING/NUKING probs nuke lol

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

