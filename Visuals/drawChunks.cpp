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


#include "camera.h"

#include "visualColours.h"


//solid fills in the chunk shape with glow
void drawDefaultChunkShape(Camera* camera, b2Vec2 posBody, DrawShape* drawShape){

    float baseColour[4] = COLOUR_DEFAULT_GROUND;
    float shade = 1.0f - camera->getGlow();

    glBegin(GL_POLYGON);
        glColor4f(baseColour[0]*shade, baseColour[1]*shade, baseColour[2]*shade, baseColour[3]);
        for (int i=0;i<drawShape->pointCount;i++){
            camera->placePoint(posBody, drawShape->shapePoints[i]);
        }
    glEnd();
}


//basic ground chunk
void drawChunkId0(Camera* camera, b2Vec2 posBody, b2Vec2* points){

    //order of points in b2Vec2 array, top left -> bottom left -> bottom right -> top right

    float baseColour[4] = COLOUR_DEFAULT_GROUND;
    float shade = 1.0f - camera->getGlow();

    //starting at [1],.. and ending at ...,[0] to save colour changing
    float underCol[4] = COLOUR_BACK_GAME; //colour underneath the terrain
    glBegin(GL_POLYGON);
        glColor4f(baseColour[0]*shade, baseColour[1]*shade, baseColour[2]*shade, baseColour[3]);
        camera->placePoint(posBody, points[3]);
        camera->placePoint(posBody, points[0]);
        glColor4f(underCol[0], underCol[1], underCol[2], underCol[3]);
        camera->placePoint(posBody, b2Vec2(points[1].x, points[1].y - 35.0f));
        camera->placePoint(posBody, b2Vec2(points[2].x, points[2].y - 35.0f));
    glEnd();
    //draw solid dark background fill under terrain
    glBegin(GL_POLYGON);
        camera->placePoint(posBody, b2Vec2(points[1].x, points[1].y - 35.0f));
        camera->placePoint(posBody, b2Vec2(points[2].x, points[2].y - 35.0f));
        camera->placePoint(posBody, b2Vec2(points[2].x, points[2].y - 100.0f));
        camera->placePoint(posBody, b2Vec2(points[1].x, points[1].y - 100.0f));
    glEnd();

}


//mountain backing chunk shape
void drawChunkId1(Camera* camera, b2Vec2 posBody, b2Vec2* points){

    //order of points in b2Vec2 array, bottom right -> upper far right -> top right -> top left -> bottom left

    float shade = 1.0f - camera->getGlow();

    float baseColour[4] = COLOUR_DEFAULT_GROUND;
    float underCol[4] = COLOUR_BACK_GAME; //colour underneath the terrain
    glBegin(GL_POLYGON);
        //set all connecting points to the ground default colour
        glColor4f(baseColour[0]*shade, baseColour[1]*shade, baseColour[2]*shade, baseColour[3]);
        camera->placePoint(posBody, points[4]);
        camera->placePoint(posBody, points[0]);
        camera->placePoint(posBody, points[1]);

        //set all unconnected (top) points to the darker colour
        glColor4f(underCol[0]*shade, underCol[1]*shade, underCol[2]*shade, underCol[3]);
        camera->placePoint(posBody, points[2]);
        camera->placePoint(posBody, points[3]);
    glEnd();

}

//semi transparent overlay for surface rocks
void drawChunkId2(Camera* camera, b2Vec2 posBody, b2Vec2* points) {
    float shade = 1.0f - camera->getGlow();
    float baseColour[4] = COLOUR_DEFAULT_GROUND;
    float surfColour[4] = COLOUR_BLOOD_RED;
    glBegin(GL_POLYGON);
        glColor4f(baseColour[0]*shade, baseColour[1]*shade, baseColour[2]*shade, 0.6f);
        camera->placePoint(posBody, points[0]); //0 and 1 are on the ground
        camera->placePoint(posBody, points[1]);
        shade -= 0.1f;
        glColor4f(surfColour[0]*shade, surfColour[1]*shade, surfColour[2]*shade, 0.2f);
        camera->placePoint(posBody, points[2]); //2 and 3 are in the air
        camera->placePoint(posBody, points[3]);
    glEnd();

}

//switch based method for drawing chunk shapes
void Camera :: drawChunkShape(b2Body* body, DrawShape* drawShape){

    b2Vec2 posBody = getCamBodyPos(body);

    switch (drawShape->drawId) {
        case 0 : {drawChunkId0(this, posBody, drawShape->shapePoints); break;} //basic ground chunk shape
        case 1 : {drawChunkId1(this, posBody, drawShape->shapePoints); break;} //mountain backing chunk shape
        case 2 : {drawDefaultChunkShape(this, posBody, drawShape); drawChunkId2(this, posBody, drawShape->shapePoints); break;} //surface rock shape
        default : {drawDefaultChunkShape(this, posBody, drawShape);} //default shape shading
    }
}
