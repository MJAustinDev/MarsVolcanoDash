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


#ifndef MANAGER_H
#define MANAGER_H

#include "gameConfig.h"
#include "gameManagement.h"

struct Button {

    GLfloat col[3];
    GLfloat pos[4];
    int id;
};

/*
Main manager, controls drawing, menu and game options, and uses the GameManager to set up and run the game
The way buttons and the graphics system is right now will likely change when I overhaul the graphics
*/
class Manager{


public:

    Manager(GLFWwindow* w, Camera* cam);
    ~Manager();

    void process(bool** keys);
    void draw();

private:

    void setButton(Button* but, int iD, GLfloat* p, GLfloat* c);
    Button* getButton(int id);

    void configEasy();
    void configNormal();
    void configArmageddon();
    void configTwoPlayer();

    void processMenu(bool** keys);
    void processGame(bool** keys);

    bool isSelected(Button* button);
    void buttonSelection(bool** keys, int lower, int higher); //handles button selection within given ID range

    int selected = 0;

    gameConfig config;
    GameManager* gameMan = nullptr;
    GLFWwindow* window;

    bool onMenu = true;
    Button eas; //easy
    Button nor; //normal
    Button arm; //Armageddon
    Button two; //two player mode
    Button ext; //exit game

    bool paused = false; //game is paused
    bool dead = false; //game is over
    Button res; //resume
    Button ret; //return to menu

    double timerGame = 0.0;
    double timerMenu = 0.0;
    double timerDraw = 0.0;
    Camera* camera = nullptr;

};


#endif
