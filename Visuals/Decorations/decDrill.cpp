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

//constructor for drill 1 and 2
DecDrill :: DecDrill(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour) : Decoration(pos, ptrColour){

    b2Vec2 points[8];
    points[0].Set(0.0f, 0.0f); //main cockpit backing
    points[1].Set(0.0f, 3.5f);
    points[2].Set(-0.25f, 7.0f);
    points[3].Set(-5.0f, 7.0f);
    points[4].Set(-5.0f, 0.0f);
    addShape(points, 5);

    points[2].Set(5.0f, 3.5f); //front drill connector backing
    points[3].Set(5.0f, 0.0f);
    addShape(points, 4);

    b2Vec2 tCoords[4] = {b2Vec2(1.0f, 0.0f), b2Vec2(1.0f, 1.0f), b2Vec2(0.0f, 1.0f), b2Vec2(0.0f, 0.0f)};
    points[0].Set(-2.5f, 6.75f); //random texture number
    points[1].Set(-5.0f, 6.75f);
    points[2].Set(-5.0f, 0.5f);
    points[3].Set(-2.5f, 0.5f);
    addTexture(randText0to9(), tCoords, points, 4);

    points[0].Set(-0.25f, 3.5f); //cockpit window
    points[1].Set(-0.5f, 6.75f);
    points[2].Set(-2.5f, 6.75f);
    points[3].Set(-2.5f, 0.75f);
    points[4].Set(-0.5f, 0.75f);
    addDetail(points, 5);

    //set ridges on the main body
    setBar(b2Vec2(-5.25f, 7.0f), b2Vec2(5.5f, 0.25f));
    setBar(b2Vec2(-5.25f, 0.0f), b2Vec2(10.5f, 0.25f));
    setBar(b2Vec2(0.0f, 3.25f), b2Vec2(5.25f, 0.25f));

    for (int i=0; i<10; i++){ //start of drill arm, diagonal bar alternates
        setCrate((i%2 == 0) ? 1.0f : -1.0f, b2Vec2(1.0f, 1.0f), b2Vec2(3.5f, 3.5f + (2.0f * ((float) i))));
    }
    setCrate(1.0f, b2Vec2(2.0f, 0.75f), b2Vec2(6.5f, 21.75f)); //horizontal bar of arm
    setCrate(-1.0f, b2Vec2(1.0f, 1.0f), b2Vec2(9.5f, 21.5f)); //arm box

    switch(id){
        case DEC_CODE_DRILL_1 : {
            setBar(b2Vec2(9.0f, 11.5f), b2Vec2(0.2f, 10.0f)); //left cable
            setBar(b2Vec2(9.8f, 11.5f), b2Vec2(0.2f, 10.0f)); //right cable
            setDrillPiece(0.0f, b2Vec2(1.0f, 1.0f), b2Vec2(9.5f, 6.5f)); //drill piece
            break;
        }
        case DEC_CODE_DRILL_2 : {
            setBar(b2Vec2(9.0f, 16.5f), b2Vec2(0.2f, 5.0f)); //left cable before break point
            setBar(b2Vec2(9.8f, 11.4f), b2Vec2(0.2f, 10.1f)); //right cable
            setDrillPiece(0.25f, b2Vec2(1.0f, 1.0f), b2Vec2(10.75f, 6.5f)); //drill piece
            points[0].Set(9.0f, 16.5f); //left cable top break
            points[1].Set(9.0f, 15.0f);
            points[2].Set(9.2f, 16.5f);
            addDetail(points, 3);

            points[0].y = 11.2f; //start of cable bottom break
            points[1].y = 11.4f;
            points[2].y = 11.5f;
            points[3].Set(9.2f, 11.2f);
            addDetail(points, 4);

            points[0].Set(8.9f, 11.5f); //centre of cable bottom break
            points[3].Set(9.0f, 11.7f);
            addDetail(points, 4);

            points[1].Set(8.7f, 11.6f); //end of cable bottom break
            points[2].Set(8.8f, 11.7f);
            addDetail(points, 4);
        break;}
        default : {} //no extra detail by default
    }

    if (hasBase){addBase(baseLevel, b2Vec2(-5.25f, 5.25f));}
}

