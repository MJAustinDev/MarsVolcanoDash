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
#include "gameModeSettings.h"

//base class all enemies are built off
class BaseEnemy {

protected:

    BaseEnemy(int ticks, b2World* w, b2Vec2 pos, b2Vec2 linVel, float angle);
    BaseEnemy(int ticks, b2World* w, b2Vec2 pos,b2Vec2 linVel) : BaseEnemy(ticks,w,pos,linVel,0.0f) {}; //send over position and linear velocity
    BaseEnemy(int ticks, b2World* w, b2Vec2 pos): BaseEnemy(ticks,w,pos,b2Vec2(0.0f,0.0f),0.0f) {}; //send over just position

    ~BaseEnemy();
    void process(); //process game events for base enemy object

    float colour[4]; //object's colour
    int ttl; //ticks to live
    bool terminate = false; //if zero ttl then object can be destroyed
    b2World* world;
    b2Body* body;

};

//base class all big enemies are built off
class BaseBreakableEnemy : protected BaseEnemy {

protected:

    BaseBreakableEnemy(int ticks, b2World* w, b2Vec2 pos, int numSen);
    ~BaseBreakableEnemy();

    void process(); //process game events for breakable base enemy object

    bool collided = false; //if object has collided or not
    int numSensors;
    b2Fixture** sensors;

};


//third size of meteor, fired off centrally outwards
class Fireball : private BaseEnemy {

friend class Node<Fireball>;
friend class Meteor;
friend class EnemyManager;

private:

    Fireball(b2World* w, b2Vec2 pos, b2Vec2 linVel, float s, b2Vec2 force, float* col);
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

    Fragment(b2World* w, b2Vec2 pos, b2Vec2 linVel, float angle, b2PolygonShape* shp, float* col);
    ~Fragment(){}; //base class destroys body

    void draw(Camera* camera);
    b2PolygonShape shape;

};

//on contact breaks into fragments, is a combination of polygons similar to a boxcar body. See: http://boxcar2d.com/about.html
class Boulder : BaseBreakableEnemy{

friend class EnemyManager;
friend class Node<Boulder>;

public:
    //static and public so it can be used by menu animation, is safe to be public as it only modifies the passed parameters
    static void insertSortPoints(float* mags, float* angs); //order points via insertion sort

private:

    Boulder(b2World* w, b2Vec2 pos, float sizeRand, float maxForce);
    ~Boulder(); //involves extra steps

    int overWriteEqual(float* mags, float* angs); //remove equal points by overwriting and then ignoring them
    bool defineShapes(float* mags, float* angs, int valid); //attempt to build the bolder's shapes
    void setDefaultShapes(float sizeRand); //if random shapes fail then create default shape

    void afterCollide(LinkedList<Fragment>* fragments); //break into fragments upon collision
    void draw(Camera* camera);

    //points revolving around a centre 0,0... angle and mag from
    b2PolygonShape* shapes[8] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

};


//player's car class
class Player {

friend class GameManager;
friend class EnemyManager;

private:

    Player(b2World* world, float* col);
    ~Player();

    b2Vec2 getPos(){return mainBody->GetPosition();};

    void processInput(bool keyW, bool keyS, bool keyA, bool keyD);
    void draw(Camera* camera);

    float colour[4];
    float wheelDrawAng = 0.0f; //angle to draw wheels at (prevent rapid flickering when using real angle)
    CarDetails detail; //used to draw details on top of the car body

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

    EnemyManager(b2World* w, Player** p, mvd::game_ctrl::GameModeSettings* cfig);
    ~EnemyManager();

    void attemptSpawn(); //spawn big enemy if correct ticks to live have reached zero
    void process(); //process enemy game events
    void drawLava(Camera* camera); //draws the lava quadrilateral
    void draw(Camera* camera); //draw all enemies to the screen

    template <typename T> void spawnBigEnemy(LinkedList<T>* objList, b2Vec2 pos, float sizeRand, float maxForce);
    template <typename T> void processEnemy(LinkedList<T>* objList);
    template <typename T> void afterProcess(LinkedList<T>* objList);
    template <typename T1, typename T2> void afterProcessBreakable(LinkedList<T1>* bigObjList, LinkedList<T2>* smallObjList);

    mvd::game_ctrl::GameModeSettings* config; //game configuration settings pointer, controls size of enemies, their spawn times, etc

    b2World* world;
    Player** player; //ultimately points to the player in the lead

    float lavaX; //x position of lava
    float lavaSpeed; //speed lava moves along

    //lava draw shape coordinates
    //set corner points
    b2Vec2 baseShape[4] = {b2Vec2(5.0f, -128.0f), b2Vec2(-35.0f, 128.0f), b2Vec2(-128.0f, 128.0f), b2Vec2(-128.0f, -128.0f)}; //bottom right -> top right -> top left -> bottom left
    //random edge points
    b2Vec2 edgeShape[32]; //0-7 bottom edge, 8-15 right edge, 16-23 top edge, 24-31 left edge
    //random internal points
    b2Vec2 innerShape[32];

    //enemies controlled by the enemy manager
    LinkedList<Fireball> fireballs;
    LinkedList<Meteor> meteors;
    LinkedList<Fragment> fragments;
    LinkedList<Boulder> boulders;

};



#endif
