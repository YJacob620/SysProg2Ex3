// ID: *****1536
// EMAIL: yairjacob620@gmail.com

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "catan.hpp"
#include "doctest.h"

TEST_CASE("Board initialize") {
  Board b = Board();

  CHECK(b.town_slots[23]->special_neigh == b.town_slots[24]->special_neigh);
  CHECK(b.town_slots[51]->special_neigh == b.town_slots[22]->special_neigh);
  CHECK(b.town_slots[6]->special_neigh == b.town_slots[5]->special_neigh);
  CHECK(b.town_slots[21]->special_neigh == b.town_slots[4]->special_neigh);
  CHECK(b.town_slots[23]->special_neigh->clockwise_neigh == b.town_slots[24]);
  CHECK(b.road_slots[53]->clockwise_neigh == b.town_slots[24]);

  CHECK(b.lands[0].edges[0] == b.lands[2].edges[3]);
  CHECK(b.lands[0].edges[3] == b.lands[5].edges[0]);
  CHECK(b.lands[1].edges[1] == b.lands[2].edges[4]);
  CHECK(b.lands[4].edges[5] != b.lands[5].edges[2]);
  CHECK(b.lands[1].edges[1] != b.lands[2].edges[5]);
  CHECK(b.lands[6].edges[0] != b.lands[0].edges[4]);

  CHECK(b.lands[0].vertices[0] == b.lands[2].vertices[4]);
  CHECK(b.lands[0].vertices[0] == b.lands[1].vertices[2]);
  CHECK(b.lands[1].vertices[1] == b.lands[2].vertices[5]);
  CHECK(b.lands[5].vertices[2] == b.lands[4].vertices[4]);
  CHECK(b.lands[5].vertices[1] == b.lands[4].vertices[5]);
  CHECK(b.lands[5].vertices[1] == b.lands[0].vertices[3]);
  CHECK(b.lands[6].vertices[0] == b.lands[1].vertices[4]);
  CHECK(b.lands[0].vertices[3] != b.lands[4].vertices[4]);
  CHECK(b.lands[3].vertices[3] != b.lands[4].vertices[0]);

  CHECK(b.lands[1].vertices[2] == b.lands[6].vertices[3]->special_neigh->cclockwise_neigh->next_neigh->clockwise_neigh->special_neigh->clockwise_neigh->next_neigh->clockwise_neigh->next_neigh->clockwise_neigh->next_neigh->clockwise_neigh->special_neigh->cclockwise_neigh);
  CHECK(b.lands[18].vertices[0] == b.lands[12].vertices[0]->special_neigh->cclockwise_neigh->prev_neigh->cclockwise_neigh->special_neigh->cclockwise_neigh->prev_neigh->cclockwise_neigh->prev_neigh->cclockwise_neigh->special_neigh->clockwise_neigh->prev_neigh->cclockwise_neigh->special_neigh->clockwise_neigh);

  bool valid_lands = true;
  for (Land land : b.lands) {
    for (int i = 0; i < VERTICES; ++i) {
      townSlot *tsp = land.vertices[i];
      roadSlot *rsp = land.edges[i];
      if (!(tsp->next_neigh == rsp || tsp->prev_neigh == rsp || tsp->special_neigh == rsp)) {
        valid_lands = false;
        break;
      }
      if (!(rsp->cclockwise_neigh == tsp || rsp->clockwise_neigh == tsp)) {
        valid_lands = false;
        break;
      }
    }
    if (!valid_lands) {
      break;
    }
  }
  CHECK(valid_lands);

  CHECK(b.town_slots[2]->land_neighs.size() == 2);
  CHECK(b.town_slots[2]->land_neighs[0] == 3);
  CHECK(b.town_slots[2]->land_neighs[1] == 4);
  CHECK(b.town_slots[5]->land_neighs.size() == 2);
  CHECK(b.town_slots[5]->land_neighs[0] == 6);
  CHECK(b.town_slots[5]->land_neighs[1] == 1);
  CHECK(b.town_slots[20]->land_neighs.size() == 3);
  CHECK(b.town_slots[20]->land_neighs[0] == 16);
  CHECK(b.town_slots[20]->land_neighs[1] == 17);
  CHECK(b.town_slots[20]->land_neighs[2] == 5);
  CHECK(b.town_slots[23]->land_neighs.size() == 3);
  CHECK(b.town_slots[23]->land_neighs[0] == 18);
  CHECK(b.town_slots[23]->land_neighs[1] == 7);
  CHECK(b.town_slots[23]->land_neighs[2] == 6);
  CHECK(b.town_slots[35]->land_neighs.size() == 1);
  CHECK(b.town_slots[35]->land_neighs[0] == 11);
  CHECK(b.town_slots[42]->land_neighs.size() == 1);
  CHECK(b.town_slots[42]->land_neighs[0] == 14);

  CHECK(b.road_slots[8]->cclockwise_neigh->prev_neigh == b.road_slots[7]);
  CHECK(b.road_slots[8]->cclockwise_neigh->special_neigh == b.road_slots[62]);
}

