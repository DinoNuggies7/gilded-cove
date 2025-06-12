#include "map.h"
#include "draw.h"
#include "object.h"
#include "player.h"
#include "enemy.h"
#include "weapons.h"

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

int main() {
	srand(time(NULL));

	initscr();
	cbreak();
	noecho();
	curs_set(0);
	
	generate_map(20, 20, 5);
	spawn_enemies(5);
	spawn_objects(1);

	init_player();
	player.hp = player.hp_max = 10;
	player.dc = 10;
	player.weapon = W_HANDS;

	player.xp = 0;
	player.level = 1;
	player.floor = 0;

	while (true) {
		clear();
		draw_screen();
		mvprintw(map.h, 0, "Health: %d/%d   DC: %d   XP: %d/%d   Floor: %d\n", player.hp, player.hp_max, player.dc, player.xp, player.level * 10, player.floor);
		refresh();

		update_player();
		if (player.hp <= 0) break;

		for (int i = 0; i < enemies; i++) {
			if (enemy[i].hp > 0)
				update_enemy(&enemy[i]);
			else {
				player.xp += enemy[i].type * 2 + 2;
				kill_enemy(i);

				if (player.xp >= player.level * 10) {
					player.hp_max += 2;
					player.hp = player.hp_max;
					player.dc = 10 + player.level / 5;
					player.level++;
				}
			}
		}
	}
	endwin();

	free(player.map);
	free(player.room);
	free_enemies();
	free_map();

	return 0;
}
