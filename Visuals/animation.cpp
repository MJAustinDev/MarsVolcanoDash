
#include "animation.h"
#include "gameEntities.h" //includes rangedRandom


Animation :: Animation (){

    defShapeMountain(); //set mountain range shapes

    defShapeLavaFlow(); //set lava flow shapes

    //set up lava burst's random points
    for (int i=0;i<16;i++){
        burstRnd[i].Set(randRanged(-0.90f, -0.55f), randRanged(0.21f, 0.6f));
    }

    for (int i=0;i<5;i++){
        resetMeteor(i); //set all meteor settings randomly
    }
    for (int i=0;i<11;i++){
        resetBoulder(i); //set all boulder settings randomly
    }

};

Animation  :: ~Animation (){};

//moves a given meteor/boulder depending on it's velocity and direction
void moveItem(b2Vec2* pos, float vel, float dir){
    pos->Set(pos->x + (vel*cos(dir)), pos->y + (vel*sin(dir)));
}

//updates active item positions (meteors/boulders)
void Animation  :: process(){

    //update meteor positions
    for (int i=0;i<5;i++){
        moveItem(&(meteorPos[i]), meteorVel[i], meteorDir[i]);
        if (meteorPos[i].y > 1.5){
            resetMeteor(i);
        }
    }
    //update boulder positions
    for (int i=0;i<11;i++){
        moveItem(&(bouldPos[i]), bouldVel[i], bouldDir[i]);
        bouldAng[i] += (0.02f * bouldRotDir[i]);
        if (bouldPos[i].y < -1.5){
            resetBoulder(i);
        }
    }
}

//main draw method
void Animation :: draw(Camera* camera){

    b2Vec2 posBody(0.0f, 0.0f);
    float* colour;
    float glow = camera->getGlow();
    float shade = 1.0f - glow;
    float heatShade = 0.7 - (2.0f*glow);

    //draw meteors
    for (int i=0;i<5;i++){
        colour = meteorCol[i];
        glColor4f(colour[0]*shade, colour[1]*shade, colour[2]*shade, colour[3]);
        camera->drawHotball(meteorPos[i], meteorRad[i], colour, shade, heatShade);
    }

    //draw far back most mountain behind the boulders
    colour = mountCol[0];
    glColor4f(colour[0]*shade, colour[1]*shade, colour[2]*shade, colour[3]);
    camera->drawB2PolygonShape(posBody, &(mountains[0]), 0.0f);

    //draw boulders
    for (int i=0;i<11;i++){
        colour = bouldCol[i];
        for (int j=0;j<8;j++){
            camera->drawHotFrag(bouldPos[i], fragments[i][j], bouldAng[i], colour, shade, heatShade);
        }
    }

    //draw the rest of the mountain range
    for (int i=1;i<4;i++){
        colour = mountCol[i];
        glColor4f(colour[0]*shade, colour[1]*shade, colour[2]*shade, colour[3]);
        camera->drawB2PolygonShape(posBody, &(mountains[i]), 0.0f);
    }

    //draw volcano lava flow
    for (int i=0;i<3;i++){
        colour = lavaFlCol[i];
        glColor4f(colour[0]*shade, colour[1]*shade, colour[2]*shade, colour[3]);
        camera->drawB2PolygonShape(posBody, &(lavaFlow[i]), 0.0f);
    }

    //draw volcano burst
    float burstShift = glow * 0.2f;
    for (int i=0;i<16;i++){
        colour = burstCol[1 + (i%4)];
        glColor4f(colour[0]*shade, colour[1]*shade, colour[2]*shade, shade*0.2f);
        glBegin(GL_POLYGON);
            camera->placePoint(posBody, burstBase[0]);
            camera->placePoint(posBody, burstBase[1]);
            camera->placePoint(posBody, b2Vec2(burstRnd[i].x , burstRnd[i].y + burstShift));
        glEnd();
    }

    colour = burstCol[0];
    glColor4f(colour[0]*shade, colour[1]*shade, colour[2]*shade, shade*0.4f);
    for (int i=0;i<2;i++){
        glBegin(GL_POLYGON);
            camera->placePoint(posBody, burstBase[0]);
            camera->placePoint(posBody, burstBase[1]);
            camera->placePoint(posBody, b2Vec2(burstSide[i].x, burstSide[i].y + burstShift));
        glEnd();
    }
}

//sets mountain range shapes
void Animation  :: defShapeMountain(){
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

//sets lava flow shapes
void Animation :: defShapeLavaFlow(){
    b2Vec2 points[8];

    //overflow
    points[0].Set(-0.6f, 0.15f);
    points[1].Set(-0.85, 0.2f);
    points[2].Set(-0.66f, 0.13f);
    lavaFlow[0].Set(points,3);

    //top flow
    points[0].Set(-0.6f, 0.15f);
    points[1].Set(-0.66f, 0.13f);
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

//randomly configures a given meteor
void Animation :: resetMeteor(int i){

    meteorPos[i].Set(-0.75f, 0.1f); //spawn point
    meteorRad[i] = randRanged(0.04f, 0.08f); //radius
    meteorDir[i] = randRanged(1.0f, 1.8f); //direction
    meteorVel[i] = randRanged(0.006f, 0.01f); //velocity
}

//randomly configures a given boulder
void Animation :: resetBoulder(int i){

    bouldPos[i].Set(0.7f, 1.25f); //spawn point
    bouldAng[i] = randRanged(0.0f, 2.0f*M_PI); //starting angle
    bouldDir[i] = randRanged(4.45f, 4.9f); //direction
    bouldVel[i] = randRanged(0.006f, 0.015f); //velocity
    bouldRotDir[i] = (float) randNegPos(); //rotational direction either -1 or 1

    //pick 8 points randomly
    float mags[8];
    float angs[8];
    for (int j=0;j<8;j++){
        angs[j] = randRanged(0.0,M_PI+M_PI-0.02);
        mags[j] = randRanged(0.04f, 0.1f);
    }
    //use boulder's basic insertion sort
    Boulder :: insertSortPoints(mags, angs);

    //set fragment b2vec2s, no need to worry about 'degenerate' polygons so can simplify down the process
    for (int j=0;j<7;j++){
        //if acute then set the points for valid drawing, else set to 0 so nothing is drawn
        if (angs[j+1] - angs[j] < M_PI){
            fragments[i][j][0].Set(mags[j]*cos(angs[j]), mags[j]*sin(angs[j]));
            fragments[i][j][1].Set(mags[j+1]*cos(angs[j+1]), mags[j+1]*sin(angs[j+1]));
        } else {
            fragments[i][j][0].Set(0.0f, 0.0f);
            fragments[i][j][1].Set(0.0f, 0.0f);
        }
    }
    //final point to starting point
    if ((M_PI-angs[7]+angs[0]) < 0){ //same as in the define shapes boulder method 2pi-ang[7]+ang[0] < pi => pi-ang[7]+ang[0] < 0
        fragments[i][7][0].Set(mags[7]*cos(angs[7]), mags[7]*sin(angs[7]));
        fragments[i][7][1].Set(mags[0]*cos(angs[0]), mags[0]*sin(angs[0]));
    } else {
        fragments[i][7][0].Set(0.0f, 0.0f);
        fragments[i][7][1].Set(0.0f, 0.0f);
    }
}
