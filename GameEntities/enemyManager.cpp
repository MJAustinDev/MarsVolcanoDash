/*

MIT License

Copyright (c) 2023 Matthew James Austin

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

#include "visualColours.h"

//Pythagorean theorem for displacement between 2 points
float pointDistance(b2Vec2 a, b2Vec2 b){
    float x = a.x - b.x;
    float y = a.y - b.y;
    return sqrt((x*x) + (y*y));
}

//law of cosigns to return angle of point against front triangle of lava shape
float cosignRuleAngle(b2Vec2 pos){

    float a = pointDistance(b2Vec2(-35,128), pos); //top left point of triangle to position
    //b = 256 and is right hand span of the triangle
    float c = pointDistance(b2Vec2(-35,-128), pos); //bottom left point of triangle to position
    return acos(((c*c) - (65536) - (a*a)) / (-512*a)); //law of cosigns rearranged to get angle from all 3 sides, with b filled in as it's constant

}

EnemyManager :: EnemyManager(b2World* w, Player** p, GameConfig* cfig){

    world = w;
    player = p;
    config = cfig;

    lavaX = (*player)->getPos().x - 128;
    lavaSpeed = config->lavaSpeed;

    //set random draw points inside of lava polygon
    for (int i=0;i<32;i++){
        bool valid = false;
        while (!valid){
            innerShape[i].Set(randRanged(-128.0,5.0), randRanged(-128, 128)); //lava point is P
            /*                               B=(-35,128)
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
                                             A=(-35,-128)   C=(5,128)
            if P is behind or on triangle's left edge (P.x<=-5) then P lies inside the main lava rectangle,
            else if (angle ABP is less than or equal to angle ABC) then P is inside the lava front triangle */
            valid = (innerShape[i].x <= -35.0f) || (cosignRuleAngle(innerShape[i]) <= 0.15); //ABC is approx 0.15 radians
        }
    }

    //set random edge draw points for lava polygon
    for (int i=0;i<8;i++){
        edgeShape[i].Set(randRanged(-128,5.0),-128); //bottom edge
        edgeShape[i+8].x = randRanged(-35,5); //right edge x axis
        edgeShape[i+8].y = (-6.4*edgeShape[i+8].x) -96;//right edge y axis using y = mx + c, using B and C m = -6.4 and c = -96
        edgeShape[i+16].Set(randRanged(-128,-35.0),128); //top edge
        edgeShape[i+24].Set(-128, randRanged(-128,128)); //left edge
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
                config->ttls[j][i] = randRanged(config->spawnTimes[j], 4.0f*config->spawnTimes[j]); //reset ttl
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
    afterProcessBreakable(&meteors, &fireballs);
    afterProcessBreakable(&boulders, &fragments);

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
}

//draws the lava quadrilateral
void EnemyManager :: drawLava(Camera* camera){

    b2Vec2 posBody = camera->getCamPos(b2Vec2(lavaX, (*player)->getPos().y));
    float shade = 0.95f-(camera->getGlow());
    float alphaEdge = 0.11f-(camera->getGlow());
    float alphaInner = (camera->getGlow())*0.125;
    float dispPos = (10.0f*(camera->getGlow()));
    float dispNeg = dispPos*-1;
    float* dispX = &dispPos;
    float* dispY = &dispPos;

    //set up used colours
    float baseColour[4] = COLOUR_LAVA_0;
    //internal point colours
    float c1[4] = COLOUR_LAVA_1;
    float c2[4] = COLOUR_LAVA_2;
    float c3[4] = COLOUR_LAVA_3;
    float c4[4] = COLOUR_LAVA_4;
    float* c;
    //shade all colours once then reuse later
    for (int i=0;i<5;i++){
        switch(i){
            case 0 : {c = baseColour; break;}
            case 1 : {c = c1; break;}
            case 2 : {c = c2; break;}
            case 3 : {c = c3; break;}
            case 4 : {c = c4; break;}
        }
        for (int j=0;j<3;j++){
            c[j] *= shade;
        }
    }

    //draw base shape
    glBegin(GL_POLYGON);
        glColor4f(baseColour[0], baseColour[1], baseColour[2],0.90f);
        for (int i=0;i<4;i++){
            camera->placePoint(posBody, baseShape[i]);
        }
    glEnd();

    //draw inner shading
    for (int k=0;k<4;k++){ //for each outer point
        glBegin(GL_POLYGON);
            glColor4f(baseColour[0], baseColour[1], baseColour[2],0.25f);
            camera->placePoint(posBody, baseShape[k]);
            for (int j=0;j<4;j++){ //for each pair of edge points
                //one edge
                glColor4f(baseColour[0], baseColour[1], baseColour[2], alphaEdge);
                camera->placePoint(posBody, edgeShape[j+(k*8)]);
                for (int i=0;i<8;i++){ //for 8 inner points
                    switch (i%4){ //displacement will in part carry from the previous, so just set the changed axis
                        case 0 : {c = c1; dispX = &dispPos; break;} //displacement X +, displacement Y +
                        case 1 : {c = c2; dispY = &dispNeg; break;} //displacement X +, displacement Y -
                        case 2 : {c = c3; dispX = &dispNeg; break;} //displacement X -, displacement Y -
                        case 3 : {c = c4; dispY = &dispPos; break;} //displacement X -, displacement Y +
                    }
                    glColor4f(c[0], c[1], c[2], alphaInner);
                    b2Vec2 movePoint = innerShape[i+(j*8)]; //split into 4 chunks each 8 wide
                    movePoint.x+=(*dispX);
                    movePoint.y+=(*dispY);
                    camera->placePoint(posBody, movePoint);
                }
                //another edge
                glColor4f(baseColour[0], baseColour[1], baseColour[2], alphaEdge);
                camera->placePoint(posBody, edgeShape[j+4]);
            }
        glEnd();
    }
}

