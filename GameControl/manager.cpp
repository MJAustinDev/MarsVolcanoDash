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

#include "manager.h"


Manager :: Manager(GLFWwindow* w, Camera* cam) {

    camera = cam;
    window = w;

    float xWidth = 0.3f;
    GLfloat pos[4] = {-xWidth,0.9,xWidth,0.7};
    //TODO -- REDFINE COLOUR WITH UNIQUE TEXTURE WITH TEXT
    GLfloat col[5][3] = {
    {1,0,1}, //easy's colour
    {0,1,0}, //normals's colour
    {0,0,1}, //armageddon's colour
    {1,0,0}, //two players's colour
    {1,1,1} //exits's colour
    };
    for (int i=0;i<5;i++){
        Button* but = getButton(i);
        setButton(but,i,pos,col[i]);
        pos[1] -= 0.4;
        pos[3] -= 0.4;
    }
    pos[1] = 0.3;
    pos[3] = 0.1;
    setButton(&res,5,pos,col[1]);
    pos[1] = -0.1;
    pos[3] = -0.3;
    setButton(&ret,6,pos,col[3]);

}


Manager :: ~Manager(){
    if (gameMan != nullptr){
        delete gameMan;
    }
}

//process events
void Manager :: process(bool** keys){

    if (onMenu && (timerMenu < glfwGetTime())) {
        processMenu(keys); //process main menu activity
        timerMenu = glfwGetTime() + 0.01;
    } else if (!onMenu && timerGame < glfwGetTime()){
        processGame(keys); //process game activity
        timerGame = glfwGetTime() + 0.01;
    }

}

//handle menu button selection
void Manager :: buttonSelection(bool** keys, int lower, int higher){

    //W is pressed -- cycle upwards
    if (*(keys[2])){
        selected--;
        *(keys[2]) = false; //prevent button selection from cycling uncontrollably
    }
    //S is pressed -- cycle downwards
    if (*(keys[3])){
        selected++;
        *(keys[3]) = false; //prevent button selection from cycling uncontrollably
    }

    //catch selected
    if (selected<lower){
        selected = higher;
    }
    if (selected > higher){
        selected = lower;
    }

}

//process events when on main menu
void Manager :: processMenu(bool** keys){

    buttonSelection(keys, 0, 4); //let user scan up/down the options

    //ENTER is pressed -- user has selected an option
    if (*(keys[6])){
        switch(selected){
            case 0 : {configEasy(); break;}
            case 1 : {configNormal(); break;}
            case 2 : {configArmageddon(); break;}
            case 3 : {configTwoPlayer(); break;}
            case 4 : {glfwSetWindowShouldClose(window, GL_TRUE); break;} //exit button kill game loop
        }
        selected = 5; //set selected for in game resume option
        glClearColor(0.01,0.01,0.1,1.0); //set background to game background
    }

}


//process events when game is running
void Manager :: processGame(bool** keys){

    //game is not over and not paused
    if (!dead && !paused){
        dead = gameMan->process(keys);
        //ESCAPE is pressed -- user has paused game
         if (*(keys[7])){
            paused = true;
         }
    } else {
        //either dead or paused
        if (dead){
            buttonSelection(keys, 6, 6); //only return to menu is a valid option
        } else {
            buttonSelection(keys, 5, 6); //resume and return to menu are valid options
        }
        //ENTER is pressed -- user has selected an option
        if (*(keys[6])){
            switch (selected) {
                case 6 : { //return to main menu
                    dead = false;
                    selected = 0;
                    delete gameMan;
                    gameMan = nullptr;
                    onMenu = true;
                    glClearColor(0.5,0.5,0.5,1.0);
                    //don't break as want to execute case 5 code as well
                }
                case 5 : {paused = false; *(keys[6]) = false; break;} //resume game
            }
        }
    }
}


//sets button structure values TODO -- GRAPHICS SYSTEM OVERHALL
void Manager :: setButton(Button* but, int iD, GLfloat* p, GLfloat* c){
    //position takes form {x1,y1,x2,y2} //top left to bottom right
    but->id = iD;
    for (int i=0;i<4;i++){
        but->pos[i] = p[i];
    }
    for (int i=0;i<3;i++){
        but->col[i] = c[i];
    }

}

//return's a pointer to a button from its id
Button* Manager :: getButton(int id){
    switch(id){
        case 0 : {return &eas;}
        case 1 : {return &nor;}
        case 2 : {return &arm;}
        case 3 : {return &two;}
        case 4 : {return &ext;}
        case 5 : {return &res;}
        case 6 : {return &ret;}
        default : {return nullptr;}
    }
}

//returns if a button is selected or not, used on current graphics system -- TODO OVERHALL GRAPHICS
bool Manager :: isSelected(Button* button){
    return (button->id == selected);
}

//draws either the menu or the running game
void Manager :: draw(){

    if (timerDraw < glfwGetTime()) {
        int pxW;
        int pxH;
        glfwGetFramebufferSize(window, &pxW, &pxH); //get screen size
        glViewport(0, 0, pxW, pxH); //recentre screen
        glClear(GL_COLOR_BUFFER_BIT); //clear screen

        if (onMenu){
            //draw all 5 main menu buttons
            for (int i=0;i<5;i++){
                Button* but = getButton(i);
                camera->drawButtonIcon(isSelected(but),but->col,but->pos);
            }
        } else {
            if (!dead && !paused){
                camera->processInput();
            }
            gameMan->draw(camera); //draw game world
            if (dead || paused){
                //TODO -- DRAW BOARDER FOR PAUSE/DEATH MENU
                camera->drawButtonIcon(isSelected(&ret),ret.col,ret.pos);
                if (dead) {
                    //TODO -- DRAW SCORE
                } else {
                    camera->drawButtonIcon(isSelected(&res),res.col,res.pos);
                }
            }
        }
        glfwSwapBuffers(window);

        timerDraw = (1.0/60.0) + glfwGetTime(); //aiming for 60 fps
    }
}


