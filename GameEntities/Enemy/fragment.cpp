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


#include "gameEntities.h"


Fragment :: Fragment(b2World* w, b2Vec2 pos, b2Vec2 linVel, float angle, b2PolygonShape* shp, float* col) : BaseEnemy(1500,w,pos,linVel,angle){

    b2FixtureDef defFix;
    defFix.density = 1.0f;
    shape.Set(shp->m_vertices,3);
    defFix.friction = 1.0f;
    defFix.shape = &shape;
    body ->CreateFixture(&defFix);
    for (int i=0;i<3;i++){
        colour[i] = col[i]; //set colour from parameter passed array, i.e., same colour as the boulder it spawned off
    }
}

//draw fragment to the viewport
void Fragment :: draw(Camera* camera){
    b2Vec2 posBody = camera->getCamBodyPos(body);
    float angle = body->GetAngle();
    b2Vec2 points[2] = {shape.m_vertices[0], shape.m_vertices[1]};
    forceEdgePoints(points, &shape);
    camera->drawHotFrag(posBody, points, angle, colour, 0.4f, 1.0f - (4.0f*camera->getGlow())); //looks like heat has pushed to outside on impact
}



















