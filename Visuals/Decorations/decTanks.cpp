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

#include "decoration.h"

//set details for intact tank
void Decoration :: setTank1(float baseLevel){

    //background shape
    b2PolygonShape* shape = new b2PolygonShape;
    b2Vec2 points[8];
    points[0].Set(-3.5f, 0.0f);
    points[1].Set(3.5f, 0.0f);
    points[2].Set(3.5f, 12.0f);
    points[3].Set(-3.5f, 12.0f);
    shape->Set(points, 4);
    addShape(shape);

    for (int i=0; i<=1; i++){
        //set top and bottom ridges
        float shift = ((float) i) * 11.8f;
        shape = new b2PolygonShape;
        points[0].Set(-3.75f, 0.0f + shift);
        points[1].Set(3.75f, 0.0f + shift);
        points[2].Set(3.75f, 0.5f + shift);
        points[3].Set(-3.75f, 0.5f + shift);
        shape->Set(points, 4);
        addDetail(shape);

        //ladder poles
        shift = ((float) i) * 1.25f;
        shape = new b2PolygonShape;
        points[0].Set(-3.0f + shift, 0.0f);
        points[1].Set(-2.75f + shift, 0.0f);
        points[2].Set(-2.75f + shift, 13.1f);
        points[3].Set(-3.0f + shift, 13.1f);
        shape->Set(points, 4);
        addDetail(shape);
    }

    //ladder bars
    for (int i=0; i<=17; i++){
        float shift = ((float) i) * 0.7f;
        shape = new b2PolygonShape;
        points[0].Set(-3.25f, 0.75f + shift);
        points[1].Set(-1.25f, 0.75f + shift);
        points[2].Set(-1.25f, 1.0f + shift);
        points[3].Set(-3.25f, 1.0f + shift);
        shape->Set(points, 4);
        addDetail(shape);
    }

    //diamond symbol
    shape = new b2PolygonShape;
    points[0].Set(1.4, 11.0f);
    points[1].Set(0.5f, 8.875f);
    points[2].Set(1.4f, 6.75f);
    points[3].Set(2.3f, 8.875f);
    shape->Set(points, 4);
    addDetail(shape);

    //number on tank
    b2Vec2 tCoords[4] = {b2Vec2(1.0f, 0.0f), b2Vec2(1.0f, 1.0f), b2Vec2(0.0f, 1.0f), b2Vec2(0.0f, 0.0f)};
    shape = new b2PolygonShape;
    points[0].Set(-0.5f, 0.75f);
    points[1].Set(3.25f, 0.75f);
    points[2].Set(3.25f, 6.0f);
    points[3].Set(-0.5f, 6.0f);
    shape->Set(points, 4);
    addTexture(11+randModRanged(10), tCoords, (*shape));
    delete shape;
}
