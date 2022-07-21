#ifndef GAMECONFIG_H
#define GAMECONFIG_H

/*

index [0] affects meteors, index [1] affects boulders
indices [x][0,...,8] represent the 9 possible spawn positions,
with [0] being behind the player, [4] being straight above the player and [8] infront of the player
i.e., [0] => (spawnX = player.x-32), [4] => (spawnX = player.x), [8] => (spawnX = player.x+32)

*/

struct gameConfig {

    int spawnTimes[2]; //default minimum spawn time for enemy
    int ttls[2][9]; //ticks to live, counts time for enemy to spawn at given position
    float spawns[2][9]; //contains y axis value for enemy to spawn above the player at a given position
    float maxSizes[2]; //maximum size that is used to create enemy
    float forces[2]; //initial force applied to enemy when spawned in
    bool twoPlayer; //if game mode has 2 players or not

};

#endif
