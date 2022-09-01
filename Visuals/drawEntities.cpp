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


//TODO MOVE ELSE WHERE
void Camera :: drawLava(float lavaX, float playerY, b2Vec2* baseShape, b2Vec2* edgeShape, b2Vec2* innerShape){

    b2Vec2 posBody = getCamPos(b2Vec2(lavaX, playerY));
    float shade = 0.95f-glow;
    float alphaEdge = 0.11f-glow;
    float alphaInner = glow*0.125;
    float dispPos = (10.0f*glow);
    float dispNeg = dispPos*-1;
    float* dispX = &dispPos;
    float* dispY = &dispPos;

    //colours are scoped locally so can multiply with shade just once
    float baseColour[3] = {0.96f*shade, 0.41f*shade, 0.02f*shade};
    //internal point colours
    float c1[3] = {0.97f*shade, 0.2f*shade, 0.17f*shade};
    float c2[3] = {0.95f*shade, 0.95f*shade, 0.09f*shade};
    float c3[3] = {0.89f*shade, 0.37f*shade, 0.13f*shade};
    float c4[3] = {1.0f*shade, 0.4f*shade, 0.93f*shade};
    float* c;

    //draw base shape
    glBegin(GL_POLYGON);
    glColor4f(baseColour[0], baseColour[1], baseColour[2],0.90f);
    for (int i=0;i<4;i++){
        placePoint(posBody, baseShape[i]);
    }
    glEnd();

    //draw inner shading
    for (int k=0;k<4;k++){ //for each outer point
        glBegin(GL_POLYGON);
        glColor4f(baseColour[0], baseColour[1], baseColour[2],0.25f);
        placePoint(posBody, baseShape[k]);
        for (int j=0;j<4;j++){ //for each pair of edge points
            //one edge
            glColor4f(baseColour[0], baseColour[1], baseColour[2], alphaEdge);
            placePoint(posBody, edgeShape[j+(k*8)]);
            for (int i=0;i<8;i++){ //for 8 inner points
                switch (i%4){ //displacement will in part carry from the previous, so just set the changed axis
                    case 0 : {c = c1; dispX = &dispPos; break;} //displacement X +, displacement Y +
                    case 1 : {c = c2; dispY = &dispNeg; break;} //displacement X +, displacement Y -
                    case 2 : {c = c3; dispX = &dispNeg; break;} //displacement X -, displacement Y -
                    case 3 : {c = c4; dispY = &dispPos; break;} //displacement X -, displacement Y +
                }
                glColor4f(c[0], c[1], c[2], alphaInner);
                b2Vec2 movePoint = innerShape[i+(j*8)]; //split into 4 chunks each 8 wide
                movePoint.x+=(*dispX);
                movePoint.y+=(*dispY);
                placePoint(posBody, movePoint);
            }
            //another edge
            glColor4f(baseColour[0], baseColour[1], baseColour[2], alphaEdge);
            placePoint(posBody, edgeShape[j+4]);
        }
        glEnd();
    }
}
