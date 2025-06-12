#include "enemy.h"
#include "enemies.h"
#include "player.h"
#include "map.h"
#include "draw.h"

#include <ncurses.h>
#include <stdlib.h>

Enemy *enemy;
int enemies = 0;

void init_enemy(Enemy *this, int type) {
	do {
		this->x = rand() % (map.w - 2) + 1;
		this->y = rand() % (map.h - 2) + 1;
	}
	while (map.data[this->x + this->y * map.w]);

	this->type = type;
	this->hp = E_HP[type];
}

void update_enemy(Enemy *this) {
	bool agro = false;

	for (int i = 0; i < map.rooms; i++) {
		if (in_room(player.x, player.y, i)) {
			if (player.x >= this->x - E_RANGE[this->type]
			 && player.x <= this->x + E_RANGE[this->type]
			 && player.y >= this->y - E_RANGE[this->type]
			 && player.y <= this->y + E_RANGE[this->type]) {
				agro = true;
			}
			break;
		}
	}

	int dir;
	if (agro)
		dir = dir_to_player(this->x, this->y);
	else
		dir = rand() % 4;

	int nx = this->x, ny = this->y;
	switch (dir) {
		case 0:
			nx--;
			break;
		case 1:
			ny++;
			break;
		case 2:
			ny--;
			break;
		case 3:
			nx++;
			break;
	}

	if (map.data[nx + ny * map.w])
		return;

	for (int i = 0; i < enemies; i++) {
		if (nx == enemy[i].x && ny == enemy[i].y) {
			nx = ny = 0;
			return;
		}
	}

	if (nx == player.x && ny == player.y) {
		nx = ny = 0;

		int damage = 0;
		int roll = rand() % 20 + 1;
		if (roll >= player.dc) {
			damage = rand() % E_DAMAGE[this->type] + 1;
			player.hp -= damage;
		}

		if (damage > 0)
			printw("> %c hit you! >~<\n", E_CHAR[this->type]);
		else
			printw("> %c missed you ^-^\n", E_CHAR[this->type]);

		int cury = getcury(stdscr);
		move(0, 0);
		draw_screen();
		move(cury, 0);

		while (getch() != ' ');
		return;
	}

	this->x = nx;
	this->y = ny;
}

void kill_enemy(int gid) {
	enemies--;
	for (int i = gid; i < enemies; i++) {
		enemy[i] = enemy[i+1];
	}
}

void spawn_enemies(int num) {
	enemies = num;
	enemy = malloc(enemies * sizeof(Enemy));

	for (int i = 0; i < enemies; i++) {
		init_enemy(&enemy[i], E_RAT);
	}
}

void free_enemies() {
	enemies = 0;
	free(enemy);
}
