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

//rotates then scales the passed b2Vec2(x, y) points
void transformPoints(b2Vec2* points, int num, float ang, b2Vec2 mag){

    //rotate
    for (int i=0; i<num; i++){
        float c = cos(ang);
        float s = sin(ang);
        points[i].Set((points[i].x * c) - (points[i].y * s), (points[i].y * c) + (points[i].x * s));
    }

    //scale (and mirror if scale is negative)
    for (int i=0; i<num; i++){
        points[i].x *= mag.x;
        points[i].y *= mag.y;
    }
}

//sets out ship shape relative to rotation and scale
void setShipShape(Decoration* dec, float ang, b2Vec2 mag){

    b2PolygonShape* shape = new b2PolygonShape; //main body of the ship
    b2Vec2 points[8];
    points[0].Set(8.8f, 1.75f);
    points[1].Set(5.0f, 2.0f);
    points[2].Set(-5.0f, 2.0f);
    points[3].Set(-5.0f, -2.0f);
    points[4].Set(7.5f, -2.0f);
    points[5].Set(9.0f, -1.5f);
    transformPoints(&points[0], 6, ang, mag);
    shape->Set(points, 6);
    dec->addShape(shape);

    shape = new b2PolygonShape; //engine extension
    points[0].Set(-5.0f, 0.0f);
    points[1].Set(-5.75f, 0.0f);
    points[2].Set(-6.0f, 0.5f);
    points[3].Set(-6.25f, 2.0f);
    points[4].Set(-5.0f, 2.0f);
    transformPoints(&points[0], 5, ang, mag);
    shape->Set(points, 5);
    dec->addShape(shape);

    shape = new b2PolygonShape; //bridge extension
    points[0].Set(1.0f, 2.0f);
    points[1].Set(-4.25f, 2.0f);
    points[2].Set(-3.25f, 2.6f);
    points[3].Set(-0.25f, 2.5f);
    transformPoints(&points[0], 4, ang, mag);
    shape->Set(points, 4);
    dec->addShape(shape);

    shape = new b2PolygonShape; //bridge window
    points[0].Set(-3.70f, 2.15f);
    points[1].Set(-3.25f, 2.4f);
    points[2].Set(-0.25f, 2.3f);
    points[3].Set(0.5f, 2.0f);
    points[4].Set(-2.25f, 1.75f);
    transformPoints(&points[0], 5, ang, mag);
    shape->Set(points, 5);
    dec->addDetail(shape);

    shape = new b2PolygonShape; //armour panel
    points[0].Set(-2.0f, 0.5f);
    points[1].Set(-2.75f, 1.75f);
    points[2].Set(-4.75f, 2.1f);
    points[3].Set(-4.75f, 0.5f);
    points[4].Set(-4.5f, -2.1f);
    points[5].Set(-2.75f, -1.75f);
    transformPoints(&points[0], 6, ang, mag);
    shape->Set(points, 6);
    dec->addDetail(shape);

    shape = new b2PolygonShape; //top armour panel wing
    points[0].Set(-4.75f, 2.1f);
    points[1].Set(-6.0f, 2.25f);
    points[2].Set(-5.75f, 1.0f);
    points[3].Set(-4.75f, 0.5f);
    transformPoints(&points[0], 4, ang, mag);
    shape->Set(points, 4);
    dec->addDetail(shape);

    shape = new b2PolygonShape; //cargo section
    points[0].Set(-1.5f, 1.0f);
    points[1].Set(-1.25f, -2.25f);
    points[2].Set(7.0f, -2.25f);
    points[3].Set(7.25f, 1.0f);
    transformPoints(&points[0], 4, ang, mag);
    shape->Set(points, 4);
    dec->addDetail(shape);

    shape = new b2PolygonShape; //top engine first section
    points[0].Set(-6.19f, 1.75f);
    points[1].Set(-7.0f, 1.5f);
    points[2].Set(-7.0f, 0.5f);
    points[3].Set(-6.0f, 0.5f);
    transformPoints(&points[0], 4, ang, mag);
    shape->Set(points, 4);
    dec->addDetail(shape);

    shape = new b2PolygonShape; //top engine second section
    points[0].Set(-5.8f, 0.1f);
    points[1].Set(-7.0f, 0.25f);
    points[2].Set(-7.0f, 0.5f);
    points[3].Set(-6.0f, 0.5f);
    transformPoints(&points[0], 4, ang, mag);
    shape->Set(points, 4);
    dec->addDetail(shape);

    shape = new b2PolygonShape; //bottom engine
    points[0].Set(-5.0f, -0.1f);
    points[1].Set(-6.0f, -0.35f);
    points[2].Set(-6.0f, -1.5f);
    points[3].Set(-5.0f, -1.75f);
    transformPoints(&points[0], 4, ang, mag);
    shape->Set(points, 4);
    dec->addDetail(shape);

}

//set out details for ship 1 (floating intact)
void Decoration :: setShip1(float baseLevel, float ang, b2Vec2 mag){

    setShipShape(this, ang, mag);

}












