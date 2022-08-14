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

//Contains information needed to draw textured text onto menu buttons
struct MenuTexture {

    unsigned int textureID;
    //coordinates follow pattern of top (+y), left (-x), bottom (-y), right (+x)
    float worldCoords[4];
    float textCoords[4];

};

//Contains values for defining menu buttons and textures
struct ButtonDefaults {

    //glowing border around the buttons
    float highlights[9][4] = {
        {0.9f, -0.3f, 0.7f, 0.3f}, //easy
        {0.5f, -0.3f, 0.3f, 0.3f}, //normal
        {0.1f, -0.3f, -0.1f, 0.3f}, //hard
        {-0.3f, -0.3f, -0.5f, 0.3f}, //two player
        {-0.7f, -0.3f, -0.9f, 0.3f}, //exit
        {0.5f, -0.5f, 0.3f, 0.5f}, //resume
        {0.1f, -0.5f, -0.1f, 0.5f}, //return
        {0.5f, -0.5f, 0.3f, 0.5f}, //score
        {0.5f, -0.5f, 0.3f, 0.5f} //wins
    };

    //main backing panel of the buttons
    float backings[9][4] = {
        {0.88f, -0.28f, 0.72f, 0.28f}, //easy
        {0.48f, -0.28f, 0.32f, 0.28f}, //normal
        {0.08f, -0.28f, -0.08f, 0.28f}, //hard
        {-0.32f, -0.28f, -0.48f, 0.28f}, //two player
        {-0.72f, -0.28f, -0.88f, 0.28f}, //exit
        {0.48f, -0.48f, 0.32f, 0.48f}, //resume
        {0.08f, -0.48f, -0.08f, 0.48f}, //return
        {0.48f, -0.48f, 0.32f, 0.48f}, //score
        {0.48f, -0.48f, 0.32f, 0.48f} //wins
    };

    //positioning of button's main texture
    float textWorld[10][4] = {
        {0.88f, -0.28f, 0.72f, 0.28f}, //easy
        {0.48f, -0.28f, 0.32f, 0.28f}, //normal
        {0.08f, -0.28f, -0.08f, 0.28f}, //hard
        {-0.32f, -0.28f, -0.48f, 0.28f}, //two player
        {-0.72f, -0.28f, -0.88f, 0.28f}, //exit
        {0.48f, -0.28f, 0.32f, 0.28f}, //resume
        {0.08f, -0.28f, -0.08f, 0.28f}, //return
        {0.48f, -0.48f, 0.32f, 0.08f}, //score
        {0.48f, -0.02f, 0.32f, 0.30f}, //wins
        {0.48f, -0.48f, 0.32f, -0.1f} //'player' on player X wins
    };

    //beginning point for score numbers to exist at
    float scoreStart[4] = {0.48f, 0.38f, 0.32f, 0.48f};
    float scoreSpace = 0.08f; //knock by the next digit's position by

    float onlyPlayer[4] = {1, 0.41, 0, 1}; //texture coordinates to get only 'Player' from 'Two Player'
    float textWorldNum[4] = {0.48f, -0.12f, 0.32f, 0.0f}; //number of winning players texture to world coordinates


};


//Contains information to draw world chunk shapes
struct DrawShape {

    b2Vec2 shapePoints[8];
    int pointCount = 0;
    int drawId;

};

//Contains information to draw player car details
struct CarDetails {

