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

#include <cassert>

#include "chunk.h"
#include "rangedRandom.h"

namespace mvd {

namespace game_ctrl {

Chunk::Chunk(b2World* p_world, int p_id, b2Vec2 p_pos) :
    m_changeInY(0.0f),
    m_body(nullptr),
    m_shapes() {

    b2BodyDef defBody;
    defBody.type = b2_staticBody;
    defBody.position = p_pos;
    m_body = p_world->CreateBody(&defBody);

    //define shapes via switch
    switch(p_id){
        case -1 : {defSegmentStart(); break;}
        case 0 : {defSegment0(); break;}
        case 1 : {defSegment1(); break;}
        case 2 : {defSegment2(); break;}
        case 3 : {defSegment3(); break;}
        case 4 : {defSegment4(); break;}
        case 5 : {defSegment5(); break;}

        default : {defSegmentDefault();}
    }
}

Chunk::~Chunk() {
    // must clear box2d body via box2d destructors
    m_body->GetWorld()->DestroyBody(m_body);
}

b2Vec2 Chunk::getPosition() {
    return m_body->GetPosition();
}

float Chunk::getChangeInY() {
    return m_changeInY;
}

void Chunk::draw(Camera &p_camera) {

    // cycle through and draw all attached shapes inside of the list
    for (auto &shape : m_shapes) {
        // TODO REWORD THIS..
        float baseColour[4] = {0.73f, 0.0f, 0.0f, 1.0f}; // CURSED
        float shade = 1.0f - p_camera.getGlow();

        glBegin(GL_POLYGON);
            glColor4f(baseColour[0]*shade, baseColour[1]*shade, baseColour[2]*shade, baseColour[3]);
            for (int i=0; i<shape.m_count; i++) {
                p_camera.placePoint(p_camera.getCamBodyPos(m_body), shape.m_points.at(i));
            }
        glEnd();
    }
}

void Chunk::addShape(std::array<b2Vec2, 8> p_points, int p_count, int p_id) {
    b2PolygonShape shape;
    shape.Set(p_points.begin(), p_count);
    m_body->CreateFixture(&shape, 0.0f);

    m_shapes.push_back(DrawShape(p_points, p_count, p_id));
}

void Chunk::addRock(float p_x, std::vector<float> &p_yCoords, std::vector<float> &p_magnitudes, float p_minimumMagnitude) {
    // y coordinate and magnitude vectors must be equal
    assert(p_yCoords.size() == p_magnitudes.size());

    // set all magnitures within the min to ... REWORD
    for (auto &magnitude : p_magnitudes) {
        if (magnitude != 0.0f) {
            magnitude = randRanged(p_minimumMagnitude, magnitude);
        }
    }

    for (unsigned int i=0; i < p_yCoords.size()-1; i++) {
        std::array<b2Vec2, 8> points = {
            b2Vec2(p_x, p_yCoords.at(i)),
            b2Vec2(p_x+2, p_yCoords.at(i+1)),
            b2Vec2(p_x+2, p_yCoords.at(i+1) + p_magnitudes.at(i+1)),
            b2Vec2(p_x, p_yCoords.at(i) + p_magnitudes.at(i))
        };
        addShape(points, 4, 2);
        p_x += 2.0f;
    }
}

// TODO RE THINK THIS
std::array<b2Vec2, 8> getDefaultBase() {
    std::array<b2Vec2, 8> points = {
        b2Vec2(-32.0f, 0.0f),
        b2Vec2(-32.0f, -3.0f),
        b2Vec2(32.0f, -3.0f),
        b2Vec2(32.0f, 0.0f)
    };
    return points;
}

// TODO RE THINK THIS
void changeHeight(std::vector<float> &p_coords, float p_change) {
    for (auto &coord: p_coords) {
        coord += p_change;
    }
}

// just a flat platform used when invalid numbers are entered as segment identifier
void Chunk::defSegmentDefault() {
    std::array<b2Vec2, 8> points = getDefaultBase();
    addShape(points, 4, 0);
}

// define starting segment
void Chunk::defSegmentStart() {
    // main base plate
    std::array<b2Vec2, 8> points = {
        b2Vec2(-200.0f, 0.0f),
        b2Vec2(-200.0f, -3.0f),
        b2Vec2(32.0f, -3.0f),
        b2Vec2(32.0f, 0.0f)
    };
    addShape(points, 4, 0);

    //start of mountain slope
    points = {
        b2Vec2(-32.0f,0.0f),
        b2Vec2(-20.0f, 0.0f),
        b2Vec2(-25.0f, 3.0f)
    };
    addShape(points, 3, 0);


    points = {
        b2Vec2(-32.0f, 0.0f),
        b2Vec2(-25.0f, 3.0f),
        b2Vec2(-30.0f, 10.0f)
    };
    addShape(points, 3, 0);

    points = {
        b2Vec2(-32.0f, 0.0f),
        b2Vec2(-30.0f, 10.0f),
        b2Vec2(-32.0f, 32.0f)
    };
    addShape(points, 3, 0);

    // backing of mountain slope
    points = {
        b2Vec2(-32.0f, 0.0f),
        b2Vec2(-32.0f, 32.0f),
        b2Vec2(-100.0f, 400.0f),
        b2Vec2(-200.0f, 400.0f),
        b2Vec2(-200.0f, 0.0f)
    };
    addShape(points, 5, 1);
}

// flat base with randomly assigned rocks
void Chunk::defSegment0() {

    std::array<b2Vec2, 8> points = getDefaultBase();
    addShape(points, 4, 0);

    float x = -32.0f;
    std::vector<float> yCoords(9, 0.0f);

    for (int i=0; i < 4; i++) {
        std::vector<float> magnitudes = {
            0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f
        };
        addRock(x, yCoords, magnitudes, 0.2f);
        x += 16.0f;
    }
}

// downward slope
void Chunk::defSegment1() {

    std::array<b2Vec2, 8> points = {
        b2Vec2(-32.0f, 0.0f),
        b2Vec2(-32.0f, -3.0f),
        b2Vec2(32.0f, -19.0f),
        b2Vec2(32.0f, -16.0f)
    };
    addShape(points, 4, 0);

    float x = -32.0f;
    std::vector<float> yCoords = {
        0.0f, -0.5f, -1.0f, -1.5f, -2.0f, -2.5f, -3.0f, -3.5f, -4.0f
    };

    for (int i=0; i < 4; i++) {
        std::vector<float> magnitudes = {
            0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f
        };
        addRock(x, yCoords, magnitudes, 0.2f);
        x += 16.0f;
        changeHeight(yCoords, -4.0f);
    }
    m_changeInY = -16;
}

// upward slope
void Chunk::defSegment2() {

    std::array<b2Vec2, 8> points = {
        b2Vec2(-32.0f, 0.0f),
        b2Vec2(-32.0f, -3.0f),
        b2Vec2(32.0f, 13.0f),
        b2Vec2(32.0f, 16.0f)
    };
    addShape(points, 4, 0);

    float x = -32.0f;
    std::vector<float> yCoords = {
        0.0f, 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f
    };

    for (int i=0; i < 4; i++) {
        std::vector<float> magnitudes = {
            0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f
        };
        addRock(x, yCoords, magnitudes, 0.2f);
        x += 16.0f;
        changeHeight(yCoords, 4.0f);
    }
    m_changeInY = 16;
}

// greater downward slope with more ragid rocks
void Chunk::defSegment3() {

    std::array<b2Vec2, 8> points = {
        b2Vec2(-32.0f, 0.0f),
        b2Vec2(-32.0f, -3.0f),
        b2Vec2(32.0f, -35.0f),
        b2Vec2(32.0f, -32.0f)
    };
    addShape(points, 4, 0);

    float x = -32.0f;
    std::vector<float> yCoords = {
        0.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, -6.0f, -7.0f, -8.0f
    };

    for (int i=0; i < 4; i++) {
        std::vector<float> magnitudes = {
            0.0f, 2.0f, 4.0f, 1.0f, 3.0f, 1.0f, 4.0f, 2.0f, 0.0f
        };
        addRock(x, yCoords, magnitudes, 0.2f);
        x += 16.0f;
        changeHeight(yCoords, -8.0f);
    }
    m_changeInY = -32;
}

// flat base with more raged randomly assigned rocks
void Chunk::defSegment4() {

    std::array<b2Vec2, 8> points = {
        b2Vec2(-32,0.0),
        b2Vec2(-32,-3.0),
        b2Vec2(32,-3.0),
        b2Vec2(32,0.0)
    };
    addShape(points, 4, 0);

    float x = -32.0f;
    std::vector<float> yCoords(9, 0.0f);

    for (int i=0; i < 4; i++) {
        std::vector<float> magnitudes = {
            0.0f, 1.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f
        };
        addRock(x, yCoords, magnitudes, 0.2f);
        x += 16.0f;
    }
}

// ramp with a pit
void Chunk::defSegment5() {

    std::array<b2Vec2, 8> points = {
        b2Vec2(-32.0f, 0.0f),
        b2Vec2(-32.0f, -3.0f),
        b2Vec2(32.0f, -3.0f),
        b2Vec2(32.0f, 0.0f)
    };
    addShape(points, 4, 0);

    // rocks in the gap, want drawn under both ramps
    std::vector<float> yCoords(9, 0.0f);
    std::vector<float> magnitudes = {
        5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 1.0f, 1.0f, 2.0f, 3.0f
    };
    addRock(0.0f, yCoords, magnitudes, 0.2f);

    // ramp up
    points = {
        b2Vec2(-32.0f, 0.0f),
        b2Vec2(0.0f, 10.0f),
        b2Vec2(0.1f, 0.0f)
    };
    addShape(points, 3, -1);

    yCoords = {
        0.0f, 0.625f, 1.25f, 1.875f, 2.5f, 3.125f,
        3.75f, 4.375f, 5.0f, 5.625f, 6.25f, 6.875f,
        7.5f, 8.125f, 8.75f, 9.375f, 10.0f
    };
    magnitudes = std::vector<float>(17 ,1.0f);
    magnitudes.at(0) = 0.0f; // {0.0f, 1.0f, 1.0f, ..., 1.0f}
    addRock(-32.0f, yCoords, magnitudes, 0.2f);

    // down ramp
    points = {
        b2Vec2(16.0f, 6.0f),
        b2Vec2(15.7f, 0.0f),
        b2Vec2(32.0f, 0.0f)
    };
    addShape(points, 3, -1);

    yCoords = {
        6.0f, 5.25f, 4.5f, 3.75f, 3.0f, 2.25f, 1.5f, 0.75f, 0.0f
    };
    magnitudes = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f
    };
    addRock(16.0f, yCoords, magnitudes, 0.2f);
}

}; // end of namespace game_ctrl

}; // end of namespace mvd
