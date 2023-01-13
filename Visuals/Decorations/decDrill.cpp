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

//sets gridded crate
void setCrate(Decoration* dec, float mirror, b2Vec2 mag, b2Vec2 shift){
    if (mirror == 0.0f || mag.x == 0.0f || mag.y == 0.0f){
        return;
    }

    for (int i=-1; i<=1; i+=2){
        b2PolygonShape* shape = new b2PolygonShape; //vertical bar
        b2Vec2 points[8];
        points[0].Set(1.0f * ((float) i), 0.0f);
        points[1].Set(1.0f * ((float) i), 2.0f);
        points[2].Set(0.55f * ((float) i), 2.0f);
        points[3].Set(0.55f * ((float) i), 0.0f);
        transformDecPoints(points, 4, 0.0f, mag, shift);
        shape->Set(points, 4);
        dec->addShape(shape);

        shape = new b2PolygonShape; //inner vertical bar
        points[0].Set(1.0f * ((float) i), 0.0f);
        points[1].Set(1.0f * ((float) i), 2.0f);
        points[2].Set(0.75f * ((float) i), 2.0f);
        points[3].Set(0.75f * ((float) i), 0.0f);
        transformDecPoints(points, 4, 0.0f, mag, shift);
        shape->Set(points, 4);
        dec->addDetail(shape);

        float yShift = (i == -1) ? 0.0f : -1.55f;
        shape = new b2PolygonShape; //horizontal bar
        points[0].Set(-0.55f, 2.0f + yShift);
        points[1].Set(-0.55f, 1.55f + yShift);
        points[2].Set(0.55f, 1.55f + yShift);
        points[3].Set(0.55f, 2.0f + yShift);
        transformDecPoints(points, 4, 0.0f, mag, shift);
        shape->Set(points, 4);
        dec->addShape(shape);

        yShift = (i == -1) ? 0.0f : -1.75f;
        points[0].Set(-0.75f, 2.0f + yShift);
        points[1].Set(-0.75f, 1.75f + yShift);
        points[2].Set(0.75f, 1.75f + yShift);
        points[3].Set(0.75f, 2.0f + yShift);
        shape = new b2PolygonShape; //inner horizontal bar
        transformDecPoints(points, 4, 0.0f, mag, shift);
        shape->Set(points, 4);
        dec->addDetail(shape);

        shape = new b2PolygonShape; //diagonal bar
        points[0].Set(0.55f * mirror, 1.75f);
        points[1].Set(0.75f * mirror, 1.75f);
        points[2].Set(0.75f * mirror, 1.55f);
        points[3].Set(-0.55f * mirror, 0.25f);
        points[4].Set(-0.75f * mirror, 0.25f);
        points[5].Set(-0.75f * mirror, 0.45f);
        transformDecPoints(points, 6, 0.0f, mag, shift);
        shape->Set(points, 6);
        dec->addDetail(shape);

    }


}

//sets drill piece
void setDrillPiece(Decoration* dec, float ang, b2Vec2 mag, b2Vec2 shift){

    b2PolygonShape* shape = new b2PolygonShape; //drill triangle shape
    b2Vec2 points[8];
    points[0].Set(0.0f, 0.0f);
    points[1].Set(1.0f, 5.0f);
    points[2].Set(-1.0f, 5.0f);
    transformDecPoints(points, 3, ang, mag, shift);
    shape->Set(points, 3);
    dec->addShape(shape);

    for (int i=0; i<13; i++) { //drill groves
        float yShift = ((float) i) * 0.4f;
        float xMag = 1.0f + (((float) i) * 0.8f);
        shape = new b2PolygonShape;
        points[0].Set(-0.1f * xMag, 0.2f + yShift);
        points[1].Set(-0.1f * xMag, 0.0f + yShift);
        points[2].Set(0.1f * xMag, 0.1f + yShift);
        points[3].Set(0.1f * xMag, 0.3f + yShift);
        transformDecPoints(points, 4, ang, mag, shift);
        shape->Set(points, 4);
        dec->addDetail(shape);
    }
}

