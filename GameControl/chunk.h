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

#pragma once

#include <RecompileBox2d/box2d/box2d.h>

#include <array>

#include "camera.h"
#include "linkedList.h" // TODO REMOVE THIS

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

}; // end of namespace game_ctrl

}; // end of namespace mvd
