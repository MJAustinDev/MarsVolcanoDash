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


#include "gameManagement.h"
#include "decorationCodes.h"


Chunk :: Chunk(b2World* w, int segID, GLfloat x, GLfloat y){

    world = w;
    b2BodyDef defBody;
    defBody.type = b2_staticBody;
    defBody.position.Set(x, y);
    body = world->CreateBody(&defBody);

    //define shapes via switch
    switch(segID){
        case -1 : {defSegmentStart(); break;}
        case 0 : {defSegment0(); break;}
        case 1 : {defSegment1(); break;}
        case 2 : {defSegment2(); break;}
        case 3 : {defSegment3(); break;}
        case 4 : {defSegment4(); break;}
        case 5 : {defSegment5(); break;}

        default : {defSegmentDefault();}
    }
}


Chunk :: ~Chunk(){
    world->DestroyBody(body); //body has to be destroyed by the b2World
    while(backDecs.first != nullptr){
        backDecs.remFront();
    }
}

//camera tries to draw chunk to world
void Chunk :: draw(Camera* camera){

    //cycle through and draw all attached shapes inside of the linked list
    if (shapes.resetCycle()){
        do {
            camera->drawChunkShape(body, shapes.cycle->obj);
        } while (shapes.cycleUp());
    }
}

//camera draws background decorations to the world
void Chunk :: drawBackDecs(Camera* camera){

    //cycle through and draw all background decorations
    if (backDecs.resetCycle()){
        do {
            backDecs.cycle->obj->draw(camera);
        } while(backDecs.cycleUp());
    }
}

//camera draws foreground decorations to the world
void Chunk :: drawForeDecs(Camera* camera){

    //cycle through and draw all background decorations
    if (foreDecs.resetCycle()){
        do {
            foreDecs.cycle->obj->draw(camera);
        } while(foreDecs.cycleUp());
    }
}


//creates polygon shape from series of passed points, adding it to the linked list, handles dynamic allocation and pointers
void Chunk :: addShape(b2Vec2* points, int num, int drawId){
    DrawShape* drawShape = new DrawShape;
    for (int i=0;i<num;i++){
        drawShape->shapePoints[i] = points[i]; //set all points
    }
    drawShape->pointCount = num; //store number of points
    drawShape->drawId = drawId; //set draw method identifier
    b2PolygonShape shape;
    shape.Set(points, num); //set shape for fixture
    body->CreateFixture(&shape,0.0f); //create box2d fixture from shape
    shapes.addEnd(drawShape); //store shape position in linked list
    drawShape = nullptr; //render pointer null as object is remembered by linked list
}

//adds a rock (series of connected shapes with varying height) over a space on the main chunk ground
/*
x - x position relative to chunk's b2body
y - series of y coordinates that represent the upper edge of the chunk's ground (series for slopped main chunks)
num - number of 2 meter separated points to build the rock from
minMag - minimum size of rock points off the ground
mag - passed as maximum magnitude that points of the rock should reach off the ground
*/
void Chunk :: addRock(float x, float* y, int num, float minMag, float* mag){

    //increase all magnitudes randomly
    for (int i=0;i<num;i++){
        if (mag[i] != 0.0f){
            mag[i] = randRanged(minMag,mag[i]);
        }
    }

    //breaking down into segments so concave shapes can be draw as multiple convex without having to program triangle splitting on the graphics front
    b2Vec2 points[8];
    for (int i=0;i<num-1;i++){
        points[0].Set(x, y[i]);
        points[1].Set(x+2, y[i+1]);
        points[2].Set(x+2, y[i+1] + mag[i+1]);
        points[3].Set(x, y[i] + mag[i]);
        addShape(points, 4, 2);
        x+=2;
    }
}

//adds a decoration compensating for base height
void Chunk :: addDecoration(int id, b2Vec2 decPos, float* ptrColour, float lowest, LinkedList<Decoration>* ptr, float ang, b2Vec2 mag){

    b2Vec2 relPos = body->GetPosition(); //get relative position to the chunks body
    lowest -= decPos.y; //work out the shift difference between the chunks lowest point and the decorations position relative the the chunk
    relPos += decPos; //adjust decorations position to map to the world rather than to the chunk

    ptr->addEnd(new Decoration(id, relPos, ptrColour, lowest, ang, mag)); //add new decoration to the chunk

}

