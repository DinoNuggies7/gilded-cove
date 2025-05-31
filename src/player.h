#pragma once

#include <stdbool.h>

extern struct Player {
	int x, y;
	int dir;
	bool *map;
	bool *room;
	int hp, hp_max, dc, weapon;
} player;

void init_player();
void update_player();

int dir_to_player(int x, int y);
void discover_room();
