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

/*

//sets the number on the side of the tank randomly between 0 and 9
void setTankNumber(Decoration* dec){
    //number on tank
    b2Vec2 tCoords[4] = {b2Vec2(1.0f, 0.0f), b2Vec2(1.0f, 1.0f), b2Vec2(0.0f, 1.0f), b2Vec2(0.0f, 0.0f)};
    b2PolygonShape shape;
    b2Vec2 points[8];
    points[0].Set(-0.5f, 0.75f);
    points[1].Set(3.25f, 0.75f);
    points[2].Set(3.25f, 6.0f);
    points[3].Set(-0.5f, 6.0f);
    shape.Set(points, 4);
    dec->addTexture(randText0to9(), tCoords, shape);
}

//sets a tank ridge, shifts up by given amount
void setTankRidges(Decoration* dec, float shift){
    b2PolygonShape* shape = new b2PolygonShape;
    b2Vec2 points[8];
    points[0].Set(-3.75f, 0.0f + shift);
    points[1].Set(3.75f, 0.0f + shift);
    points[2].Set(3.75f, 0.5f + shift);
    points[3].Set(-3.75f, 0.5f + shift);
    shape->Set(points, 4);
    dec->addDetail(shape);
}

//sets ladder pole from base to given height
void setTankLadderPole(Decoration* dec, float height){
    for (int i=0; i<=1; i++){
        //ladder poles
        float shift = ((float) i) * 1.25f;
        b2PolygonShape* shape = new b2PolygonShape;
        b2Vec2 points[8];
        points[0].Set(-3.0f + shift, 0.0f);
        points[1].Set(-2.75f + shift, 0.0f);
        points[2].Set(-2.75f + shift, height);
        points[3].Set(-3.0f + shift, height);
        shape->Set(points, 4);
        dec->addDetail(shape);
    }
}

//adds uniform bars across the ladder pole
void setTankLadderBars(Decoration* dec, int incNum){
    //ladder bars
    for (int i=0; i<=incNum; i++){
        float shift = ((float) i) * 0.7f;
        b2PolygonShape* shape = new b2PolygonShape;
        b2Vec2 points[8];
        points[0].Set(-3.25f, 0.75f + shift);
        points[1].Set(-1.25f, 0.75f + shift);
        points[2].Set(-1.25f, 1.0f + shift);
        points[3].Set(-3.25f, 1.0f + shift);
        shape->Set(points, 4);
        dec->addDetail(shape);
    }
}

//set details for intact tank
void Decoration :: setTank1(float baseLevel){

    //background shape
    b2PolygonShape* shape = new b2PolygonShape;
    b2Vec2 points[8];
    points[0].Set(-3.5f, 0.0f);
    points[1].Set(3.5f, 0.0f);
    points[2].Set(3.5f, 12.0f);
    points[3].Set(-3.5f, 12.0f);
    shape->Set(points, 4);
    addShape(shape);

    //set top and bottom ridges
    setTankRidges(this, 11.8f);
    setTankRidges(this, 0.0f);
    setTankLadderPole(this, 13.1f); // ladder poles
    setTankLadderBars(this, 17); //ladder bars

    //diamond symbol
    shape = new b2PolygonShape;
    points[0].Set(1.4, 11.0f);
    points[1].Set(0.5f, 8.875f);
    points[2].Set(1.4f, 6.75f);
    points[3].Set(2.3f, 8.875f);
    shape->Set(points, 4);
    addDetail(shape);

    setTankNumber(this); //number on tank
    addBase(baseLevel, b2Vec2(-3.75f, 3.75f));
}


//set details for exploded tank
void Decoration :: setTank2(float baseLevel){

    //background shape
    b2PolygonShape* shape = new b2PolygonShape;
    b2Vec2 points[8];
    points[0].Set(-3.5f, 0.0f);
    points[1].Set(3.5f, 0.0f);
    points[2].Set(3.5f, 6.0f);
    points[3].Set(-3.5f, 6.0f);
    shape->Set(points, 4);
    addShape(shape);

    //add broken container shards
    for (int i=-1; i<=1; i+=2){

        shape = new b2PolygonShape; //outer shards
        points[0].Set(3.5f * ((float) i), 6.0f);
        points[1].Set(randRanged(3.5f, 4.0f)  * ((float) i), randRanged(7.2f, 10.0f));
        points[2].Set(3.0f * ((float) i), 6.0f);
        shape->Set(points, 3);
        addShape(shape);

        for (int j=0; j<3 ;j++){ //inner shards
            shape = new b2PolygonShape;
            points[0].Set(((float) j) * ((float) i), 6.0f);
            points[1].Set(randRanged(((float) j)-0.1f, (((float) j)+0.9f)) * ((float) i), randRanged(7.4f, 11.5f));
            points[2].Set((((float) j) + 1.0f) * ((float) i), 6.0f);
            shape->Set(points, 3);
            addShape(shape);

            //add broken diamond symbol to 2 of the shard segments
            if ((i == 1) && (j == 0 || j == 1)){
                shape = new b2PolygonShape;
                b2Vec2 half = points[0] + points[1]; //works out halfway point between top point and the two sides
                points[0].Set(half.x / 2.0f, half.y / 2.0f);
                half = points[2] + points[1]; //repeat for the next side
                points[2].Set(half.x / 2.0f, half.y / 2.0f);
                points[3].Set((randRanged(0.25f, 0.75f) + ((float) j)) * ((float) i), randRanged(6.1f, 6.5f)); //set point 3 at the bottom of the shard
                shape->Set(points, 4);
                addDetail(shape);
            }
        }
    }

    setTankRidges(this, 0.0f); //bottom ridge
    setTankNumber(this); //number on tank
    setTankLadderPole(this, 5.8f); // ladder poles before breaking
    setTankLadderBars(this, 7); //ladder bars up to breaking

    shape = new b2PolygonShape; //left ladder pole after breaking
    points[0].Set(-3.0f, 5.8f);
    points[1].Set(-3.25f, 9.4f);
    points[2].Set(-3.05f, 9.5f);
    points[3].Set(-2.75f, 5.8f);
    shape->Set(points, 4);
    addDetail(shape);

    shape = new b2PolygonShape; //right ladder pole after breaking
    points[0].Set(-1.75f, 5.8f);
    points[1].Set(-1.7f, 6.7f);
    points[2].Set(-1.55f, 6.9f);
    points[3].Set(-1.5f, 5.8f);
    shape->Set(points, 4);
    addDetail(shape);

    //remaining broken ladder bars
    for (int i=0; i<=3; i++){
        float xShift = ((float) i) * -0.1f;
        float yShift = ((float) i) * 0.75f;
        shape = new b2PolygonShape;
        points[0].Set(-3.25f + xShift, 6.5f + yShift);
        points[1].Set(-3.3f + xShift, 6.7f + yShift);
        points[2].Set(-2.45f + xShift, 6.7f + yShift);
        points[3].Set(-2.40f + xShift, 6.5f + yShift);
        shape->Set(points, 4);
        addDetail(shape);
    }
    addBase(baseLevel, b2Vec2(-3.75f, 3.75f));
}
*/
