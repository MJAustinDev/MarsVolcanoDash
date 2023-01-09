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

void setLivingQuarter(LinkedList<b2PolygonShape>* shapes, LinkedList<b2PolygonShape>* details){
    //backing
    b2PolygonShape* shape = new b2PolygonShape;
    b2Vec2 points[8];
    points[0].Set(-4.0f, 0.0f);
    points[1].Set(4.0f, 0.0f);
    points[2].Set(4.0f, 6.5f);
    points[3].Set(-4.0f, 6.5f);
    shape->Set(points, 4);
    shapes->addEnd(shape);

    //ridge both on floor and roof
    for (int i=0; i<=6 ; i+=6){
        shape = new b2PolygonShape;
        points[0].Set(-4.25f, 0.0f + ((float)i));
        points[1].Set(4.25f, 0.0f + ((float)i));
        points[2].Set(4.25f, 0.5f + ((float)i));
        points[3].Set(-4.25f, 0.5f + ((float)i));
        shape->Set(points, 4);
        details->addEnd(shape);
    }

    //add door window
    shape = new b2PolygonShape;
    points[0].Set(-0.75f, 3.0f);
    points[1].Set(0.75f, 3.0f);
    points[2].Set(0.75f, 3.5f);
    points[3].Set(-0.75f, 3.5f);
    shape->Set(points, 4);
    details->addEnd(shape);

}



void Decoration :: setLQ1(float baseLevel){

    setLivingQuarter(&shapes, &details); //add most living quarter details

    //add the door way
    for (int i=-1; i<=1; i+=2){
        b2PolygonShape* shape = new b2PolygonShape;
        b2Vec2 points[8];
        points[0].Set(0.0f, 3.75f + 1.5f);
        points[1].Set(0.8f * ((float) i), 3.75f + 1.25f);
        points[2].Set(0.7f * ((float) i), 3.75f + 0.85f);
        points[3].Set(0.0f, 3.75f + 1.0f);
        shape->Set(points, 4);
        details.addEnd(shape);

        shape = new b2PolygonShape;
        points[0].Set(0.8f * ((float) i), 3.75f + 1.25f);
        points[1].Set(0.7f * ((float) i), 3.75f + 0.85f);
        points[2].Set(0.85f * ((float) i), 3.75f + 0.7f);
        points[3].Set(1.25f * ((float) i), 3.75f + 0.8f);
        shape->Set(points, 4);
        details.addEnd(shape);

        shape = new b2PolygonShape;
        points[0].Set(0.85f * ((float) i), 3.75f + 0.7f);
        points[1].Set(1.25f * ((float) i), 3.75f + 0.8f);
        points[2].Set(1.5f * ((float) i), 3.75f + 0.0f);
        points[3].Set(1.05f * ((float) i), 3.75f + 0.0f);
        shape->Set(points, 4);
        details.addEnd(shape);

        shape = new b2PolygonShape;
        points[0].Set(1.5f * ((float) i), 3.75f + 0.0f);
        points[1].Set(1.05f * ((float) i), 3.75f + 0.0f);
        points[2].Set(1.0f * ((float) i), 0.0f);
        points[3].Set(1.5f * ((float) i), 0.0f);
        shape->Set(points, 4);
        details.addEnd(shape);
    }
    addBase(baseLevel, b2Vec2(4.25f, -4.25f));
}

void Decoration :: setLQ2(float baseLevel){

    setLivingQuarter(&shapes, &details); //add most living quarter details

    //add the door way
    for (int i=-1; i<=1; i+=2){
        b2PolygonShape* shape = new b2PolygonShape;
        b2Vec2 points[8];
        points[0].Set(1.5f * ((float) i), 6.5f);
        points[1].Set(1.05f * ((float) i), 6.5f);
        points[2].Set(1.05f * ((float) i), 0.0f);
        points[3].Set(1.5f * ((float) i), 0.0f);
        shape->Set(points, 4);
        details.addEnd(shape);
    }
    addBase(baseLevel, b2Vec2(4.25f, -4.25f));
}

void Decoration :: setTunnel1(float baseLevel){

}
