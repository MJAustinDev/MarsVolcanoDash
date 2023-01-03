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

#include "camera.h"
#include "visualColours.h"

//handle camera zoom from user input
void Camera :: processInput(){
    //PLUS  is pressed -- zoom in
    if (*(keyPlus)){
        zoom = zoom + 0.001;
    }
    //MINUS is pressed -- zoom out
    if (*(keyMinus)){
        zoom = zoom - 0.001;
    }
    catchZoom();
}

//set camera position and zoom
void Camera :: setCamPos(float x, float y, float z){
    posX = x;
    posY = y;
    zoom = z; //dont want to catch so that it can be used on the main menu
}

//catches zoom for game play view
void Camera :: catchZoom(){
    if (zoom<0.016){
        zoom = 0.016;
        return; //no point bothering with the below as it is now false
    }
    if (zoom>0.1){
        zoom = 0.1;
    }
}

//shift glow value up/down within the range of 0.0 to 0.1
void Camera :: updateGlow(){
    glow += glowChange;
    if (glow < 0.0f){
        glowChange *= -1; //switch change direction
        glow = glowChange;
    } else if (glow > 0.1f){
        glowChange *= -1; //switch change direction
        glow = 0.1f + glowChange;
    }
}

//draws a rectangle directly to the screen without scaling (-1 to 1 range directly)
void Camera :: drawPureRect(float* colour, float coords[4]){ //coords takes form of {top, left, bottom, right}
    float shade = 1.0f-(0.75f*glow);
    glColor4f(colour[0]*shade, colour[1]*shade, colour[2]*shade, 1.0f);
    glBegin(GL_POLYGON);
        glVertex2f(coords[1], coords[0]);
        glVertex2f(coords[1], coords[2]);
        glVertex2f(coords[3], coords[2]);
        glVertex2f(coords[3], coords[0]);
    glEnd();
}

