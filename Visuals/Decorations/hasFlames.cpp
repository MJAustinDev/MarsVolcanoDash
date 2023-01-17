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

//bare constructor, no need for extra code as intended to be used as an inherited only class
HasFlames :: HasFlames(){}

HasFlames :: ~HasFlames(){
    while(flames.first != nullptr){ //must deallocate all stored flames
        flames.remFront();
    }
}

//adds a new flame to the end of the linked list
void HasFlames :: addFlame(b2Vec2* points){
    flames.addEnd(new Flame(points));
}

//draws each flame stored in the object
void HasFlames :: draw(b2Vec2 pos, Camera* camera){
    if(flames.resetCycle()){
        do {
            b2Vec2* points = &(flames.cycle->obj->points[0]); //access the flame points array
            float c1[4] = COLOUR_LAVA_0;
            float c2[4] = COLOUR_LAVA_4;
            float c3[4] = COLOUR_LAVA_3;
            float* c = &c1[0];
            float glow = 0.9f - camera->getGlow();
            float shift = ((points[0].x < 0.0f) ? -1.0f : 1.0f) * camera->getGlow(); //how much the top point will shift from it's base position
            glBegin(GL_POLYGON); //start of triangle
                for(int i=0;i<3;i++){
                    switch(i){ //set colour depending on the point
                        case 0 : {c = &c1[0]; break;}
                        case 1 : {c = &c2[0]; break;}
                        case 2 : {c = &c3[0]; break;}
                    }
                    glColor4f((c[0])*glow, (c[1])*glow, (c[2])*glow, 0.5f*glow);
                    b2Vec2 flamePos = points[i];
                    flamePos.x += (i == 0) ? shift : 0.0f; //shift point if it's the top point
                    camera->placePoint(camera->getCamPos(pos), flamePos); //use camera to place the opengl point
                }
            glEnd(); //end of triangle
        } while (flames.cycleUp());
    }
}
