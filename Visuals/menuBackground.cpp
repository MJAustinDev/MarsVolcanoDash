
#include "manager.h"

MenuBackground :: MenuBackground(){

    defShapeMountain(); //set mountain range shapes

    defShapeLavaFlow(); //set lava flow shapes


};

MenuBackground :: ~MenuBackground(){};

void MenuBackground :: process(){

    //update meteor positions

    //update boulder positions

    //TODO -- {USE LOOPS
    //MOVE ALL ALONG PATH
    //IF AT END OF PATH RESET
    //RESET INVOLVES RESETTING SIZE AND SPEED} -- TODO -- END

}

void MenuBackground :: draw(Camera* camera){

    b2Vec2 posBody(0.0f, 0.0f);

    b2Vec2 points[8];
    points[0].Set(-1,-1);
    points[1].Set(1,-1);
    points[2].Set(0,0);

    b2PolygonShape shape;
    shape.Set(points, 3);

    //draw mountain range
    glColor4f(0,1,0,1);
    float* colour;
    float shade = 1.0f - camera->getGlow();
    for (int i=0;i<4;i++){
        colour = mountCol[i];
        glColor4f(colour[0]*shade, colour[1]*shade, colour[2]*shade, colour[3]);
        camera->drawB2PolygonShape(posBody, &(mountains[i]), 0.0f);
    }

    //draw volcano highlight

    //draw volcano lava flow
    for (int i=0;i<3;i++){
        colour = lavaFlCol[i];
        glColor4f(colour[0]*shade, colour[1]*shade, colour[2]*shade, colour[3]);
        camera->drawB2PolygonShape(posBody, &(lavaFlow[i]), 0.0f);
    }

    //draw volcano burst

    //draw meteors

    //draw boulders


}

void MenuBackground :: defShapeMountain(){
    b2Vec2 points[8];
    //far back mountain
    points[0].Set(1.0f, -1.0f);
    points[1].Set(0.61f, -0.6f);
    points[2].Set(0.1f, -1.0f);
    mountains[0].Set(points, 3);

    //bottom of volcano
    points[0].Set(-1, -1);
    points[1].Set(-1, -0.5);
    points[2].Set(-0.95, -0.35);
    points[3].Set(-0.5, -0.43);
    points[4].Set(1.0f, -1.0f);
    mountains[1].Set(points, 5);

    //top of volcano
    points[0].Set(-0.5, -0.43);
    points[1].Set(-0.95, -0.35);
    points[2].Set(-0.85, 0.2f);
    points[3].Set(-0.6, 0.15f);
    mountains[2].Set(points, 4);

    //far right mountain
    points[0].Set(1.0f, -1.0f);
    points[1].Set(1.0f, -0.75f);
    points[2].Set(0.65f, -0.81f);
    points[3].Set(0.23f, -1.0f);
    mountains[3].Set(points, 4);

}

void MenuBackground :: defShapeLavaFlow(){
    b2Vec2 points[8];

    //overflow
    points[0].Set(-0.6f, 0.15f);
    points[1].Set(-0.85, 0.2f);
    points[2].Set(-0.7f, 0.15f);
    lavaFlow[0].Set(points,3);

    //top flow
    points[0].Set(-0.6f, 0.15f);
    points[1].Set(-0.7f, 0.15f);
    points[2].Set(-0.7f, -0.45f);
    points[3].Set(-0.5, -0.43);
    lavaFlow[1].Set(points,4);

    //bottom flow
    points[0].Set(-0.5, -0.43);
    points[1].Set(-0.7f, -0.45f);
    points[2].Set(-0.65f, -1.0f);
    points[3].Set(-0.05f, -1.0f);
    lavaFlow[2].Set(points,4);

}


