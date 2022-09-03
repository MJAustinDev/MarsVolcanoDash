
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
        glColor4f(underCol[0], underCol[1], underCol[2], underCol[3]);
        camera->placePoint(posBody, b2Vec2(points[1].x, points[1].y - 15.0f));
        camera->placePoint(posBody, b2Vec2(points[2].x, points[2].y - 15.0f));
        glColor4f(baseColour[0]*shade, baseColour[1]*shade, baseColour[2]*shade, baseColour[3]);
        camera->placePoint(posBody, points[3]);
        camera->placePoint(posBody, points[0]);
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

        //set all unconnected points to the darker colour
        glColor4f(underCol[0]*shade, underCol[1]*shade, underCol[2]*shade, underCol[3]);
        camera->placePoint(posBody, points[2]);
        camera->placePoint(posBody, points[3]);
    glEnd();

}





void Camera :: drawChunkShape(b2Body* body, DrawShape* drawShape){

    b2Vec2 posBody = getCamBodyPos(body);

    switch (drawShape->drawId) {
        case 0 : {drawChunkId0(this, posBody, drawShape->shapePoints); break;} //basic ground chunk shape
        case 1 : {drawChunkId1(this, posBody, drawShape->shapePoints); break;} //mountain backing chunk shape
        default : {drawDefaultChunkShape(this, posBody, drawShape);} //default shape shading
    }
}
