#include "catan.hpp"

int main(int argc, char const *argv[]) {
  std::string args;
  for (int i = 1; i < argc; ++i) {
    args += string(argv[i]) + " ";
  }
  std::istringstream new_cin(args);

  catanGame game = catanGame(new_cin);
  game.setPlayersNames({"Amit", "Yossi", "Dana"});
  game.start();

  return 0;
}
