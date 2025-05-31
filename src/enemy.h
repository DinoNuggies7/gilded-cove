#pragma once

#include "enemies.h"

typedef struct {
	int x, y, type;
	int hp;
} Enemy;

extern Enemy *enemy;
extern int enemies;

void init_enemy(Enemy *enemy, int type);
void update_enemy(Enemy *enemy);
void kill_enemy(int gid);

void spawn_enemies(int num);
void free_enemies();
