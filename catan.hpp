#pragma once
#include "board.hpp"
#include "player.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

/* Defaults */
#define DEF_PLAYERNUM 3
#define DEF_OWNEDTOWNS 2
#define DEF_OWNEDROADS 2
#define DEF_OWNERSHIP                           \
  {                                             \
    {{22, 8}, {21, 8}}, {{20, 16}, {19, 15}}, { \
      {18, 10}, { 17, 10 }                      \
    }                                           \
  }
#define POINTS_FOR_VICTORY 10
#define MIN_ROLL 2
#define MAX_ROLL 12
#define BAD_ROLL 7

/* Resources */
#define RESOURCE_NAMES \
  { "Brick", "Wool", "Wood", "Grain", "Ore" }
#define BRICKS HILLS
#define WOOL PASTURES
#define WOOD FOREST
#define GRAIN FIELDS
#define ORE MOUNTAINS

/* Actions */
#define ROLL_DICE 0
#define STATUS 1
#define USE_DEVCARD 2
#define BUY_DEVCARD 3
#define TRADE 4
#define BUILD 5
#define END_TURN 6

/* Development cards */
#define DEVCARD_COST \
  { 0, 1, 0, 1, 1 }
#define S_DEVCARD_COST "[Cost: {1 Wool, 1 Grain, 1 Ore}]"
#define DEVCARD_NAMES \
  { "Monopol", "Road building", "Year of Plenty", "Knight", "Victory point card" }
#define MONOPOL 0
#define ROADBUILD 1
#define YEAROFPLENTY 2
#define KNIGHT 3
#define VICTORY 4
#define DEF_DECK                                                                \
  { MONOPOL, MONOPOL, ROADBUILD, ROADBUILD, YEAROFPLENTY, YEAROFPLENTY, KNIGHT, \
    KNIGHT, KNIGHT, VICTORY, VICTORY, VICTORY, VICTORY }
#define YOP_AMOUNT 2     // Year of Plenty resource gain amount
#define RB_AMOUNT 2      // Road Building free roads amount
#define KNIGHTS_FOR_VP 3 // Amount of knight cards required for a victory point

/* Trade */
#define TRADE_RES 0
#define TRADE_CARDS 1
#define FIN_OFFER 2
#define FIN_SELECT ORE + 1

/* Build */
#define BUILDROAD 0
#define BUILDTOWN 1
#define BUILDCITY 2
#define ROADCOST \
  { 1, 0, 1, 0, 0 }
#define TOWNCOST \
  { 1, 1, 1, 1, 0 }
#define CITYCOST \
  { 0, 0, 0, 2, 3 }
#define S_ROADCOST "[Cost: {1 Brick, 1 Wood}]"
#define S_TOWNCOST "[Cost: {1 Brick, 1 Wood, 1 Wool, 1 Grain}]"
#define S_CITYCOST "[Cost: {3 Ore, 2 Grain}]"

/* Queries */
#define BACK -1 // Should not be >= 0.
#define DEF_INPUT BACK - 1
#define YES 0
#define NO 1

using std::cin;
using std::cout;
using std::deque;
using std::endl;
using std::string;
using std::vector;

class catanGame {
public:
  Board *board;
  vector<Player *> players;
  vector<unsigned> victory_points;
  deque<unsigned> devcard_deck;

  catanGame(const std::istringstream &input_stream);
  ~catanGame();
  void setPlayersNames(const vector<string> &names);
  string getPlayerName(const unsigned &player_id) const;
  void start();
  bool assertWinner(const unsigned &player_id) const;
  void printAvailableActions(const bool &has_rolled_dice) const;
  void rollDice(const unsigned &roll_result) const;
  bool act(const unsigned &player_id, const int &action);
  bool useDevcard(const unsigned &player_id);
  bool useMonopol(const unsigned &player_id) const;
  bool useRoadbuild(const unsigned &player_id) const;
  bool useYearofplenty(const unsigned &player_id) const;
  bool buyDevcard(const unsigned &player_id);
  bool startTrade(const unsigned &player_id);
  bool finishTrade(const unsigned &player_id1,
                   const vector<vector<unsigned>> &offer1,
                   const unsigned &player_id2,
                   const vector<vector<unsigned>> &offer2);
  bool buildQuery(const unsigned &player_id);
  bool hasEnoughRes(const unsigned &player_id,
                    const vector<unsigned> &cost) const;
  bool buildRoad(const unsigned &player_id, const vector<unsigned> &cost) const;
  vector<unsigned> getRoadOptions(const unsigned &player_id) const;
  bool buildTown(const unsigned &player_id);
  vector<unsigned> getTownOptions(const unsigned &player_id) const;
  bool buildCity(const unsigned &player_id) const;

private:
  static const vector<string> resource_names;
  static const vector<string> devcard_names;
  static const vector<unsigned> road_cost;
  static const vector<unsigned> town_cost;
  static const vector<unsigned> city_cost;
  static const vector<unsigned> devcard_cost;

  void shuffleDeck();
  void displayVictoryPoints(const unsigned &player_id) const;
  void displayStatus(const unsigned &player_id) const;
  void displayAllStatus() const;
  void displayResources(const unsigned &player_id) const;
  void displayAllResources() const;
  void displayDevcards(const unsigned &player_id, const bool &can_insert) const;
  void displayTowns(const unsigned &player_id, const bool &show_cities) const;
  void displayBuildings(const unsigned &player_id) const;
  bool checkThreeKnights(const unsigned &player_id);
  vector<vector<unsigned>> selectOfferOf(const unsigned &player_id) const;
  vector<unsigned> selectResourcesOf(const unsigned &player_id) const;
  vector<unsigned> selectDevcardsOf(const unsigned &player_id) const;
};
