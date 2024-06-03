#include "catan.hpp"

const vector<string> catanGame::resource_names = RESOURCE_NAMES;
const vector<string> catanGame::devcard_names = DEVCARD_NAMES;
const vector<unsigned> catanGame::road_cost = ROADCOST;
const vector<unsigned> catanGame::town_cost = TOWNCOST;
const vector<unsigned> catanGame::city_cost = CITYCOST;
const vector<unsigned> catanGame::devcard_cost = DEVCARD_COST;
std::istringstream temp_input_stream("");

void checkDefinitions() {
  /* Check DEF_OWNERSHIP */
  {
    std::vector<vector<vector<unsigned>>> ownership = DEF_OWNERSHIP;
    if (ownership.size() != DEF_PLAYERNUM) {
      throw std::runtime_error("Invalid #define DEF_OWNEDTOWNS (1)");
    }
    bool ans = true;
    for (int i = 0; i < DEF_PLAYERNUM; ++i) {
      if (ownership[i].size() != 2 ||
          ownership[i][0].size() != DEF_OWNEDTOWNS ||
          ownership[i][1].size() != DEF_OWNEDROADS) {
        ans = false;
        break;
      }
      for (int j = 0; j < DEF_OWNEDTOWNS; ++j) {
        if (ownership[i][0][j] >= TOWNSLOTS) {
          ans = false;
          break;
        }
      }
      for (int j = 0; j < DEF_OWNEDROADS; ++j) {
        if (ownership[i][1][j] >= ROADSLOTS_NORMAL + ROADSLOTS_SPECIAL) {
          ans = false;
          break;
        }
      }
    }
    if (!ans) {
      throw std::runtime_error("Invalid #define DEF_OWNEDTOWNS (2)");
    }
  }

  /* Check RESOURCE_NAMES */
  {
    vector<string> res_names = RESOURCE_NAMES;
    vector<unsigned> def_res = DEF_RESOURCES;
    if (res_names.size() != def_res.size()) {
      throw std::runtime_error(
          "Invalid #define RESOURCE_NAMES or #define DEF_RESOURCES");
    }
  }

  /* Check DEVCARD_NAMES */
  {
    vector<string> devc_names = DEVCARD_NAMES;
    vector<unsigned> def_devc = DEF_DEVCARDS;
    if (devc_names.size() != def_devc.size()) {
      throw std::runtime_error(
          "Invalid #define DEVCARD_NAMES or #define DEF_DEVCARDS");
    }
  }
}

int getRandomInt(int min, int max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(min, max);
  return distr(gen);
}

