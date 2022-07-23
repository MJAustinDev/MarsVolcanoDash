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


#include "gameManagement.h"


Chunk :: Chunk(b2World* w, int segID, GLfloat x, GLfloat y){

    world = w;
    b2BodyDef defBody;
    defBody.type = b2_staticBody;
    defBody.position.Set(x, y);
    body = world->CreateBody(&defBody);

    //define shapes via switch TODO -- EXPAND WITH NEW SEGMENT TYPES
    switch(segID){
        case -1 : {defSegmentStart(); break;}
        case 0 : {defSegment0(); break;}
    }
}


Chunk :: ~Chunk(){
    world->DestroyBody(body); //body has to be destroyed by the b2World
}

//camera tries to draw chunk to world
void Chunk :: draw(Camera* camera){
    GLfloat col[3] = {1.0f,0.0f,0.0f}; //TODO -- UPDATE COLOURING/GRAPHICS SYSTEM

    //cycle through all attached shapes inside of the linked list
    if (shapes.resetCycle()){
        do {
            camera->drawB2Polygon(body,(shapes.cycle->obj),col); //use camera to draw b2polygon shape
        } while (shapes.cycleUp());
    }
}


//creates polygon shape from series of passed points, adding it to the linked list, handles dynamic allocation and pointers
void Chunk :: addShape(b2Vec2* points, int num){
    b2PolygonShape* shape = new b2PolygonShape(); //define shape in memory
    shape->Set(points,num); //set shape
    shapes.addEnd(shape); //store shape position in linked list
    body->CreateFixture(shape,0.0f); //create box2d fixture from shape
    shape = nullptr; //render pointer null as object is remembered by linked list (shouldn't matter as ptr will go out of scope but good practice)
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
    for (int i=2;i<num;i++){
        mag[i] = randRanged(minMag,mag[i]);
    }

    //breaking down into segments so concave shapes can be draw as multiple convex without having to program triangle splitting on the graphics front

    //generate first triangle of rock
    b2Vec2 points[8];
    points[0].Set(x,y[0]);
    points[1].Set(x+2,y[1]);
    points[2].Set(x+2,y[num-1] + mag[num-1]);
    addShape(points,3);

    //generate each rock segment
    int i=num-1;
    x += 2;
    while (i>2){
        points[0].Set(x,y[i]);
        points[1].Set(x+2,y[i-1]);
        points[2].Set(x+2,y[i-1] + mag[i-1]);
        points[3].Set(x,y[i] + mag[i]);
        addShape(points,4);
        i--;
        x +=2;
    }

    //generate last triangle
    points[0].Set(x,y[i]);
    points[1].Set(x+2,y[1]);
    points[2].Set(x,y[i]+mag[i]);
    addShape(points,3);
}

//define starting segment -- TODO redesign the volcano edge
void Chunk :: defSegmentStart(){

    b2Vec2 points[8];
    points[0].Set(-32,0.0);
    points[1].Set(-32,-3.0);
    points[2].Set(32,-3.0);
    points[3].Set(32,0.0);
    addShape(points,4);

    points[0].Set(-32.0f,0.0f);
    points[1].Set(-20.0f,0.0f);
    points[2].Set(-25.0f,3.0f);
    addShape(points,3);

    points[0].Set(-32.0f,0.0f);
    points[1].Set(-25.0f,3.0f);
    points[2].Set(-30.0f,10.0f);
    addShape(points,3);

    points[0].Set(-32.0f,0.0f);
    points[1].Set(-30.0f,10.0f);
    points[2].Set(-32.0f,32.0f);
    addShape(points,3);

}


//flat base with randomly assigned rocks
void Chunk :: defSegment0(){

    b2Vec2 points[8];
    points[0].Set(-32,0.0);
    points[1].Set(-32,-3.0);
    points[2].Set(32,-3.0);
    points[3].Set(32,0.0);
    addShape(points,4);

    float x = -32;
    float y[8] = {0,0,0,0,0,0,0,0};
    float mag[8] = {0,0,1,1,1.1,1,1,1};

    for (int i=0;i<4;i++){
        addRock(x,y,8,0.2,mag);
        x = x + 16;
    }
}
