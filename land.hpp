// ID: *****1536
// EMAIL: yairjacob620@gmail.com

#pragma once
#include "building_slot.hpp"

#define VERTICES 6
#define HILLS 0
#define PASTURES 1
#define FOREST 2
#define FIELDS 3
#define MOUNTAINS 4
#define DESERT 5

using std::vector;


class Land {
    public:
    /// @brief The land will yield resources when the dice hits this number.
    unsigned dice_num;
    /// @brief Type of the land (HILLS/PASTURES/FOREST/FIELDS/MOUNTAINS)
    unsigned type;
    Player* owner;
    vector<roadSlot*> edges;
    vector<townSlot*> vertices;

    // Land();
    Land(unsigned _id, unsigned _type);
};
