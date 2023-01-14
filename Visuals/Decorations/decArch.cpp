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

#include "decoration.h"

//constructor for arches 1, 2, 3, 4 and 5
DecArch :: DecArch(int id, b2Vec2 pos, float baseLevel, bool hasBase, float* ptrColour) : Decoration(pos, ptrColour){

    b2Vec2 baseL(0.0f, 0.0f);
    b2Vec2 baseR(0.0f, 0.0f);
    switch(id){
        case DEC_CODE_ARCH_1 : {
            setArchPole(-1.0f);
            setArchPole(1.0f);
            baseL.Set(-6.0f, -5.0f);
            baseR.Set(5.0f, 6.0f);
            break;
        }
        case DEC_CODE_ARCH_2 : {
            setArchStart(-1.0f);
            setArchPole(-1.0f);
            setArchPole(1.0f);
            baseL.Set(-10.0f, -5.0f);
            baseR.Set(5.0f, 6.0f);
            break;
        }
        case DEC_CODE_ARCH_3 : {
            setArchStart(1.0f);
            setArchPole(1.0f);
            setArchPole(-1.0f);
            baseL.Set(-6.0f, -5.0f);
            baseR.Set(5.0f, 10.0f);
            break;
        }
        case DEC_CODE_ARCH_4 : {
            setArchEnd(-1.0f);
            setArchPole(1.0f);
            baseR.Set(5.0f, 6.0f);
            break;
        }
        case DEC_CODE_ARCH_5 : {
            setArchPole(-1.0f);
            setArchEnd(1.0f);
            baseL.Set(-6.0f, -5.0f);
            break;
        }
        default : {}
    }

    if (hasBase){
        addBase(baseLevel, baseL);
        addBase(baseLevel, baseR);
    }

}

DecArch :: ~DecArch(){} //no extra dynamic memory to deallocate

//sets a straight archway backing shape with highlights
void DecArch :: setArchPole(float mirror){

    //backing arch shape
    b2Vec2 points[8];
    points[0].Set(6.0f * mirror, 12.0f); //top piece
    points[1].Set(6.0f * mirror, 10.0f);
    points[2].Set(0.0f, 10.0f);
    points[3].Set(0.0f, 12.0f);
    addShape(points, 4);

    points[0].Set(6.0f * mirror, 10.0f); //leg piece
    points[1].Set(6.0f * mirror, 0.0f);
    points[2].Set(5.0f * mirror, 0.0f);
    points[3].Set(5.0f * mirror, 10.0f);
    addShape(points, 4);

    points[0].Set(5.0f * mirror, 10.0f); //triangle
    points[1].Set(5.0f * mirror, 6.0f);
    points[2].Set(3.0f * mirror, 10.0f);
    addShape(points, 3);

    //highlights
    points[0].Set(6.0f * mirror, 12.25f); //top highlight
    points[1].Set(6.0f * mirror, 12.0f);
    points[2].Set(0.0f, 12.0f);
    points[3].Set(0.0f, 12.25f);
    addDetail(points, 4);

    points[0].Set(3.0f * mirror, 10.0f);  //under highlight
    points[1].Set(3.0f * mirror, 9.75f);
    points[2].Set(0.0f, 9.75f);
    points[3].Set(0.0f, 10.0f);
    addDetail(points, 4);

    points[2].Set(5.0f * mirror, 5.75f);//slope connection highlight
    points[3].Set(5.0f * mirror, 6.0f);
    //keep points 0 and 1 the same as the last (want to connect up)
    addDetail(points, 4);

    points[0].Set(6.0f * mirror, 6.0f);  //under leg highlight
    points[1].Set(6.0f * mirror, 5.75f);
    points[2].Set(5.0f * mirror, 5.75f);
    points[3].Set(5.0f * mirror, 6.0f);
    addDetail(points, 4);
}

//sets start sloped point of an archway
void DecArch :: setArchStart(float mirror){

    b2Vec2 points[8];
    points[0].Set(6.0f * mirror, 12.0f); //main quad backing shape
    points[1].Set(6.0f * mirror, 0.0f);
    points[2].Set(10.0f * mirror, 0.0f);
    points[3].Set(8.0f * mirror, 12.0f);
    addShape(points, 4);

    points[0].Set(8.0f * mirror, 12.0f); //angled highlight
    points[1].Set(9.0f * mirror, 6.0f);
    points[2].Set(8.75f * mirror, 6.0f);
    points[3].Set(7.75f * mirror, 12.0f);
    addDetail(points, 4);

    points[0].Set(6.0f * mirror, 12.25f); //top highlight
    points[1].Set(6.0f * mirror, 12.0f);
    points[2].Set(8.25f * mirror, 12.0f);
    points[3].Set(8.5f * mirror, 12.25f);
    addDetail(points, 4);

    points[0].Set(6.0f * mirror, 6.0f); //bottom highlight
    points[1].Set(6.0f * mirror, 5.75f);
    points[2].Set(9.5f * mirror, 5.75f);
    points[3].Set(9.25f * mirror, 6.0f);
    addDetail(points, 4);

}

//sets broken end point of an archway
void DecArch :: setArchEnd(float mirror){

    b2Vec2 points[8];
    points[0].Set(0.0f, 12.0f); //main backing shape
    points[1].Set(0.0f, 10.0f);
    points[2].Set(1.75f * mirror, 10.0f);
    points[3].Set(3.5f * mirror, 11.25f);
    points[4].Set(4.0f * mirror, 12.0f);
    addShape(points, 5);

    points[0].Set(0.0f, 12.25f); //top highlight
    points[1].Set(0.0f, 12.0f);
    points[2].Set(4.25f * mirror, 12.0f);
    points[3].Set(4.75f * mirror, 12.25f);
    addDetail(points, 4);

    points[0].Set(0.0f, 10.0f); //under highlight
    points[1].Set(0.0f, 9.75f);
    points[2].Set(2.25f * mirror, 9.75f);
    points[3].Set(2.0f * mirror, 10.0f);
    addDetail(points, 4);
}
