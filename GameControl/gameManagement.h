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

#pragma once

#include <list>

#include "chunk.h"
#include "gameEntities.h"

namespace mvd {

namespace game_ctrl {

//stores game objects and handles all game events
class GameManager {

public:

    GameManager(mvd::game_ctrl::GameModeSettings* config);
    ~GameManager();

    bool process(bool* keys); //process game events
    void draw(Camera* camera); //draws game objects to screen

    float getScore(){return score;}; //returns the score in form of physical distance travelled
    bool getWinnerP1(){return (player == playerLead);}; //returns if player 1 is a head of player 2

private:

    // TODO REWORD THESE 3
    void addChunk(int chunkID); //dynamically adds new terrain to the world
    void processChunkAddition(); //check if new terrain needs to be added
    void processChunkRemoval(); //removes terrain if behind lava

     LinkedList<Chunk> chunks; //stores terrain chunks in linked list // CHANGE TO STD LIST...
    //std::list<chunk> m_chunks;

    float nextChunkX = 0.0f; //x position of the next chunk to be added
    float nextChunkY = 0.0f; //y position of the next chunk to be added

    float score = 0.0; //end score of the player

    b2World m_world;
    Player* player; // TODO CHANGE type
    Player* player2; // TODO CHANGE type
    Player* playerLead; // TODO YEET points to who ever is in the lead
    EnemyManager m_enemyManager;


    /*
    b2World* world;
    Player* player;
    Player* player2;
    Player* playerLead; //points to who ever is in the lead
    EnemyManager* enemies; //handles all enemy activity
    */
};

}; // end of namespace game_ctrl

}; // end of namespace mvd
