
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

    gameMan = new GameManager(&config);
    onMenu = false;

}

//configure Armageddon mode's settings
void Manager :: configArmageddon(){

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
        config.spawns[1][i] = 64;
    }

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

    gameMan = new GameManager(&config);
    onMenu = false;
}