//just a flat platform used when invalid numbers are entered as segment identifier
void Chunk :: defSegmentDefault(){
    b2Vec2 points[8]; //main base plate
    points[0].Set(-32,0.0);
    points[1].Set(-32,-3.0);
    points[2].Set(32,-3.0);
    points[3].Set(32,0.0);
    addShape(points,4,0);
}

//define starting segment
void Chunk :: defSegmentStart(){

    b2Vec2 points[8]; //main base plate
    points[0].Set(-200,0.0);
    points[1].Set(-200,-3.0);
    points[2].Set(32,-3.0);
    points[3].Set(32,0.0);
    addShape(points,4,0); //use ground chunk shading
    float lowest = -3.0f;

    //start of mountain slope
    points[0].Set(-32.0f,0.0f);
    points[1].Set(-20.0f,0.0f);
    points[2].Set(-25.0f,3.0f);
    addShape(points,3,-1); //use default shading

    points[0].Set(-32.0f,0.0f);
    points[1].Set(-25.0f,3.0f);
    points[2].Set(-30.0f,10.0f);
    addShape(points,3,-1); //use default shading

    points[0].Set(-32.0f,0.0f);
    points[1].Set(-30.0f,10.0f);
    points[2].Set(-32.0f,32.0f);
    addShape(points,3,-1); //use default shading

    //backing of mountain slope
    points[0].Set(-32.0f, 0.0f);
    points[1].Set(-32.0f, 32.0f);
    points[2].Set(-100.0f, 400.0f);
    points[3].Set(-200.0f, 400.0f);
    points[4].Set(-200.0f, 0.0f);
    addShape(points, 5, 1); //use backing mountain shading


    //TEMP TESTING


    //add back ground base
    float c[4]; //used to keep consistent colouring
    //random chance how the base will look
    int lqCode = (randModRanged(2) == 0) ? DEC_CODE_LQ_1 : DEC_CODE_LQ_2;
    int domeCode1 = (randModRanged(2) == 0) ? DEC_CODE_DOME_1 : DEC_CODE_DOME_2;
    int domeCode2 = (randModRanged(2) == 0) ? DEC_CODE_DOME_1 : DEC_CODE_DOME_2;

    //add decorations that make up the base
    addBackDec(lqCode, b2Vec2(5.0f, 3.0f), lowest);
    backDecs.last->obj->getColour(&c[0]); //get colour
    addBackDec(domeCode1, b2Vec2(5.0f, 9.5f), &c[0], lowest, true);
    addBackDec(DEC_CODE_TUNNEL_2, b2Vec2(17.25f, 3.0f), &c[0], lowest, true);
    addBackDec(DEC_CODE_TUNNEL_3, b2Vec2(-7.25f, 3.0f), &c[0], lowest, true);
    addBackDec(domeCode2, b2Vec2(29.5f, 3.0f), &c[0], lowest);
    addBackDec(domeCode2, b2Vec2(-19.5f, 3.0f), &c[0], lowest);

    addBackDec(DEC_CODE_TANK_1, b2Vec2(40.0f, 0.0f), lowest);
    addBackDec(DEC_CODE_TANK_2, b2Vec2(50.0f, 0.0f), lowest);

    //flying in distance
    for (int i=-30; i<=105; i+=15){
        float scale = randRanged(0.1f, 0.6f);
        addBackDec(DEC_CODE_SHIP_1, b2Vec2(((float)i), 48.0f + randRanged(-0.3f, 5.0f)), lowest, randRanged(0.0f, 1.0f), b2Vec2(scale, scale));
    }

    //addBackDec(DEC_CODE_SHIP_1, b2Vec2(75.0f, 5.0f), lowest, -1.3f, b2Vec2(5.5f, 5.5f)); // up close crashed
    addBackDec(DEC_CODE_DRILL_1, b2Vec2(60.0f, 2.5f), lowest);


}


//flat base with randomly assigned rocks
void Chunk :: defSegment0(){

    b2Vec2 points[8]; //main base plate
    points[0].Set(-32,0.0);
    points[1].Set(-32,-3.0);
    points[2].Set(32,-3.0);
    points[3].Set(32,0.0);
    addShape(points,4,0);
    float lowest = -3.0f;

    float x = -32;
    float y[9] = {0,0,0,0,0,0,0,0,0};
    for (int i=0;i<4;i++){
        float mag[9] = {0,1,1,1,1,1,1,1,0};
        addRock(x,y,9,0.2,mag);
        x += 16;
    }
}

