#ifndef RANGEDRANDOM_H
#define RANGEDRANDOM_H

/*
Same code of random numbers either between 0 and 1 or a minimum and maximum range are used a lot in the code,
so making callable functions helps keep the files size down and easier to maintain.
*/

inline float randZeroToOne(){
    return ((float) rand() / (float) RAND_MAX);
}


inline float randRanged(float low, float high){
    return (((high - low) * randZeroToOne()) + low);
}

#endif
