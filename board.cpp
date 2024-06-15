// ID: *****1536
// EMAIL: yairjacob620@gmail.com

#include "board.hpp"


Board::Board() {
    /* Allocate memory for building slots */
    this->town_slots = vector<townSlot*>(TOWNSLOTS);
    for (int i = 0; i < TOWNSLOTS; ++i) {
        town_slots[i] = new townSlot(i);
    }
    this->road_slots = vector<roadSlot*>(ROADSLOTS);
    for (int i = 0; i < ROADSLOTS; ++i) {
        road_slots[i] = new roadSlot(i);
    }

    /* Set Building Slots neighbors */
    // Set desert BS
    for (int i = 0; i < INNER_CYCLE_TS; ++i) {
        int next_i = (i + 1) % INNER_CYCLE_TS;
        town_slots[i]->next_neigh = road_slots[i];
        town_slots[next_i]->prev_neigh = road_slots[i];
        road_slots[i]->clockwise_neigh = town_slots[next_i];
        road_slots[i]->cclockwise_neigh = town_slots[i];

        int special_i = TOWNSLOTS + (i + 1) % INNER_CYCLE_TS;
        town_slots[i]->special_neigh = road_slots[special_i];
        town_slots[i]->special_neigh->cclockwise_neigh = town_slots[i];
    }
    // Set middle cycle BS
    {
        int special_i1 = TOWNSLOTS;
        int special_i2 = TOWNSLOTS + INNER_CYCLE_TS + 1;
        for (int i = INNER_CYCLE_TS; i < INNER_CYCLE_TS + MID_CYCLE_TS; ++i) {
            int next_i = i + 1;
            if (i == INNER_CYCLE_TS + MID_CYCLE_TS - 1) {
                next_i = INNER_CYCLE_TS;
            }
            town_slots[i]->next_neigh = road_slots[i];
            town_slots[next_i]->prev_neigh = road_slots[i];
            road_slots[i]->clockwise_neigh = town_slots[next_i];
            road_slots[i]->cclockwise_neigh = town_slots[i];

            if (i % (VERTICES / 2) == 0) {
                town_slots[i]->special_neigh = road_slots[special_i1++];
                town_slots[i]->special_neigh->clockwise_neigh = town_slots[i];
            }
            else {
                if (special_i2 == ROADSLOTS) {
                    special_i2 = TOWNSLOTS + INNER_CYCLE_TS;
                }
                town_slots[i]->special_neigh = road_slots[special_i2++];
                town_slots[i]->special_neigh->cclockwise_neigh = town_slots[i];
            }
        }
    }
    // Set outer cycle BS
    {
        int special_i = TOWNSLOTS + INNER_CYCLE_TS;
        int jump_count = 3, max_jump = VERTICES / 2;
        for (int i = INNER_CYCLE_TS + MID_CYCLE_TS; i < TOWNSLOTS; ++i) {
            int next_i = i + 1;
            if (i == TOWNSLOTS - 1) {
                next_i = INNER_CYCLE_TS + MID_CYCLE_TS;
            }
            town_slots[i]->next_neigh = road_slots[i];
            town_slots[next_i]->prev_neigh = road_slots[i];
            road_slots[i]->clockwise_neigh = town_slots[next_i];
            road_slots[i]->cclockwise_neigh = town_slots[i];

            if (jump_count == max_jump) {
                town_slots[i]->special_neigh = road_slots[special_i++];
                town_slots[i]->special_neigh->clockwise_neigh = town_slots[i];
                jump_count = 0;
                if (max_jump == VERTICES / 2) {
                    max_jump = VERTICES / 3;
                }
                else {
                    max_jump = VERTICES / 2;
                }
            }
            ++jump_count;
        }
    }

    /* Set boards lands*/
    {
        this->lands = vector<Land>();
        std::deque<int> land_type_order = LAND_TYPES;
        std::deque<int> land_id_order = LAND_IDS;
        for (int i = 0; i < LAND_TOTAL; ++i) {
            lands.push_back(Land(land_id_order.front(), land_type_order.front()));
            land_id_order.pop_front();
            land_type_order.pop_front();
        }
    }

    // Set desert land Building Slots
    for (int i = 0; i < VERTICES; ++i) {
        lands[0].vertices[i] = town_slots[i];
        lands[0].edges[i] = road_slots[i];
    }
    // Set middle cycle lands BS
    {
        int ts_index = VERTICES, land_ts_index = VERTICES / 2, center_ts_index = VERTICES - 1;
        for (int i = 1; i <= INNER_CYCLE_TS; ++i) {
            int land_rs_index = (land_ts_index + VERTICES - 1) % VERTICES;
            townSlot* tmp_ts = town_slots[center_ts_index];
            lands[i].vertices[land_ts_index] = tmp_ts;
            lands[i].edges[land_rs_index] = tmp_ts->next_neigh;
            lands[i].edges[land_ts_index] = tmp_ts->special_neigh;
            for (int j = 0; j < VERTICES - 2; ++j) {
                land_ts_index = (land_ts_index + 1) % VERTICES;
                lands[i].vertices[land_ts_index] = town_slots[ts_index];
                lands[i].edges[land_ts_index] = town_slots[ts_index]->next_neigh;
                ++ts_index;
            }
            --ts_index;
            lands[i].edges[land_ts_index] = town_slots[ts_index]->special_neigh;
            center_ts_index = (center_ts_index + 1) % VERTICES;
            lands[i].vertices[land_rs_index] = town_slots[center_ts_index];
            land_ts_index = (land_ts_index + VERTICES / 2) % VERTICES;
        }
        lands[INNER_CYCLE_TS].vertices[0] = town_slots[VERTICES];
        lands[INNER_CYCLE_TS].edges[0] = road_slots[TOWNSLOTS];
    }

    // Set outer cycle lands BS
    {
        int land_ts_index = VERTICES / 2;
        townSlot* tmp_ts = town_slots[INNER_CYCLE_TS + MID_CYCLE_TS - 1];
        int jumps_before_special = VERTICES / 2 - 1;
        for (int i = INNER_CYCLE_TS + 1; i <= MID_CYCLE_TS; ++i) {
            lands[i].vertices[land_ts_index] = tmp_ts;
            lands[i].edges[land_ts_index] = tmp_ts->special_neigh;
            townSlot* tmp_ts2 = tmp_ts->special_neigh->clockwise_neigh;
            for (int j = 0; j < jumps_before_special; ++j) {
                land_ts_index = (land_ts_index + 1) % VERTICES;
                lands[i].vertices[land_ts_index] = tmp_ts2;
                lands[i].edges[land_ts_index] = tmp_ts2->next_neigh;
                tmp_ts2 = tmp_ts2->next_neigh->clockwise_neigh;
            }
            land_ts_index = (land_ts_index + 1) % VERTICES;
            lands[i].vertices[land_ts_index] = tmp_ts2;
            lands[i].edges[land_ts_index] = tmp_ts2->special_neigh;
            tmp_ts2 = tmp_ts2->special_neigh->cclockwise_neigh;
            tmp_ts = tmp_ts2;
            for (int j = 0; j < VERTICES - jumps_before_special - 2; ++j) {
                land_ts_index = (land_ts_index + 1) % VERTICES;
                lands[i].vertices[land_ts_index] = tmp_ts2;
                lands[i].edges[land_ts_index] = tmp_ts2->prev_neigh;
                tmp_ts2 = tmp_ts2->prev_neigh->cclockwise_neigh;
            }
            land_ts_index = (land_ts_index + 1) % VERTICES;
            if (i % 2 == 1) {
                ++jumps_before_special;
            }
            else {
                land_ts_index = (land_ts_index + 1) % VERTICES;
                --jumps_before_special;
            }
        }
    }

    /* Set townSlots' resource yields */
    int neigh_index1 = INNER_CYCLE_TS, neigh_index2 = 1, neigh_index3;
    for (unsigned j = 0; j < INNER_CYCLE_TS; ++j) {
        int i = (j + INNER_CYCLE_TS - 1) % VERTICES;
        town_slots[i]->land_neighs.push_back(neigh_index1);
        town_slots[i]->land_neighs.push_back(neigh_index2);
        neigh_index1 = neigh_index2;
        ++neigh_index2;
    }

    neigh_index1 = INNER_CYCLE_TS, neigh_index2 = INNER_CYCLE_TS + 1, neigh_index3 = 1;
    bool alternate = false;
    int three = VERTICES / 2;
    for (unsigned j = 0; j < MID_CYCLE_TS; ++j) {
        int i = j + INNER_CYCLE_TS;
        town_slots[i]->land_neighs.push_back(neigh_index1);
        town_slots[i]->land_neighs.push_back(neigh_index2);
        town_slots[i]->land_neighs.push_back(neigh_index3);
        if (alternate) {
            neigh_index3 = neigh_index1;
            neigh_index1 = neigh_index2;
            ++neigh_index2;
            alternate = false;
        }
        else if (j % three <= 1) {
            ++neigh_index1;
            ++neigh_index2;
            if (neigh_index2 >= LAND_TOTAL) {
                neigh_index2 = INNER_CYCLE_TS + 1;
            }
        }
        else {
            alternate = true;
            neigh_index1 = neigh_index3 + 1;
        }
    }

    neigh_index1 = INNER_CYCLE_TS, neigh_index2 = MID_CYCLE_TS;
    for (unsigned j = 0; j < OUTER_CYCLE_TS; ++j) {
        ++neigh_index1;
        int i = INNER_CYCLE_TS + MID_CYCLE_TS + j;
        town_slots[i]->land_neighs.push_back(neigh_index1);
        if (neigh_index2 > 0) {
            town_slots[i]->land_neighs.push_back(neigh_index2);
        }
        if (town_slots[i]->next_neigh->clockwise_neigh->special_neigh == nullptr) {
            neigh_index2 = -1;
            --neigh_index1;
        }
        else {
            neigh_index2 = neigh_index1;
        }
    }
}

#include <iostream>
Board::~Board() {
    for (int i = 0; i < TOWNSLOTS; ++i) {
        delete(town_slots[i]);
    }
    for (int i = 0; i < ROADSLOTS; ++i) {
        delete(road_slots[i]);
    }
}
