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


//used by some decorations rotates then scales the passed b2Vec2(x, y) points
inline void transformDecPoints(b2Vec2* points, int num, float ang = 0.0f, b2Vec2 mag = b2Vec2(1.0f, 1.0f), b2Vec2 shift = b2Vec2(0.0f, 0.0f)){

    //rotate
    if (ang != 0.0f){
        for (int i=0; i<num; i++){
            float c = cos(ang);
            float s = sin(ang);
            points[i].Set((points[i].x * c) - (points[i].y * s), (points[i].y * c) + (points[i].x * s));
        }
        }

    //scale (and mirror if scale is negative)
    if (mag != b2Vec2(1.0f, 1.0f)){
        for (int i=0; i<num; i++){
            points[i].x *= mag.x;
            points[i].y *= mag.y;
        }
    }

    //shift
    if (shift != b2Vec2(0.0f, 0.0f)){
        for (int i=0; i<num; i++){
            points[i] += shift;
        }
    }
}

//back/foreground decoration, no effect on game play just makes the world look better
class Decoration {

private:

    unsigned int id; //texture id used by camera
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
    void addBase(float baseLevel, b2Vec2 sides){ //sides is the coordinates that the base spans too left and right side (x -> left cord, y -> right cord)

        //if base has some space
        if (baseLevel < 0.0f && sides.x != sides.y){
            b2Vec2 points[8];
            points[0].Set(sides.x, 0.0f);
            points[1].Set(sides.x, baseLevel);
            points[2].Set(sides.y, baseLevel);
            points[3].Set(sides.y, 0.0f);
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

class DecArch : public Decoration {

public:
    DecArch(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour);
    ~DecArch();

private:
    void setArchPole(float mirror);
    void setArchStart(float mirror);
    void setArchEnd(float mirror);

};

/*

void setTank1(float baseLevel);
void setTank2(float baseLevel);
//void setPanel1(float baseLevel);
void setDrill1(float baseLevel);
void setDrill2(float baseLevel);
void setArch1(float baseLevel);
void setArch2(float baseLevel);
void setArch3(float baseLevel);
void setArch4(float baseLevel);
void setArch5(float baseLevel);
void setShip1(float baseLevel, float ang, b2Vec2 mag);
*/

#endif
