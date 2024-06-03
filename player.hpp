#pragma once
#include "building_slot.hpp"
#include <stdexcept>
#include <string>
#include <vector>

#define DEF_RESOURCES \
  { 0, 0, 0, 0, 0 } // bricks, wool, wood, grain, ores
#define DEF_DEVCARDS \
  { 0, 0, 0, 0, 0 } // monopol, road building, year of plenty, knight, victory

using std::vector;

class townSlot;
class roadSlot;

class Player {
public:
  vector<unsigned> resources;
  vector<unsigned> devcards;
  vector<townSlot *> owned_towns;
  vector<roadSlot *> owned_roads;
  std::string name;

  Player();
  Player(std::string name);
  void addResource(const unsigned &resource, const int &amount);
  void addDevCard(const unsigned &card_type, const int &amount);

  /// @brief Expends resources of the player. Assumes COST is less than or equals to what the player has.
  /// @param cost A vector representing the cost (in resources) to expend.
  void expendResources(const vector<unsigned> &cost);
  void addResources(const vector<unsigned> &cost);
};