void inputInt(int &var) {
  if (!temp_input_stream.eof()) {
    temp_input_stream >> var;
    if (temp_input_stream.fail()) {
      goto input_from_cin;
    }
    return;
  }
input_from_cin:
  cin >> var;
  if (cin.fail()) {
    var = DEF_INPUT;
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

/// @brief Checks if an int type is in a certain range.
/// @param var int to check.
/// @param min Minimum value in range (inclusive).
/// @param max Maximum value in range (exclusive).
/// @return true if above condition is true, false otherwise.s
bool inRange(const int &var, const int &min, const int &max) {
  return var >= min && var <= max - 1;
}

template <typename T>
string printVector(const vector<T> &vec) {
  string ans = "{" + std::to_string(vec[0]);
  for (int i = 1; i < vec.size(); ++i) {
    ans += ", " + std::to_string(vec[i]);
  }
  ans += "}";
  return ans;
}

int yesNoQuery() {
  int input = DEF_INPUT;
  while (true) {
    cout << YES << " - Yes" << endl;
    cout << NO << " - No" << endl;
    inputInt(input);
    if (input == YES || input == NO) {
      break;
    }
  }
  return input;
}

catanGame::catanGame(const std::istringstream &input_stream) {
  if (!input_stream.str().empty()) {
    temp_input_stream.str(input_stream.str());
  }

  checkDefinitions();
  this->board = new Board();
  this->victory_points = vector<unsigned>(DEF_PLAYERNUM, DEF_OWNEDTOWNS);
  this->devcard_deck = DEF_DECK;
  shuffleDeck();
  this->players = vector<Player *>(DEF_PLAYERNUM);

  vector<vector<vector<unsigned>>> ownership = DEF_OWNERSHIP;
  for (int i = 0; i < DEF_PLAYERNUM; ++i) {
    this->players[i] = new Player();
    vector<vector<unsigned>> player_ownership = ownership[i];

    for (int j = 0; j < DEF_OWNEDTOWNS; ++j) {
      players[i]->owned_towns.push_back(
          board->town_slots[player_ownership[0][j]]);
      players[i]->owned_towns[j]->owner = players[i];
    }
    for (int j = 0; j < DEF_OWNEDROADS; ++j) {
      players[i]->owned_roads.push_back(
          board->road_slots[player_ownership[1][j]]);
      players[i]->owned_roads[j]->owner = players[i];
    }
  }
}

catanGame::~catanGame() {
  delete (board);
  for (int i = 0; i < DEF_PLAYERNUM; ++i) {
    delete (this->players[i]);
  }
}

void catanGame::setPlayersNames(const vector<string> &names) {
  for (int i = 0; i < names.size() && i < DEF_PLAYERNUM; ++i) {
    players[i]->name = names[i];
  }
}

string catanGame::getPlayerName(const unsigned &player_id) const {
  string ans = "Player ";
  if (players[player_id]->name.empty()) {
    return ans + std::to_string(player_id + 1);
  }
  return ans + players[player_id]->name;
}

void catanGame::shuffleDeck() {
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(devcard_deck.begin(), devcard_deck.end(), g);
}

void catanGame::start() {
  cout << "Game started." << endl;
  unsigned curr_player_id = 0; // ID of player who has its turn right now
  while (true) {
    cout << endl
         << getPlayerName(curr_player_id) << " turn." << endl;
    displayStatus(curr_player_id);
    if (assertWinner(curr_player_id)) {
      break;
    }
    int action = DEF_INPUT;
    bool dice_rolled = false;
    while (true) {
      cout << endl
           << getPlayerName(curr_player_id) << " - Which action would you like to play? Insert a valid number:"
           << endl;
      printAvailableActions(dice_rolled);
      inputInt(action);
      if (!dice_rolled) {
        if (action == ROLL_DICE) {
          rollDice(getRandomInt(MIN_ROLL, MAX_ROLL));
          dice_rolled = true;
        }
        else if (!(action == STATUS || action == USE_DEVCARD)) {
          action = DEF_INPUT;
        }
      }
      if (action == END_TURN) {
        break;
      }
      bool success = act(curr_player_id, action);
      if (success && action == USE_DEVCARD) {
        break;
      }
    }
    cout << endl
         << getPlayerName(curr_player_id) << " turn end." << endl;
    curr_player_id = (curr_player_id + 1) % DEF_PLAYERNUM;
  }
}

bool catanGame::assertWinner(const unsigned &player_id) const {
  if (victory_points[player_id] >= POINTS_FOR_VICTORY) {
    cout << getPlayerName(player_id) << " won the game.";
    return true;
  }
  return false;
}

void catanGame::printAvailableActions(const bool &has_rolled_dice) const {
  cout << STATUS << " - Display game status" << endl;
  cout << USE_DEVCARD << " - Use owned development card" << endl;
  if (!has_rolled_dice) {
    cout << ROLL_DICE << " - Roll dice" << endl;
  }
  else {
    cout << BUY_DEVCARD << " - Buy a new development card" << endl;
    cout << TRADE << " - Trade with another player" << endl;
    cout << BUILD << " - Build or upgrade a town or a road" << endl;
    cout << END_TURN << " - End turn" << endl;
  }
}

void catanGame::rollDice(const unsigned &roll_result) const {
  cout << endl
       << "Rolled " << roll_result << endl;
  if (roll_result == BAD_ROLL) {
    cout << "Players with a total of " << BAD_ROLL << " resources and more must select "
         << "half of them to discard.\n"
         << endl;
    vector<unsigned> to_discard;
    for (int i = 0; i < DEF_PLAYERNUM; ++i) {
      unsigned sum = 0;
      for (const unsigned &i : players[i]->resources) {
        sum += i;
      }
      if (sum >= BAD_ROLL) {
        cout << getPlayerName(i) << " - select " << sum / 2
             << " resources to discard:" << endl;
        while (true) {
          to_discard = selectResourcesOf(i);
          unsigned sum2 = 0;
          for (const unsigned &i : to_discard) {
            sum2 += i;
          }
          if (sum2 != sum / 2) {
            cout << "Invalid selection (" << sum2 << " total resources). Select exactly "
                 << sum / 2 << endl;
          }
          else {
            players[i]->expendResources(to_discard);
            cout << getPlayerName(i) << " discarded half of their resources." << endl;
            return;
          }
        }
      }
    }
  }
  for (int i = 0; i < DEF_PLAYERNUM; ++i) {
    vector<unsigned> total_added_resources =
        vector<unsigned>(resource_names.size());
    for (const townSlot *town_slot : players[i]->owned_towns) {
      for (const unsigned &land_index : town_slot->land_neighs) {
        if (board->lands[land_index].dice_num == roll_result) {
          int amount_to_add = 1;
          if (town_slot->built_upgrade) {
            ++amount_to_add;
          }
          unsigned resource_type = board->lands[land_index].type;
          total_added_resources[resource_type] += amount_to_add;
        }
      }
    }
    for (int j = 0; j < total_added_resources.size(); ++j) {
      if (total_added_resources[j] > 0) {
        players[i]->addResource(j, total_added_resources[j]);
        cout << "Added " << total_added_resources[j] << " " << resource_names[j]
             << " to " << getPlayerName(i) << endl;
      }
    }
  }
}

bool catanGame::act(const unsigned &player_id, const int &action) {
  bool success = false;
  if (action == STATUS) {
    displayAllStatus();
  }
  else if (action == USE_DEVCARD) {
    success = useDevcard(player_id);
  }
  else if (action == BUY_DEVCARD) {
    success = buyDevcard(player_id);
  }
  else if (action == TRADE) {
    success = startTrade(player_id);
  }
  else if (action == BUILD) {
    success = buildQuery(player_id);
  }
  return success;
}

bool catanGame::useDevcard(const unsigned &player_id) {
  cout << endl;
  int card_to_use = DEF_INPUT;
  while (true) {
    cout << "Which card would you like to use? Insert a valid number or insert "
         << BACK << " to go back." << endl;
    cout << "Note that expending a development card will immediately end this "
            "turn."
         << endl
         << "   ";
    displayDevcards(player_id, true);
    inputInt(card_to_use);
    if (card_to_use == BACK) {
      break;
    }
    if (inRange(card_to_use, 0, devcard_names.size()) &&
        players[player_id]->devcards[card_to_use] > 0) {
      bool has_used_card = false;
      if (card_to_use == MONOPOL) {
        has_used_card = useMonopol(player_id);
      }
      else if (card_to_use == ROADBUILD) {
        has_used_card = useRoadbuild(player_id);
      }
      else if (card_to_use == YEAROFPLENTY) {
        has_used_card = useYearofplenty(player_id);
      }
      if (has_used_card) {
        cout << getPlayerName(player_id) << " lost 1 "
             << devcard_names[card_to_use] << " card" << endl;
        devcard_deck.push_back(card_to_use);
        return true;
      }
    }
    else {
      card_to_use = -1;
    }
  }
  return false;
}

bool catanGame::useMonopol(const unsigned &player_id) const {
  int res_to_steal = DEF_INPUT;
  while (true) {
    displayAllResources();
    cout << "Select a resource to monopolize (steal from all other players):"
         << endl
         << "Insert a valid number, or insert " << BACK
         << " to go back (in which case you will keep the card)." << endl;
    for (int i = 0; i < resource_names.size(); ++i) {
      cout << i << " - " << resource_names[i] << endl;
    }
    inputInt(res_to_steal);
    if (res_to_steal == BACK) {
      break;
    }
    if (inRange(res_to_steal, 0, resource_names.size())) {
      for (int i = 0; i < DEF_PLAYERNUM; ++i) {
        if (i == player_id) {
          continue;
        }
        unsigned to_add = players[i]->resources[res_to_steal];
        if (to_add > 0) {
          players[player_id]->resources[res_to_steal] += to_add;
          players[i]->resources[res_to_steal] = 0;
          cout << "Got " << to_add << " " << resource_names[res_to_steal]
               << " from " << getPlayerName(i) << endl;
        }
      }
      players[player_id]->addDevCard(MONOPOL, -1);
      return true;
    }
    else {
      res_to_steal = DEF_INPUT;
    }
  }
  return false;
}

bool catanGame::useRoadbuild(const unsigned &player_id) const {
  cout << endl;
  vector<unsigned> avail_options = getRoadOptions(player_id);
  cout << "Available roads to build: ";
  if (avail_options.empty()) {
    cout << "None" << endl;
  }
  else {
    cout << printVector(avail_options) << endl;
  }
  int input = DEF_INPUT;
  while (true) {
    cout << "Are you sure you wish to use a Road Building card? " << endl
         << "Note that this operation CANNOT be canceled after starting (going "
            "back will not cancel it)."
         << endl;
    input = yesNoQuery();
    cout << endl;
    if (input == YES) {
      break;
    }
    if (input == NO) {
      return false;
    }
  }
  for (int i = 0; i < RB_AMOUNT; ++i) {
    while (!buildRoad(player_id, vector<unsigned>(resource_names.size(), 0)))
      ;
  }
  players[player_id]->addDevCard(ROADBUILD, -1);
  cout << "Lost 1 " << devcard_names[MONOPOL] << " card" << endl;

  return true;
}

bool catanGame::useYearofplenty(const unsigned &player_id) const {
  cout << "Are you sure you wish to expend a Year of Plenty card?" << endl;
  if (yesNoQuery() == NO) {
    return false;
  }
  for (int i = 0; i < YOP_AMOUNT; ++i) {
    int rand = getRandomInt(0, resource_names.size() - 1);
    players[player_id]->addResource(rand, 1);
    cout << getPlayerName(player_id) << " received 1 " << resource_names[rand]
         << endl;
  }
  players[player_id]->addDevCard(YEAROFPLENTY, -1);

  return true;
}

bool catanGame::buyDevcard(const unsigned &player_id) {
  cout << endl;
  displayResources(player_id);
  cout << "Development card cost - " << S_DEVCARD_COST << endl;
  if (devcard_deck.empty()) {
    cout << "No more development cards left in deck" << endl;
    return false;
  }
  if (!hasEnoughRes(player_id, devcard_cost)) {
    return false;
  }
  cout << "Are you sure you wish to buy a development card?" << endl;
  if (yesNoQuery() == NO) {
    return false;
  }
  unsigned new_devcard = devcard_deck.front();
  devcard_deck.pop_front();
  players[player_id]->expendResources(devcard_cost);
  players[player_id]->addDevCard(new_devcard, 1);
  cout << "Successfully bought a development card - "
       << devcard_names[new_devcard] << endl;
  if (new_devcard == KNIGHT) {
    if (checkThreeKnights(player_id)) {
      ++victory_points[player_id];
    }
  }
  else if (new_devcard == VICTORY) {
    ++victory_points[player_id];
  }
  return true;
}

bool catanGame::startTrade(const unsigned &player_id) {
  int to_trade = DEF_INPUT;
  while (true) {
    cout << endl
         << "Select a player to trade with. Insert a valid number, or insert "
         << BACK << " to go back  " << endl;
    for (int i = 0; i < DEF_PLAYERNUM; ++i) {
      if (i == player_id) {
        continue;
      }
      cout << i + 1 << " - " << getPlayerName(i) << endl;
    }
    inputInt(to_trade);
    if (to_trade == BACK) {
      return false;
    }
    --to_trade;
    if (inRange(to_trade, 0, DEF_PLAYERNUM) && to_trade != player_id) {
      vector<vector<unsigned>> self_offer, other_offer;
      other_offer = selectOfferOf(to_trade);
      if (other_offer.empty()) {
        return false;
      }
      self_offer = selectOfferOf(player_id);
      if (self_offer.empty()) {
        return false;
      }
      if (finishTrade(player_id, self_offer, to_trade, other_offer)) {
        return true;
      }
      return false;
    }
  }
  return false;
}

bool catanGame::finishTrade(const unsigned &player_id1,
                            const vector<vector<unsigned>> &offer1,
                            const unsigned &player_id2,
                            const vector<vector<unsigned>> &offer2) {
  cout << endl
       << "Are you sure you wish to finish the trade? Inserting " << NO
       << " will cancel it and go back." << endl;
  if (yesNoQuery() == NO) {
    return false;
  }
  if (offer1.size() != offer2.size() || offer1.size() != 2) {
    throw std::runtime_error("Invalid offers (1)");
  }
  if (offer1[0].size() != offer2[0].size() ||
      offer1[0].size() != resource_names.size()) {
    throw std::runtime_error("Invalid offers (2)");
  }
  if (offer1[1].size() != offer2[1].size() ||
      offer1[1].size() != devcard_names.size()) {
    throw std::runtime_error("Invalid offers (3)");
  }

  players[player_id1]->expendResources(offer1[0]);
  players[player_id1]->addResources(offer2[0]);
  players[player_id2]->expendResources(offer2[0]);
  players[player_id2]->addResources(offer1[0]);

  bool three_knights1 = false, three_knights2 = false;
  if (checkThreeKnights(player_id1)) {
    three_knights1 = true;
  }
  else if (checkThreeKnights(player_id2)) {
    three_knights2 = true;
  }
  for (int i = 0; i < offer1[1].size(); ++i) {
    players[player_id1]->devcards[i] -= offer1[1][i];
    players[player_id2]->devcards[i] += offer1[1][i];

    players[player_id1]->devcards[i] += offer2[1][i];
    players[player_id2]->devcards[i] -= offer2[1][i];
    if (i == KNIGHT) {
      if (three_knights1) {
        if (!checkThreeKnights(player_id1)) {
          --victory_points[player_id1];
          if (checkThreeKnights(player_id2)) {
            ++victory_points[player_id2];
          }
        }
      }
      else if (three_knights2) {
        if (!checkThreeKnights(player_id2)) {
          --victory_points[player_id2];
          if (checkThreeKnights(player_id1)) {
            ++victory_points[player_id1];
          }
        }
      }
    }
    else if (i == VICTORY) {
      victory_points[player_id1] -= offer1[1][i];
      victory_points[player_id2] += offer1[1][i];

      victory_points[player_id1] += offer2[1][i];
      victory_points[player_id2] -= offer2[1][i];
    }
  }
  cout << "Trade finished successfully." << endl;
  return true;
}

bool catanGame::buildQuery(const unsigned &player_id) {
  int to_build = DEF_INPUT;
  while (true) {
    cout << endl;
    displayResources(player_id);
    cout << endl;
    cout << "What would you like to build? Insert a valid number or insert "
         << BACK << " to go back." << endl;
    cout << BUILDROAD << " - Build a road " << S_ROADCOST << endl;
    cout << BUILDTOWN << " - Build a town " << S_TOWNCOST << endl;
    cout << BUILDCITY << " - Upgrade a town to a city " << S_CITYCOST << endl;
    inputInt(to_build);
    if (to_build == BACK) {
      break;
    }
    if (to_build == BUILDROAD) {
      if (buildRoad(player_id, road_cost)) {
        return true;
      }
    }
    else if (to_build == BUILDTOWN) {
      if (buildTown(player_id)) {
        return true;
      }
    }
    else if (to_build == BUILDCITY) {
      if (buildCity(player_id)) {
        return true;
      }
    }
  }
  return false;
}

bool catanGame::hasEnoughRes(const unsigned &player_id,
                             const vector<unsigned> &cost) const {
  for (int i = 0; i < resource_names.size(); ++i) {
    if (players[player_id]->resources[i] < cost[i]) {
      cout << "Not enough resources." << endl;
      return false;
    }
  }
  return true;
}

bool catanGame::buildRoad(const unsigned &player_id,
                          const vector<unsigned> &cost) const {
  if (!hasEnoughRes(player_id, cost)) {
    cout << endl;
    return false;
  }
  vector<unsigned> avail_options = getRoadOptions(player_id);
  int road_to_build = DEF_INPUT;
  cout << endl;
  while (true) {
    cout << "Available roads to build: ";
    if (avail_options.empty()) {
      cout << "None" << endl;
    }
    else {
      cout << printVector(avail_options) << endl;
    }
    cout << "Select a road of the above to build or insert " << BACK
         << " to go back." << endl;
    inputInt(road_to_build);
    if (road_to_build == BACK) {
      break;
    }
    if (std::find(avail_options.begin(), avail_options.end(), road_to_build) != avail_options.end()) {
      players[player_id]->expendResources(road_cost);
      board->road_slots[road_to_build]->owner = players[player_id];
      players[player_id]->owned_roads.push_back(board->road_slots[road_to_build]);
      cout << getPlayerName(player_id) << " successfully built road " << road_to_build << endl;
      return true;
    }
  }
  return false;
}

vector<unsigned> catanGame::getRoadOptions(const unsigned &player_id) const {
  vector<unsigned> ans = vector<unsigned>(), tmp = vector<unsigned>();
  // Check next to owned towns
  for (const townSlot *town : players[player_id]->owned_towns) {
    if (town->next_neigh->owner == nullptr) {
      tmp.push_back(town->next_neigh->id);
    }
    if (town->prev_neigh->owner == nullptr) {
      tmp.push_back(town->prev_neigh->id);
    }
    if (town->special_neigh->owner == nullptr) {
      tmp.push_back(town->special_neigh->id);
    }
  }

  // Check next to owned roads
  for (const roadSlot *road : players[player_id]->owned_roads) {
    if (road->clockwise_neigh->next_neigh->owner == nullptr) {
      tmp.push_back(road->clockwise_neigh->next_neigh->id);
    }
    if (road->clockwise_neigh->prev_neigh->owner == nullptr) {
      tmp.push_back(road->clockwise_neigh->prev_neigh->id);
    }
    if (road->clockwise_neigh->special_neigh != nullptr) {
      if (road->clockwise_neigh->special_neigh->owner == nullptr) {
        tmp.push_back(road->clockwise_neigh->special_neigh->id);
      }
    }
    if (road->cclockwise_neigh->next_neigh->owner == nullptr) {
      tmp.push_back(road->cclockwise_neigh->next_neigh->id);
    }
    if (road->cclockwise_neigh->prev_neigh->owner == nullptr) {
      tmp.push_back(road->cclockwise_neigh->prev_neigh->id);
    }
    if (road->cclockwise_neigh->special_neigh != nullptr) {
      if (road->cclockwise_neigh->special_neigh->owner == nullptr) {
        tmp.push_back(road->cclockwise_neigh->special_neigh->id);
      }
    }
  }

  // Remove duplicates
  for (int i = 0; i < tmp.size(); ++i) {
    if (std::find(ans.begin(), ans.end(), tmp[i]) == ans.end()) {
      ans.push_back(tmp[i]);
    }
  }
  return ans;
}

bool catanGame::buildTown(const unsigned &player_id) {
  if (!hasEnoughRes(player_id, town_cost)) {
    return false;
  }
  vector<unsigned> avail_options = getTownOptions(player_id);
  int town_to_build = DEF_INPUT;
  cout << endl;
  while (true) {
    cout << "Available towns to build: ";
    if (avail_options.empty()) {
      cout << "None" << endl;
    }
    else {
      cout << printVector(avail_options) << endl;
    }
    cout << "Select a town of the above to build or insert " << BACK
         << " to go back." << endl;
    inputInt(town_to_build);
    if (town_to_build == BACK) {
      break;
    }
    if (std::find(avail_options.begin(), avail_options.end(), town_to_build) != avail_options.end()) {
      board->town_slots[town_to_build]->owner = players[player_id];
      players[player_id]->owned_towns.push_back(board->town_slots[town_to_build]);
      ++victory_points[player_id];
      cout << getPlayerName(player_id) << " successfully built town " << town_to_build << endl;
      return true;
    }
  }
  return false;
}

vector<unsigned> catanGame::getTownOptions(const unsigned &player_id) const {
  vector<unsigned> ans = vector<unsigned>();
  townSlot *town_opt;
  bool to_add, repeat_again = true;
  for (const roadSlot *road : players[player_id]->owned_roads) {
    town_opt = road->clockwise_neigh;
    to_add = true;

  check_neighs:
    if (town_opt->owner != nullptr) {
      to_add = false;
    }
    else if (town_opt->next_neigh->clockwise_neigh->owner != nullptr ||
             town_opt->next_neigh->cclockwise_neigh->owner != nullptr) {
      to_add = false;
    }
    else if (town_opt->prev_neigh->clockwise_neigh->owner != nullptr ||
             town_opt->prev_neigh->cclockwise_neigh->owner != nullptr) {
      to_add = false;
    }
    else if (town_opt->special_neigh != nullptr) {
      if (town_opt->special_neigh->clockwise_neigh->owner != nullptr ||
          town_opt->special_neigh->cclockwise_neigh->owner != nullptr) {
        to_add = false;
      }
    }

    if (to_add) {
      if (std::find(ans.begin(), ans.end(), town_opt->id) == ans.end()) {
        ans.push_back(town_opt->id);
      }
    }
    if (repeat_again) {
      repeat_again = false;
      town_opt = road->cclockwise_neigh;
      to_add = true;
      goto check_neighs;
    }
    repeat_again = true;
  }
  return ans;
}

bool catanGame::buildCity(const unsigned &player_id) const {
  if (!hasEnoughRes(player_id, city_cost)) {
    return false;
  }
  int input = DEF_INPUT;
  while (true) {
    displayTowns(player_id, false);
    cout << "Select a town of the above to upgrade to a city or insert " << BACK
         << " to go back." << endl;
    inputInt(input);
    if (input == BACK) {
      break;
    }
    for (int i = 0; i < players[player_id]->owned_towns.size(); ++i) {
      if (players[player_id]->owned_towns[i]->id == input &&
          !players[player_id]->owned_towns[i]->built_upgrade) {
        players[player_id]->expendResources(city_cost);
        board->town_slots[input]->built_upgrade = true;
        cout << "Player 1 successfully upgraded town " << input << " to a city." << endl;
        return true;
      }
    }
  }
  return false;
}

void catanGame::displayVictoryPoints(const unsigned &player_id) const {
  cout << getPlayerName(player_id)
       << " victory points: " << victory_points[player_id] << endl;
}

void catanGame::displayStatus(const unsigned &player_id) const {
  cout << "   ";
  displayVictoryPoints(player_id);
  cout << "   ";
  displayResources(player_id);
  cout << "   ";
  displayDevcards(player_id, false);
  cout << "   ";
  displayBuildings(player_id);
}

void catanGame::displayAllStatus() const {
  cout << endl
       << "Game status:" << endl;
  for (int i = 0; i < DEF_PLAYERNUM; ++i) {
    displayStatus(i);
  }
  cout << endl;
}

void catanGame::displayDevcards(const unsigned &player_id,
                                const bool &can_insert) const {
  vector<unsigned> owned_devcards = players[player_id]->devcards;
  bool no_devcards = true;
  for (int i = 0; i < devcard_names.size(); ++i) {
    if (owned_devcards[i] > 0) {
      if (no_devcards) {
        cout << getPlayerName(player_id) << " development cards:" << endl;
        no_devcards = false;
      }
      cout << "       ";
      if (!(i == VICTORY || i == KNIGHT)) {
        if (can_insert) {
          cout << i << " - ";
        }
      }
      cout << devcard_names[i] << " [Owned: " << owned_devcards[i] << ']'
           << endl;
    }
  }
  if (no_devcards) {
    cout << getPlayerName(player_id) << " doesn't have any development cards."
         << endl;
    ;
  }
}

/// @brief Prints a list of a given players' towns.
/// @param player_id given player ID.
/// @param show_cities Whether to print towns that are cities.
void catanGame::displayTowns(const unsigned &player_id, const bool &show_cities) const {
  int i = 0, lim = players[player_id]->owned_towns.size();
  cout << getPlayerName(player_id) << " towns: {";
  if (show_cities) {
    cout << players[player_id]->owned_towns[0]->id;
    if (players[player_id]->owned_towns[0]->built_upgrade) {
      cout << " (city)";
    }
  }
  else {
    while (i < lim) {
      if (!players[player_id]->owned_towns[i]->built_upgrade) {
        cout << players[player_id]->owned_towns[i]->id;
        break;
      }
      ++i;
    }
  }
  while (++i < lim) {
    if (players[player_id]->owned_towns[i]->built_upgrade) {
      if (show_cities) {
        cout << ", " << players[player_id]->owned_towns[i]->id;
        cout << " (city)";
      }
    }
    else {
      cout << ", " << players[player_id]->owned_towns[i]->id;
    }
  }
  cout << '}' << endl;
}

void catanGame::displayBuildings(const unsigned &player_id) const {
  displayTowns(player_id, true);
  cout << "   ";
  cout << getPlayerName(player_id) << " roads: {"
       << players[player_id]->owned_roads[0]->id;
  for (int i = 1; i < players[player_id]->owned_roads.size(); ++i) {
    cout << ", " << players[player_id]->owned_roads[i]->id;
  }
  cout << '}' << endl;
}

/* Private functions */
void catanGame::displayResources(const unsigned &player_id) const {
  cout << getPlayerName(player_id) << " resources: {" << resource_names[0]
       << ": " << players[player_id]->resources[0];
  for (int i = 1; i < resource_names.size(); ++i) {
    cout << ", " << resource_names[i] << " = "
         << players[player_id]->resources[i];
  }
  cout << '}' << endl;
}

void catanGame::displayAllResources() const {
  cout << "All players resources:" << endl;
  for (int i = 0; i < DEF_PLAYERNUM; ++i) {
    cout << "    ";
    displayResources(i);
  }
}

bool catanGame::checkThreeKnights(const unsigned &player_id) {
  if (players[player_id]->resources[KNIGHT] == KNIGHTS_FOR_VP) {
    return true;
  }
  return false;
}

vector<vector<unsigned>>
catanGame::selectOfferOf(const unsigned &player_id) const {
  vector<vector<unsigned>> offer = {vector<unsigned>(resource_names.size(), 0),
                                    vector<unsigned>(devcard_names.size(), 0)};
  int input = DEF_INPUT;
  bool empty_trade = true;
  while (true) {
    cout << endl
         << "Select " << getPlayerName(player_id) << "'s offer" << endl;
    cout << "Insert a valid number or insert " << BACK
         << " to cancel the trade:" << endl;
    cout << TRADE_RES << " - Select resources" << endl;
    cout << TRADE_CARDS << " - Select development cards" << endl;
    cout << FIN_OFFER << " - Finish offer" << endl;
    inputInt(input);
    if (input == BACK) {
      break;
    }
    if (input == FIN_OFFER) {
      if (empty_trade) {
        cout << "Notice: " << getPlayerName(player_id) << "'s offer is empty."
             << endl;
      }
      return offer;
    }
    if (input == TRADE_RES) {
      offer[0] = selectResourcesOf(player_id);
      if (offer[0].empty()) {
        cout << "Resource selection canceled." << endl;
        if (offer[1].empty()) {
          empty_trade = true;
        }
      }
      else {
        cout << endl
             << "Successfully selected resources of " << getPlayerName(player_id)
             << "'s offer." << endl
             << "Note that re-selecting resources will override this selection."
             << endl;
        empty_trade = false;
      }
    }
    else if (input == TRADE_CARDS) {
      offer[1] = selectDevcardsOf(player_id);
      if (offer[1].empty()) {
        cout << "Development cards selection canceled.." << endl;
        if (offer[0].empty()) {
          empty_trade = true;
        }
      }
      else {
        cout << endl
             << "Successfully selected development cards of "
             << getPlayerName(player_id) << "'s offer." << endl
             << "Note that re-selecting development cards will override this "
                "selection."
             << endl;
        empty_trade = false;
      }
    }
  }
  return {};
}

vector<unsigned> catanGame::selectResourcesOf(const unsigned &player_id) const {
  vector<unsigned> selection = vector<unsigned>(resource_names.size(), 0);
  int item = DEF_INPUT, amount = DEF_INPUT;
  bool done = false;
  while (true) {
    cout << endl;
    displayResources(player_id);
    cout << "Current selection: " << printVector(selection) << endl;
    cout << "Select a resource to add to selection. " << endl
         << "Insert a valid number or insert " << BACK
         << " to cancel and go back:" << endl;
    cout << BRICKS << " - " << resource_names[BRICKS] << endl;
    cout << WOOL << " - " << resource_names[WOOL] << endl;
    cout << WOOD << " - " << resource_names[WOOD] << endl;
    cout << GRAIN << " - " << resource_names[GRAIN] << endl;
    cout << ORE << " - " << resource_names[ORE] << endl;
    cout << FIN_SELECT << " - finish selection and proceed" << endl;
    inputInt(item);
    if (item == BACK) {
      break;
    }
    if (item == FIN_SELECT) {
      done = true;
      break;
    }
    if (inRange(item, 0, resource_names.size())) {
      while (true) {
        cout << "How many " << resource_names[item] << " to add to selection? "
             << endl
             << "Insert a positive integer or insert " << BACK << " to cancel."
             << endl;
        inputInt(amount);
        if (amount == BACK) {
          break;
        }
        if (amount > 0) {
          cout << endl;
          if (amount >
              int(players[player_id]->resources[item] - selection[item])) {
            cout << "Invalid selection - " << getPlayerName(player_id)
                 << " doesn't have enough " << resource_names[item] << endl;
            break;
          }
          selection[item] += amount;
          cout << "Added " << amount << " " << resource_names[item]
               << " to selection." << endl;
          break;
        }
      }
    }
  }
  if (done) {
    return selection;
  }
  return {};
}

vector<unsigned> catanGame::selectDevcardsOf(const unsigned &player_id) const {
  vector<unsigned> selection = vector<unsigned>(devcard_names.size(), 0);
  int item = DEF_INPUT, amount = DEF_INPUT;
  bool done = false;
  while (true) {
    cout << endl;
    displayDevcards(player_id, false);
    cout << "Current selection: " << printVector(selection) << endl;
    cout << "Select a development card to add to selection. " << endl
         << "Insert a valid number or insert " << BACK << " to cancel:" << endl;
    cout << MONOPOL << " - " << devcard_names[MONOPOL] << endl;
    cout << ROADBUILD << " - " << devcard_names[ROADBUILD] << endl;
    cout << YEAROFPLENTY << " - " << devcard_names[YEAROFPLENTY] << endl;
    cout << KNIGHT << " - " << devcard_names[KNIGHT] << endl;
    cout << VICTORY << " - " << devcard_names[VICTORY] << endl;
    cout << FIN_SELECT << " - finish selection and proceed" << endl;
    inputInt(item);
    if (item == BACK) {
      break;
    }
    if (item == FIN_SELECT) {
      done = true;
      break;
    }
    if (inRange(item, 0, devcard_names.size())) {
      while (true) {
        cout << "How many " << devcard_names[item] << " to add to selection? "
             << endl
             << "Insert a positive integer or insert " << BACK << " to go back."
             << endl;
        inputInt(amount);
        if (amount == BACK) {
          break;
        }
        if (amount > 0) {
          cout << endl;
          if (amount >
              int(players[player_id]->devcards[item] - selection[item])) {
            cout << "Invalid selection - " << getPlayerName(player_id)
                 << " doesn't have enough " << devcard_names[item] << " cards"
                 << endl;
            break;
          }
          selection[item] += amount;
          cout << "Added " << amount << " " << devcard_names[item]
               << " to selection." << endl;
          break;
        }
      }
    }
  }
  if (done) {
    return selection;
  }
  return {};
}