    //coordinate points of car details relative to car body
    b2Vec2 points[19][4] = {
        //window
        {b2Vec2(-0.1f,1.1f), b2Vec2(1.4f,1.1f), b2Vec2(1.05f,1.84f), b2Vec2(-0.1f,1.85f)}, //bottom left, bottom right, top right, top left

        //front light
        {b2Vec2(2.4,1.0), b2Vec2(2.6,0.7), b2Vec2(2.98,0.55), b2Vec2(2.96,0.9)}, //top left, bottom left, bottom right, top right

        //back light
        {b2Vec2(-2.92,1.0), b2Vec2(-2.97,0.5), b2Vec2(-2.75,0.7), b2Vec2(-2.70,1.0)}, //top left, bottom left, bottom right, top right

        //door handle
        {b2Vec2(0.0,0.9),b2Vec2(0.0,0.8),b2Vec2(0.4,0.8),b2Vec2(0.5,0.9)}, //top left, bottom left, bottom right, top right

        //door line -- top ridge, mid ridge, bottom ridge
        {b2Vec2(1.50f,0.9f), b2Vec2(1.45f,1.0f), b2Vec2(1.25,0.65) , b2Vec2(1.30,0.55)}, //forward right, forward left, backward left, backward right
        {b2Vec2(1.25,0.65) , b2Vec2(1.30,0.55), b2Vec2(0.92,0.50), b2Vec2(0.85,0.58)}, //forward left, forward right, backward right, backward left
        {b2Vec2(0.92,0.50), b2Vec2(0.85,0.58), b2Vec2(0.67,0.0), b2Vec2(0.75,0.0)}, //forward right, forward left, backward left, backward right

        //door back line
        {b2Vec2(-0.3,1.85), b2Vec2(-0.375,1.85), b2Vec2(-0.375,0), b2Vec2(-0.3,0)}, //top right, top left, bottom left, bottom right

        //front wheel guard -- back guard, mid guard, front guard, front ridge
        {b2Vec2(2.15,0.60), b2Vec2(2.2,0.7), b2Vec2(1.5,0.7), b2Vec2(1.5,0.60)}, //bottom right, top right, top left, bottom left
        {b2Vec2(2.7,0.4), b2Vec2(2.65,0.33), b2Vec2(2.15,0.60), b2Vec2(2.2,0.7)}, //top right, bottom right, bottom left, top left
        {b2Vec2(2.85,0.2), b2Vec2(2.8,0.1), b2Vec2(2.65,0.33), b2Vec2(2.7,0.4)}, //top right, bottom right, bottom left, top left
        {b2Vec2(3.05f,0.1), b2Vec2(3.05f,0.2), b2Vec2(2.85,0.2), b2Vec2(2.8,0.1)}, //bottom right, top right, top left, bottom left

        //rear wheel guard -- front guard, mid guard, back guard, back ridge
        {b2Vec2(-2.15,0.60), b2Vec2(-2.2,0.7), b2Vec2(-0.5,0.7), b2Vec2(-0.5,0.60)}, //bottom left, top left, top right, bottom right
        {b2Vec2(-2.7,0.4), b2Vec2(-2.65,0.33), b2Vec2(-2.15,0.60), b2Vec2(-2.2,0.7)}, //top left, bottom left, bottom right, top right
        {b2Vec2(-2.85,0.2), b2Vec2(-2.8,0.1), b2Vec2(-2.65,0.33), b2Vec2(-2.7,0.4)}, //top left, bottom left, bottom right, top right
        {b2Vec2(-3.05f,0.1), b2Vec2(-3.05f,0.2), b2Vec2(-2.85,0.2), b2Vec2(-2.8,0.1)}, //bottom left, top left, top right, bottom right

        //bumpers -- rear bumper, mid bumper, front bumper
        {b2Vec2(-3.1f,-0.15f), b2Vec2(-3.15f,-0.4f), b2Vec2(-2.9,-0.4), b2Vec2(-2.8,-0.15)}, //top left, bottom left, bottom right, top right
        {b2Vec2(-0.8f,-0.15f), b2Vec2(-0.7f,-0.4f), b2Vec2(0.7,-0.4), b2Vec2(0.8,-0.15)}, //top left, bottom left, bottom right, top right
        {b2Vec2(3.1f,-0.15f), b2Vec2(3.15f,-0.4f), b2Vec2(2.9,-0.4), b2Vec2(2.8,-0.15)} //top right, bottom right, bottom left, top left

    };

    //shade value of car details at corresponding coordinate points
    float colour [19][4] = {
        //window
        {0.65f,0.85f,1.0f,0.85f}, //bottom left, bottom right, top right, top left

        //front light
        {0.9f, 0.9f, 1.0f, 1.0f}, //top left, bottom left, bottom right, top right

        //back light
        {1.0f, 1.0f, 0.9f, 0.9f}, //top left, bottom left, bottom right, top right

        //door handle
        {0.9f, 0.85f, 0.8f, 1.0f}, //top left, bottom left, bottom right, top right

        //door line -- top ridge, mid ridge, bottom ridge
        {0.88f, 1.0f, 1.0f, 0.58f}, //forward right, forward left, backward left, backward right
        {1.0f, 0.58f, 0.58f, 1.0f}, //forward left, forward right, backward right, backward left
        {0.58f, 1.0f, 1.0f, 0.58f}, //forward right, forward left, backward left, backward right

        //door back line
        {1.0f, 0.85f, 0.65f, 0.75f}, //top right, top left, bottom left, bottom right

        //front wheel guard -- back guard, mid guard, front guard, front ridge
        {0.58f, 1.0f, 1.0f, 0.68f}, //bottom right, top right, top left, bottom left
        {1.0f, 0.58f, 0.58f, 1.0f}, //top right, bottom right, bottom left, top left
        {1.0f, 0.58f, 0.58f, 1.0f}, //top right, bottom right, bottom left, top left
        {0.78f, 1.0f, 1.0f, 0.58f}, //bottom right, top right, top left, bottom left

        //rear wheel guard -- front guard, mid guard, back guard, back ridge
        {0.58f, 1.0f, 1.0f, 0.68f}, //bottom left, top left, top right, bottom right
        {1.0f, 0.58f, 0.58f, 1.0f}, //top left, bottom left, bottom right, top right
        {1.0f, 0.58f, 0.58f, 1.0f}, //top left, bottom left, bottom right, top right
        {0.78f, 1.0f, 1.0f, 0.58f}, //bottom left, top left, top right, bottom right

        //bumpers -- rear bumper, mid bumper, front bumper
        {1.0f,1.0f,0.52f,1.0f}, //top left, bottom left, bottom right, top right
        {1.0f,0.52f,0.52f,1.0f}, //top left, bottom left, bottom right, top right
        {1.0f,1.0f,0.52f,1.0f} //top right, bottom right, bottom left, top left
    };

};

#endif
