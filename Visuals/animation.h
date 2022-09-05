#ifndef ANIMATION_H
#define ANIMATION_H

#include "visualColours.h"
#include "camera.h"

class Animation {

public:

    Animation();
    ~Animation();

    void process();
    void draw(Camera* camera);

private:

    void defShapeMountain();
    void defShapeLavaFlow();
    void resetMeteor(int i);
    void resetBoulder(int i);

    //
    b2PolygonShape mountains[4];
    float mountCol[4][4] = {COLOUR_BLOOD_RED, COLOUR_DEFAULT_GROUND, COLOUR_DEFAULT_GROUND, COLOUR_LAVA_1};

    b2PolygonShape lavaFlow[3];
    float lavaFlCol[3][4] = {COLOUR_LAVA_0, COLOUR_LAVA_0, COLOUR_LAVA_0};

    //
    float meteorRad[5];
    float meteorVel[5];
    float meteorDir[5];
    b2Vec2 meteorPos[5];
    float meteorCol[5][4] = {COLOUR_ORANGE, COLOUR_PURPLE, COLOUR_ORANGE, COLOUR_PURPLE, COLOUR_ORANGE};

    //
    b2Vec2 bouldPos[11];
    float bouldVel[11];
    float bouldDir[11];
    float bouldAng[11];
    b2Vec2 fragments[11][8][2];
    float bouldCol[11][4] = {COLOUR_PURPLE, COLOUR_ORANGE, COLOUR_PURPLE, COLOUR_ORANGE, COLOUR_PURPLE, COLOUR_ORANGE, COLOUR_PURPLE, COLOUR_ORANGE, COLOUR_PURPLE, COLOUR_ORANGE, COLOUR_PURPLE};



};



#endif // ANIMATION_H
