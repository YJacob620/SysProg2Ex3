#include "building_slot.hpp"


roadSlot::roadSlot(unsigned id) : building_slot(id) {
    this->clockwise_neigh = nullptr;
    this->cclockwise_neigh = nullptr;
}

townSlot::townSlot(unsigned id) : building_slot(id) {
    this->built_upgrade = false;
    this->next_neigh = nullptr;
    this->prev_neigh = nullptr;
    this->special_neigh = nullptr;
    this->land_neighs = std::vector<unsigned>();
}