//draws a texture within a given rectangular range
void Camera :: drawPureRectText(float* colour, unsigned int textID, float wCoords[4], float tCoords[4]){ //coords takes form of {top, left, bottom, right}
    if (textID != (unsigned int) -1){ //if texture loaded successfully
        float shade = 1.0f-(0.75f*glow);
        glColor4f(colour[0]*shade, colour[1]*shade, colour[2]*shade, 1.0f);
        glEnable (GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textID);
            glBegin(GL_POLYGON);
                glTexCoord2f(tCoords[1], tCoords[0]);
                glVertex2f(wCoords[1], wCoords[0]);

                glTexCoord2f(tCoords[1], tCoords[2]);
                glVertex2f(wCoords[1], wCoords[2]);

                glTexCoord2f(tCoords[3], tCoords[2]);
                glVertex2f(wCoords[3], wCoords[2]);

                glTexCoord2f(tCoords[3], tCoords[0]);
                glVertex2f(wCoords[3], wCoords[0]);
            glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}


//crops up a lot, return b2Vec of world points position relative to camera view
b2Vec2 Camera :: getCamPos(b2Vec2 pos){
    pos.x -= posX;
    pos.y -= posY;
    pos.x *= zoom;
    pos.y *= zoom;
    return pos;
}

//crops up a lot, return b2Vec of bodys position relative to camera view
b2Vec2 Camera :: getCamBodyPos(b2Body* body){
    return getCamPos(body->GetPosition()); //position of main body on camera
}

//place a opengl point in the world view given a rotation
void Camera :: placeRotatePoint(b2Vec2 posBody, b2Vec2 pos, float angle){
    //x' = posBody.x + (((pos.x*cos(angle))+(pos.y*-sin(angle)))*zoom); //rotate, scale, then translate
    //y' = posBody.y + (((pos.y*cos(angle))+(temp*sin(angle)))*zoom);
    glVertex2f(posBody.x + (((pos.x*cos(angle)) + (pos.y*-sin(angle)))*zoom), posBody.y + (((pos.y*cos(angle)) + (pos.x*sin(angle)))*zoom));
}

//place a opengl point in the world view
void Camera :: placePoint(b2Vec2 posBody, b2Vec2 pos){
    glVertex2f(posBody.x+(pos.x*zoom),posBody.y+(pos.y*zoom)); //scale then translate
}

//registers the circular points for opengl, assumes glBegin has already been called
void Camera :: placeCirclePoints(int res, int lower, int upper, b2Vec2 posBody, float radius, float baseAngle){

    float angle;
    float zr = zoom*radius; //zoom-radius save re-calulating
    for (int i=lower;i<upper;i++){
        angle = i*((2*M_PI)/(GLfloat)res) + baseAngle;
        glVertex2f(posBody.x + (zr*cos(angle)), posBody.y + (zr*sin(angle)));
    }

}

//marks down a solid polygon, assumes colour has been set externally
void Camera :: drawB2PolygonShape(b2Vec2 posBody, b2PolygonShape* shape, float angle){

    glBegin(GL_POLYGON);
        for (int i=0;i<shape->m_count;i++){
            placeRotatePoint(posBody,shape->m_vertices[i],angle);
        }
    glEnd();

}

//draws a circle with different shaded centre and circumference
void Camera :: drawHotball(b2Vec2 posBody, float radius, float* colour, float glowCen, float glowOut){
    int res = 36; //number of connected points on the circumference, i.e., how circle-y the ball should be
    glBegin(GL_POLYGON);
        glColor4f(colour[0]*glowCen,colour[1]*glowCen,colour[2]*glowCen,1.0f);
        placePoint(posBody, b2Vec2(0.0f,0.0f));
        glColor4f(colour[0]*glowOut,colour[1]*glowOut,colour[2]*glowOut,1.0f);
        placeCirclePoints(res,0,res+1,posBody,radius,0.0f); //+1 as have to fit the same final point on circumference again
    glEnd();
}

//draws triangle with different shaded edge points and centre point(0.0, 0.0)
void Camera :: drawHotFrag(b2Vec2 posBody, b2Vec2* points, float angle, float* colour, float glowCen, float glowOut){
    glBegin(GL_POLYGON);
        glColor4f(colour[0]*glowCen,colour[1]*glowCen,colour[2]*glowCen,1.0f);
        placeRotatePoint(posBody,b2Vec2(0.0f,0.0f),angle);
        glColor4f(colour[0]*glowOut,colour[1]*glowOut,colour[2]*glowOut,1.0f);
        placeRotatePoint(posBody,points[0],angle);
        placeRotatePoint(posBody,points[1],angle);
    glEnd();
}

//draws a texture within a 4 sided b2 polygon shape
void Camera :: drawTextB2Polygone(unsigned int id, b2Vec2 tCoords[4], b2Vec2 posBody, b2PolygonShape* shape, float angle){

    //catch invalid texture or shape count
    if ((id == (unsigned int)-1) || (shape->m_count != 4)){return;}

    glEnable(GL_TEXTURE_2D); //start texture
        glBindTexture(GL_TEXTURE_2D, id);
        glBegin(GL_POLYGON);

            for (int i=0;i<shape->m_count;i++){
                glTexCoord2f(tCoords[i].x, tCoords[i].y); //set coordinate relative to texture
                placeRotatePoint(posBody,shape->m_vertices[i],angle); //place point in world
            }

        glEnd();

    glDisable(GL_TEXTURE_2D); //end texture

}

//covers the screen in a moving transparent dust storm
void Camera :: drawDust(){
    unsigned int texID = getTexture(19);
    if (19 != (unsigned int)-1){ //if texture hasn't been found don't attempt to draw
        float shade = 1.0f-(0.75f*glow);
        float colour[4] = COLOUR_BLOOD_RED;
        glColor4f(colour[0]*shade, colour[1]*shade, colour[2]*shade, shade*0.3);

        //move dust storm
        dustShift += 0.0085f;
        if (dustShift>0.5f){
            dustShift = 0.0f;
        }

        glEnable (GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texID);
            for (int i=0;i<5;i++){ //repeat for 5 times to cover the total span of 4 when dustShift is not zero
                float xShift = (i*0.5) - dustShift;
                glBegin(GL_POLYGON);
                    glTexCoord2f(0, 0); //bottom left
                    glVertex2f(-1 + xShift, -1);

                    glTexCoord2f(1, 0); //bottom right
                    glVertex2f(-0.5+xShift, -1);

                    glTexCoord2f(1, 1); //top right
                    glVertex2f(-0.5+xShift,1);

                    glTexCoord2f(0, 1); //top left
                    glVertex2f(-1+xShift, 1);
                glEnd();
            }
        glDisable(GL_TEXTURE_2D);
    }
}
