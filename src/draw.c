#include "draw.h"
#include "map.h"
#include "player.h"
#include "enemy.h"
#include "object.h"

#include <ncurses.h>

void draw_screen() {
  for (int j = 0; j < map.h; j++) {
  	for (int i = 0; i < map.w; i++) {
  		if (map.data[i + j * map.w] && player.map[i + j * map.w]) {
  			char ch[3] = "  \0";
  			if ((j - 1 >= 0 && map.data[i + (j-1) * map.w]) || (j + 1 < map.h && map.data[i + (j+1) * map.w]))
  				ch[0] = '|';
  			if ((i - 1 >= 0 && map.data[i-1 + j * map.w]) || (i + 1 < map.w && map.data[i+1 + j * map.w])) {
  				ch[0] = '-';
  				if ((i + 1 >= map.w || !map.data[i+1 + j * map.w]))
  					ch[1] = ' ';
  				else
  					ch[1] = '-';
  			}
  			
  			printw("%s", ch);
	  	}
	  	else {
	  		int room[2] = {0, 0}, rooms = 0;
	  		for (int t = 0; t < map.rooms; t++) {
	  			if (in_room(player.x, player.y, t) && rooms < 2) {
	  				room[rooms] = t;
	  				rooms++;
	  			}
	  		}
	  		if (in_room(i, j, room[0]) || (rooms > 1 && in_room(i, j, room[1])))
	  			printw(". ");
	  		else
	  			printw("  ");
	  	}
	  }
  	printw("\n");
  }

	for (int i = 0; i < objects; i++) {
		for (int j = 0; j < map.rooms; j++) {
			if (player.room[j]) {
				if (in_room(player.x, player.y, j) && in_room(object[i].x, object[i].y, j)) {
					mvprintw(object[i].y, object[i].x * 2, "%c", O_CHAR[object[i].type]);
					break;
				}
			}
		}
	}
	for (int i = 0; i < enemies; i++) {
		for (int j = 0; j < map.rooms; j++) {
			if (player.room[j]) {
				if (in_room(player.x, player.y, j) && in_room(enemy[i].x, enemy[i].y, j)) {
					mvprintw(enemy[i].y, enemy[i].x * 2, "%c", E_CHAR[enemy[i].type]);
					break;
				}
			}
		}
	}

	mvprintw(player.y, player.x * 2, "@");
}
