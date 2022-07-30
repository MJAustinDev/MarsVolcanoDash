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

#ifndef CAMERA_H
#define CAMERA_H

#include <RecompileBox2d/box2d/box2d.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "visualStructures.h"


class Camera{

public:

    Camera(GLfloat x, GLfloat y, GLfloat z, bool* keyP, bool* keyM){posX = x; posY = y; zoom = z; keyPlus = keyP; keyMinus = keyM;};
    ~Camera(){};

    void drawButtonIcon(bool highlight, GLfloat* col, GLfloat* pos); //TODO --OVERHAUL

    void centreCam(b2Vec2 vec){posX = vec.x; posY = vec.y;};
    void processInput();
    void updateGlow();

    //draw player car
    void drawCarBody(b2Body* body, b2PolygonShape* mainShape, b2PolygonShape* roofShape, float* colour);
    void drawWheel(b2Body* body, b2CircleShape* shape, float* colour, float baseAngle);

    //draw enemies
    void drawFireball(b2Body* body, b2CircleShape* shape, float* colour);
    void drawMeteor(b2Body* body, b2CircleShape* shape, float* colour);
    void drawFragment(b2Body* body, b2PolygonShape* shape, float* colour);
    void drawBoulder(b2Body* body, b2PolygonShape** shapes, float* colour);

    //draw terrain and lava
    void drawChunkShape(b2Body* body, DrawShape* drawShape);
    void drawLava(float lavaX, float playerY, b2Vec2* baseShape, b2Vec2* edgeShape, b2Vec2* innerShape);

private:

    //get main point in world relative to camera position
    b2Vec2 getCamPos(b2Vec2 pos);
    b2Vec2 getCamBodyPos(b2Body* body);

    //glVertex2f place points relative to a translated body and camera position
    void placePoint(b2Vec2 posBody, b2Vec2 pos);
    void placeRotatePoint(b2Vec2 posBody, b2Vec2 pos, float angle);
    void placeCirclePoints(int res, int lower, int upper, b2Vec2 posBody, float radius, float baseAngle);

    //draw basic shapes
    void drawB2PolygonShape(b2Vec2 posBody, b2PolygonShape* shape, float angle); //draw b2PolygonShape from body position and angle
    void drawHotball(b2Vec2 posBody, float radius, float* colour, float glowCen, float glowOut); //draws circle with different colours at centre and circumfrence
    void drawHotFrag(b2Vec2 posBody, b2Vec2* points, float angle, float* colour, float glowCen, float glowOut);

    //draw chunks dependant on identifier
    void drawDefaultChunkShape(b2Vec2 posBody, DrawShape* drawShape);
    void drawChunkId0(b2Vec2 posBody, b2Vec2* points);


    GLfloat posX;
    GLfloat posY;
    GLfloat zoom;

    bool* keyPlus;
    bool* keyMinus;

    CarDetails carDetail;

    float glow = 0.1;
    float glowChange = +0.005;



};


#endif
