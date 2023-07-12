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


#include "manager.h"

namespace mvd {

namespace game_ctrl {

/*
TODO REWORD/YEET THIS
Contains game mode configuration settings -- feel free to change and recompile on your own pull down!
Most of the names are intuitive enough, but you can look at the gameConfig.h file for more details

Kept most of the code sprawled out so it's easier to modify for more custom game modes.
For example the i=0 to i<9 for loop could call a function that sets all meteor and boulder spawns to the same parameter passed values,
but this way it's easier to add verity in spawns. e.g. p_settings.spawns[0][i] = 128 + (i%4);
*/

// TODO NEW FUNC FOR SETTING BASED ON AN ENUM... HAVE CALLED DIRECTLY RATHER THAN PASSED AROUND


//configure easy mode's settings
void setGameModeEasy(GameModeSettings &p_settings) {

    p_settings.twoPlayer = false;

    p_settings.spawnTimes[0] = 3000;
    p_settings.maxSizes[0] = 2;
    p_settings.forces[0] = 1000;

    p_settings.spawnTimes[1] = 2000;
    p_settings.maxSizes[1] = 2;
    p_settings.forces[1] = 2250;

    for (int i=0;i<9;i++){
        p_settings.ttls[0][i] = randRanged(p_settings.spawnTimes[0],(4*p_settings.spawnTimes[0]));
        p_settings.spawns[0][i] = 128;
        p_settings.ttls[1][i] = randRanged(p_settings.spawnTimes[1],(4*p_settings.spawnTimes[1]));
        p_settings.spawns[1][i] = 96;
    }
    p_settings.lavaSpeed = 0.1f;
}

//configure normal mode's settings
void setGameModeNormal(GameModeSettings &p_settings) {

    p_settings.twoPlayer = false;

    p_settings.spawnTimes[0] = 1500;
    p_settings.maxSizes[0] = 5;
    p_settings.forces[0] = 3000;

    p_settings.spawnTimes[1] = 1000;
    p_settings.maxSizes[1] = 9;
    p_settings.forces[1] = 4500;

    for (int i=0;i<9;i++){
        p_settings.ttls[0][i] = randRanged(p_settings.spawnTimes[0],(4*p_settings.spawnTimes[0]));
        p_settings.spawns[0][i] = 128;
        p_settings.ttls[1][i] = randRanged(p_settings.spawnTimes[1],(4*p_settings.spawnTimes[1]));
        p_settings.spawns[1][i] = 96;
    }
    p_settings.lavaSpeed = 0.15f;
}

//configure hard mode's settings
void setGameModeHard(GameModeSettings &p_settings) {

    p_settings.twoPlayer = false;

    p_settings.spawnTimes[0] = 750;
    p_settings.maxSizes[0] = 9;
    p_settings.forces[0] = 6000;


    p_settings.spawnTimes[1] = 500;
    p_settings.maxSizes[1] = 19;
    p_settings.forces[1] = 9000;

    for (int i=0;i<9;i++){
        p_settings.ttls[0][i] = randRanged(p_settings.spawnTimes[0],(4*p_settings.spawnTimes[0]));
        p_settings.spawns[0][i] = 96;
        p_settings.ttls[1][i] = randRanged(p_settings.spawnTimes[1],(4*p_settings.spawnTimes[1]));
        p_settings.spawns[1][i] = 80;
    }
    p_settings.lavaSpeed = 0.2f;
}

//configure two player mode's settings
void setGameModeTwoPlayer(GameModeSettings &p_settings) {

    p_settings.twoPlayer = true;
    p_settings.spawnTimes[0] = 1500;
    p_settings.maxSizes[0] = 5;
    p_settings.forces[0] = 3000;

    p_settings.spawnTimes[1] = 1000;
    p_settings.maxSizes[1] = 9;
    p_settings.forces[1] = 4500;

    for (int i=0;i<9;i++){
        p_settings.ttls[0][i] = randRanged(p_settings.spawnTimes[0],(4*p_settings.spawnTimes[0]));
        p_settings.spawns[0][i] = 128;
        p_settings.ttls[1][i] = randRanged(p_settings.spawnTimes[1],(4*p_settings.spawnTimes[1]));
        p_settings.spawns[1][i] = 96;
    }
    p_settings.lavaSpeed = 0.05f;
}

}; // end of namespace game_ctrl

}; // end of namespace mvd
