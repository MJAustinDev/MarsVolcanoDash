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

#include "decoration.h"

//constructor for the tunnel decoration
DecTunnel :: DecTunnel(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour) : Decoration(pos, ptrColour) {

    b2Vec2 points[8];
    points[0].Set(-6.0f, 5.5f); //main tunnel shape
    points[1].Set(6.0f, 5.5f);
    points[2].Set(6.0f, 1.5f);
    points[3].Set(-6.0f, 1.5f);
    addShape(points, 4);

    points[0].Set(-5.25f, 4.75f); //tunnel window
    points[1].Set(5.25f, 4.75f);
    points[2].Set(5.25f, 4.25f);
    points[3].Set(-5.25f, 4.25f);
    addDetail(points, 4);

    for (int i=0; i<=1; i++){ //edge ridges
        float shift = ((float) i) * 12.25f;
        points[0].Set(-6.25f + shift, 5.75f);
        points[1].Set(-6.0f + shift, 5.75f);
        points[2].Set(-6.0f + shift, 1.25f);
        points[3].Set(-6.25f + shift, 1.25f);
        addDetail(points, 4);
    }

    switch(id){
        case DEC_CODE_TUNNEL_2 : {setTunnelToDome(1.0f); break;} //connector on right side
        case DEC_CODE_TUNNEL_3 : {setTunnelToDome(-1.0f); break;} //connector on left side
        default : {} //no extra details by default
    }

    if(hasBase){
        for (int i=-1; i<=1; i+=2){ //set connectors from tunnel bottom to the base
            float mirror = (float) i;
            points[0].Set(6.0f * mirror, 1.5f);
            points[1].Set(5.0f * mirror, 1.5f);
            points[2].Set(5.25f * mirror, 0.0f);
            points[3].Set(6.0f * mirror, 0.0f);
            addShape(points, 4);
        }
        addBase(baseLevel, b2Vec2(-6.0f, -5.25f)); //add base on left
        addBase(baseLevel, b2Vec2(5.25f, 6.0f)); //add base on right
    }

}

DecTunnel :: ~DecTunnel(){} //no extra dynamic memory to deallocate

//adds a connector shape that fits between the tunnel and a dome
void DecTunnel :: setTunnelToDome(float mirror){
    b2Vec2 points[8];
    points[0].Set(6.0f * mirror, 5.5f); //quad between tunnel and dome
    points[1].Set(6.0f * mirror, 1.5f);
    points[2].Set(6.4f * mirror, 1.25f);
    points[3].Set(6.75f * mirror, 4.25f);
    addShape(points, 4);

    points[0].Set(6.0f * mirror, 5.5f); //final triangle on top of the connector quad
    points[1].Set(6.75f * mirror, 4.25f);
    points[2].Set(7.85f * mirror, 5.75f);
    addShape(points, 3);
}
