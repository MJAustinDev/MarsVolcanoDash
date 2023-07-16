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
#include <list>
#include <vector>

#include "camera.h"

namespace mvd {

namespace game_ctrl {

/**
 * Chunk class handles allocation, deallocation and drawing of game terrain.
 */
class Chunk{

public:

    /**
     * Constructor for Chunk objects
     * @param p_world pointer to the box2d world that the chunk exists within
     * @param p_id chunk id, used to generate different chunks
     * @param p_pos position in the world that the body exists at
     */
    Chunk(b2World* p_world, int p_id, b2Vec2 p_pos);
    ~Chunk();

    /**
     * Getter for chunk's position
     * @returns position of chunk's body relative to the world
     */
    b2Vec2 getPosition();

    /**
     * Getter for chunk's y axis change
     * @returns change in y axis due to chunk's gradient
     */
    float getChangeInY();

    /**
     * Draws the chunk to the screen
     * @param p_camera camera used to draw the chunk
     */
    void draw(Camera &p_camera);

private:

    /**
     * Structure that contains draw information for all attached shapes
     */
    struct DrawShape {
        DrawShape(std::array<b2Vec2, 8> &p_points, int p_count, int p_id) :
            m_points(p_points), m_count(p_count), m_id(p_id) {};

        std::array<b2Vec2, 8> m_points;
        int m_count;
        int m_id;
    };

    float m_changeInY = 0.0f;
    b2Body* m_body;
    std::list<DrawShape> m_shapes;

    //define what the world chunk should be
    // TODO RECONSIDER... NOT IDEAL... MAYBE USE RUN TIME POLYGNOMIALISM?? MORE CONTROL/BETTER CONTROL OVER DRAWING TOO?...
    void defSegmentDefault();
    void defSegmentStart();
    void defSegment0();
    void defSegment1();
    void defSegment2();
    void defSegment3();
    void defSegment4();
    void defSegment5();

    /**
     * Adds a new piece of terrain to the chunk.
     * Handles box2d fixture generation and stores the shape's points for drawing
     * @param p_points array containing all the points that make up the shape
     * @param p_count total number of points that the shape has
     * @param p_id unique id that effects drawing of the shape
     */
    void addShape(std::array<b2Vec2, 8> p_points, int p_count, int p_id);

    /**
     * Adds a series of small surface shapes to a chunk (rocks/dust)
     * @param p_x starting x coordinate of the series
     * @param p_yCoords vector containing all y coordinates along surface of the chunk
     * @param p_magnitudes vector containg maximum height of the rock at each y coordinate
     * @param p_minimumMagnitude minimum height of any rock
     */
    void addRock(float p_x, std::vector<float> &p_yCoords, std::vector<float> &p_magnitudes, float p_minimumMagnitude);

}; // end of class Chunk

}; // end of namespace game_ctrl

}; // end of namespace mvd
