#include "draw.h"
#include "map.h"
#include "player.h"
#include "enemy.h"

#include <ncurses.h>

void draw_map() {
  for (int j = 0; j < map.h; j++) {
  	for (int i = 0; i < map.w; i++) {
  		if (player.map[i + j * map.w])
	  		printw("%c ", map.data[i + j * map.w] ? '#' : '.');
	  	else
	  		printw("  ");
	  }
  	printw("\n");
  }
}

void draw_objects() {
	mvprintw(player.y, player.x * 2, "@");

	for (int i = 0; i < enemies; i++) {
		for (int j = 0; j < map.rooms; j++) {
			if (player.room[j]) {
				if (player.x >= map.room[j].x1
				 && player.x <= map.room[j].x2
				 && player.y >= map.room[j].y1
				 && player.y <= map.room[j].y2) {
					if (enemy[i].x >= map.room[j].x1
					 && enemy[i].x <= map.room[j].x2
					 && enemy[i].y >= map.room[j].y1
					 && enemy[i].y <= map.room[j].y2) {
						mvprintw(enemy[i].y, enemy[i].x * 2, "%c", E_CHAR[enemy[i].type]);
					}
				}
			}
		}
	}
}
