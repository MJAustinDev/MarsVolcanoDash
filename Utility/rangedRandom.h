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

#ifndef RANGEDRANDOM_H
#define RANGEDRANDOM_H

/*
Same code involving random numbers are used a lot in the code,
so making callable functions helps keep the files size down and easier to maintain.
*/

//returns a random float between 0 and 1
inline float randZeroToOne(){
    return ((float) rand() / (float) RAND_MAX);
}

//returns a random float within the specified range
inline float randRanged(float low, float high){
    return (((high - low) * randZeroToOne()) + low);
}

//returns a random int between 0 and high, (0 inclusive, high exclusive)
inline int randModRanged(int high){
    return (rand()%high);
}

//returns either -1 or +1
inline int randNegPos(){
    if (randModRanged(2) == 0){
        return 1;
    } else {
        return -1;
    }
}

#endif
