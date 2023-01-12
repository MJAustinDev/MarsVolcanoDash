#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

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

#include "camera.h"

//attempt to load a transparent png as a texture
unsigned int loadTexture(const char* path){

    int w, h, chan; //image width, height, and channels
    unsigned char* data = stbi_load(path, &w, &h, &chan, STBI_rgb_alpha);
    if (data){
        unsigned int tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        stbi_image_free(data);
        return tex;
    } else {
        return (unsigned int)-1;
    }
}

//load all the required textures
void Camera :: loadTextures(){

    stbi_set_flip_vertically_on_load(true);

    //"./Visuals/TextTexture/"
    texEasy = loadTexture("./Visuals/Textures/easy.png");
    texNormal = loadTexture("./Visuals/Textures/normal.png");
    texHard = loadTexture("./Visuals/Textures/hard.png");
    texPlayer = loadTexture("./Visuals/Textures/player.png");
    texExit = loadTexture("./Visuals/Textures/exit.png");
    texResume = loadTexture("./Visuals/Textures/resume.png");
    texReturn = loadTexture("./Visuals/Textures/return.png");
    texScore = loadTexture("./Visuals/Textures/score.png");
    texWins = loadTexture("./Visuals/Textures/wins.png");
    texDust = loadTexture("./Visuals/Textures/dust.png");

    //numbers
    char numPath[] = "./Visuals/Textures/numX.png";
    for (int i=0;i<10;i++){
        numPath[22] = (char)(48+i); //replace X with valid ASCII for corresponding number
        texNum[i] = loadTexture(numPath);
    }

}

//free memory space properly when textures aren't needed
void Camera :: unloadTextures(){
    for (int i=0;i<19;i++){
        unsigned int tex = getTexture(i); //called at the end of program so setting the object's unsigned int to 0 is irrelevant
        if (getTexture(i)!=(unsigned int)-1){
            glDeleteTextures(1, &tex);
        }
    }

}

//returns the loaded in number value of a texture from its identifier number
unsigned int Camera :: getTexture(int id){
    switch (id) {
        case 0 :
        case 1 :
        case 2 :
        case 3 :
        case 4 :
        case 5 :
        case 6 :
        case 7 :
        case 8 :
        case 9 : {return texNum[id];}
        case 10 : {return texEasy;}
        case 11 : {return texNormal;}
        case 12 : {return texHard;}
        case 13 : {return texPlayer;}
        case 14 : {return texExit;}
        case 15 : {return texResume;}
        case 16 : {return texReturn;}
        case 17 : {return texScore;}
        case 18 : {return texWins;}
        case 19 : {return texDust;}

        default : {return (unsigned int) -1;} //invalid identifier return texture doesn't exist
    }
}

//set window's icon
void Camera :: setWindowIcon(GLFWwindow* window){

    stbi_set_flip_vertically_on_load(false);

    GLFWimage icon[1];
    int chan;
    icon[0].pixels = stbi_load("./Visuals/Icons/MarsVolcanoDashIcon.png", &icon[0].width, &icon[0].height, &chan, STBI_rgb_alpha);
    glfwSetWindowIcon(window, 1, icon);
    stbi_image_free(icon[0].pixels);
}
