
#include "gameEntities.h"

#include <iostream>
using namespace std;

EnemyManager :: EnemyManager(b2World* w, Player** p, gameConfig* cfig){

    world = w;
    player = p;

    //TODO -- ADD LAVA SPEED TO CONFIG -- not priotity
    lavaSpeed = 0.1f;
    lavaX = (*player)->getPos().x - 128;
    config = cfig;

}

EnemyManager :: ~EnemyManager(){

    //empty all linked lists allowing for objects to be removed by the b2world destructors
    while(fireballs.first!=nullptr){
        fireballs.remFront();
    }
    while(meteors.first!=nullptr){
        meteors.remFront();
    }
    while(fragments.first!=nullptr){
        fragments.remFront();
    }
    while(boulders.first!=nullptr){
        boulders.remFront();
    }

}

//spawn a large enemy type, either meteor or boulder
template <typename T>
void EnemyManager :: spawnBigEnemy(LinkedList<T>* objList, b2Vec2 pos, float sizeRand, float maxForce){
    T* obj = new T(world,pos,sizeRand,maxForce);
    objList->addEnd(obj);
    obj = nullptr;
}

//check if big enemies can be spawned or not
void EnemyManager :: attemptSpawn(){

    //for each possible spawn position
    for (int i=0;i<9;i++){
        b2Vec2 pos((i*8)-32+(*player)->getPos().x,0); //get world x position relative to the player
        //for each enemy type
        for(int j=0;j<2;j++){
            config->ttls[j][i] --; //lower ticks to live
            //if should spawn or not
            if (config->ttls[j][i] <= 0){
                config->ttls[j][i] = randRanged(config->spawnTimes[j],4.0f*config->spawnTimes[j]); //reset ttl
                pos.y = (*player)->getPos().y + config->spawns[j][i]; //get world y position relative to player
                //switch for potential future enemy types
                switch (j){
                    case 0 : {spawnBigEnemy(&meteors, pos, config->maxSizes[j], config->forces[j]); break;}
                    case 1 : {spawnBigEnemy(&boulders, pos, config->maxSizes[j], config->forces[j]); break;}
                }
            }
        }
    }

}

//process enemy game events
void EnemyManager :: process(){

    //check if new enemies can spawn
    attemptSpawn();

    //check collisions
    processEnemy(&fireballs);
    processEnemy(&fragments);
    processEnemy(&meteors);
    processEnemy(&boulders);

    //process ticks to live and "break-ability"
    afterProcess(&fireballs);
    afterProcess(&fragments);
    afterProcessBreakable(&meteors,&fireballs);
    afterProcessBreakable(&boulders,&fragments);

    //handle lava movement
    if (lavaX < (*player)->getPos().x -128){
        lavaX = (*player)->getPos().x -128;
    } else {
        lavaX += lavaSpeed;
    }

}

//process game events for a given type of enemy
template <typename T>
void EnemyManager :: processEnemy(LinkedList<T>* objList){
    if (objList->resetCycle()){
        do {
            objList->cycle->obj->process();
        } while (objList->cycleUp());
    }
}

//process after game events for a small enemy type
template <typename T>
void EnemyManager :: afterProcess(LinkedList<T>* objList){

    if (objList->resetCycle()){
        do {
            if (objList->cycle->obj->terminate){
                objList->remCycle();
            }
        } while (objList->cycleUp());
    }
}

//process after game events for a large enemy type
template <typename T1, typename T2>
void EnemyManager :: afterProcessBreakable(LinkedList<T1>* bigObjList, LinkedList<T2>* smallObjList){
    if (bigObjList->resetCycle()){
        do {
            if (bigObjList->cycle->obj->collided){
                bigObjList->cycle->obj->afterCollide(smallObjList);
                bigObjList->remCycle();
            }
            else if (bigObjList->cycle->obj->terminate){
                bigObjList->remCycle();
            }
        } while (bigObjList->cycleUp());
    }
};


//draw enemies that exist in the world
void EnemyManager :: draw(Camera* camera){

    //draw meteors
    if (meteors.resetCycle()){
        do {
            meteors.cycle->obj->draw(camera);
        } while (meteors.cycleUp());
    }

    //draw boulders
    if (boulders.resetCycle()){
        do {
            boulders.cycle->obj->draw(camera);
        } while (boulders.cycleUp());
    }

    //draw fireballs
    if (fireballs.resetCycle()){
        do {
            fireballs.cycle->obj->draw(camera);
        } while (fireballs.cycleUp());
    }

    //draw fragments
    if (fragments.resetCycle()){
        do {
            fragments.cycle->obj->draw(camera);
        } while (fragments.cycleUp());
    }

    //draw lava -- TODO MAKE PERNIMENT VAR TYPE ONCE LAVA STYLE AND SIZE HAS BEEN FINALISED
    GLfloat bodyPos[2] = {lavaX,0};
    GLfloat posShape[8] = {0,-64,-5,64,-20,64,-20,-64};
    GLfloat col[3] = {0,1,0};
    camera->drawPolygon(bodyPos,0.0,posShape,4,col);

}