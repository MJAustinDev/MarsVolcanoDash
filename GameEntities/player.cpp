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

//draw car's body then wheels
void Player :: draw(Camera* camera){

    float glow = camera->getGlow();

    //draw main car body
    b2Vec2 posBody = camera->getCamBodyPos(mainBody);
    float angle = mainBody->GetAngle();
    glColor4f(colour[0]*0.5,colour[1]*0.5,colour[2]*0.5,1.0f);
    camera->drawB2PolygonShape(posBody, &mainShape, angle);
    camera->drawB2PolygonShape(posBody, &roofShape, angle);

    //draw car details
    for (int j=0;j<19;j++){
        glBegin(GL_POLYGON);
        for (int i=0;i<4;i++){
            float shade = (detail.colour[j][i]-glow); //workout glow-shade of point
            glColor4f(colour[0]*shade,colour[1]*shade,colour[2]*shade,1.0f);
            camera->placeRotatePoint(posBody,detail.points[j][i],angle);
        }
        glEnd();
    }

    //draw both wheels over main body
    posBody = camera->getCamBodyPos(wheelFront);
    for (int w=0;w<2;w++){
        //draw wheel
        int res = 36; //resolution, how circle-y the circle should look
        float radius = wheelShape.m_radius;
        float shade = 1.0f - glow;
        float colShade[3] = {colour[0]*shade, colour[1]*shade, colour[2]* shade}; //save re-calulating it each time the colour is reset

        //outer tyre
        glBegin(GL_POLYGON);
        glColor4f(colShade[0], colShade[1], colShade[2], 1.0f);
        camera->placeCirclePoints(res, 0, res, posBody, radius, wheelDrawAng);
        glEnd();

        //inner ring
        radius -= 0.2;
        glBegin(GL_POLYGON);
        glColor4f(colour[0]*0.1, colour[1]*0.1, colour[2]*0.1, 1.0f);
        camera->placeCirclePoints(res, 0, res, posBody, radius, wheelDrawAng);
        glEnd();

        //draw wheel hub
        //right hub section
        b2Vec2 pos(0.0f,-0.2f);
        glBegin(GL_POLYGON);
        glColor4f(colShade[0], colShade[1], colShade[2], 0.5f); //set to hub colour
        camera->placeRotatePoint(posBody,pos,wheelDrawAng); //draw bottom hub point
        glColor4f(colShade[0], colShade[1], colShade[2], 1.0f); //set to rim colour
        camera->placeCirclePoints(res,-4,5,posBody,radius,wheelDrawAng); //draw rim points
        glColor4f(colShade[0], colShade[1], colShade[2], 0.5f); //set to hub colour
        pos.y *= -1; //flip y to top hub point
        camera->placeRotatePoint(posBody,pos,wheelDrawAng); //draw top hub point
        glEnd();

        //left hub section
        glBegin(GL_POLYGON);
        camera->placeRotatePoint(posBody,pos,wheelDrawAng); //draw top hub point
        glColor4f(colShade[0], colShade[1], colShade[2], 1.0f); //set to rim colour
        camera->placeCirclePoints(res,14,23,posBody,radius,wheelDrawAng); //draw rim points
        pos.y *= -1; //flip y to bottom hub point
        glColor4f(colShade[0], colShade[1], colShade[2], 0.5f); //set to hub colour
        camera->placeRotatePoint(posBody,pos,wheelDrawAng); //draw bottom hub point
        glEnd();

        posBody = camera->getCamBodyPos(wheelBack);
    }

}
