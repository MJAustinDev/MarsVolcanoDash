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

//constructor for cargo ship decoration (both flying and crashed)
DecShip :: DecShip(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour, float ang, b2Vec2 mag) : Decoration(pos, ptrColour){

    shipID = id; //stored so different draw methods can be used

    setBaseShip(ang, mag);
    switch(shipID){
        case DEC_CODE_SHIP_1 : { //flying
            float speed = randRanged(0.2f, 0.5f); //decide on a speed randomly
            dispX = (speed * cos(ang)); //work out displacements
            dispY = (speed * sin(ang));
            //force a minimum Y speed value (so ships should be out of view when deallocated)
            if (dispY < 0.0f){
                dispY = ((dispY > -0.1f)) ? -0.1f : dispY; //minimum Y speed
            } else {
                dispY = ((dispY < 0.1f)) ? 0.1f : dispY; //minimum Y speed
            }
            dispX *= (mag.x < 0.0f) ? -1.0f : 1.0f; //compensate for direction
            dispY *= (mag.y < 0.0f) ? -1.0f : 1.0f;
            break;
        }
        case DEC_CODE_SHIP_2 : {break;} //crashed
        default : {}
    }

}

DecShip :: ~DecShip(){} //TODO -- DEALLOCATE EXTRA VISUAL EFFECTS

//unique draw method
void DecShip :: draw(Camera* camera){

    float glow = 0.5 - camera->getGlow();
    b2Vec2 posBody = camera->getCamPos(pos); //get world pos relative to camera

    //draw all shapes
    glColor4f(colour[0]*glow, colour[1]*glow, colour[2]*glow, colour[3]*0.9);
    if(shapes.resetCycle()){
        do {
            camera->drawB2PolygonShape(posBody, shapes.cycle->obj, 0.0f);
        } while (shapes.cycleUp());
    }

    //draw all on top details
    glow += 0.25;
    glColor4f(colour[0]*glow, colour[1]*glow, colour[2]*glow, 1.0f);
    if (details.resetCycle()){
        do {
            camera->drawB2PolygonShape(posBody, details.cycle->obj, 0.0f);
        } while (details.cycleUp());
    }

    //draw all textures
    glow += 0.20;
    glColor4f(colour[0]*glow, colour[1]*glow, colour[2]*glow, 1.0f);
    if(tShapes.resetCycle()){
        do {
            TextShape* ptr = tShapes.cycle->obj;
            camera->drawTextB2Polygone(ptr->id, ptr->tCoords, posBody, &(ptr->shape), 0.0f);
        } while (tShapes.cycleUp());
    }

    switch(shipID){
        case DEC_CODE_SHIP_1 : {pos += b2Vec2(dispX, dispY); break;} //flying TODO -- DRAW ENGINE FLAMES
        case DEC_CODE_SHIP_2 : {break;} //crashed TODO -- DRAW RUBBLE/BACKGROUND FIRE
        default : {}
    }
}

