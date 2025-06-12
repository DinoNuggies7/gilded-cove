#include "map.h"

#include <stdlib.h>

struct Map map;

void generate_map(int w, int h, int lines) {
	map.rooms = lines + 1;
	map.data = malloc(w * h * sizeof(bool));
	map.room = malloc(map.rooms * sizeof(Room));
	map.w = w;
	map.h = h;

	for (int i = 0; i < w * h; i++)
		map.data[i] = 0;

	for (int i = 0; i < w; i++) {
		map.data[i] = 1;
		map.data[i + (h-1) * w] = 1;
	}

	for (int i = 0; i < h; i++) {
		map.data[0   + i * w] = 1;
		map.data[w-1 + i * w] = 1;
	}

	Room room = {0, 0, w-1, h-1};
	int room_w = room.x2 - room.x1;
	int room_h = room.y2 - room.y1;

	for (int i = 0; i < lines; i++) {
		int x, y;
		if (room_w >= room_h) {
			do x = rand() % (room_w - 2) + room.x1 + 1;
			while (
				map.data[x     + (room.y1 + 1) * w] ||
				map.data[x - 1 + (room.y1 + 1) * w] ||
				map.data[x + 1 + (room.y1 + 1) * w] ||
				map.data[x + (room.y1) * w] == 0 ||
				map.data[x + (room.y2) * w] == 0 );

			for (int j = room.y1 + 1; j < room.y2; j++)
				map.data[x + j * w] = 1;

			y = rand() % (room_h - 2) + room.y1 + 1;

			map.data[x + y * w] = 0;

			map.room[i] = room;

			if (x - room.x1 >= room.x2 - x) {
				room.x2 = x;
				map.room[i].x1 = x;
			}
			else {
				room.x1 = x;
				map.room[i].x2 = x;
			}
			room_w = room.x2 - room.x1;
		}
		else {
			do y = rand() % (room_h - 2) + room.y1 + 1;
			while (
				map.data[room.x1 + 1 + (y    ) * w] ||
				map.data[room.x1 + 1 + (y - 1) * w] ||
				map.data[room.x1 + 1 + (y + 1) * w] ||
				map.data[room.x1 + y * w] == 0 ||
				map.data[room.x2 + y * w] == 0 );

			for (int j = room.x1 + 1; j < room.x2; j++)
				map.data[j + y * w] = 1;

			x = rand() % (room_w - 2) + room.x1 + 1;

			map.data[x + y * w] = 0;

			map.room[i] = room;

			if (y - room.y1 >= room.y2 - y) {
				room.y2 = y;
				map.room[i].y1 = y;
			}
			else {
				room.y1 = y;
				map.room[i].y2 = y;
			}
			room_h = room.y2 - room.y1;
		}
	}

	map.room[lines] = room;
}

void free_map() {
	map.rooms = 0;
	free(map.data);
	free(map.room);
}

bool in_room(int x, int y, int room) {
	return x >= map.room[room].x1
			&& x <= map.room[room].x2
			&& y >= map.room[room].y1
			&& y <= map.room[room].y2;
}
