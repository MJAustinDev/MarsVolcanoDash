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

#ifndef DECORATION_H
#define DECORATION_H

#include "decorationCodes.h"
#include "camera.h"
#include "visualColours.h"
#include "linkedList.h"
#include "rangedRandom.h"

//back/foreground decoration, no effect on game play just makes the world look better
class Decoration {

protected:

    float colour[4]; //decorations's colour

    b2Vec2 pos; //position of the decoration relative to the world
    LinkedList<b2PolygonShape> shapes; //all decoration base details to be drawn
    LinkedList<b2PolygonShape> details; //all extra on top details to be drawn

    //structure stores texture drawing information
    struct TextShape {
        unsigned int id; //texture id
        b2Vec2 tCoords[4]; //texture sheet coordinates
        b2PolygonShape shape; //texture world coordinates

        TextShape(unsigned int id, b2Vec2 tCoords[4], b2Vec2 points[8], int num){
            this->id = id;
            for (int i=0; i<4; i++){
                this->tCoords[i] = tCoords[i];
            }
            this->shape.Set(points, num);
        }
    };

    //list of textures to add over if needed
    LinkedList<TextShape> tShapes;

    //performs rotation, scale, then translation on given points array
    void transformDecPoints(b2Vec2 points[8], int num, float ang = 0.0f, b2Vec2 mag = b2Vec2(1.0f, 1.0f), b2Vec2 shift = b2Vec2(0.0f, 0.0f));

private:

    //adds a b2PolygonShape to the passed linked list
    void addTo(LinkedList<b2PolygonShape>* store, b2Vec2 points[8], int num){
        b2PolygonShape* ptr = new b2PolygonShape; //dynamically allocate memory for shape
        ptr->Set(points, num); //set shape
        store->addEnd(ptr); //store shape
        ptr = nullptr; //clear pointer for sake of completeness
    }

public:

    Decoration(b2Vec2 pos, float* ptrColour);
    ~Decoration();

    virtual void draw(Camera* camera);

    //sets pasted pointer to the values of this decorations colour (used to keep continuity between 'attached' decorations)
    void getColour(float* ptrRet){
        for (int i=0;i<4;i++){
            ptrRet[i] = colour[i];
        }
    }

    //adds shape to the linked list assume (0, 0) is the very top centre of the base
    void addShape(b2Vec2 points[8], int num){addTo(&shapes, points, num);};

    //adds an on top detail
    void addDetail(b2Vec2 points[8], int num){addTo(&details, points, num);}

    // add new texture
    void addTexture(unsigned int id, b2Vec2 tCoords[4], b2Vec2 points[8], int num){
        if ((id != (unsigned int)-1) && (num == 4)){ //make sure input is valid
            tShapes.addEnd(new TextShape(id, tCoords, points, num));
        }
    };

    //add a base to the decoration (prevent floating decorations), baseLevel is the level the base drops too relative to the shape pos
    void addBase(float baseLevel, b2Vec2 sides, float topLevel = 0.0f){ //sides is the coordinates that the base spans too left and right side (x -> left cord, y -> right cord)

        //if base has some space
        if (baseLevel < topLevel && sides.x != sides.y){
            b2Vec2 points[8];
            points[0].Set(sides.x, topLevel);
            points[1].Set(sides.x, baseLevel);
            points[2].Set(sides.y, baseLevel);
            points[3].Set(sides.y, topLevel);
            addShape(points, 4);
        }
    };
};

//default decoration (invalid id)
class DecDefault : public Decoration {

public:
    DecDefault(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour);
    ~DecDefault();

};

//dome decoration (uses default draw method)
class DecDome : public Decoration {

public:
    DecDome(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour);
    ~DecDome();

private:
    void setDome1();
    void setDome2();

};

//living quarters (uses default draw method)
class DecLQ : public Decoration {

public:
    DecLQ(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour);
    ~DecLQ();

private:
    void setLQ1();
    void setLQ2();

};

//base tunnel (uses default draw method)
class DecTunnel : public Decoration {

public:
    DecTunnel(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour);
    ~DecTunnel();

private:
    void setTunnelToDome(float mirror);

};

//gas tank (uses custom draw method)
class DecTank : public Decoration {

public :
    DecTank(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour);
    ~DecTank();

    void draw(Camera* camera); //TODO -- HAVE MOVING BACKGROUND FLAMES FOR BROKEN TANKER

private:
    void setTankBacking(float height);
    void setTankRidge(float shift);
    void setLadderPoles(float height);
    void setLadderBars(int num);
    void setBrokenShards(float mirror);
    void setFlames(); //TODO

    //TODO -- STORE REFERENCE FOR DRAWING BACKGROUND FLAMES IN LINKED LIST

};

//archway (uses default draw method)
class DecArch : public Decoration {

public:
    DecArch(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour);
    ~DecArch();

private:
    void setArchPole(float mirror);
    void setArchStart(float mirror);
    void setArchEnd(float mirror);

};

//drill machine (uses default draw method)
class DecDrill : public Decoration {

public:
    DecDrill(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour);
    ~DecDrill();

private:
    void setCrate(float mirror, b2Vec2 mag, b2Vec2 shift);
    void setBar(b2Vec2 pos, b2Vec2 dim);
    void setDrillPiece(float ang, b2Vec2 mag, b2Vec2 shift);

};

//cargo ship (uses custom draw method)
class DecShip : public Decoration {

public:
    DecShip(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour, float ang = 0.0f, b2Vec2 mag = b2Vec2(1.0f, 1.0f));
    ~DecShip();

    void draw(Camera* camera);

private:
    void setBaseShip(float ang, b2Vec2 mag);

    int shipID;
    float dispX; //magnitude of travel in the x axis
    float dispY; //magnitude of travel in the y axis

};

//TODO -- DESIGN AND IMPLEMENT SOLAR PANELS

#endif
