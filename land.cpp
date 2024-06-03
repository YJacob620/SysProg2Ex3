#include "land.hpp"


Land::Land(unsigned _id, unsigned _type) {
    // Land::Land() {
    this->dice_num = _id;
    this->type = _type;
    this->owner = nullptr;
    this->edges = vector<roadSlot*>(VERTICES, nullptr);
    this->vertices = vector<townSlot*>(VERTICES, nullptr);
}