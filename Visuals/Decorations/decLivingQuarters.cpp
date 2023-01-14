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

//constructor for living quarters's 1 and 2
DecLQ :: DecLQ(int id, b2Vec2 pos, float baseLevel, float* ptrColour) : Decoration(pos, ptrColour){

    b2PolygonShape shape;
    b2Vec2 points[8];
    points[0].Set(-4.0f, 0.0f); //backing shape
    points[1].Set(4.0f, 0.0f);
    points[2].Set(4.0f, 6.5f);
    points[3].Set(-4.0f, 6.5f);
    shape.Set(points, 4);
    addShape(shape);

    points[0].Set(-0.75f, 3.0f); //add door window
    points[1].Set(0.75f, 3.0f);
    points[2].Set(0.75f, 3.5f);
    points[3].Set(-0.75f, 3.5f);
    shape.Set(points, 4);
    addDetail(shape);


    b2Vec2 tCoords[4] = {b2Vec2(1.0f, 0.0f), b2Vec2(1.0f, 1.0f), b2Vec2(0.0f, 1.0f), b2Vec2(0.0f, 0.0f)};
    points[0].Set(1.25f, 6.0f); //add number texture to right side of living quarters
    points[1].Set(4.0f, 6.0f);
    points[2].Set(4.0f, 0.5f);
    points[3].Set(1.25f, 0.5f);
    shape.Set(points, 4);
    addTexture(randText0to9(), tCoords, shape);

    for (int i=0; i<=1; i++){

        float shift = ((float)i) * 6.0f;
        points[0].Set(-4.25f, 0.0f + shift); //add top and bottom ridges
        points[1].Set(4.25f, 0.0f + shift);
        points[2].Set(4.25f, 0.5f + shift);
        points[3].Set(-4.25f, 0.5f + shift);
        shape.Set(points, 4);
        addDetail(shape);

        //add left side box
        shift = ((float) i) * 1.75f;
        points[0].Set(-3.75f + shift, 5.75f); //vertical bars
        points[1].Set(-3.5f + shift, 5.75f);
        points[2].Set(-3.5f + shift, 0.75f);
        points[3].Set(-3.75f + shift, 0.75f);
        shape.Set(points, 4);
        addDetail(shape);

        //horizontal bars
        shift = ((float) i) * -4.75f;
        points[0].Set(-3.75f, 5.75f + shift);
        points[1].Set(-2.0f, 5.75f + shift);
        points[2].Set(-2.0f, 5.5f + shift);
        points[3].Set(-3.75f, 5.5f + shift);
        shape.Set(points, 4);
        addDetail(shape);

        shift = ((float) i) * -5.5f; //diagonal bars
        float mirror = (i == 0) ? 1.0f : -1.0f;
        points[0].Set((-3.75f * mirror) + shift, 5.5f);
        points[1].Set((-2.0f * mirror) + shift, 0.75f);
        points[2].Set((-1.75f * mirror) + shift, 1.0f);
        points[3].Set((-3.5f * mirror) + shift, 5.75f);
        shape.Set(points, 4);
        addDetail(shape);
    }

    //add doorway
    switch(id){
        case DEC_CODE_LQ_1 : {setLQ1(); break;}
        case DEC_CODE_LQ_2 : {setLQ2(); break;}
        default : {}
    }

    addBase(baseLevel, b2Vec2(-4.25f, 4.25f));

}

DecLQ :: ~ DecLQ(){} //no extra dynamically allocated memory to release

