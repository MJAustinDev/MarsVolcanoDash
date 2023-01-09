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

#ifndef DECORATION_H
#define DECORATION_H

#include <camera.h>
#include <visualColours.h>
#include <linkedList.h>
#include <rangedRandom.h>

/*

WIP NOTES - Have reference to the chunk it 'exists in'
 so when the chunk is removed the game manager removes all decorations that the chunk created

 Have new .h with pre-processor reference to the decoration IDs

*/

//back/foreground decoration, no effect on game play just makes the world look better
class Decoration {

private:

    unsigned int id; //texture id used by camera
    float colour[4]; //decorations's colour

    b2Vec2 pos; //position of the decoration relative to the world

    //b2PolygonShape base; //base plate for raised decorations (on slopes) -- MAYBE???

    LinkedList<b2PolygonShape> shapes; //all decoration details to be drawn
    LinkedList<b2PolygonShape> details; //all extra on top details to be drawn


    struct TextShape {
        unsigned int id; //texture id
        b2Vec2 tCoords[4]; //texture sheet coordinates
        b2PolygonShape shape; //texture world coordinates

        TextShape(unsigned int id, b2Vec2 tCoords[4], b2PolygonShape shape){
            this->id = id;
            for (int i=0; i<4; i++){
                this->tCoords[i] = tCoords[i];
            }
            this->shape = shape;
        }
    };

    //list of textures to add over if needed
    LinkedList<TextShape> tShapes;

    //adds shape to the linked list assume (0, 0) is the very top centre of the base
    void addShape(b2PolygonShape* shape){
        shapes.addEnd(shape);
    };

    //adds an on top detail
    void addDetail(b2PolygonShape* detail){
        details.addEnd(detail);
    };

    // add new texture
    void addTexture(unsigned int id, b2Vec2 tCoords[4], b2PolygonShape shape){
        if ((id != (unsigned int)-1) && (shape.m_count == 4)){ //make sure input is valid
            tShapes.addEnd(new TextShape(id, tCoords, shape));
        }
    };

    //add a base to the decoration (prevent floating decorations), baseLevel is the level the base drops too relative to the shape pos
    void addBase(float baseLevel, b2Vec2 sides){ //sides is the coordinates that the base spans too left and right side (x -> left cord, y -> right cord)

        //if base has some space
        if (baseLevel < 0.0f && sides.x != sides.y){
        //if (baseLevel != 0.0f && sides.x != sides.y){
            b2Vec2 points[8];
            points[0].Set(sides.x, 0.0f);
            points[1].Set(sides.x, baseLevel);
            points[2].Set(sides.y, baseLevel);
            points[3].Set(sides.y, 0.0f);
            b2PolygonShape* base = new b2PolygonShape;
            base->Set(points, 4);
            addShape(base);
            base = nullptr;
        }
    };

    //add new set id methods
    void setDefault(float baseLevel);
    void setID0(float baseLevel);

public:

    Decoration(int id, b2Vec2 pos, float baseLevel){
        this->pos = pos;

        //set colour randomly
        float colPur[4] = COLOUR_PURPLE;
        float colOrg[4] = COLOUR_ORANGE;
        float* rndColour = nullptr;
        switch (randModRanged(2)){
            case 0 : {rndColour = &colPur[0]; break; }
            case 1 : {rndColour = &colOrg[0]; break; }
        }
        for (int i=0;i<4;i++){
            colour[i] = rndColour[i];
        }

        //set decoration's shapes and textures
        switch(id){
            //case 0 : setID0(); break;
            default : {setDefault(baseLevel);}
        }
    };

    Decoration(int id, b2Vec2 pos) : Decoration(id, pos, (-pos.y)){}; //default is to assume that the ground is at 0.0f

    ~Decoration(){
        //clear both linked lists
        while(shapes.first != nullptr){
            shapes.remFront();
        }
        while (tShapes.first != nullptr){
            tShapes.remFront();
        }
    };

    void draw(Camera* camera){

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
    };

};


#endif
