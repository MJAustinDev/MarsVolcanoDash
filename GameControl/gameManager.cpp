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

#include "gameManagement.h"
#include "visualColours.h"

namespace mvd {

namespace game_ctrl {

b2Vec2 kGravity(0.0f, -9.81f);

GameManager :: GameManager(mvd::game_ctrl::GameModeSettings* p_settings) :
    m_world(kGravity),
    m_enemyManager(&m_world, &playerLead, p_settings) {

    // TODO BETTER WAY OF DOING THIS... add terrain to world
    addChunk(-1); //starting chunk // TODO USE NAMED ENUM...
    addChunk(-2); //flat platform
    addChunk(-2); //flat platform
    addChunk(-2); //flat platform

    //define player // TODO YEET THIS code...
    float colour[4] = COLOUR_PURPLE;
    player = new Player(&m_world, colour);
    if (p_settings->twoPlayer){
        float colour2[4] = COLOUR_ORANGE;
        player2 = new Player(&m_world, colour2);
    } else {
        player2 = nullptr;
    }
    playerLead = player;
}

GameManager :: ~GameManager() {

    while (chunks.first!=nullptr){
        chunks.remFront(); //have to destroy via linked list to prevent world attempting to delete bodies multiple times
    }

    delete player; // TODO YEET

}

//adds new chunk to the linked list, handles memory allocation and pointers
void GameManager :: addChunk(int chunkID){

    Chunk* chunk = new Chunk(&m_world, chunkID, nextChunkX, nextChunkY); //define chunk in memory
    chunks.addEnd(chunk); //add to the linked list
    //update next chunk position
    nextChunkX += 64.0f;
    nextChunkY += chunk->changeY;
    chunk = nullptr; //clear pointer, chunk is stored in the linked list

}

//handle game events
bool GameManager :: process(bool* keys){
    //handle user input
    player->processInput(keys[2], keys[3], keys[0], keys[1]);
    score = player->getPos().x;
    if (player2!=nullptr){
        player2->processInput(keys[10], keys[11], keys[8], keys[9]);
        if (score > player2->getPos().x){
            score = player2->getPos().x;
            playerLead = player; //player 1 is in the lead
        } else {
            playerLead = player2; //player 2 is in the lead
        }
    }

    m_world.Step((1.0f/60.0f), 8, 3); //run physics simulation
    processChunkAddition(); //check if new chunks need to be generated
    m_enemyManager.process(); //process enemy activity
    processChunkRemoval(); //remove old chunks if needed

    //check if game is over
    if (score < (m_enemyManager.lavaX - 18.5f)){ //while in  game score is viewed as players x position
        return true;
    }
    return false;
}

//check if new terrain should be added
void GameManager :: processChunkAddition() {
    //add new chunk if the leading player is 256 meters behind the next chunk's spawn point
    while (nextChunkX<=playerLead->getPos().x+256){
        // addChunk(randModRanged(6)); // USE CONSTANT FOR MAXIMUM...
        addChunk(-2); // TODO RE IMPLEMENT RANGED
    }
}

//removes chunks if they are 128 meters behind the lava
void GameManager :: processChunkRemoval() {
    //safety catch, but chunks should never be empty (needed in very early testing stages, but you cant die being too careful)
    if (chunks.first!=nullptr){
        bool repeat = true;
        //remove chunks from the front of the linked list until no chunks are behind the lava
        while (repeat){
            if (chunks.first->obj->getPos().x <= m_enemyManager.lavaX - 128) {
                chunks.remFront();
                if (chunks.first==nullptr){
                    repeat = false; //safety catch, shouldn't be hit. Was an issue before death mechanic
                }
            } else {
                repeat = false;
            }
        }
    }
}

//draw world and game objects to the screen
void GameManager :: draw(Camera* camera){
    camera->centreCam(playerLead->mainBody->GetPosition()); //centre camera around the in lead player

    m_enemyManager.draw(camera); //draw enemies before player and terrain

    //draw player/players
    player->draw(camera);
    if (player2!=nullptr){
        player2->draw(camera);
    }
    m_enemyManager.drawLava(camera); //draw lava after player

    camera->drawDust(); //draw dust storm over all entities and lava

    //draw terrain
    if (chunks.resetCycle()){
        do {
            chunks.cycle->obj->draw(camera);
        } while (chunks.cycleUp());
    }
}

}; // end of namespace game_ctrl

}; // end of namespace mvd
