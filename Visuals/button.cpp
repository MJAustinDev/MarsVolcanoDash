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

#include "manager.h"

Button :: Button(int id){
    buttonID = id;
}

Button :: ~Button(){
    while (textures.first!=nullptr){
        textures.remFront();
    }
}

//sets local highlight and backing panel based off parameters
void Button :: setCoords(float high[4], float bck[4]){
    for (int i=0;i<4;i++){
        highlight[i] = high[i];
        backing[i] = bck[i];
    }
}

//sets texture's opengl id, world draw position and texture sampling position
void Button :: setTexture(MenuTexture* ptr, unsigned int id, float* wCoords, float* tCoords){
    ptr->textureID = id;
    for (int i=0;i<4;i++){
        ptr->worldCoords[i] = wCoords[i];
        ptr->textCoords[i] = tCoords[i];
    }
}

//draws button to the screen including textures
void Button :: draw(Camera* camera, bool selected){
    //set button's draw colours
    float colour[4] = COLOUR_PURPLE;
    float backColour[4] = COLOUR_PURPLE_BUTTON;
    if (!selected) {
        for (int i=0;i<3;i++){
            colour[i] *= 0.5f; //dull main colour if not selected
        }
    }

    //draw highlight then backing panel over it
    camera->drawPureRect(colour, highlight);
    camera->drawPureRect(backColour, backing);
    //draw every stored texture onto the button
    if (textures.resetCycle()){
        do {
            MenuTexture* texture = textures.cycle->obj;
            camera->drawPureRectText(colour, texture->textureID, texture->worldCoords, texture->textCoords);
            texture = nullptr;
        } while (textures.cycleUp());
    }
}
