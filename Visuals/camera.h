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

//used when drawing boulders and fragments to access the non-centre points from the b2PolygonShape
inline void forceEdgePoints(b2Vec2* points, b2PolygonShape* shape){
    if (points[0].x == 0.0f && points[0].y == 0.0f){
        points[0] = shape->m_vertices[2];
    } else if (points[1].x == 0.0f && points[1].y == 0.0f){
        points[1] = shape->m_vertices[2];
    }
}

//main class that draws to the viewport
class Camera {

public:

    Camera(GLFWwindow* window, float x, float y, float z, bool* keyP, bool* keyM){posX = x; posY = y; zoom = z; keyPlus = keyP; keyMinus = keyM; loadTextures(); setWindowIcon(window);};
    ~Camera(){unloadTextures();};

    //getters and setters
    float getGlow(){return glow;}; //returns the value of glow
    unsigned int getTexture(int id); //returns the loaded texture number that corresponds to the inputted identifier
    void setCamPos(float x, float y, float z); //resets the camera position

    //draw using pure opengl coordinates
    void drawPureRect(float* colour, float coords[4]); //draws a rectangle of form {top, left, bottom, right}
    void drawPureRectText(float* colour, unsigned int textID, float wCoords[4], float tCoords[4]); //similar to drawPureRect but allows a texture to be drawn instead

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
    void drawChunkShape(b2Body* body, DrawShape* drawShape); //draw terrain shapes

    //draws martian dust over the viewport
    void drawDust();

    void centreCam(b2Vec2 vec){posX = vec.x; posY = vec.y;}; //centres the camera around a given b2Vec2 coordinate
    void processInput(); //allows for zoom to be modified by the player
    void updateGlow(); //shifts the glow value along within it's given range


private:

    //set window icon
    void setWindowIcon(GLFWwindow* window);

    //attempt to load all text textures into opengl 2D textures
    void loadTextures();
    void unloadTextures();

    void catchZoom(); //locks the zoom level between a fixed range

    //camera drawing to viewport vars
    float posX;
    float posY;
    float zoom;

    //user input keys
    bool* keyPlus;
    bool* keyMinus;

    float glow = 0.1; //shifts up and down between a range
    float glowChange = +0.005;
    float dustShift = 0.0f; //shifts up then resets

    //contains the numbers of the loaded textures
    unsigned int texNum[10]; //numbers correspond to index, (0 to 9 => [0] to [9])
    unsigned int texEasy;
    unsigned int texNormal;
    unsigned int texHard;
    unsigned int texPlayer;
    unsigned int texExit;
    unsigned int texResume;
    unsigned int texReturn;
    unsigned int texScore;
    unsigned int texWins;
    unsigned int texDust;

};


#endif
