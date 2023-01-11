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


#include "gameEntities.h"

/*
Boulders take the form of "boxcar" bodies, i.e., multiple connected triangles revolving around a point
See here for a better explanation: http://boxcar2d.com/about.html
My final year university project revolved around boxcars so a lot of this code has been carried over and simplified from what I built for my degree

Boulders break into the triangle fragments when contacting another physics object during simulation
*/

bool approxEqual(float a, float b){
    //using 0.02 as its approximately a degree in radians, box2d can just about handle 0.01 difference within a 1 meter range... but cutting it safe
    return (a >= (b-0.02)) && (a <= (b+0.02));
}


Boulder :: Boulder(b2World* w, b2Vec2 pos, float sizeRand, float maxForce) : BaseBreakableEnemy(400,w,pos,8){

    //force a minimum, maximum size
    if (sizeRand<1.2f){
        sizeRand = 1.2f;
    }

    float angs[8];
    float mags[8];

    //generate points
    for (int i=0;i<8;i++){
        angs[i] = randRanged(0.0, M_PI+M_PI-0.02); //ranged from 0 to (2pi - 1 degree)
        mags[i] = randRanged(1.0f, sizeRand); //ranged from 1 meter to parameter passed limit
    }

    insertSortPoints(mags,angs); //order points via insertion sort (smallest angle then smallest magnitude)

    int valid = overWriteEqual(mags,angs); //overwrite equal points

    //define shapes
    if ((valid < 2) || !defineShapes(mags,angs,valid)){
        setDefaultShapes(sizeRand); //use default design if random design happened to cause degenerate polygons
    }

    b2FixtureDef defFix;
    defFix.friction = 1.0f;
    defFix.density = 3.0f;
    for (int i=0;i<8;i++){
        if (shapes[i]!=nullptr){
            defFix.shape = shapes[i];
            sensors[i] = body->CreateFixture(&defFix);
        }
    }

    //fire off boulder with a random force with a 180 deg range facing downwards
    float angle = randRanged(M_PI,2.0f*M_PI);
    maxForce = randRanged(0.0f,maxForce);
    b2Vec2 force(maxForce*cos(angle),maxForce*sin(angle));
    body->ApplyLinearImpulseToCenter(force,true);
}


Boulder :: ~Boulder(){
    for (int i=0;i<8;i++){
        if (shapes[i]!=nullptr){
            delete shapes[i];
        }
    }
}

//insertion sort on magnitude and angles
void Boulder :: insertSortPoints(float* mags, float* angs){
    float temp;
    //scan up from 1
    for (int i=1;i<8;i++){
        //scan down from i
        for (int j=i;j>0;j--){
            //if j is smaller than j-1 then swap, if equal swap if j magnitude is smaller than j-1
            if ((angs[j] <= angs[j-1]) || (approxEqual(angs[j],angs[j-1]) && (mags[j]<mags[j-1]))) {
                temp = angs[j]; //swap angles
                angs[j] = angs[j-1];
                angs[j-1] = temp;
                temp = mags[j]; //swap magnitudes
                mags[j] = mags[j-1];
                mags[j-1] = temp;
            } else {
                j = 0; //in position
            }
        }
    }
}

//removes equal points by over writing them with the rest of the array
int Boulder :: overWriteEqual(float* mags, float* angs){
    int valid = 8; //assume already ordered
    //scan upwards
    for (int i=0;i<valid-1;i++){
        if (approxEqual(angs[i],angs[i+1]) && approxEqual(mags[i],mags[i+1])){
            //overwrite the next point
            for (int j=i+1;j<valid;j++){
                angs[j] = angs[j+1];
                mags[j] = mags[j+1];
            }
            valid--;
            i--; //rescan current i as i+1 has changed
        }
    }
    return valid;
}

//attempts to create polygon shapes from the points, returns false if no valid polygons can form
bool Boulder :: defineShapes(float* mags, float* angs, int valid){

    bool check = false;
    b2Vec2 points[3]; //all polygons are triangles
    points[0].Set(0.0f,0.0f);

    //start with first point
    float x1 = mags[0] * cos(angs[0]);
    float y1 = mags[0] * sin(angs[0]);
    float x2;
    float y2;

    //for each valid point except the last (last winds around to the beginning point)
    for (int i=0;i<valid-1;i++){
        //get coordinates of next point in cycle
        x2 = mags[i+1] * cos(angs[i+1]);
        y2 = mags[i+1] * sin(angs[i+1]);
        //if angle between ith and i+1th point is acute
        if ((angs[i+1]-angs[i]) < M_PI){
            //if points are not on the same line, then make a box2d polygon shape
            if (!(approxEqual(angs[i],angs[i+1])) && !(approxEqual(M_PI+angs[i],angs[i+1]))){
                points[1].Set(x1,y1); //want CCW winding of points, hence [1] is point 1, and [2] is point 2
                points[2].Set(x2,y2);
                shapes[i] = new b2PolygonShape(); //set box2d shape
                shapes[i]->Set(points,3);
                check = true;
            }
        }
        //set next 1st coordinates to current 2nd coordinates for next winding
        x1 = x2;
        y1 = y2;
    }

    //if final point to 1st point angle is acute (was 2*M_PI... < M_PI... but can take M_PI off both sides)
    if ((M_PI-angs[valid-1]+angs[0]) < 0){
        //get coordinates of 1st point
        x2 = mags[0] * cos(angs[0]);
        y2 = mags[0] * sin(angs[0]);
        //if points are not on the same line, then make a box2d polygon shape
        if (!(approxEqual(angs[0],angs[valid-1])) && !(approxEqual(M_PI+angs[0],angs[valid-1]))){
            points[1].Set(x1,y1); //want CCW winding of points still
            points[2].Set(x2,y2);
            shapes[valid-1] = new b2PolygonShape(); //set box2d shape
            shapes[valid-1]->Set(points,3);
            check = true;
        }
    }
    return check;
}

//sets default boulder shapes, used when the original mags and angs only create degenerate polygons (e.g all points same on the line, etc)
void Boulder :: setDefaultShapes(float sizeRand){
    float angs[8];
    float mags[8];
    //generate points
    for (int i=0;i<8;i++){
        angs[i] = ((2.0f*M_PI)/8.0f) * i;
        mags[i] = 1 + (i%4) + sizeRand;
    }
    defineShapes(mags,angs,8);

}

//break up boulder into smaller fragments
void Boulder :: afterCollide(LinkedList<Fragment>* fragments){

    for (int i=0;i<8;i++){
        if (shapes[i]!=nullptr){
            Fragment* fragment = new Fragment(world, body->GetPosition(), body->GetLinearVelocity(), body->GetAngle(), shapes[i], colour);
            fragments->addEnd(fragment);
            fragment = nullptr;
        }
    }
}

//draws boulder to the viewport
void Boulder :: draw(Camera* camera){
    b2Vec2 posBody = camera->getCamBodyPos(body);
    float angle = body->GetAngle();
    float shade = 1.0f - (2.0f*camera->getGlow());

    for (int i=0;i<8;i++){
        if (shapes[i]!=nullptr){
            b2Vec2 points[2] = {shapes[i]->m_vertices[0], shapes[i]->m_vertices[1]};
            forceEdgePoints(points, shapes[i]);
            camera->drawHotFrag(posBody, points, angle, colour, shade, 0.4f); //keep outside looking cold
        }
    }
}



