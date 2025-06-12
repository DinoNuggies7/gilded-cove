#include "player.h"
#include "enemy.h"
#include "object.h"
#include "weapons.h"
#include "map.h"
#include "draw.h"

#include <ncurses.h>
#include <stdlib.h>
#include <math.h>

struct Player player;

void init_player() {
	do {
		player.x = rand() % (map.w - 2) + 1;
		player.y = rand() % (map.h - 2) + 1;
	}
	while (map.data[player.x + player.y * map.w]);

	player.dir = -1;

	player.map = malloc(map.w * map.h * sizeof(bool));
	for (int i = 0; i < map.w * map.h; i++)
		player.map[i] = false;

	player.room = malloc(map.rooms * sizeof(bool));
	for (int i = 0; i < map.rooms; i++)
		player.room[i] = false;

	discover_room();
}

void update_player() {
	while (player.dir < 0) {
  	switch (getch()) {
  		case 'h':
				player.dir = 0;
  	    break;
  	  case 'j':
  	    player.dir = 1;
  	    break;
  	  case 'k':
  	    player.dir = 2;
  	    break;
  	  case 'l':
  	    player.dir = 3;
  	    break;
  	  case '.':
  	  	player.dir = 4;
  	  	break;
  	  case 'Q':
  	  	printw("> are you sure you want to quit? (y/n)\n");
  	  	if (getch() == 'y') {
	  	  	player.hp = 0;
	  	  	player.dir = 4;
	  	  }
  	  	break;
  	}
  }

	int nx = player.x, ny = player.y;
	switch (player.dir) {
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
	player.dir = -1;

	if (map.data[nx + ny * map.w])
		return;

	for (int i = 0; i < enemies; i++) {
		if (nx == enemy[i].x && ny == enemy[i].y) {
			nx = ny = 0;

			int damage = 0;
			int roll = rand() % 20 + 1;
			if (roll >= E_DC[enemy[i].type]) {
				damage = rand() % W_DAMAGE[player.weapon] + 1;
				enemy[i].hp -= damage;
			}

			if (damage > 0)
				printw("> you hit %c! .o.\n", E_CHAR[enemy[i].type]);
			else
				printw("> you missed %c .-.\n", E_CHAR[enemy[i].type]);

			int cury = getcury(stdscr);
			move(0, 0);
			draw_screen();
			move(cury, 0);

			while (getch() != ' ');
			return;
		}
	}

	for (int i = 0; i < objects; i++) {
		if (nx == object[i].x && ny == object[i].y)
			use_object[object[i].type](&object[i]);
	}

	player.x = nx;
	player.y = ny;

	discover_room();
}

int dir_to_player(int x, int y) {
	float dx = player.x - x;
	float dy = player.y - y;
	float magnitude = sqrt(dx * dx + dy * dy);
	if (magnitude != 0) {
		dx /= magnitude;
		dy /= magnitude;
	}
	else
		dx = dy = 0;

	int dir;
	if (dx < -0.5) dir = 0;
	if (dy >  0.5) dir = 1;
	if (dy < -0.5) dir = 2;
	if (dx >  0.5) dir = 3;

	return dir;
}

void discover_room() {
	for (int i = 0; i < map.rooms; i++) {
		if (!player.room[i]) {
			if (in_room(player.x, player.y, i)) {
				player.room[i] = true;
				for (int j = map.room[i].y1; j <= map.room[i].y2; j++) {
					for (int t = map.room[i].x1; t <= map.room[i].x2; t++) {
						player.map[t + j * map.w] = true;
					}
				}
				break;
			}
		}
	}
}