DecDrill :: ~DecDrill(){} //no dynamically allocated memory to clear as of yet

//sets a gridded create
void DecDrill :: setCrate(float mirror, b2Vec2 mag, b2Vec2 shift){

    for (int i=-1; i<=1; i+=2){
        b2Vec2 points[8];
        points[0].Set(1.0f * ((float) i), 0.0f);  //vertical bar
        points[1].Set(1.0f * ((float) i), 2.0f);
        points[2].Set(0.55f * ((float) i), 2.0f);
        points[3].Set(0.55f * ((float) i), 0.0f);
        transformDecPoints(points, 4, 0.0f, mag, shift);
        addShape(points, 4);

        points[0].Set(1.0f * ((float) i), 0.0f); //inner vertical bar
        points[1].Set(1.0f * ((float) i), 2.0f);
        points[2].Set(0.75f * ((float) i), 2.0f);
        points[3].Set(0.75f * ((float) i), 0.0f);
        transformDecPoints(points, 4, 0.0f, mag, shift);
        addDetail(points, 4);

        float yShift = (i == -1) ? 0.0f : -1.55f;
        points[0].Set(-0.55f, 2.0f + yShift); //horizontal bar
        points[1].Set(-0.55f, 1.55f + yShift);
        points[2].Set(0.55f, 1.55f + yShift);
        points[3].Set(0.55f, 2.0f + yShift);
        transformDecPoints(points, 4, 0.0f, mag, shift);
        addShape(points, 4);

        yShift = (i == -1) ? 0.0f : -1.75f;
        points[0].Set(-0.75f, 2.0f + yShift); //inner horizontal bar
        points[1].Set(-0.75f, 1.75f + yShift);
        points[2].Set(0.75f, 1.75f + yShift);
        points[3].Set(0.75f, 2.0f + yShift);
        transformDecPoints(points, 4, 0.0f, mag, shift);
        addDetail(points, 4);

        points[0].Set(0.55f * mirror, 1.75f); //diagonal bar
        points[1].Set(0.75f * mirror, 1.75f);
        points[2].Set(0.75f * mirror, 1.55f);
        points[3].Set(-0.55f * mirror, 0.25f);
        points[4].Set(-0.75f * mirror, 0.25f);
        points[5].Set(-0.75f * mirror, 0.45f);
        transformDecPoints(points, 6, 0.0f, mag, shift);
        addDetail(points, 6);
    }
}

//sets a solid rectangular bar
void DecDrill :: setBar(b2Vec2 pos, b2Vec2 dim){
    b2Vec2 points[8];
    points[0] = pos;
    points[1] = pos;
    points[1].y += dim.y;
    points[2] = pos + dim;
    points[3] = pos;
    points[3].x += dim.x;
    addDetail(points, 4);
}

//sets a drill piece that rotates around it's tip
void DecDrill :: setDrillPiece(float ang, b2Vec2 mag, b2Vec2 shift){

    b2Vec2 points[8];
    points[0].Set(0.0f, 0.0f); //drill triangle shape
    points[1].Set(1.0f, 5.0f);
    points[2].Set(-1.0f, 5.0f);
    transformDecPoints(points, 3, ang, mag, shift);
    addShape(points, 3);

    for (int i=0; i<13; i++) { //drill groves
        float yShift = ((float) i) * 0.4f;
        float xMag = 1.0f + (((float) i) * 0.8f);
        points[0].Set(-0.1f * xMag, 0.2f + yShift);
        points[1].Set(-0.1f * xMag, 0.0f + yShift);
        points[2].Set(0.1f * xMag, 0.1f + yShift);
        points[3].Set(0.1f * xMag, 0.3f + yShift);
        transformDecPoints(points, 4, ang, mag, shift);
        addDetail(points, 4);
    }
}
