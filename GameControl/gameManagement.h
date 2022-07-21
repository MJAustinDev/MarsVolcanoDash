#ifndef GAMEMANAGEMENT_H
#define GAMEMANAGEMENT_H

#include "gameEntities.h"


/*
World chunk, contains terrain for player car to drive across
*/
class Chunk{

friend class GameManager;
friend class Node<Chunk>;

private:

    Chunk(b2World* w, int segID, GLfloat x, GLfloat y);
    ~Chunk();

    b2Vec2 getPos(){return body->GetPosition();};

    void draw(Camera* camera); //draw each shape attached to main body

    void defSegmentStart();
    void defSegment0();
    //TODO -- EXPAND SEGMENTS FOR MORE VARIETY

    void addShape(b2Vec2* points, int num); //adds polygon shape to linked list
    void addRock(float x, float* y, int points, float minMag, float* manMag); //adds a rock over a space on the chunk's ground

    b2World* world;
    b2Body* body;
    LinkedList<b2PolygonShape> shapes; //shapes that body's fixture are comprised of, stored so can be drawn

};



/*
Stores game objects and handles all game events
*/
class GameManager{

//TODO -- POSSIBLY MAKE COMPLETELY PRIVATE AND HAVE AS A FRIEND TO THE MANAGER CLASS

public:

    GameManager(gameConfig* config);
    ~GameManager();

    bool process(bool** keys); //process game events
    void draw(Camera* camera); //draws game objects to screen

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
