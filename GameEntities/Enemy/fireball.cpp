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


Fireball :: Fireball(b2World* w, b2Vec2 pos, b2Vec2 linVel, float s, b2Vec2 force, float* col) : BaseEnemy(1500,w,pos,linVel){

    shape.m_p.Set(0,0);
    shape.m_radius = s;
    b2FixtureDef defFix;
    defFix.density = 1.5f;
    defFix.shape = &shape;
    body->CreateFixture(&defFix);
    body->ApplyLinearImpulseToCenter(force,true);
    for (int i=0;i<3;i++){
        colour[i] = col[i]; //set colour from parameter passed array, i.e., same colour as the meteor it spawned off
    }

}

void Fireball :: draw(Camera* camera){
    float glow = camera->getGlow();
    camera->drawHotball(camera->getCamBodyPos(body), shape.m_radius, colour,(2.0f*glow), 1.0f - glow);
}
