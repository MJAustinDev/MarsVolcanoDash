/*

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "gameIcon.h" //set compiled game icon

#include <time.h>
#include "manager.h" //further includes the likes of box2d, glfw3, etc

// A[0], D[1], W[2], S[3], +[4], -[5], ENTER[6], ESC[7], J[8], L[9], I[10], K[11]
bool keys[12] = {false, false, false, false, false, false, false, false, false, false, false, false};

//sets key variable if pressed/released
void setKey(int i, int action){
    if (action == GLFW_PRESS){
        keys[i] = true;
    } else if (action == GLFW_RELEASE){
        keys[i] = false;
    }
}

//handle user key input
static void inputHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
    int i;
    switch(key){
        case GLFW_KEY_A : {i = 0; break;}
        case GLFW_KEY_D : {i = 1; break;}
        case GLFW_KEY_W : {i = 2; break;}
        case GLFW_KEY_S : {i = 3; break;}
        case GLFW_KEY_EQUAL : {i = 4; break;}
        case GLFW_KEY_MINUS : {i = 5; break;}
        case GLFW_KEY_ENTER : {i = 6; break;}
        case GLFW_KEY_ESCAPE : {i = 7; break;}
        case GLFW_KEY_J : {i = 8; break;}
        case GLFW_KEY_L : {i = 9; break;}
        case GLFW_KEY_I : {i = 10; break;}
        case GLFW_KEY_K : {i = 11; break;}
        default : {return; } //return as don't want to set any key
    }
    setKey(i, action);
}

//adjusts the viewport so aspect ratio is maintained
void sizeHandler(GLFWwindow* window, int width, int height){
    float vpX, vpY, vpW, vpH;
    if (width < height){
        float stretch = width/1280.0f;
        vpX = 0.0f;
        vpY = (height-(960*stretch))/2.0f;
        vpW = width;
        vpH = height*stretch;
    } else {
        float stretch = height/960.0f;
        vpX = (width-(1280*stretch))/2.0f;
        vpY = 0.0f;
        vpW = 1280*stretch;
        vpH = height;
    }
    glViewport(vpX, vpY, vpW, vpH);
}

//used at start to guarantee the viewport's aspect ratio
void forceRatio(GLFWwindow* window){
    int w;
    int h;
    glfwGetWindowSize(window, &w, &h);
    sizeHandler(window, w, h);
    glfwPollEvents();
}

int main(){

    srand(time(NULL));

    if (!glfwInit()){
        return -1;// Initialization failed
    }

    glfwWindowHint(GLFW_MAXIMIZED, 1); //attempt to start with a maximised window
    GLFWwindow* window = glfwCreateWindow(640, 480, "Mars Volcano Dash", NULL, NULL);

    if (!window){
        glfwTerminate(); //terminate glfw before exiting
        return -1; //window creation failed
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, inputHandler);
    glfwSetWindowSizeCallback(window, sizeHandler);
    glClearColor(0.0f,0.0f,0.0f,1.0f); //set outside viewport clear colour

    //set blending function for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    //define key game objects
    Camera camera(window, 0.0f, 0.0f, 1.0f, &(keys[4]), &(keys[5])); //give camera access to + and - keys
    Manager manager(window, &camera);

    //force screen size to be in ratio
    forceRatio(window);

    //game loop, runs until glfw is told to kill the window
    while (!glfwWindowShouldClose(window)){

        manager.process(keys); //attempt to process game events

        manager.draw(); //attempt to draw the game to the screen

        glfwPollEvents(); //glfw hands events/user input
    }

    return 0;
}


