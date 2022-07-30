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


#include "gameEntities.h"

#include <iostream>
using namespace std;

//Pythagorean theorem for displacement between 2 points
float pointDistance(b2Vec2 a, b2Vec2 b){
    float x = a.x - b.x;
    float y = a.y - b.y;
    return sqrt((x*x) + (y*y));
}

//law of cosigns to return angle of point against front triangle of lava shape
float cosignRuleAngle(b2Vec2 pos){

    float a = pointDistance(b2Vec2(-5,128), pos); //top left point of triangle to position
    //b = 256 and is right hand span of the triangle
    float c = pointDistance(b2Vec2(-5,-128), pos); //bottom left point of triangle to position
    return acos(((c*c) - (65536) - (a*a)) / (-512*a)); //law of cosigns rearranged to get angle from all 3 sides, with b filled in as it's constant

}

EnemyManager :: EnemyManager(b2World* w, Player** p, gameConfig* cfig){

    world = w;
    player = p;

    //TODO -- ADD LAVA SPEED TO CONFIG -- not priority
    lavaSpeed = 0.1f;
    lavaX = (*player)->getPos().x - 128;
    config = cfig;

    //set random draw points inside of lava polygon
    for (int i=0;i<32;i++){
        bool valid = false;
        while (!valid){
            lavaShapeInner[i].Set(randRanged(-128.0,5.0), randRanged(-128, 128)); //lava point is P
            /*                               B=(-5,128)
            |--------------------------------|\
            |                                | \
            |                                |  \
            |                                |   \
            |                                |    \
            |                                |     \
            |                                |      \
            |                                |       \
            |                                |        \
            |                                |         \
            |                                |          \
            |                                |           \
            |                                |            \
            |--------------------------------|-------------|
                                             A=(-5,-128)   C=(5,128)
            if P is behind or on triangle's left edge (P.x<=-5) then P lies inside the main lava rectangle,
            else if (angle ABP is less than or equal to angle ABC) then P is inside the lava front triangle */
            valid = (lavaShapeInner[i].x <= -5.0f) || (cosignRuleAngle(lavaShapeInner[i]) <= 0.039); //ABC is approx 0.039 radians
        }
    }

    //set random edge draw points for lava polygon
    for (int i=0;i<8;i++){
        lavaShapeEdge[i].Set(randRanged(-128,5.0),-128); //bottom edge
        lavaShapeEdge[i+8].x = randRanged(-5,5); //right edge x axis
        lavaShapeEdge[i+8].y = (-25.6*lavaShapeEdge[i+8].x) + 1; //right edge y axis using y = mx + c, using B and C m = -25.6 and c = 1
        lavaShapeEdge[i+16].Set(randRanged(-128,-5.0),128); //top edge
        lavaShapeEdge[i+24].Set(-128, randRanged(-128,128)); //left edge
    }
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

    //draw lava
    camera->drawLava(lavaX, (*player)->getPos().y, lavaShapePoints, lavaShapeEdge, lavaShapeInner);
}
