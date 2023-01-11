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

#include "decoration.h"



//sets details and shape for a side of an arch
void setArchPole(Decoration* dec, float mirror){

    //backing shape
    b2PolygonShape* shape = new b2PolygonShape; //top piece
    b2Vec2 points[8];
    points[0].Set(6.0f * mirror, 12.0f);
    points[1].Set(6.0f * mirror, 10.0f);
    points[2].Set(0.0f, 10.0f);
    points[3].Set(0.0f, 12.0f);
    shape->Set(points, 4);
    dec->addShape(shape);

    shape = new b2PolygonShape; //leg piece
    points[0].Set(6.0f * mirror, 10.0f);
    points[1].Set(6.0f * mirror, 0.0f);
    points[2].Set(5.0f * mirror, 0.0f);
    points[3].Set(5.0f * mirror, 10.0f);
    shape->Set(points, 4);
    dec->addShape(shape);

    shape = new b2PolygonShape; //triangle
    points[0].Set(5.0f * mirror, 10.0f);
    points[1].Set(5.0f * mirror, 6.0f);
    points[2].Set(3.0f * mirror, 10.0f);
    shape->Set(points, 3);
    dec->addShape(shape);

    //highlights
    shape = new b2PolygonShape; //top highlight
    points[0].Set(6.0f * mirror, 12.25f);
    points[1].Set(6.0f * mirror, 12.0f);
    points[2].Set(0.0f, 12.0f);
    points[3].Set(0.0f, 12.25f);
    shape->Set(points, 4);
    dec->addDetail(shape);

    shape = new b2PolygonShape; //under highlight
    points[0].Set(3.0f * mirror, 10.0f);
    points[1].Set(3.0f * mirror, 9.75f);
    points[2].Set(0.0f, 9.75f);
    points[3].Set(0.0f, 10.0f);
    shape->Set(points, 4);
    dec->addDetail(shape);

    shape = new b2PolygonShape; //slope connection highlight
    //keep points 0 and 1 the same as the last (want to connect up)
    points[2].Set(5.0f * mirror, 5.75f);
    points[3].Set(5.0f * mirror, 6.0f);
    shape->Set(points, 4);
    dec->addDetail(shape);

    shape = new b2PolygonShape; //under leg highlight
    points[0].Set(6.0f * mirror, 6.0f);
    points[1].Set(6.0f * mirror, 5.75f);
    points[2].Set(5.0f * mirror, 5.75f);
    points[3].Set(5.0f * mirror, 6.0f);
    shape->Set(points, 4);
    dec->addDetail(shape);

}

//set details for the flat topped archway
void Decoration :: setArch1(float baseLevel){
    setArchPole(this, -1.0f); //left arch
    setArchPole(this, 1.0f); //right arch
    addBase(baseLevel, b2Vec2(-6.0f, 6.0f));
}

void setStartArch(Decoration* dec, float mirror){

    //main quad backing shape
    b2PolygonShape* shape = new b2PolygonShape;
    b2Vec2 points[8];
    points[0].Set(6.0f * mirror, 12.0f);
    points[1].Set(6.0f * mirror, 0.0f);
    points[2].Set(10.0f * mirror, 0.0f);
    points[3].Set(8.0f * mirror, 12.0f);
    shape->Set(points, 4);
    dec->addShape(shape);

    //angled highlight
    shape = new b2PolygonShape;
    points[0].Set(8.0f * mirror, 12.0f);
    points[1].Set(9.0f * mirror, 6.0f);
    points[2].Set(8.75f * mirror, 6.0f);
    points[3].Set(7.75f * mirror, 12.0f);
    shape->Set(points, 4);
    dec->addDetail(shape);

    //top highlight
    shape = new b2PolygonShape;
    points[0].Set(6.0f * mirror, 12.25f);
    points[1].Set(6.0f * mirror, 12.0f);
    points[2].Set(8.25f * mirror, 12.0f);
    points[3].Set(8.5f * mirror, 12.25f);
    shape->Set(points, 4);
    dec->addDetail(shape);

    //bottom highlight
    shape = new b2PolygonShape;
    points[0].Set(6.0f * mirror, 6.0f);
    points[1].Set(6.0f * mirror, 5.75f);
    points[2].Set(9.5f * mirror, 5.75f);
    points[3].Set(9.25f * mirror, 6.0f);
    shape->Set(points, 4);
    dec->addDetail(shape);

}

//set details for the left starting archway
void Decoration :: setArch2(float baseLevel){

    setArchPole(this, -1.0f); //left arch
    setArchPole(this, 1.0f); //right arch
    setStartArch(this, -1.0f);
    addBase(baseLevel, b2Vec2(-10.0f, 6.0f));
}

//set details for the right starting archway
void Decoration :: setArch3(float baseLevel){

    setArchPole(this, -1.0f); //left arch
    setArchPole(this, 1.0f); //right arch
    setStartArch(this, 1.0f);
    addBase(baseLevel, b2Vec2(-6.0f, 10.0f));
}

//set details for broken archway end point
void setBrokeArch(Decoration* dec, float mirror){

    //main backing shape
    b2PolygonShape* shape = new b2PolygonShape;
    b2Vec2 points[8];
    points[0].Set(0.0f, 12.0f);
    points[1].Set(0.0f, 10.0f);
    points[2].Set(1.75f * mirror, 10.0f);
    points[3].Set(3.5f * mirror, 11.25f);
    points[4].Set(4.0f * mirror, 12.0f);
    shape->Set(points, 5);
    dec->addShape(shape);

    //highlights
    shape = new b2PolygonShape; //top highlight
    points[0].Set(0.0f, 12.25f);
    points[1].Set(0.0f, 12.0f);
    points[2].Set(4.25f * mirror, 12.0f);
    points[3].Set(4.75f * mirror, 12.25f);
    shape->Set(points, 4);
    dec->addDetail(shape);

    shape = new b2PolygonShape; //under highlight
    points[0].Set(0.0f, 10.0f);
    points[1].Set(0.0f, 9.75f);
    points[2].Set(2.25f * mirror, 9.75f);
    points[3].Set(2.0f * mirror, 10.0f);
    shape->Set(points, 4);
    dec->addDetail(shape);

}

//set details for leftward ending point arch
void Decoration :: setArch4(float baseLevel){
    setArchPole(this, 1.0f); //right arch
    setBrokeArch(this, -1.0f); //leftward broken arch
    addBase(baseLevel, b2Vec2(5.0f, 6.0f));

}

//set details for rightward ending point arch
void Decoration :: setArch5(float baseLevel){
    setArchPole(this, -1.0f); //left arch
    setBrokeArch(this, 1.0f); //rightward broken arch
    addBase(baseLevel, b2Vec2(-6.0f, -5.0f));
}
