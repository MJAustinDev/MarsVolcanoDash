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

#ifndef VISUALSTRUCTURES_H
#define VISUALSTRUCTURES_H


//Contains information to draw world chunk shapes
struct DrawShape{

    b2Vec2 shapePoints[8];
    int pointCount = 0;
    int drawId;

};

//Contains information to draw player car details
struct CarDetails {

    b2Vec2 points[19][4] = {
        //window
        {b2Vec2(-0.1f,1.1f), b2Vec2(1.4f,1.1f), b2Vec2(1.05f,1.84f), b2Vec2(-0.1f,1.85f)},

        //front light
        {b2Vec2(2.4,1.0), b2Vec2(2.6,0.7), b2Vec2(2.98,0.55), b2Vec2(2.96,0.9) },

        //back light
        {b2Vec2(-2.92,1.0), b2Vec2(-2.97,0.5), b2Vec2(-2.75,0.7), b2Vec2(-2.70,1.0)},

        //door handle
        {b2Vec2(0.0,0.9),b2Vec2(0.0,0.8),b2Vec2(0.3,0.8),b2Vec2(0.3,0.9)},

        //door line
        {b2Vec2(1.50f,0.9f), b2Vec2(1.45f,1.0f), b2Vec2(1.25,0.65) , b2Vec2(1.30,0.55)},
        {b2Vec2(1.25,0.65) , b2Vec2(1.30,0.55), b2Vec2(0.92,0.50), b2Vec2(0.85,0.58)},
        {b2Vec2(0.92,0.50), b2Vec2(0.85,0.58), b2Vec2(0.67,0.0), b2Vec2(0.75,0.0)},

        //door back line
        {b2Vec2(-0.3,1.85), b2Vec2(-0.375,1.85), b2Vec2(-0.375,0), b2Vec2(-0.3,0)},

        //front wheel guard
        {b2Vec2(2.15,0.60), b2Vec2(2.2,0.7), b2Vec2(1.5,0.7), b2Vec2(1.5,0.60)},
        {b2Vec2(2.7,0.4), b2Vec2(2.65,0.33), b2Vec2(2.15,0.60), b2Vec2(2.2,0.7)},
        {b2Vec2(2.85,0.2), b2Vec2(2.8,0.1), b2Vec2(2.65,0.33), b2Vec2(2.7,0.4)},
        {b2Vec2(3.05f,0.1), b2Vec2(3.05f,0.2), b2Vec2(2.85,0.2), b2Vec2(2.8,0.1)},

        //rear wheel guard
        {b2Vec2(-2.15,0.60), b2Vec2(-2.2,0.7), b2Vec2(-0.5,0.7), b2Vec2(-0.5,0.60)},
        {b2Vec2(-2.7,0.4), b2Vec2(-2.65,0.33), b2Vec2(-2.15,0.60), b2Vec2(-2.2,0.7)},
        {b2Vec2(-2.85,0.2), b2Vec2(-2.8,0.1), b2Vec2(-2.65,0.33), b2Vec2(-2.7,0.4)},
        {b2Vec2(-3.05f,0.1), b2Vec2(-3.05f,0.2), b2Vec2(-2.85,0.2), b2Vec2(-2.8,0.1)},

        //bumpers
        {b2Vec2(-3.1f,-0.15f), b2Vec2(-3.15f,-0.4f), b2Vec2(-2.9,-0.4), b2Vec2(-2.8,-0.15)}, //rear bumper
        {b2Vec2(-0.8f,-0.15f), b2Vec2(-0.7f,-0.4f), b2Vec2(0.7,-0.4), b2Vec2(0.8,-0.15)}, //mid bumper
        {b2Vec2(3.1f,-0.15f), b2Vec2(3.15f,-0.4f), b2Vec2(2.9,-0.4), b2Vec2(2.8,-0.15)} //front bumper

    };

    //TODO -- PICK MAXIMUM SHADE VALUES FOR ALL CAR DETAILS -- window + bumpers are complete
    float colour [19][4] = {
        //window
        {0.65f,0.85f,1.0f,0.85f},

        //front light
        {1.0f, 1.0f, 1.0f, 1.0f},

        //back light
        {1.0f, 1.0f, 1.0f, 1.0f},

        //door handle
        {1.0f, 1.0f, 1.0f, 1.0f},

        //door line
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},

        //door back line
        {1.0f, 1.0f, 1.0f, 1.0f},

        //front wheel guard
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},

        //rear wheel guard
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},

        //bumpers
        {1.0f,1.0f,0.52f,1.0f}, //rear bumper
        {1.0f,0.52f,0.52f,1.0f}, //mid bumper
        {1.0f,1.0f,0.52f,1.0f} //front bumper
    };

};

#endif
