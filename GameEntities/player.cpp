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


Player :: Player(b2World* world, float* col){

    //default spawn point
    float x = 0;
    float y = 20;
    //set base colour
    for (int i=0;i<3;i++){
        colour[i] = col[i];
    }

    //define car body shapes
    b2Vec2 points[5];
    points[0].Set(-3.0f,-0.2f);
    points[1].Set(3.0f,-0.2f);
    points[2].Set(2.9f,1.0f);
    points[3].Set(2.4f,1.1f);
    points[4].Set(-2.9f,1.1f);
    mainShape.Set(points,5); //chassis

    points[0].Set(-0.8f,1.1f);
    points[1].Set(1.5f,1.1f);
    points[2].Set(1.1f,1.94f);
    points[3].Set(-0.6f,1.95f);
    roofShape.Set(points,4); //roof

    //define wheel shape
    wheelShape.m_p.Set(0.0f,0.0f);
    wheelShape.m_radius = 1.0f;

    //define car body
    b2BodyDef defBody;
    defBody.type = b2_dynamicBody;
    defBody.position.Set(x, y);
    mainBody = world->CreateBody(&defBody);

    b2FixtureDef defFix;
    defFix.filter.categoryBits = 0x1000; //prevent players from colliding with each other
    defFix.filter.maskBits = 0x0FFF; //prevent players from colliding with each other

    defFix.shape = &mainShape;
    defFix.density = 1.0f;
    defFix.friction = 0.3f;
    mainBody->CreateFixture(&defFix);
    defFix.shape = &roofShape;
    mainBody->CreateFixture(&defFix);

    //define wheels
    defBody.position.Set(-1.8f+x,-0.65f+y);
    wheelBack = world->CreateBody(&defBody);

    defFix.shape = &wheelShape;
    defFix.friction = 5.0f;
    defFix.density = 1.1f;
    wheelBack->CreateFixture(&defFix);

    defBody.position.Set(1.8f+x,-0.65f+y);
    wheelFront = world->CreateBody(&defBody);
    wheelFront->CreateFixture(&defFix);

    //define wheel joints
    b2Vec2 axis(0.0f,1.0f); //axis of wheel suspention
    b2WheelJointDef defJoint;
    defJoint.Initialize(mainBody,wheelBack,wheelBack->GetPosition(),axis);
    defJoint.collideConnected = false;
    defJoint.enableMotor = true;
    defJoint.enableLimit = true;
    defJoint.lowerTranslation = -0.4;
    defJoint.upperTranslation = 0.2f;
    defJoint.maxMotorTorque = 25;
    motorBack = (b2WheelJoint*) world->CreateJoint(&defJoint);

    defJoint.Initialize(mainBody,wheelFront,wheelFront->GetPosition(),axis);
    defJoint.collideConnected = false;
    defJoint.enableMotor = true;
    defJoint.enableLimit = true;
    defJoint.lowerTranslation = -0.4;
    defJoint.upperTranslation = 0.2f;
    defJoint.maxMotorTorque = 50;
    motorFront = (b2WheelJoint*) world->CreateJoint(&defJoint);

}

Player :: ~Player(){
    //no dynamically stored objects as of yet
}

//draw car's body then wheels
void Player :: draw(Camera* camera){

    //draw main car body
    camera->drawCarBody(mainBody,&mainShape,&roofShape,colour);

    //draw both wheels over main body
    camera->drawWheel(wheelFront,&wheelShape,colour,wheelDrawAng);
    camera->drawWheel(wheelBack,&wheelShape,colour,wheelDrawAng);

}


//process user input
void Player :: processInput(bool keyW, bool keyS, bool keyA, bool keyD){

    //A is pressed -- tilt car body ccw
    if (keyA){
        mainBody->ApplyAngularImpulse(3,true);
    }

    //D is pressed -- tilt car body cw
    if (keyD){
        mainBody->ApplyAngularImpulse(-3,true);
    }

    //W is pressed -- accelerate wheels for rightwards movement
    if (keyW){
        motorFront->SetMotorSpeed(-100.0f);
        motorBack->SetMotorSpeed(-100.0f);
        wheelDrawAng-=0.15;
    }

    //S is pressed -- accelerate wheels for leftwards movement
    if (keyS){
        motorFront->SetMotorSpeed(100.0f);
        motorBack->SetMotorSpeed(100.0f);
        wheelDrawAng+=0.15;
    }

    //neither W or S are pressed -- stop wheel motors
    if (!keyS && !keyW) {
        motorFront->SetMotorSpeed(0.0f);
        motorBack->SetMotorSpeed(0.0f);
    }

}