//downward slope
void Chunk :: defSegment1(){

    b2Vec2 points[8]; //main base plate
    points[0].Set(-32,0.0);
    points[1].Set(-32,-3.0);
    points[2].Set(32,-19.0);
    points[3].Set(32,-16.0);
    addShape(points,4,0);
    float lowest = -19.0;

    float x = -32;
    float y[9] = {0, -0.5, -1, -1.5, -2, -2.5, -3, -3.5, -4};
    for (int i=0;i<4;i++){
        float mag[9] = {0,1,1,1,1,1,1,1,0};
        addRock(x,y,9,0.2,mag);
        x += 16;
        for (int j=0;j<9;j++){
            y[j] -= 4;
        }
    }
    changeY = -16;
}

//upward slope
void Chunk :: defSegment2(){

    b2Vec2 points[8]; //main base plate
    points[0].Set(-32,0.0);
    points[1].Set(-32,-3.0);
    points[2].Set(32,13.0);
    points[3].Set(32,16.0);
    addShape(points,4,0);
    float lowest = -3.0f;

    float x = -32;
    float y[9] = {0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4};
    for (int i=0;i<4;i++){
        float mag[9] = {0,1,1,1,1,1,1,1,0};
        addRock(x,y,9,0.2,mag);
        x += 16;
        for (int j=0;j<9;j++){
            y[j] += 4;
        }
    }
    changeY = 16;
}

//greater downward slope with more ragid rocks
void Chunk :: defSegment3(){

    b2Vec2 points[8]; //main base plate
    points[0].Set(-32,0.0);
    points[1].Set(-32,-3.0);
    points[2].Set(32,-35.0);
    points[3].Set(32,-32.0);
    addShape(points,4,0);
    float lowest = -35.0f;

    float x = -32;
    float y[9] = {0, -1, -2, -3, -4, -5, -6, -7, -8};
    for (int i=0;i<4;i++){
        float mag[9] = {0,2,4,1,3,1,4,2,0};
        addRock(x,y,9,0.2,mag);
        x += 16;
        for (int j=0;j<9;j++){
            y[j] -= 8;
        }
    }
    changeY = -32;
}

//flat base with more raged randomly assigned rocks
void Chunk :: defSegment4(){

    b2Vec2 points[8]; //main base plate
    points[0].Set(-32,0.0);
    points[1].Set(-32,-3.0);
    points[2].Set(32,-3.0);
    points[3].Set(32,0.0);
    addShape(points,4,0);
    float lowest = -3.0f;

    float x = -32;
    float y[9] = {0,0,0,0,0,0,0,0,0};

    for (int i=0;i<4;i++){
        float mag[9] = {0,1,2,3,2,3,2,1,0};
        addRock(x,y,9,0.2,mag);
        x += 16;
    }
}

//ramp with a pit
void Chunk :: defSegment5(){

    b2Vec2 points[8]; //main base plate
    points[0].Set(-32,0.0);
    points[1].Set(-32,-3.0);
    points[2].Set(32,-3.0);
    points[3].Set(32,0.0);
    addShape(points,4,0);
    float lowest = -3.0f;

    //rocks in the gap, want drawn under both ramps
    float y1[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    float mag1[9] = {5,4,3,2,1,1,1,2,3};
    addRock(0.0f, y1, 9, 0.2f, mag1);

    //ramp up
    points[1].Set(0.0f, 10.0f);
    points[2].Set(0.1f, 0.0f);
    addShape(points, 3, -1);
    float y2[17] = {0, 0.625, 1.25, 1.875, 2.5, 3.125, 3.75, 4.375, 5, 5.625, 6.25, 6.875, 7.5, 8.125, 8.75, 9.375, 10};
    float mag2[17] = {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    addRock(-32.0f, y2, 17, 0.2f, mag2);

    //down ramp
    points[0].Set(16, 6.0f);
    points[1].Set(15.7f, 0.0f);
    points[2].Set(32.0f, 0.0f);
    addShape(points, 3, -1);
    float y3[9] = {6, 5.25, 4.5, 3.75, 3, 2.25, 1.5, 0.75, 0};
    float mag3[9] = {0,1,1,1,1,1,1,1,0};
    addRock(16.0f, y3, 9, 0.2f, mag3);

}
