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


#include "manager.h"


/*
Contains game mode configuration settings -- feel free to change and recompile on your own pull down!
Most of the names are intuitive enough, but you can look at the gameConfig.h file for more details

Kept most of the code sprawled out so it's easier to modify for more custom game modes.
For example the i=0 to i<9 for loop could call a function that sets all meteor and boulder spawns to the same parameter passed values,
but this way it's easier to add verity in spawns. e.g. config.spawns[0][i] = 128 + (i%4);
*/

//configure easy mode's settings
void Manager :: configEasy(){

    config.twoPlayer = false;

    config.spawnTimes[0] = 3000;
    config.maxSizes[0] = 2;
    config.forces[0] = 1000;

    config.spawnTimes[1] = 2000;
    config.maxSizes[1] = 2;
    config.forces[1] = 2250;

    for (int i=0;i<9;i++){
        config.ttls[0][i] = randRanged(config.spawnTimes[0],(4*config.spawnTimes[0]));
        config.spawns[0][i] = 128;
        config.ttls[1][i] = randRanged(config.spawnTimes[1],(4*config.spawnTimes[1]));
        config.spawns[1][i] = 96;
    }
    config.lavaSpeed = 0.1f;

    gameMan = new GameManager(&config);
    onMenu = false;

}

//configure normal mode's settings
void Manager :: configNormal(){

    config.twoPlayer = false;

    config.spawnTimes[0] = 1500;
    config.maxSizes[0] = 5;
    config.forces[0] = 3000;

    config.spawnTimes[1] = 1000;
    config.maxSizes[1] = 9;
    config.forces[1] = 4500;

    for (int i=0;i<9;i++){
        config.ttls[0][i] = randRanged(config.spawnTimes[0],(4*config.spawnTimes[0]));
        config.spawns[0][i] = 128;
        config.ttls[1][i] = randRanged(config.spawnTimes[1],(4*config.spawnTimes[1]));
        config.spawns[1][i] = 96;
    }
    config.lavaSpeed = 0.15f;

    gameMan = new GameManager(&config);
    onMenu = false;

}

//configure hard mode's settings
void Manager :: configHard(){

    config.twoPlayer = false;

    config.spawnTimes[0] = 750;
    config.maxSizes[0] = 9;
    config.forces[0] = 6000;


    config.spawnTimes[1] = 500;
    config.maxSizes[1] = 19;
    config.forces[1] = 9000;

    for (int i=0;i<9;i++){
        config.ttls[0][i] = randRanged(config.spawnTimes[0],(4*config.spawnTimes[0]));
        config.spawns[0][i] = 96;
        config.ttls[1][i] = randRanged(config.spawnTimes[1],(4*config.spawnTimes[1]));
        config.spawns[1][i] = 80;
    }
    config.lavaSpeed = 0.2f;

    gameMan = new GameManager(&config);
    onMenu = false;

}

//configure two player mode's settings
void Manager :: configTwoPlayer() {

    config.twoPlayer = true;
    config.spawnTimes[0] = 1500;
    config.maxSizes[0] = 5;
    config.forces[0] = 3000;

    config.spawnTimes[1] = 1000;
    config.maxSizes[1] = 9;
    config.forces[1] = 4500;

    for (int i=0;i<9;i++){
        config.ttls[0][i] = randRanged(config.spawnTimes[0],(4*config.spawnTimes[0]));
        config.spawns[0][i] = 128;
        config.ttls[1][i] = randRanged(config.spawnTimes[1],(4*config.spawnTimes[1]));
        config.spawns[1][i] = 96;
    }
    config.lavaSpeed = 0.05f;

    gameMan = new GameManager(&config);
    onMenu = false;
}
