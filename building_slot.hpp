#pragma once
#include "player.hpp"
#include <vector>

#define SITTING true
#define STANDING false

using std::vector;

class Player;

class building_slot {
public:
  Player *owner;
  const unsigned id;

  building_slot(unsigned id_value) : owner(nullptr), id(id_value){};
};

class townSlot;

class roadSlot : public building_slot {
public:
  townSlot *clockwise_neigh;  // Clockwise or to-outer-cycle neighbor
  townSlot *cclockwise_neigh; // Counter clockwise or to-inner-cycle neighbor
  roadSlot(unsigned id_value);
};

class townSlot : public building_slot {
public:
  bool built_upgrade; // Whether this town has been upgraded to a city.
  roadSlot *next_neigh;
  roadSlot *prev_neigh;
  roadSlot *special_neigh;      // Neighbor that connects to a different cycle.
  vector<unsigned> land_neighs; // Indexes of neighboring lands.

  townSlot(unsigned id);
};