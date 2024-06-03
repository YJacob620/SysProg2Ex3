#include <iostream>

#include "board.cpp"
#include "building_slot.cpp"
#include "catan.cpp"
#include "catan.hpp"
#include "land.cpp"
#include "player.cpp"

int main(int argc, char const *argv[]) {
  std::string args;
  for (int i = 1; i < argc; ++i) {
    args += string(argv[i]) + " ";
  }
  std::istringstream new_cin(args);

  catanGame game = catanGame(new_cin);
  game.start();

  return 0;
}
