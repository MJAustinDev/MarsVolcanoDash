/*

MIT License

Copyright (c) 2022-2023 Matthew James Austin

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

/*
Meteors break into smaller "fireball" circles that are fired off with a random force directed from the centre to the circumference
*/

Meteor :: Meteor(b2World* w, b2Vec2 pos, float sizeRand, float maxForce) : BaseBreakableEnemy(400,w,pos,1){

    //force minimum, maximum size
    if (sizeRand<1.2f){
        sizeRand = 1.2f;
    }

    sizeRand = randRanged(1.0f,sizeRand); //forces set size to 1 to parameter passed limit

    shape.m_p.Set(0,0);
    shape.m_radius = sizeRand*1.5; //sensor is 1.5 * size of meteor, so meteor will explode above targets
    b2FixtureDef defFix;
    defFix.shape = &shape;
    defFix.isSensor = true;

    sensors[0] = body->CreateFixture(&defFix);

    shape.m_radius = sizeRand;
    defFix.isSensor = false;
    defFix.density = 1.5f;
    body->CreateFixture(&defFix);

    //fire off meteor with a random force with a 180 deg range facing downwards
    float angle = randRanged(M_PI,2.0f*M_PI);
    maxForce = randRanged(0.0f,maxForce);
    b2Vec2 force(maxForce*cos(angle), maxForce*sin(angle));
    body->ApplyLinearImpulseToCenter(force,true);

}

//fire off multiple fireballs after impact
void Meteor :: afterCollide(LinkedList<Fireball>* fireballs){

    //break into 8 smaller fireballs
    float s = shape.m_radius;
    b2Vec2 pos = body->GetPosition();

    //apply random force and angle shift
    float power = randRanged(500.0f,1500.0f);
    float angRand = randRanged(0.0f,(M_PI/4.0f)); //shifts angle for variety, prevents always firing at 0, 45, 90, ... 315 degrees

    for (int i=0;i<8;i++){
        float angle = ((M_PI/4) * i) + angRand;
        b2Vec2 force(power*cos(angle), power*sin(angle));
        b2Vec2 posBall(pos.x+(s*cos(angle)), pos.y+(s*sin(angle)));
        Fireball* fireball = new Fireball(world,posBall,body->GetLinearVelocity(), s*0.34,force,colour);
        fireballs->addEnd(fireball);
        fireball = nullptr;
    }
}

//draw meteor to viewport
void Meteor :: draw(Camera* camera){
    float glow = camera->getGlow();
    camera->drawHotball(camera->getCamBodyPos(body), shape.m_radius, colour, 1.0f - glow, 0.5f - (2.0f*glow));
}
