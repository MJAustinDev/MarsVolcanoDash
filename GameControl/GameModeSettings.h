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

# pragma once

namespace mvd {

namespace game_ctrl {

/*
 TODO RE WORD THIS... AND USE DYOXGEN
index [0] affects meteors, index [1] affects boulders
indices [x][0,...,8] represent the 9 possible spawn positions,
with [0] being behind the player, [4] being straight above the player and [8] infront of the player
i.e., [0] => (spawnX = player.x-32), [4] => (spawnX = player.x), [8] => (spawnX = player.x+32)

*/
struct GameModeSettings {
    // TODO REWORD SO USING STD ARRAY
    int spawnTimes[2]; //default minimum spawn time for enemy
    int ttls[2][9]; //ticks to live, counts time for enemy to spawn at given position
    float spawns[2][9]; //contains y axis value for enemy to spawn above the player at a given position
    float maxSizes[2]; //maximum size that is used to create enemy
    float forces[2]; //initial force applied to enemy when spawned in
    bool twoPlayer; //if game mode has 2 players or not
    float lavaSpeed; //how fast the lava moves when in range of the player

};

/**
 * @brief Modifies the values in the passed structure for easy mode
 * @param p_settings Game Mode Settings structure to be configured
 */
void setGameModeEasy(GameModeSettings &p_settings);

/**
 * @brief Modifies the values in the passed structure for normal mode
 * @param p_settings Game Mode Settings structure to be configured
 */
void setGameModeNormal(GameModeSettings &p_settings);

/**
 * @brief Modifies the values in the passed structure for hard mode
 * @param p_settings Game Mode Settings structure to be configured
 */
void setGameModeHard(GameModeSettings &p_settings);

/**
 * @brief Modifies the values in the passed structure for two player mode
 * @param p_settings Game Mode Settings structure to be configured
 */
void setGameModeTwoPlayer(GameModeSettings &p_settings);

}; // end of namespace game_ctrl

}; // end of namespace mvd
