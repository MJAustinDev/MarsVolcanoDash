
#include <time.h>
#include <iostream>
using namespace std;

#include "manager.h" //further includes the likes of box2d, glfw3, etc


//TODO -- CLEAN INTO MORE CONSISE SYSTEM -- not priority
bool keyD = false;
bool keyA = false;
bool keyW = false;
bool keyS = false;
bool keyPlus = false;
bool keyMinus = false;
bool keyEnter = false;
bool keyEsc = false;
bool keyL = false;
bool keyJ = false;
bool keyI = false;
bool keyK = false;



void errorHandler(int error, const char* description) {
    cout << "Error: " << error << endl << "Description: " << description << endl;
}

bool setKeyBool(int action, bool prev){
    if (action == GLFW_PRESS){
        return true;
    } else if (action == GLFW_RELEASE) {
        return false;
    } else {
        return prev;
    }
}

static void inputHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {

    switch(key){
        case GLFW_KEY_EQUAL : {keyPlus = setKeyBool(action,keyPlus); break;}
        case GLFW_KEY_MINUS : {keyMinus = setKeyBool(action,keyMinus); break;}
        case GLFW_KEY_W : {keyW = setKeyBool(action,keyW); break;}
        case GLFW_KEY_S : {keyS = setKeyBool(action,keyS); break;}
        case GLFW_KEY_A : {keyA = setKeyBool(action,keyA); break;}
        case GLFW_KEY_D : {keyD = setKeyBool(action,keyD); break;}
        case GLFW_KEY_ENTER : {keyEnter = setKeyBool(action,keyEnter); break;}
        case GLFW_KEY_ESCAPE : {keyEsc = setKeyBool(action,keyEsc); break;}
        case GLFW_KEY_I : {keyI = setKeyBool(action,keyI); break;}
        case GLFW_KEY_K : {keyK = setKeyBool(action,keyK); break;}
        case GLFW_KEY_J : {keyJ = setKeyBool(action,keyJ); break;}
        case GLFW_KEY_L : {keyL = setKeyBool(action,keyL); break;}
    }

}


int main(){

    srand(time(NULL));

    if (!glfwInit()){
        return -1;// Initialization failed
    }


    //set error call back
    glfwSetErrorCallback(errorHandler);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Mars Volcano Dash", NULL, NULL); //TODO -- CONFIGURE SIZE AND FINAL NAME -- not priority
    if (!window){
        glfwTerminate(); //terminate glfw before exiting
        return -1; //window creation failed
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window,inputHandler);

    //TODO -- CONFIGRE BETTER POSITION/METHOD
    bool** keys = new bool*[12];
    keys[0] = &keyA;
    keys[1] = &keyD;
    keys[2] = &keyW;
    keys[3] = &keyS;
    keys[4] = &keyPlus;
    keys[5] = &keyMinus;
    keys[6] = &keyEnter;
    keys[7] = &keyEsc;
    keys[8] = &keyJ;
    keys[9] = &keyL;
    keys[10] = &keyI;
    keys[11] = &keyK;

    //define key game objects
    Camera camera(0.0f,0.0f,0.1f,&keyPlus, &keyMinus);
    Manager manager(window,&camera);
    glClearColor(0.5,0.5,0.5,1.0); //set to menu background colour

    //game loop, runs until glfw is told to kill the window
    while (!glfwWindowShouldClose(window)){

        manager.process(keys); //attempt to process game events

        manager.draw(); //attempt to draw the game to the screen

        glfwPollEvents(); //glfw hands events/user input
    }

    delete[] keys; //TODO -- CONFIGRE BETTER POSITION/METHOD

    return 0;
}


