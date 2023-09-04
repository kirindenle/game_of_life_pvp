#pragma once

#include <cstdint>
#include <set>


class infinite_game_of_life
{
public:
  infinite_game_of_life();
  ~infinite_game_of_life();
  void step();
  bool will_be_alive(uint64_t tile);
  bool will_be_alive_weird_version(uint64_t tile);
  bool was_alive(uint64_t tile);
  std::set<uint64_t> tiles;
  std::set<uint64_t> next_cells;
  
};

