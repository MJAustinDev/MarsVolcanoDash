#ifndef CAMERA_H
#define CAMERA_H

#include <RecompileBox2d/box2d/box2d.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

/*
Main class used to draw the menus and box2d worlds to the screen
Keeping it all public for now for temporary ease of access, will not remain as such by the end.
I'm going to overhaul the graphics once I've read up more on opengl, so I'll integrate the proper camera system then
*/


class Camera{

public:

    Camera(GLfloat x, GLfloat y, GLfloat z, bool* keyP, bool* keyM){posX = x; posY = y; zoom = z; keyPlus = keyP; keyMinus = keyM;};
    ~Camera(){};

    void drawButtonIcon(bool highlight, GLfloat* col, GLfloat* pos);

    void centreCam(b2Vec2 vec){posX = vec.x; posY = vec.y;};
    void processInput();

    void drawB2Polygon(b2Body* body, b2PolygonShape* shape, GLfloat* col);
    void drawCircle(b2Body* body, b2CircleShape* shape, GLfloat* col, int res);

    void drawPolygon(GLfloat* posBody, GLfloat angle, GLfloat* posShape, int total, GLfloat* col);

    GLfloat posX;
    GLfloat posY;
    GLfloat zoom;

    bool* keyPlus;
    bool* keyMinus;


};


#endif
