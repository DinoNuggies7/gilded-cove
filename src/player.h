#pragma once

#include "rlights.h"

typedef struct Player {
	Camera3D camera;
	Light* light;
} Player;

extern Player player;
extern bool capturedCursor;
extern int mode;
enum Modes {
	PLAY,
	PAUSE,

	MODES
};

void PlayerInit();
void PlayerUpdate();
void PlayerDraw();

float Dist2Player(Vector2 pos);
