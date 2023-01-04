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

#include <decoration.h>

//set decoration zero (currently just test asset, will be changed later)
void Decoration :: setID0(){

    b2PolygonShape* shape = new b2PolygonShape;
    b2Vec2 points[8];
    points[0].Set(-3.5f, 0.0f);
    points[1].Set(3.5f, 0.0f);
    points[2].Set(3.0f, 3.5f);
    points[3].Set(-3.0f, 3.5f);
    shape->Set(points, 4);
    addShape(shape);
    shape = nullptr;

    shape = new b2PolygonShape;
    points[0].Set(-3.0f, 3.5f);
    points[1].Set(3.0f, 3.5f);
    points[2].Set(3.0f, 16.5f);
    points[3].Set(-3.0f, 16.5f);
    shape->Set(points, 4);
    addShape(shape);
    shape = nullptr;

    shape = new b2PolygonShape;
    points[0].Set(-3.0f, 16.5f);
    points[1].Set(3.0f, 16.5f);
    points[2].Set(2.0f, 18.0f);
    shape->Set(points, 3);
    addShape(shape);
    shape = nullptr;

    //adds a number 7 to it
    b2Vec2 tCoords[4] = {b2Vec2(1.0f, 0.0f), b2Vec2(1.0f, 1.0f), b2Vec2(0.0f, 1.0f), b2Vec2(0.0f, 0.0f)};
    b2PolygonShape tShape;
    points[0].Set(-2.5f, 4.0f);
    points[1].Set(2.5f, 4.0f);
    points[2].Set(2.5f, 12.0f);
    points[3].Set(-2.5f, 12.0f);
    tShape.Set(points, 4);
    addTexture(18, tCoords, tShape);

}

