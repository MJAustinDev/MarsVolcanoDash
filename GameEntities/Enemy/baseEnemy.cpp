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

//base constructor
BaseEnemy :: BaseEnemy(int ticks, b2World* w, b2Vec2 pos, b2Vec2 linVel,float angle){
    ttl = ticks;
    world = w;
    b2BodyDef defBody;
    defBody.type = b2_dynamicBody;
    defBody.position.Set(pos.x,pos.y);
    defBody.linearVelocity.Set(linVel.x,linVel.y);
    defBody.angle = angle;
    body = world->CreateBody(&defBody);
};

//base destructor
BaseEnemy :: ~BaseEnemy(){
    world->DestroyBody(body);
}

//all enemies lower their ticks to live each cycle
void BaseEnemy :: process(){
    ttl--;
    if(ttl<=0){
        terminate=true;
    }
};


//breakable enemy constructor
BaseBreakableEnemy :: BaseBreakableEnemy(int ticks, b2World* w, b2Vec2 pos, int numSen) : BaseEnemy(ticks,w,pos){
    numSensors = numSen;
    sensors = new b2Fixture*[numSensors];
};

//breakable enemy destructor
BaseBreakableEnemy :: ~BaseBreakableEnemy(){
    delete[] sensors;
};

//performs base operation, then processes potential collisions
void BaseBreakableEnemy :: process(){
    BaseEnemy::process();
    if (!terminate){
        //iterates over all contacts attached to the main body, see: https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_dynamics.html#autotoc_md106
        for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next){
            b2Contact* c = ce->contact;
            //for each sensor, check if this contact contains it
            for (int i=0;i<numSensors;i++){
                if (sensors[i]!=nullptr){
                    if (c->GetFixtureA()==sensors[i] || c->GetFixtureB()==sensors[i]){
                        collided = true;
                        break;
                    }
                }
            }
            if (collided){
                break; //no point carrying on scanning
            }
        }
    }
};