//sets the door way for living quarters 1
void DecLQ :: setLQ1(){

    for (int i=-1; i<=1; i+=2){ //adds the frame of the doorway symetrically

        float mirror = (float) i;
        b2PolygonShape shape;
        b2Vec2 points[8];
        points[0].Set(0.0f, 5.25f); //top section
        points[1].Set(0.8f * mirror, 5.0f);
        points[2].Set(0.7f * mirror, 4.6f);
        points[3].Set(0.0f, 4.75f);
        shape.Set(points, 4);
        addDetail(shape);

        points[0].Set(0.8f * mirror, 5.0f); //middle section
        points[1].Set(0.7f * mirror, 4.60f);
        points[2].Set(0.85f * mirror, 4.45f);
        points[3].Set(1.25f * mirror, 4.55f);
        shape.Set(points, 4);
        addDetail(shape);

        points[0].Set(0.85f * mirror, 4.45f); //bottom section
        points[1].Set(1.25f * mirror, 4.55f);
        points[2].Set(1.5f * mirror, 3.75f);
        points[3].Set(1.05f * mirror, 3.75f);
        shape.Set(points, 4);
        addDetail(shape);

        points[0].Set(1.5f * mirror, 3.75f); //remaining vertical bars
        points[1].Set(1.05f * mirror, 3.75f);
        points[2].Set(1.0f * mirror, 0.0f);
        points[3].Set(1.5f * mirror, 0.0f);
        shape.Set(points, 4);
        addDetail(shape);
    }
}

//sets the door way for living quarters 2
void DecLQ :: setLQ2(){

    for (int i=-1; i<=1; i+=2){ //adds each doorway bar symetrically
        float mirror = (float) i;
        b2PolygonShape shape;
        b2Vec2 points[8];
        points[0].Set(1.5f * mirror, 6.5f);
        points[1].Set(1.05f * mirror, 6.5f);
        points[2].Set(1.05f * mirror, 0.0f);
        points[3].Set(1.5f * mirror, 0.0f);
        shape.Set(points, 4);
        addDetail(shape);
    }
}

/*
//sets the details that exist in both living quarters decorations



//adds a connector that maps onto the dome backing shape
void setTunnelToDome(Decoration* dec, float mirror){
    b2PolygonShape* shape = new b2PolygonShape;
    b2Vec2 points[8];
    points[0].Set(8.0f * mirror, 5.5f);
    points[1].Set(8.0f * mirror, 1.5f);
    points[2].Set(8.4f * mirror, 1.25f);
    points[3].Set(8.75f * mirror, 4.25f);
    shape->Set(points, 4);
    dec->addShape(shape);

    shape = new b2PolygonShape;
    points[0].Set(8.0f * mirror, 5.5f);
    points[1].Set(8.75f * mirror, 4.25f);
    points[2].Set(9.85f * mirror, 5.75f);
    shape->Set(points, 3);
    dec->addShape(shape);
}

//set tunnel 1 (just a flat tunnel)
void Decoration :: setTunnel1(float baseLevel){

    //main tunnel shape
    b2PolygonShape* shape = new b2PolygonShape;
    b2Vec2 points[8];
    points[0].Set(-8.0f, 5.5f);
    points[1].Set(8.0f, 5.5f);
    points[2].Set(8.0f, 1.5f);
    points[3].Set(-8.0f, 1.5f);
    shape->Set(points, 4);
    addShape(shape);

    //edge ridges
    for (int i=0; i<=1; i++){
        float shift = ((float) i) * 16.25f;
        shape = new b2PolygonShape;
        points[0].Set(-8.25f + shift, 5.75f);
        points[1].Set(-8.0f + shift, 5.75f);
        points[2].Set(-8.0f + shift, 1.25f);
        points[3].Set(-8.25f + shift, 1.25f);
        shape->Set(points, 4);
        addDetail(shape);
    }

    //tunnel window
    shape = new b2PolygonShape;
    points[0].Set(-7.25f, 4.75f);
    points[1].Set(7.25f, 4.75f);
    points[2].Set(7.25f, 4.25f);
    points[3].Set(-7.25f, 4.25f);
    shape->Set(points, 4);
    addDetail(shape);


    addBase(baseLevel, b2Vec2(-8.0f, 8.0f));

}

//set tunnel 2 (tunnel with a dome connector on the right)
void Decoration :: setTunnel2(float baseLevel){
    setTunnelToDome(this, 1.0f);
    setTunnel1(baseLevel);
}

//set tunnel 3 (tunnel with a dome connector on the left)
void Decoration :: setTunnel3(float baseLevel){
    setTunnelToDome(this, -1.0f);
    setTunnel1(baseLevel);
}

*/
