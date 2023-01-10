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

//set shape to the backing that both domes share
void setDome(b2PolygonShape* shape){

    b2Vec2 points[8];
    points[0].Set(-4.0f, 0.0f);
    points[1].Set(4.0f, 0.0f);
    points[2].Set(3.5f, 4.25f);
    points[3].Set(1.5f, 7.0f);
    points[4].Set(-1.5f, 7.0f);
    points[5].Set(-3.5f, 4.25f);
    shape->Set(points, 6);
}

//dome 1 top centre triangle
void setTri0(b2PolygonShape* shape){
    b2Vec2 points[8];
    points[0].Set(0.0f, 4.25f);
    points[1].Set(1.25f, 6.75f);
    points[2].Set(-1.25f, 6.75f);
    shape->Set(points, 3);
}

//dome 1 top outer triangle
void setTri1(b2PolygonShape* shape, float mirror){
    b2Vec2 points[8];
    points[0].Set(3.25f * mirror, 4.25f);
    points[1].Set(0.25f * mirror, 4.25f);
    points[2].Set(1.5f * mirror, 6.75f);
    shape->Set(points, 3);
}

//dome 1 bottom outer triangle
void setTri2(b2PolygonShape* shape, float mirror){
    b2Vec2 points[8];
    points[0].Set(3.75f * mirror, 0.25f);
    points[1].Set(1.25f * mirror, 0.25f);
    points[2].Set(3.25f * mirror, 4.0f);
    shape->Set(points, 3);
}

//dome 1 bottom upside down triangle
void setTri3(b2PolygonShape* shape, float mirror){
    b2Vec2 points[8];
    points[0].Set(1.0f * mirror, 0.25f);
    points[1].Set(0.25f * mirror, 4.0f);
    points[2].Set(3.0f * mirror, 4.0f);
    shape->Set(points, 3);
}

//dome 1 bottom centre triangle
void setTri4(b2PolygonShape* shape){
    b2Vec2 points[8];
    points[0].Set(0.0f, 4.0f);
    points[1].Set(0.75, 0.25f);
    points[2].Set(-0.75, 0.25f);
    shape->Set(points, 3);
}

//dome 2 main square panel
void setSquare(b2PolygonShape* shape){
    b2Vec2 points[8];
    points[0].Set(-1.25f, 6.75f);
    points[1].Set(-1.25f, 0.25f);
    points[2].Set(1.25f, 0.25f);
    points[3].Set(1.25f, 6.75f);
    shape->Set(points, 4);
}

//dome 2 outer triangle
void setTri5(b2PolygonShape* shape, float mirror){
    b2Vec2 points[8];
    points[0].Set(3.75f * mirror, 0.25f);
    points[1].Set(1.75f * mirror, 0.25f);
    points[2].Set(3.3f * mirror, 4.05f);
    shape->Set(points, 3);
}

//dome 2 inner triangle
void setTri6(b2PolygonShape* shape, float mirror){
    b2Vec2 points[8];
    points[0].Set(1.5f * mirror, 0.25f);
    points[1].Set(1.5f * mirror, 6.75f);
    points[2].Set(3.25f * mirror, 4.3f);
    shape->Set(points, 3);
}

//set details unique to dome 1
void Decoration :: setDome1(float baseLevel){

    //dome backing shape
    b2PolygonShape* shape = new b2PolygonShape;
    setDome(shape);
    addShape(shape);

    //centre two triangles
    shape = new b2PolygonShape;
    setTri0(shape);
    addDetail(shape);
    shape = new b2PolygonShape;
    setTri4(shape);
    addDetail(shape);

    //draw all symmetrical triangles
    for (int i=-1; i<=1; i+=2){ //-1 to 1 inclusive
        shape = new b2PolygonShape;
        setTri1(shape,(float)i);
        addDetail(shape);

        shape = new b2PolygonShape;
        setTri2(shape, (float)i);
        addDetail(shape);

        shape = new b2PolygonShape;
        setTri3(shape, (float)i);
        addDetail(shape);
    }

    addBase(baseLevel, b2Vec2(-4.0f, 4.0f));
}

//set details unique to dome 2
void Decoration :: setDome2(float baseLevel){

    //dome backing shape
    b2PolygonShape* shape = new b2PolygonShape;
    setDome(shape);
    addShape(shape);

    //centre two triangles
    shape = new b2PolygonShape;
    setSquare(shape);
    addDetail(shape);

    //draw all symmetrical triangles
    for (int i=-1; i<=1; i+=2){ //-1 to 1 inclusive
        shape = new b2PolygonShape;
        setTri5(shape,(float)i);
        addDetail(shape);

        shape = new b2PolygonShape;
        setTri6(shape, (float)i);
        addDetail(shape);
    }

    addBase(baseLevel, b2Vec2(-4.0f, 4.0f));
}
