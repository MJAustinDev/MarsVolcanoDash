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

//constructor for tanks 1 & 2
DecTank :: DecTank(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour) : Decoration(pos, ptrColour) {

    b2Vec2 tCoords[4] = {b2Vec2(1.0f, 0.0f), b2Vec2(1.0f, 1.0f), b2Vec2(0.0f, 1.0f), b2Vec2(0.0f, 0.0f)};
    b2Vec2 points[8];
    points[0].Set(-0.5f, 0.75f); //random 0 to 9 number on tank
    points[1].Set(3.25f, 0.75f);
    points[2].Set(3.25f, 6.0f);
    points[3].Set(-0.5f, 6.0f);
    addTexture(randText0to9(), tCoords, points, 4);

    setTankRidge(0.0f); //bottom ridge

    switch(id){
        case DEC_CODE_TANK_1 : { //intact tank
            setTankBacking(12.0f); //main rectangle shape
            setTankRidge(11.8f); //top ridge
            setLadderPoles(13.1f); //two ladder poles
            setLadderBars(18); //18 ladder bars

            points[0].Set(1.4, 11.0f); //diamond symbol
            points[1].Set(0.5f, 8.875f);
            points[2].Set(1.4f, 6.75f);
            points[3].Set(2.3f, 8.875f);
            addDetail(points, 4);
            break;
        }
        case DEC_CODE_TANK_2 : { //broken tank
            setTankBacking(6.0f); //main rectangle shape before breaking
            setBrokenShards(-1.0f); //add shards left side
            setBrokenShards(1.0f); //add shards right side
            setLadderPoles(5.8f); //two ladder poles till break point
            setLadderBars(8); //8 ladder bars till break point

            points[0].Set(-3.0f, 5.8f); //left ladder pole after breaking
            points[1].Set(-3.25f, 9.4f);
            points[2].Set(-3.05f, 9.5f);
            points[3].Set(-2.75f, 5.8f);
            addDetail(points, 4);

            points[0].Set(-1.75f, 5.8f); //right ladder pole after breaking
            points[1].Set(-1.7f, 6.7f);
            points[2].Set(-1.55f, 6.9f);
            points[3].Set(-1.5f, 5.8f);
            addDetail(points, 4);

            for (int i=0; i<=3; i++){ //remaining broken ladder bars
                float xShift = ((float) i) * -0.1f;
                float yShift = ((float) i) * 0.75f;
                points[0].Set(-3.25f + xShift, 6.5f + yShift);
                points[1].Set(-3.3f + xShift, 6.7f + yShift);
                points[2].Set(-2.45f + xShift, 6.7f + yShift);
                points[3].Set(-2.40f + xShift, 6.5f + yShift);
                addDetail(points, 4);
            }

            setFlames(); //set flames out of broken tank

            break;
        }
        default : {} //no extra details by default
    }

    if (hasBase){addBase(baseLevel, b2Vec2(-3.75f, 3.75f));}

}

DecTank :: ~DecTank(){}; //TODO -- CLEAR FLAMES LINKED LIST

//unique draw method
void DecTank :: draw(Camera* camera){
    //TODO -- DRAW FLAMES OUT OF TOP (IF BROKEN) -- VIA LINKED LIST

    Decoration::draw(camera);
}

//sets main background rectangle shape of the tank
void DecTank :: setTankBacking(float height){
    b2Vec2 points[8];
    points[0].Set(-3.5f, 0.0f);
    points[1].Set(3.5f, 0.0f);
    points[2].Set(3.5f, height);
    points[3].Set(-3.5f, height);
    addShape(points, 4);
}

//sets a ridge along the tank, shifted upward by passed float
void DecTank :: setTankRidge(float shift){
    b2Vec2 points[8];
    points[0].Set(-3.75f, shift);
    points[1].Set(3.75f, shift);
    points[2].Set(3.75f, 0.5f + shift);
    points[3].Set(-3.75f, 0.5f + shift);
    addDetail(points, 4);
}

//sets ladder poles along the tank up to the passed height
void DecTank :: setLadderPoles(float height){
    for (int i=0; i<=1; i++){
        float shift = ((float) i) * 1.25f;
        b2Vec2 points[8];
        points[0].Set(-3.0f + shift, 0.0f);
        points[1].Set(-2.75f + shift, 0.0f);
        points[2].Set(-2.75f + shift, height);
        points[3].Set(-3.0f + shift, height);
        addDetail(points, 4);
    }
}

//adds a given number of ladder bars
void DecTank :: setLadderBars(int num){
    for (int i=0; i<num; i++){
        float shift = ((float) i) * 0.7f;
        b2Vec2 points[8];
        points[0].Set(-3.25f, 0.75f + shift);
        points[1].Set(-1.25f, 0.75f + shift);
        points[2].Set(-1.25f, 1.0f + shift);
        points[3].Set(-3.25f, 1.0f + shift);
        addDetail(points, 4);
    }
}

//adds broken shards of metal on top of the broken tank
void DecTank :: setBrokenShards(float mirror){

    b2Vec2 points[8];
    points[0].Set(3.5f *  mirror, 6.0f); //outer shards
    points[1].Set(randRanged(3.5f, 4.0f)  *  mirror, randRanged(7.2f, 10.0f));
    points[2].Set(3.0f *  mirror, 6.0f);
    addShape(points, 3);

    for (int j=0; j<3 ;j++){ //inner shards
        float shift = (float) j;
        points[0].Set(shift * mirror, 6.0f);
        points[1].Set(randRanged(shift - 0.1f, shift + 0.9f) * mirror, randRanged(7.4f, 11.5f));
        points[2].Set((shift + 1.0f) * mirror, 6.0f);
        addShape(points, 3);

        //add broken diamond symbol to 2 of the shard segments
        if ((mirror == 1.0f) && (j != 2)){ //(j != 2) => (j == 0) || (j == 1)
            b2Vec2 half = points[0] + points[1]; //works out halfway point between top point and the two sides
            points[0].Set(half.x / 2.0f, half.y / 2.0f);
            half = points[2] + points[1]; //repeat for the next side
            points[2].Set(half.x / 2.0f, half.y / 2.0f);
            points[3].Set(randRanged(0.25f, 0.75f) + (shift * mirror), randRanged(6.1f, 6.5f)); //set point 3 at the bottom of the shard
            addDetail(points, 4);
        }
    }
}

//TODO
void DecTank :: setFlames(){};
