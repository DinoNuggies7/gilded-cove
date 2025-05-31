#pragma once

#include <stdbool.h>

typedef struct {
	int x1, y1, x2, y2;
} Room;

extern struct Map {
  bool *data;
  Room *room; int rooms;
  int w, h;
} map;

void generate_map(int w, int h, int lines);
void free_map();
