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

#include "gameEntities.h"
#include "visualStructures.h" //contains the DrawShape structure

namespace mvd {

namespace game_ctrl {

//world chunk, contains terrain for player car to drive across
// TODO MOVE TO OWN FILE
class Chunk{

friend class GameManager; // YEETE
friend class Node<Chunk>; // YEET

public:

    Chunk(b2World* p_world, int p_id, float p_posX, float p_posY);
    ~Chunk();

private:

    /**
     * TODO WORD UP
     */
    struct DrawShape {
        DrawShape(std::array<b2Vec2, 8> &p_points, int p_count, int p_id) :
            m_points(p_points), m_count(p_count), m_id(p_id) {};

        std::array<b2Vec2, 8> m_points;
        int m_count = 0;
        int m_id;
    };

    b2Vec2 getPos(){return body->GetPosition();};

    void draw(Camera* camera); //draw each shape attached to main body

    //define what the world chunk should be
    void defSegmentDefault();
    void defSegmentStart();
    void defSegment0();
    void defSegment1();
    void defSegment2();
    void defSegment3();
    void defSegment4();
    void defSegment5();

    void addShape(std::array<b2Vec2, 8> p_points, int p_count, int p_id);
    //void addShape(b2Vec2* points, int num, int drawId); //adds polygon shape to linked list
    void addRock(float x, float* y, int points, float minMag, float* manMag); //adds a rock over a space on the chunk's ground

    b2World* world;
    b2Body* body;
    std::list<DrawShape> m_shapes;
    //LinkedList<DrawShape> shapes; //shapes that body's fixture are comprised of, stored so can be drawn
    float changeY = 0.0f;

};


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

     LinkedList<Chunk> chunks; //stores terrain chunks in linked list // CHANGE TO LINKED LIST...
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
