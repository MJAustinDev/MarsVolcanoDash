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


Fragment :: Fragment(b2World* w, b2Vec2 pos, b2Vec2 linVel, float angle, b2PolygonShape* shp) : BaseEnemy(1500,w,pos,linVel,angle){

    b2FixtureDef defFix;
    defFix.density = 1.0f;
    shape.Set(shp->m_vertices,3);
    defFix.friction = 1.0f;
    defFix.shape = &shape;
    body ->CreateFixture(&defFix);
}

//TODO -- OVERHAUL GRAPHICS
void Fragment :: draw(Camera* camera){

    GLfloat col[3] = {0.7,0,0.1};
    camera->drawB2Polygon(body,&shape,col);

}



















