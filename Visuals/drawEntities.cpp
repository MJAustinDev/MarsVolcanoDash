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


//draw cars main body
void Camera :: drawCarBody(b2Body* body, b2PolygonShape* mainShape, b2PolygonShape* roofShape, GLfloat* colour){

    b2Vec2 posBody = getCamBodyPos(body);
    float angle = body->GetAngle();

    //draw base body
    glColor4f(colour[0]*0.5,colour[1]*0.5,colour[2]*0.5,1.0f);
    drawB2PolygonShape(posBody, mainShape, angle);
    drawB2PolygonShape(posBody, roofShape, angle);

    //draw car details
    for (int j=0;j<19;j++){
        glBegin(GL_POLYGON);
        for (int i=0;i<4;i++){
            float shade = (carDetail.colour[j][i]-glow); //workout glow-shade of point
            glColor4f(colour[0]*shade,colour[1]*shade,colour[2]*shade,1.0f);
            placeRotatePoint(posBody,carDetail.points[j][i],angle);
        }
        glEnd();
    }

}

//draws a cars wheel including hub and spoke piece which fades in towards the centre
void Camera :: drawWheel(b2Body* body, b2CircleShape* shape, float* colour, float baseAngle) {
    int res = 36; //resolution, how circle-y the circle should look
    b2Vec2 posBody = getCamBodyPos(body);
    float radius = shape->m_radius;
    float shade = 1.0f - glow;
    float colShade[3] = {colour[0]*shade, colour[1]*shade, colour[2]* shade}; //save re-calulating it each time the colour is reset

    //outer tyre
    glBegin(GL_POLYGON);
    glColor4f(colShade[0], colShade[1], colShade[2], 1.0f);
    placeCirclePoints(res, 0, res, posBody, radius, baseAngle);
    glEnd();

    radius -= 0.2;
    //inner ring
    glBegin(GL_POLYGON);
    glColor4f(colour[0]*0.1, colour[1]*0.1, colour[2]*0.1, 1.0f);
    placeCirclePoints(res, 0, res, posBody, radius, baseAngle);
    glEnd();

    //right hub section
    b2Vec2 pos(0.0f,-0.2f);
    glBegin(GL_POLYGON);
    glColor4f(colShade[0], colShade[1], colShade[2], 0.5f); //set to hub colour
    placeRotatePoint(posBody,pos,baseAngle); //draw bottom hub point
    glColor4f(colShade[0], colShade[1], colShade[2], 1.0f); //set to rim colour
    placeCirclePoints(res,-4,5,posBody,radius,baseAngle); //draw rim points
    glColor4f(colShade[0], colShade[1], colShade[2], 0.5f); //set to hub colour
    pos.y *= -1; //flip y to top hub point
    placeRotatePoint(posBody,pos,baseAngle); //draw top hub point
    glEnd();

    //left hub section
    glBegin(GL_POLYGON);
    placeRotatePoint(posBody,pos,baseAngle); //draw top hub point
    glColor4f(colShade[0], colShade[1], colShade[2], 1.0f); //set to rim colour
    placeCirclePoints(res,14,23,posBody,radius,baseAngle); //draw rim points
    pos.y *= -1; //flip y to bottom hub point
    glColor4f(colShade[0], colShade[1], colShade[2], 0.5f); //set to hub colour
    placeRotatePoint(posBody,pos,baseAngle); //draw bottom hub point
    glEnd();

}


void Camera :: drawFireball(b2Body* body, b2CircleShape* shape, float* colour){
    drawHotball(getCamBodyPos(body), shape->m_radius, colour,(2.0f*glow), 1.0f - glow);
}

void Camera :: drawMeteor(b2Body* body, b2CircleShape* shape, float* colour){
    drawHotball(getCamBodyPos(body), shape->m_radius, colour, 1.0f - glow, 0.5f - (2.0f*glow));
}


//used in draw boulder and draw fragment to access the non-centre points from b2PolygonShape
void forceEdgePoints(b2Vec2* points, b2PolygonShape* shape){
    if (points[0].x == 0.0f && points[0].x == 0.0f){
        points[0] = shape->m_vertices[2];
    } else if (points[1].x == 0.0f && points[1].x == 0.0f){
        points[1] = shape->m_vertices[2];
    }
}

void Camera :: drawBoulder(b2Body* body, b2PolygonShape** shapes, float* colour){
    b2Vec2 posBody = getCamBodyPos(body);
    float angle = body->GetAngle();
    float shade = 1.0f - (2.0f*glow);

    for (int i=0;i<8;i++){
        if (shapes[i]!=nullptr){
            b2Vec2 points[2] = {shapes[i]->m_vertices[0], shapes[i]->m_vertices[1]};
            forceEdgePoints(points, shapes[i]);
            drawHotFrag(posBody, points, angle, colour, shade, 0.4f); //keep outside looking cold
        }
    }
}

void Camera :: drawFragment(b2Body* body, b2PolygonShape* shape, float* colour){
    b2Vec2 posBody = getCamBodyPos(body);
    float angle = body->GetAngle();
    b2Vec2 points[2] = {shape->m_vertices[0], shape->m_vertices[1]};
    forceEdgePoints(points, shape);
    drawHotFrag(posBody, points, angle, colour, 0.4f, 1.0f - (4.0f*glow)); //looks like heat has pushed to outside on impact
}

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
