
#include "gameManagement.h"

#include <iostream>
using namespace std;

GameManager :: GameManager(gameConfig* config) {

    //define world
    b2Vec2 gravity(0.0f,-9.81);
    world = new b2World(gravity);

    //add terrain to world TODO -- ADJUST START SET UP PROPERLY
    addChunk(-1); //starting chunk
    addChunk(0);
    addChunk(0);
    addChunk(0);
    addChunk(0);

    //define player
    GLfloat col[3] = {0,0,1};
    player = new Player(world,col);
    if (config->twoPlayer){
        col[0] = 1;
        col[1] = 1;
        col[2] = 0;
        player2 = new Player(world,col);
    } else {
        player2 = nullptr;
    }
    playerLead = player;

    //define enemy controller
    enemies = new EnemyManager(world,&playerLead,config);

}

GameManager :: ~GameManager() {

    while (chunks.first!=nullptr){
        chunks.remFront(); //have to destroy via linked list to prevent world attempting to delete bodies multiple times
    }
    delete enemies;
    delete player;
    delete world;

}

//adds new chunk to the linked list, handles memory allocation and pointers
void GameManager :: addChunk(int chunkID){

    Chunk* chunk = new Chunk(world,chunkID,nextChunkX,nextChunkY); //define chunk in memory
    chunks.addEnd(chunk); //add to the linked list
    //update next chunk position
    nextChunkX += 64.0f;
    //TODO -- SWITCH CASE ON ID THAT EFFECT Y POSITION
    chunk = nullptr; //clear pointer, chunk is stored in the linked list

}

//handle game events
bool GameManager :: process(bool** keys){
    //handle user input
    player->processInput(*(keys[2]), *(keys[3]), *(keys[0]), *(keys[1]));
    score = player->getPos().x;
    if (player2!=nullptr){
        player2->processInput(*(keys[10]), *(keys[11]), *(keys[8]), *(keys[9]));
        if (score > player2->getPos().x){
            score = player2->getPos().x;
            playerLead = player; //player 1 is in the lead
        } else {
            playerLead = player2; //player 2 is in the lead
        }
    }

    world->Step((1.0f/60.0f), 8, 3); //run physics simulation
    processChunkAddition(); //check if new chunks need to be generated
    enemies->process(); //process enemy activity
    processChunkRemoval(); //remove old chunks if needed

    //check if game is over
    if (score < enemies->lavaX-2.5){
        return true;
    }
    return false;
}

//check if new terrain should be added
void GameManager :: processChunkAddition() {
    //add new chunk if the leading player is 256 meters behind the next chunk's spawn point
    while (nextChunkX<=playerLead->getPos().x+256){
        //TODO -- RANDOMISE CHUNK TYPES
        addChunk(0);
    }

}

//removes chunks if they are 70 meters behind the lava
void GameManager :: processChunkRemoval() {
    //safety catch, but chunks should never be empty
    if (chunks.first!=nullptr){
        bool repeat = true;
        //remove chunks from the front of the linked list until no chunks are behind the lava
        while (repeat){
            if (chunks.first->obj->getPos().x <= enemies->lavaX - 70) {
                chunks.remFront();
                if (chunks.first==nullptr){
                    repeat = false; //safety catch, shouldn't be hit
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
    if (chunks.resetCycle()){
        do {
            chunks.cycle->obj->draw(camera);
        } while (chunks.cycleUp());
    }

    player->draw(camera);
    if (player2!=nullptr){
        player2->draw(camera);
    }

    enemies->draw(camera);

}


