
#include "gameEntities.h"

/*
Meteors break into smaller "fireball" circles that are fired off with a random force directed from the centre to the circumference
*/

Meteor :: Meteor(b2World* w, b2Vec2 pos, float sizeRand, float maxForce) : BaseBreakableEnemy(400,w,pos,1){

    sizeRand = randRanged(1.0f,sizeRand); //forces set size to 1 + random range size

    shape.m_p.Set(0,0);
    shape.m_radius = sizeRand*1.5; //sensor is 1.5 * size of meteor, so meteor will explode above targets
    b2FixtureDef defFix;
    defFix.shape = &shape;
    defFix.isSensor = true;

    sensors[0] = body->CreateFixture(&defFix);

    shape.m_radius = sizeRand;
    defFix.isSensor = false;
    defFix.density = 5.0f;
    body->CreateFixture(&defFix);

    //fire off meteor with a random force with a 180 deg range facing downwards
    float angle = randRanged(M_PI,2.0f*M_PI);
    maxForce = randRanged(0.0f,maxForce);
    b2Vec2 force(maxForce*cos(angle),maxForce*sin(angle));
    body->ApplyLinearImpulseToCenter(force,true);

}

//TODO -- OVERHAUL GRAPHICS
void Meteor :: draw(Camera* camera){

    //temp colouring system
    GLfloat col[3] = {1,1,0};
    if (collided){
        col[2] = 1;
    }

    camera->drawCircle(body,&shape,col,36);

}



void Meteor :: afterCollide(LinkedList<Fireball>* fireballs){

    //break into 8 smaller fireballs
    float s = shape.m_radius;
    b2Vec2 pos = body->GetPosition();

    //apply random force and angle shift
    float power = randRanged(100.0f,1000.0f);
    float angRand = randRanged(0.0f,(M_PI/4.0f)); //shifts angle for variety, prevents always firing at 0, 45, 90, ... 315 degrees

    for (int i=0;i<8;i++){
        float angle = ((M_PI/4) * i) + angRand;
        b2Vec2 force(power*cos(angle),power*sin(angle));
        b2Vec2 posBall(pos.x+(s*cos(angle)),pos.y+(s*sin(angle)));
        Fireball* fireball = new Fireball(world,posBall,body->GetLinearVelocity(),s*0.34,force);
        fireballs->addEnd(fireball);
        fireball = nullptr;
    }

}