//sets a ridge (basic bar starting at given position and spanning given dimensions)
void setRidge(Decoration* dec, b2Vec2 pos, b2Vec2 dim){

    b2PolygonShape* shape = new b2PolygonShape;
    b2Vec2 points[8];
    points[0] = pos;
    points[1] = pos;
    points[1].y += dim.y;
    points[2] = pos + dim;
    points[3] = pos;
    points[3].x += dim.x;
    shape->Set(points, 4);
    dec->addDetail(shape);
}

//sets main drill machine body
void setDrillBody(Decoration* dec){

    b2PolygonShape* shape = new b2PolygonShape; //main cockpit backing
    b2Vec2 points[8];
    points[0].Set(0.0f, 0.0f);
    points[1].Set(0.0f, 3.5f);
    points[2].Set(-0.25f, 7.0f);
    points[3].Set(-5.0f, 7.0f);
    points[4].Set(-5.0f, 0.0f);
    shape->Set(points, 5);
    dec->addShape(shape);

    shape = new b2PolygonShape; //front drill connector backing
    points[2].Set(5.0f, 3.5f);
    points[3].Set(5.0f, 0.0f);
    shape->Set(points, 4);
    dec->addShape(shape);

    shape = new b2PolygonShape; //texture number
    b2Vec2 tCoords[4] = {b2Vec2(1.0f, 0.0f), b2Vec2(1.0f, 1.0f), b2Vec2(0.0f, 1.0f), b2Vec2(0.0f, 0.0f)};
    points[0].Set(-2.5f, 6.75f);
    points[1].Set(-5.0f, 6.75f);
    points[2].Set(-5.0f, 0.5f);
    points[3].Set(-2.5f, 0.5f);
    shape->Set(points, 4);
    dec->addTexture(randText0to9(), tCoords, (*shape));

    //cockpit window
    points[0].Set(-0.25f, 3.5f);
    points[1].Set(-0.5f, 6.75f);
    points[2].Set(-2.5f, 6.75f);
    points[3].Set(-2.5f, 0.75f);
    points[4].Set(-0.5f, 0.75f);
    shape->Set(points, 5);
    dec->addDetail(shape);

    //set ridges on the main body
    setRidge(dec, b2Vec2(-5.25f, 7.0f), b2Vec2(5.5f, 0.25f));
    setRidge(dec, b2Vec2(-5.25f, 0.0f), b2Vec2(10.5f, 0.25f));
    setRidge(dec, b2Vec2(0.0f, 3.25f), b2Vec2(5.25f, 0.25f));

}

//set details for intact drill machine
void Decoration :: setDrill1(float baseLevel){

    setDrillBody(this);
    for (int i=0; i<10; i++){ //start of drill arm, diagonal bar alternates
        setCrate(this, (i%2 == 0) ? 1.0f : -1.0f, b2Vec2(1.0f, 1.0f), b2Vec2(3.5f, 3.5f + (2.0f * ((float) i))));
    }
    setCrate(this, 1.0f, b2Vec2(2.0f, 0.75f), b2Vec2(6.5f, 21.75f)); //horizontal bar of arm
    setCrate(this, -1.0f, b2Vec2(1.0f, 1.0f), b2Vec2(9.5f, 21.5f)); //arm box

    //add cables
    setRidge(this, b2Vec2(9.0f, 11.5f), b2Vec2(0.2f, 10.0f));
    setRidge(this, b2Vec2(9.8f, 11.5f), b2Vec2(0.2f, 10.0f));

    //add drill
    setDrillPiece(this, 0.0f, b2Vec2(1.0f, 1.0f), b2Vec2(9.5f, 6.5f));

    addBase(baseLevel, b2Vec2(-5.25f, 5.25f));
}

//set details for broken drill machine
void Decoration :: setDrill2(float baseLevel){


}