TEST_CASE("Catan initialize") {
  catanGame game = catanGame(std::istringstream());
  bool player_starting_building_slots = true;
  for (int i = 0; i < DEF_PLAYERNUM; ++i) {
    if (game.players[i]->owned_towns.size() != DEF_OWNEDTOWNS ||
        game.players[i]->owned_roads.size() != DEF_OWNEDROADS) {
      player_starting_building_slots = false;
      break;
    }
  }
  CHECK(player_starting_building_slots);

  CHECK(game.board->town_slots[22] == game.players[0]->owned_towns[0]);
  CHECK(game.board->town_slots[8] == game.players[0]->owned_towns[1]);
  CHECK(game.board->town_slots[20] == game.players[1]->owned_towns[0]);
  CHECK(game.board->town_slots[16] == game.players[1]->owned_towns[1]);
  CHECK(game.board->town_slots[18] == game.players[2]->owned_towns[0]);
  CHECK(game.board->town_slots[10] == game.players[2]->owned_towns[1]);

  bool correct_bs_ownership = true;
  for (int i = 0; i < DEF_PLAYERNUM; ++i) {
    for (townSlot *owned_town : game.players[i]->owned_towns) {
      if (owned_town->owner != game.players[i]) {
        correct_bs_ownership = false;
        break;
      }
    }
    for (roadSlot *owned_road : game.players[i]->owned_roads) {
      if (owned_road->owner != game.players[i]) {
        correct_bs_ownership = false;
        break;
      }
    }
  }
  CHECK(correct_bs_ownership);

  CHECK(game.board->road_slots[8]->owner != game.board->road_slots[7]->owner);
  CHECK(game.board->road_slots[7]->owner == game.board->road_slots[62]->owner);
}

TEST_CASE("rollDice()") {
  catanGame game = catanGame(std::istringstream());

  game.players[0]->resources = {0, 0, 0, 0, 0};
  game.players[1]->resources = {0, 0, 0, 0, 0};
  game.players[2]->resources = {0, 0, 0, 0, 0};
  game.rollDice(8);
  CHECK(game.players[0]->resources[WOOD] == 1);
  CHECK(game.players[1]->resources[WOOD] == 1);
  CHECK(game.players[2]->resources[WOOD] == 0);

  game.rollDice(6);
  CHECK(game.players[0]->resources[BRICKS] == 1);
  CHECK(game.players[1]->resources[GRAIN] == 0);
  CHECK(game.players[2]->resources[GRAIN] == 1);

  game.rollDice(6);
  CHECK(game.players[0]->resources[BRICKS] == 2);
  CHECK(game.players[1]->resources[GRAIN] == 0);
  CHECK(game.players[2]->resources[GRAIN] == 2);

  game.players[0]->owned_towns.push_back(game.board->town_slots[25]);
  game.rollDice(12);
  CHECK(game.players[0]->resources[BRICKS] == 2);
  CHECK(game.players[0]->resources[WOOD] == 1);
  CHECK(game.players[0]->resources[GRAIN] == 1);
  CHECK(game.players[1]->resources[GRAIN] == 0);
  CHECK(game.players[1]->resources[WOOD] == 1);
  CHECK(game.players[2]->resources[GRAIN] == 2);
}

TEST_CASE("Miscellaneous") {
  catanGame game = catanGame(std::istringstream());
  game.players[0]->owned_roads.push_back(game.board->road_slots[49]);
  game.board->road_slots[49]->owner = game.players[0];
  vector<unsigned> tst = game.getTownOptions(0);
  CHECK((tst.size() == 1 && tst[0] == 50));
}