//sets the design for the base ship
void DecShip :: setBaseShip(float ang, b2Vec2 mag){

    b2Vec2 points[8];
    points[0].Set(8.8f, 1.75f); //main body of the ship
    points[1].Set(5.0f, 2.0f);
    points[2].Set(-5.0f, 2.0f);
    points[3].Set(-5.0f, -2.0f);
    points[4].Set(7.5f, -2.0f);
    points[5].Set(9.0f, -1.5f);
    transformDecPoints(&points[0], 6, ang, mag);
    addShape(points, 6);

    points[0].Set(-5.0f, 0.0f); //engine extension
    points[1].Set(-5.75f, 0.0f);
    points[2].Set(-6.0f, 0.5f);
    points[3].Set(-6.25f, 2.0f);
    points[4].Set(-5.0f, 2.0f);
    transformDecPoints(&points[0], 5, ang, mag);
    addShape(points, 5);

    points[0].Set(1.0f, 2.0f); //bridge extension
    points[1].Set(-4.25f, 2.0f);
    points[2].Set(-3.25f, 2.6f);
    points[3].Set(-0.25f, 2.5f);
    transformDecPoints(&points[0], 4, ang, mag);
    addShape(points, 4);

    points[0].Set(-3.70f, 2.15f); //bridge window
    points[1].Set(-3.25f, 2.4f);
    points[2].Set(-0.25f, 2.3f);
    points[3].Set(0.5f, 2.0f);
    points[4].Set(-2.25f, 1.75f);
    transformDecPoints(&points[0], 5, ang, mag);
    addDetail(points, 5);

    points[0].Set(-2.0f, 0.5f); //armour panel
    points[1].Set(-2.75f, 1.75f);
    points[2].Set(-4.75f, 2.1f);
    points[3].Set(-4.75f, 0.5f);
    points[4].Set(-4.5f, -2.1f);
    points[5].Set(-2.75f, -1.75f);
    transformDecPoints(&points[0], 6, ang, mag);
    addDetail(points, 6);

    b2Vec2 tCoords[4] = {b2Vec2(1.0f, 0.0f), b2Vec2(1.0f, 1.0f), b2Vec2(0.0f, 1.0f), b2Vec2(0.0f, 0.0f)};
    points[0].Set(-4.5f, 1.8f); //number on armour panel
    points[1].Set(-4.5f, -1.8f);
    points[2].Set(-3.0f, -1.8f);
    points[3].Set(-3.0f, 1.8f);
    transformDecPoints(&points[0], 4, ang, mag);
    int shift = 0;
    float tempAng = ang;
    while (tempAng < 0.0f){tempAng += (2.0f*M_PI);} //bring tempAng into 0 to 360 deg range
    while(tempAng > (M_PI * 0.5f)){ //work out how many places the values in tCoords need to be shifted backwards
        tempAng -= (M_PI * 0.5f);
        shift++;
    }
    for (int i=0; i<shift; i++){ //shift tCoords so number will be the correct way up on the ship
        b2Vec2 temp = tCoords[3];
        for (int j=3; j>=1; j--){
            tCoords[j] = tCoords[j-1];
        }
        tCoords[0] = temp;
    }
    addTexture(randText0to9(), tCoords, points, 4);

    points[0].Set(-4.75f, 2.1f); //top armour panel wing
    points[1].Set(-6.0f, 2.25f);
    points[2].Set(-5.75f, 1.0f);
    points[3].Set(-4.75f, 0.5f);
    transformDecPoints(&points[0], 4, ang, mag);
    addDetail(points, 4);

    points[0].Set(-1.5f, 1.0f); //cargo section
    points[1].Set(-1.25f, -2.25f);
    points[2].Set(7.0f, -2.25f);
    points[3].Set(7.25f, 1.0f);
    transformDecPoints(&points[0], 4, ang, mag);
    addDetail(points, 4);

    for (int i=0; i<4; i++){//number on cargo section
        float xShift = ((float) i) * 1.2f;
        points[0].Set(-1.4f + xShift, -0.1f);
        points[1].Set(-1.4f + xShift, -2.20f);
        points[2].Set(-0.2f + xShift, -2.20f);
        points[3].Set(-0.2f + xShift, -0.1f);
        transformDecPoints(&points[0], 4, ang, mag);
        addTexture(randText0to9(), tCoords, points, 4);
    }

    points[0].Set(-6.19f, 1.75f); //top engine first section
    points[1].Set(-7.0f, 1.5f);
    points[2].Set(-7.0f, 0.5f);
    points[3].Set(-6.0f, 0.5f);
    transformDecPoints(&points[0], 4, ang, mag);
    addDetail(points, 4);

    points[0].Set(-5.8f, 0.1f); //top engine second section
    points[1].Set(-7.0f, 0.25f);
    points[2].Set(-7.0f, 0.5f);
    points[3].Set(-6.0f, 0.5f);
    transformDecPoints(&points[0], 4, ang, mag);
    addDetail(points, 4);

    points[0].Set(-5.0f, -0.1f); //bottom engine
    points[1].Set(-6.0f, -0.35f);
    points[2].Set(-6.0f, -1.5f);
    points[3].Set(-5.0f, -1.75f);
    transformDecPoints(&points[0], 4, ang, mag);
    addDetail(points, 4);
}









