#include "infinite_game_of_life.h"
#include <algorithm>

#define u64 uint64_t
#define i32 int32_t

i32 get_x(u64 coord) {
  return (i32)(coord >> 32);
}

i32 get_y(u64 coord) {
  return (i32)(coord);
}

u64 make_ind(i32 x, i32 y) {
  return ((u64)x << 32) | (u64)y;
}

infinite_game_of_life::infinite_game_of_life() {
}


infinite_game_of_life::~infinite_game_of_life() {
}

bool infinite_game_of_life::will_be_alive(uint64_t tile) {
  i32 x = get_x(tile);
  i32 y = get_y(tile);

  int count = 0;
  count += was_alive(make_ind(x - 1, y - 1));
  count += was_alive(make_ind(x - 1, y));
  count += was_alive(make_ind(x - 1, y + 1));
  count += was_alive(make_ind(x, y - 1));
  count += was_alive(make_ind(x, y + 1));
  count += was_alive(make_ind(x + 1, y - 1));
  count += was_alive(make_ind(x + 1, y));
  count += was_alive(make_ind(x + 1, y + 1));

  return was_alive(tile) ? count == 2 || count == 3 : count == 3;
}


bool infinite_game_of_life::will_be_alive_weird_version(uint64_t tile) {
  i32 x = get_x(tile);
  i32 y = get_y(tile);
  int count = 0;
  return ((((((((
      count += was_alive(make_ind(x - 1, y - 1)))
        += was_alive(make_ind(x - 1, y)))
        += was_alive(make_ind(x - 1, y + 1)))
        += was_alive(make_ind(x, y - 1)))
        += was_alive(make_ind(x, y + 1)))
        += was_alive(make_ind(x + 1, y - 1)))
        += was_alive(make_ind(x + 1, y)))
        += was_alive(make_ind(x + 1, y + 1))) == 3
    || (count == 2 && was_alive(tile));
}

bool infinite_game_of_life::was_alive(uint64_t tile) {
  return tiles.find(tile) != tiles.end();
}

void infinite_game_of_life::step() {
  next_cells.clear();
  for (u64 cell : tiles) {
    i32 x = get_x(cell);
    i32 y = get_y(cell);
    if (next_cells.find(make_ind(x - 1, y - 1)) != next_cells.end()) {next_cells.insert(make_ind(x - 1, y - 1));}
    if (next_cells.find(make_ind(x - 1, y    )) != next_cells.end()) { next_cells.insert(make_ind(x - 1, y - 1)); }
    if (next_cells.find(make_ind(x - 1, y + 1)) != next_cells.end()) { next_cells.insert(make_ind(x - 1, y - 1)); }
    if (next_cells.find(make_ind(x    , y - 1)) != next_cells.end()) { next_cells.insert(make_ind(x - 1, y - 1)); }
    if (next_cells.find(make_ind(x    , y    )) != next_cells.end()) { next_cells.insert(make_ind(x - 1, y - 1)); }
    if (next_cells.find(make_ind(x    , y + 1)) != next_cells.end()) { next_cells.insert(make_ind(x - 1, y - 1)); }
    if (next_cells.find(make_ind(x + 1, y - 1)) != next_cells.end()) { next_cells.insert(make_ind(x - 1, y - 1)); }
    if (next_cells.find(make_ind(x + 1, y    )) != next_cells.end()) { next_cells.insert(make_ind(x - 1, y - 1)); }
    if (next_cells.find(make_ind(x + 1, y + 1)) != next_cells.end()) { next_cells.insert(make_ind(x - 1, y - 1)); }
    next_cells.insert(make_ind(x - 1, y - 1));
    next_cells.insert(make_ind(x - 1, y    ));
    next_cells.insert(make_ind(x - 1, y + 1));
    next_cells.insert(make_ind(x    , y - 1));
    next_cells.insert(make_ind(x    , y    ));
    next_cells.insert(make_ind(x    , y + 1));
    next_cells.insert(make_ind(x + 1, y - 1));
    next_cells.insert(make_ind(x + 1, y    ));
    next_cells.insert(make_ind(x + 1, y + 1));
  }

  auto first = next_cells.begin();
  auto last = next_cells.end();
  if (first != last) {
    for (auto i = first; ++i != last; ) {
      if (will_be_alive(*i)) {
        next_cells.insert(first++, *i);
      }
    }
  }
  next_cells.erase(first, last);

  std::swap(tiles, next_cells);
}