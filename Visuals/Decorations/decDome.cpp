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

#include "decoration.h"

//constructor for dome's 1 and 2
DecDome :: DecDome(int id, b2Vec2 pos, float baseLevel, float* ptrColour) : Decoration(pos, ptrColour) {

    //set shape to the backing that both domes share
    b2PolygonShape shape;
    b2Vec2 points[8];
    points[0].Set(-4.0f, 0.0f);
    points[1].Set(4.0f, 0.0f);
    points[2].Set(3.5f, 4.25f);
    points[3].Set(1.5f, 7.0f);
    points[4].Set(-1.5f, 7.0f);
    points[5].Set(-3.5f, 4.25f);
    shape.Set(points, 6);
    addShape(shape);

    switch(id){
        case DEC_CODE_DOME_1 : {setDome1(); break;}
        case DEC_CODE_DOME_2 : {setDome2(); break;}
        default : {} //no details by default
    }
    addBase(baseLevel, b2Vec2(-4.0f, 4.0f));

}

DecDome :: ~DecDome(){} //no extra dynamic memory to deallocate

//set window details specific to dome 1
void DecDome :: setDome1(){

    b2PolygonShape shape;
    b2Vec2 points[8];
    points[0].Set(0.0f, 4.25f); //centre top triangle shape
    points[1].Set(1.25f, 6.75f);
    points[2].Set(-1.25f, 6.75f);
    shape.Set(points, 3);
    addDetail(shape);

    points[0].Set(0.0f, 4.0f); //bottom centre triangle
    points[1].Set(0.75, 0.25f);
    points[2].Set(-0.75, 0.25f);
    shape.Set(points, 3);
    addDetail(shape);

    for (int i=-1; i<=1; i+=2){
        float mirror = (float) i;
        points[0].Set(3.25f * mirror, 4.25f); //dome 1 top outer triangle
        points[1].Set(0.25f * mirror, 4.25f);
        points[2].Set(1.5f * mirror, 6.75f);
        shape.Set(points, 3);
        addDetail(shape);

        points[0].Set(3.75f * mirror, 0.25f); //dome 1 bottom outer triangle
        points[1].Set(1.25f * mirror, 0.25f);
        points[2].Set(3.25f * mirror, 4.0f);
        shape.Set(points, 3);
        addDetail(shape);

        points[0].Set(1.0f * mirror, 0.25f); //dome 1 bottom upside down triangle
        points[1].Set(0.25f * mirror, 4.0f);
        points[2].Set(3.0f * mirror, 4.0f);
        shape.Set(points, 3);
        addDetail(shape);
    }
}

//set window details specific to dome 2
void DecDome :: setDome2(){

    b2PolygonShape shape;
    b2Vec2 points[8];
    points[0].Set(-1.25f, 6.75f); //dome 2 main square panel
    points[1].Set(-1.25f, 0.25f);
    points[2].Set(1.25f, 0.25f);
    points[3].Set(1.25f, 6.75f);
    shape.Set(points, 4);
    addDetail(shape);

    for (int i=-1; i<=1; i+=2){
        float mirror = (float) i;
        points[0].Set(3.75f * mirror, 0.25f); //dome 2 outer triangle
        points[1].Set(1.75f * mirror, 0.25f);
        points[2].Set(3.3f * mirror, 4.05f);
        shape.Set(points, 3);
        addDetail(shape);

        points[0].Set(1.5f * mirror, 0.25f); //dome 2 inner triangle
        points[1].Set(1.5f * mirror, 6.75f);
        points[2].Set(3.25f * mirror, 4.3f);
        shape.Set(points, 3);
        addDetail(shape);

    }
}
