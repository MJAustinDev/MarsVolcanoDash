
#include "gameEntities.h"


Fireball :: Fireball(b2World* w, b2Vec2 pos, b2Vec2 linVel, float s, b2Vec2 force) : BaseEnemy(1500,w,pos,linVel){

    shape.m_p.Set(0,0);
    shape.m_radius = s;
    b2FixtureDef defFix;
    defFix.density = 1.5f;
    defFix.shape = &shape;
    body->CreateFixture(&defFix);

    body->ApplyLinearImpulseToCenter(force,true);

}

//TODO -- OVERHAUL GRAPHICS
void Fireball :: draw(Camera* camera){

    GLfloat col[3] = {1,1,1};
    camera->drawCircle(body,&shape,col,36);

}

