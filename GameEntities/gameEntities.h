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

#ifndef GAMEENTITIES_H
#define GAMEENTITIES_H


#include <RecompileBox2d/box2d/box2d.h>
#include "linkedList.h"
#include "camera.h"
#include "rangedRandom.h"
#include "gameConfig.h"

//base class all enemies are built off
class BaseEnemy {

protected:

    BaseEnemy(int ticks, b2World* w, b2Vec2 pos, b2Vec2 linVel, float angle);
    BaseEnemy(int ticks, b2World* w, b2Vec2 pos,b2Vec2 linVel) : BaseEnemy(ticks,w,pos,linVel,0.0f) {}; //send over position and linear velocity
    BaseEnemy(int ticks, b2World* w, b2Vec2 pos) : BaseEnemy(ticks,w,pos,b2Vec2(0.0f,0.0f),0.0f) {}; //send over just position

    ~BaseEnemy();
    void process();

    int ttl;
    bool terminate = false;
    b2World* world;
    b2Body* body;

};

//base class all big enemies are built off
class BaseBreakableEnemy : protected BaseEnemy {


protected:

    BaseBreakableEnemy(int ticks, b2World* w, b2Vec2 pos, int numSen);
    ~BaseBreakableEnemy();

    void process();

    bool collided = false;
    int numSensors;
    b2Fixture** sensors;

};



//third size of meteor, fired off centrally outwards
class Fireball : private BaseEnemy {

friend class Node<Fireball>;
friend class Meteor;
friend class EnemyManager;

private:

    Fireball(b2World* w, b2Vec2 pos, b2Vec2 linVel, float s, b2Vec2 force);
    ~Fireball(){}; //base class destroys body

    void draw(Camera* camera);

    b2CircleShape shape;
};


//on contact explodes into smaller fireballs, is a circle
class Meteor : BaseBreakableEnemy{

friend class EnemyManager;
friend class Node<Meteor>;

private:

    Meteor(b2World* w, b2Vec2 pos, float sizeRand, float maxForce);
    ~Meteor(){}; //base class destroys body

    void afterCollide(LinkedList<Fireball>* fireballs); //break into fireballs after collision
    void draw(Camera* camera);

    b2CircleShape shape;

};

//smaller fragment of a boulder, is a polygon
class Fragment : private BaseEnemy{

friend class EnemyManager;
friend class Boulder;
friend class Node<Fragment>;

private:

    Fragment(b2World* w, b2Vec2 pos, b2Vec2 linVel, float angle, b2PolygonShape* shp);
    ~Fragment(){}; //base class destroys body

    void draw(Camera* camera);
    b2PolygonShape shape;

};



//on contact breaks into fragments, is a combination of polygons similar to a boxcar body. See: http://boxcar2d.com/about.html
class Boulder : BaseBreakableEnemy{

friend class EnemyManager;
friend class Node<Boulder>;

private:

    Boulder(b2World* w, b2Vec2 pos, float sizeRand, float maxForce);
    ~Boulder(); //involves extra steps

    void insertSortPoints(float* mags, float* angs); //order points via insertion sort
    int overWriteEqual(float* mags, float* angs); //remove equal points by overwriting and then ignoring them
    bool defineShapes(float* mags, float* angs, int valid); //attempt to build the bolder's shapes
    void setDefaultShapes(float sizeRand); //if random shapes fail then create default shape

    void afterCollide(LinkedList<Fragment>* fragments); //break into fragments upon collision
    void draw(Camera* camera);

    //points revolving around a centre 0,0... angle and mag from
    b2PolygonShape* shapes[8] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};

};


//player's car class
class Player {

friend class GameManager;
friend class EnemyManager;


private:

    Player(b2World* world, GLfloat* colour);
    ~Player();

    b2Vec2 getPos(){return mainBody->GetPosition();};

    void processInput(bool keyW, bool keyS, bool keyA, bool keyD);
    void draw(Camera* camera);

    GLfloat col[3]; //TODO -- UPDATE COLOUR SYSTEM

    b2Body* mainBody;
    b2Body* wheelBack;
    b2Body* wheelFront;

    b2PolygonShape mainShape;
    b2PolygonShape roofShape;
    b2CircleShape wheelShape;

    b2WheelJoint* motorBack;
    b2WheelJoint* motorFront;

};

//controls enemy spawning and behaviour
class EnemyManager {

friend class GameManager;

private:

    EnemyManager(b2World* w, Player** p, gameConfig* cfig);
    ~EnemyManager();

    void attemptSpawn(); //spawn big enemy if correct ticks to live have reached zero
    void process(); //process enemy game events
    void draw(Camera* camera);

    template <typename T> void spawnBigEnemy(LinkedList<T>* objList, b2Vec2 pos, float sizeRand, float maxForce);
    template <typename T> void processEnemy(LinkedList<T>* objList);
    template <typename T> void afterProcess(LinkedList<T>* objList);
    template <typename T1, typename T2> void afterProcessBreakable(LinkedList<T1>* bigObjList, LinkedList<T2>* smallObjList);

    gameConfig* config; //game configuration settings pointer, controls size of enemies, their spawn times, etc

    b2World* world;
    Player** player; //ultimately points to the player in the lead

    float lavaX; //x position of lava
    float lavaSpeed; //speed lava moves along

    //enemies controlled by the enemy manager
    LinkedList<Fireball> fireballs;
    LinkedList<Meteor> meteors;
    LinkedList<Fragment> fragments;
    LinkedList<Boulder> boulders;

};



#endif
