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

#ifndef GAMEMANAGEMENT_H
#define GAMEMANAGEMENT_H

#include "gameEntities.h"

#include "visualStructures.h" //contains the DrawShape structure

#include <decoration.h>

//world chunk, contains terrain for player car to drive across
class Chunk{

friend class GameManager;
friend class Node<Chunk>;

private:

    Chunk(b2World* w, int segID, float x, float y);
    ~Chunk();

    b2Vec2 getPos(){return body->GetPosition();};

    void draw(Camera* camera); //draw each shape attached to main body
    void drawBackDecs(Camera* camera); //draws all the background decorations inside this chunk
    void drawForeDecs(Camera* camera); //draws all the foreground decorations inside this chunk

    //define what the world chunk should be
    void defSegmentDefault();
    void defSegmentStart();
    void defSegment0();
    void defSegment1();
    void defSegment2();
    void defSegment3();
    void defSegment4();
    void defSegment5();

    void addShape(b2Vec2* points, int num, int drawId); //adds polygon shape to linked list
    void addRock(float x, float* y, int points, float minMag, float* manMag); //adds a rock over a space on the chunk's ground

    void addDecoration(LinkedList<Decoration>* ptr, int id, b2Vec2 decPos, float lowest, bool hasBase, float* ptrColour);
    void addBackDec(int id, b2Vec2 decPos, float lowest, bool hasBase = true,  float* ptrColour = nullptr){
        addDecoration(&backDecs, id, decPos, lowest, hasBase, ptrColour);
    }
    void addForeDec(int id, b2Vec2 decPos,  float lowest, bool hasBase = true, float* ptrColour = nullptr){
        addDecoration(&foreDecs, id, decPos, lowest, hasBase, ptrColour);
    }

    b2World* world;
    b2Body* body;
    LinkedList<DrawShape> shapes; //shapes that body's fixture are comprised of, stored so can be drawn
    float changeY = 0.0f;
    LinkedList<Decoration> backDecs; //stores background decorations in a linked list
    LinkedList<Decoration> foreDecs; //stores foreground decorations in a linked list

};


//stores game objects and handles all game events
class GameManager{

public:

    GameManager(GameConfig* config);
    ~GameManager();

    bool process(bool* keys); //process game events
    void draw(Camera* camera); //draws game objects to screen

    float getScore(){return score;}; //returns the score in form of physical distance travelled
    bool getWinnerP1(){return (player == playerLead);}; //returns if player 1 is a head of player 2

private:

    void addChunk(int chunkID); //dynamically adds new terrain to the world
    void processChunkAddition(); //check if new terrain needs to be added
    void processChunkRemoval(); //removes terrain if behind lava

    LinkedList<Chunk> chunks; //stores terrain chunks in linked list

    float nextChunkX = 0.0f; //x position of the next chunk to be added
    float nextChunkY = 0.0f; //y position of the next chunk to be added

    float score = 0.0; //end score of the player

    b2World* world;
    Player* player;
    Player* player2;
    Player* playerLead; //points to who ever is in the lead
    EnemyManager* enemies; //handles all enemy activity

};



#endif
