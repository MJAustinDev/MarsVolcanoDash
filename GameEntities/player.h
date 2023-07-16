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

#pragma once

//player's car class
class Player {

public:

    Player(b2World* world, float* col);
    ~Player();

    b2Vec2 getPosition(){return mainBody->GetPosition();};

    void processInput(bool keyW, bool keyS, bool keyA, bool keyD);
    void draw(Camera* camera);

private:

    float colour[4];
    float wheelDrawAng = 0.0f; //angle to draw wheels at (prevent rapid flickering when using real angle)
    CarDetails detail; //used to draw details on top of the car body

    b2Body* mainBody;
    b2Body* wheelBack;
    b2Body* wheelFront;

    b2PolygonShape mainShape;
    b2PolygonShape roofShape;
    b2CircleShape wheelShape;

    b2WheelJoint* motorBack;
    b2WheelJoint* motorFront;

};