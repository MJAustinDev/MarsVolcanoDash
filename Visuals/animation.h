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

#ifndef ANIMATION_H
#define ANIMATION_H

#include "visualColours.h"
#include "camera.h"

//back ground splash animation for the main menu
class Animation {

public:

    Animation();
    ~Animation();

    void process(); //moves all active  items (meteors/boulders)
    void draw(Camera &camera); //draws scene to the screen

private:

    //private setters
    void defShapeMountain();
    void defShapeLavaFlow();
    void resetMeteor(int i);
    void resetBoulder(int i);

    //mountain range vars
    b2PolygonShape mountains[4];
    float mountCol[4][4] = {COLOUR_BLOOD_RED, COLOUR_DEFAULT_GROUND, COLOUR_DEFAULT_GROUND, COLOUR_LAVA_1};

    //lava flow highlight vars
    b2PolygonShape lavaFlow[3];
    float lavaFlCol[3][4] = {COLOUR_LAVA_0, COLOUR_LAVA_0, COLOUR_LAVA_0};

    //meteor vars
    float meteorRad[5];
    float meteorVel[5];
    float meteorDir[5];
    b2Vec2 meteorPos[5];
    float meteorCol[5][4] = {COLOUR_ORANGE, COLOUR_PURPLE, COLOUR_ORANGE, COLOUR_PURPLE, COLOUR_ORANGE};

    //falling boulder vars
    b2Vec2 bouldPos[11];
    float bouldVel[11];
    float bouldDir[11];
    float bouldAng[11];
    float bouldRotDir[11];
    b2Vec2 fragments[11][8][2];
    float bouldCol[11][4] = {COLOUR_PURPLE, COLOUR_ORANGE, COLOUR_PURPLE, COLOUR_ORANGE, COLOUR_PURPLE, COLOUR_ORANGE, COLOUR_PURPLE, COLOUR_ORANGE, COLOUR_PURPLE, COLOUR_ORANGE, COLOUR_PURPLE};

    //lava burst vars
    b2Vec2 burstBase[2] = {b2Vec2(-0.85f, 0.2f) ,b2Vec2(-0.6f, 0.15f)};
    b2Vec2 burstSide[2] = {b2Vec2(-0.9f, 0.35f), b2Vec2(-0.5f, 0.25f)};
    b2Vec2 burstRnd[16];
    float burstCol[5][4] = {COLOUR_LAVA_0, COLOUR_LAVA_1, COLOUR_LAVA_2, COLOUR_LAVA_3, COLOUR_LAVA_4};

};


#endif
