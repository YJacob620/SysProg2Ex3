#pragma once
#include <deque>
#include "land.hpp"

#define LAND_TOTAL 19
#define TOWNSLOTS 54
#define ROADSLOTS_NORMAL TOWNSLOTS
#define ROADSLOTS_SPECIAL 18
#define ROADSLOTS ROADSLOTS_NORMAL+ROADSLOTS_SPECIAL
#define INNER_CYCLE_TS VERTICES
#define MID_CYCLE_TS 18
#define OUTER_CYCLE_TS 30
#define LAND_IDS {0,6,4,3,4,3,11,12,10,2,9,10,8,5,11,6,5,8,9}
#define LAND_TYPES {DESERT,HILLS,PASTURES,FOREST,FIELDS,MOUNTAINS,FOREST,FIELDS,MOUNTAINS,PASTURES,FOREST,HILLS,MOUNTAINS,PASTURES,PASTURES,FIELDS,HILLS,FOREST,FIELDS}
#define debug 2

using std::vector;

class Board {
    public:
    vector<townSlot*> town_slots;
    vector<roadSlot*> road_slots;
    // vector<roadSlot*> normal_rs;
    // vector<roadSlot*> special_rs;
    vector<Land> lands;
    Board();
    ~Board();
};