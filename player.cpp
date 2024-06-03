#include "player.hpp"

Player::Player() {
  this->resources = DEF_RESOURCES;
  this->devcards = DEF_DEVCARDS;
  this->owned_towns = vector<townSlot *>();
  this->owned_roads = vector<roadSlot *>();
}

Player::Player(std::string name) {
  Player();
  this->name = name;
}

void Player::addResource(const unsigned &resource, const int &amount) {
  resources[resource] += amount;
}

void Player::addDevCard(const unsigned &card_type, const int &amount) {
  devcards[card_type] += amount;
}

void Player::expendResources(const vector<unsigned> &cost) {
  if (cost.size() != resources.size()) {
    throw std::runtime_error("Cost vector size difference (1)");
  }
  for (int i = 0; i < resources.size(); ++i) {
    resources[i] -= cost[i];
  }
}
void Player::addResources(const vector<unsigned> &cost) {
  if (cost.size() != resources.size()) {
    throw std::runtime_error("Cost vector size difference (2)");
  }
  for (int i = 0; i < resources.size(); ++i) {
    resources[i] += cost[i];
  }
}
