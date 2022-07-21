
#include "camera.h"


//TODO -- OVERHALL GRAPHICS SYSTEM

//handle camera zoom from user input
void Camera :: processInput(){

    if (*(keyPlus)){
        zoom = zoom + 0.001;
    }

    if (*(keyMinus)){
        zoom = zoom - 0.001;
    }

    //TODO -- DECIDE BETTER ZOOM OUT CAP
    if (zoom<0.001){
        zoom = 0.001;
    }
    //TODO -- DECIDE AND IMPLEMENT A MAX ZOOM IN CAP

}

//convert b2body and polygon data into GLfloat from
void Camera :: drawB2Polygon(b2Body* body, b2PolygonShape* shape, GLfloat* col){
    GLfloat posBody[2] = {body->GetPosition().x,body->GetPosition().y};
    GLfloat* posShape = new GLfloat[2*(shape->m_count)];
    for (int i=0;i<shape->m_count;i++){
        posShape[i*2] = shape->m_vertices[i].x;
        posShape[(i*2)+1] = shape->m_vertices[i].y;
    }
    drawPolygon(posBody,body->GetAngle(),posShape,shape->m_count,col);
    delete[] posShape;
};

//draw polygon in the b2world
void Camera :: drawPolygon(GLfloat* posBody, GLfloat angle, GLfloat* posShape, int total, GLfloat* col){

    posBody[0] = zoom*(posBody[0]-posX);
    posBody[1] = zoom*(posBody[1]-posY);

    glBegin(GL_POLYGON);
    glColor3f(col[0], col[1], col[2]);
    for (int i=0;i<total;i++){
        posShape[i*2] = posShape[i*2] * zoom;
        posShape[(i*2)+1] = posShape[(i*2)+1] * zoom;
        GLfloat temp = posShape[i*2]; //x'
        posShape[i*2] = (posShape[i*2]*cos(angle)) - (posShape[(i*2)+1]*sin(angle)); //x
        posShape[(i*2)+1] = (temp*sin(angle)) + (posShape[(i*2)+1]*cos(angle)); //y
        posShape[i*2] = posShape[i*2] + posBody[0]; //translate across x axis
        posShape[(i*2)+1] = posShape[(i*2)+1] + posBody[1]; //translate across y axis
        glVertex2f(posShape[i*2],posShape[(i*2)+1]);
    }
    glEnd();
}

//draw b2circle in the b2world
void Camera :: drawCircle(b2Body* body, b2CircleShape* shape, GLfloat* col, int res){

    GLfloat posBody[2] = {zoom*(body->GetPosition().x-posX),zoom*(body->GetPosition().y-posY)}; //position of body
    GLfloat radius = shape->m_radius;
    GLfloat angle;

    glBegin(GL_POLYGON);
    glColor3f(col[0], col[1], col[2]);
    for (int i=0;i<res;i++){
        angle = i*((2*M_PI)/(GLfloat)res);
        GLfloat posShape[2] = {zoom*radius*cos(angle), zoom*radius*sin(angle)};
        posShape[0] = posShape[0] + posBody[0]; //translate across x axis
        posShape[1] = posShape[1] + posBody[1]; //translate across y axis
        glVertex2f(posShape[0],posShape[1]);
    }
    glEnd();

    angle = body->GetAngle();
    glBegin(GL_POLYGON);
    glColor3f(col[2], col[1], col[0]);
    glVertex2f(posBody[0],posBody[1]);
    glVertex2f((zoom*radius*cos(angle-0.1))+posBody[0], (zoom*radius*sin(angle-0.1))+posBody[1]);
    glVertex2f((zoom*radius*cos(angle))+posBody[0], (zoom*radius*sin(angle))+posBody[1]);
    glVertex2f((zoom*radius*cos(angle+0.1))+posBody[0], (zoom*radius*sin(angle+0.1))+posBody[1]);
    glEnd();
}




//draw boarded rectangle icon to the screen, no zooming/positioning relative to world
void Camera :: drawButtonIcon(bool highlight, GLfloat* col, GLfloat* pos){

    //TODO -- COMPLETE OVERHALL
    GLfloat colBoarder[3] = {0.5,0.5,0.5};
    if (highlight){
        colBoarder[0] = 1;
        colBoarder[1] = 1;
        colBoarder[2] = 0;
    }
    GLfloat bW = pos[0] * 0.04;
    GLfloat bH = pos[1] * 0.02;

    //draw main icon
    glBegin(GL_POLYGON);
    glColor3f(col[0], col[1], col[2]);
    glVertex2f(pos[0]-bW,pos[1]-bH); //top right
    glVertex2f(pos[2]+bW,pos[1]-bH); //top left
    glVertex2f(pos[2]+bW,pos[3]+bH); //bottom left
    glVertex2f(pos[0]-bW,pos[3]+bH); //bottom right
    glEnd();

    //draw boarder
    glBegin(GL_POLYGON);
    glColor3f(colBoarder[0], colBoarder[1],colBoarder[2]);
    glVertex2f(pos[0],pos[1]); //top right
    glVertex2f(pos[2],pos[1]); //top left
    glVertex2f(pos[2],pos[1]-bH); //bottom left
    glVertex2f(pos[0],pos[1]-bH); //bottom right
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(pos[0],pos[3]);
    glVertex2f(pos[2],pos[3]);
    glVertex2f(pos[2],pos[3]+bH);
    glVertex2f(pos[0],pos[3]+bH);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(pos[2],pos[1]-bH); //top right
    glVertex2f(pos[2]+bW,pos[1]-bH); //top left
    glVertex2f(pos[2]+bW,pos[3]+bH); //bottom left
    glVertex2f(pos[2],pos[3]+bH); //bottom right
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(pos[0],pos[1]-bH); //top right
    glVertex2f(pos[0]-bW,pos[1]-bH); //top left
    glVertex2f(pos[0]-bW,pos[3]+bH); //bottom left
    glVertex2f(pos[0],pos[3]+bH); //bottom right
    glEnd();

}






