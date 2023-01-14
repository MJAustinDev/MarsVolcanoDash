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

#include <decoration.h>

//constructor
Decoration :: Decoration(b2Vec2 pos, float* ptrColour){

        this->pos = pos; //set position

        //set colour
        float colPur[4] = COLOUR_PURPLE;
        float colOrg[4] = COLOUR_ORANGE;
        if (ptrColour == nullptr){ //compensate for default random colour selection
            switch (randModRanged(2)){
                case 0 : {ptrColour = &colPur[0]; break; }
                case 1 : {ptrColour = &colOrg[0]; break; }
            }
        }
        for (int i=0;i<4;i++){
            colour[i] = ptrColour[i];
        }
}

//destructor
Decoration :: ~Decoration(){
    //clear both linked lists
    while(shapes.first != nullptr){
        shapes.remFront();
    }
    while (tShapes.first != nullptr){
        tShapes.remFront();
    }
}

//draw the decoration
void Decoration :: draw(Camera* camera){

    float glow = 0.5 - camera->getGlow();
    b2Vec2 posBody = camera->getCamPos(pos); //get world pos relative to camera

    //draw all shapes
    glColor4f(colour[0]*glow, colour[1]*glow, colour[2]*glow, colour[3]*0.9);
    if(shapes.resetCycle()){
        do {
            camera->drawB2PolygonShape(posBody, shapes.cycle->obj, 0.0f);
        } while (shapes.cycleUp());
    }

    //draw all textures
    glow += 0.45;
    glColor4f(colour[0]*glow, colour[1]*glow, colour[2]*glow, 1.0f);
    if(tShapes.resetCycle()){
        do {
            TextShape* ptr = tShapes.cycle->obj;
            camera->drawTextB2Polygone(ptr->id, ptr->tCoords, posBody, &(ptr->shape), 0.0f);
        } while (tShapes.cycleUp());
    }

    //draw all on top details
    if (details.resetCycle()){
        do {
            camera->drawB2PolygonShape(posBody, details.cycle->obj, 0.0f);
        } while (details.cycleUp());
    }
}

//Default decoration design, not meant to be used in finished game
DecDefault :: DecDefault(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour) : Decoration(pos, ptrColour){

    b2PolygonShape shape;
    b2Vec2 points[8];
    points[0].Set(-3.5f, 0.0f); //base quad
    points[1].Set(3.5f, 0.0f);
    points[2].Set(3.0f, 3.5f);
    points[3].Set(-3.0f, 3.5f);
    shape.Set(points, 4);
    addShape(shape);

    points[0].Set(-3.0f, 3.5f); //main rectangle
    points[1].Set(3.0f, 3.5f);
    points[2].Set(3.0f, 16.5f);
    points[3].Set(-3.0f, 16.5f);
    shape.Set(points, 4);
    addShape(shape);

    points[0].Set(-3.0f, 16.5f); //top triangle
    points[1].Set(3.0f, 16.5f);
    points[2].Set(2.0f, 18.0f);
    shape.Set(points, 3);
    addShape(shape);

    b2Vec2 tCoords[4] = {b2Vec2(1.0f, 0.0f), b2Vec2(1.0f, 1.0f), b2Vec2(0.0f, 1.0f), b2Vec2(0.0f, 0.0f)};
    points[0].Set(-2.5f, 4.0f); //add a random number on to it
    points[1].Set(2.5f, 4.0f);
    points[2].Set(2.5f, 12.0f);
    points[3].Set(-2.5f, 12.0f);
    shape.Set(points, 4);
    addTexture(randText0to9(), tCoords, shape);

    points[0].Set(-3.0f, 16.5f); //add left slope of roof
    points[1].Set(-3.0f, 16.0f);
    points[2].Set(2.0f, 17.5f);
    points[3].Set(2.0f, 18.0f);
    shape.Set(points, 4);
    addDetail(shape);

    points[0].Set(2.0f, 18.0f); //add right slope of roof
    points[1].Set(2.0f, 17.5f);
    points[2].Set(3.0f, 16.0f);
    points[3].Set(3.0f, 16.5f);
    shape.Set(points, 4);
    addDetail(shape);

    addBase(baseLevel, b2Vec2(-3.5f, 3.5f));
}
