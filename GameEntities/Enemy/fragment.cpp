
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



















