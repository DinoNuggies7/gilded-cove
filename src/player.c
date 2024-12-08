#include "player.h"
#include "map.h"
#include "draw.h"
#include "object.h"

#include <raylib.h>
#include <raymath.h>

Player player;
bool capturedCursor = true;

void PlayerInit() {
	player.camera.position = (Vector3){5, 0.5, 5};
	player.camera.target = (Vector3){0, 0.5, 0};
	player.camera.up = (Vector3){0, 1, 0};
	player.camera.fovy = 80;
	player.camera.projection = CAMERA_PERSPECTIVE;
	player.light = CreateLight(LIGHT_POINT, player.camera.position, Vector3Zero(), (Color){2, 2, 2}, shader);
}

void PlayerUpdate() {
	Vector3 target = Vector3Subtract(player.camera.target, player.camera.position);
	Vector2 dir = Vector2Normalize((Vector2){target.x, target.z});
	Vector2 pos = {player.camera.position.x, player.camera.position.z};

	if (IsKeyPressed(KEY_E)) {
		Vector2 lookpos = pos;
		lookpos.x += dir.x * 0.25;
		lookpos.y += dir.y * 0.25;
		for (int i = 0; i < objects; i++) {
			Object* obj = &object[i];
			if (Vector2Distance(lookpos, obj->actor.pos) <= 0.25) {
				obj->interact(obj);
				break;
			}
		}
	}

	Vector2 vel = {0, 0};
	if (IsKeyDown(KEY_W)) {
		vel.x += dir.x;
		vel.y += dir.y;
	}
	if (IsKeyDown(KEY_S)) {
		vel.x -= dir.x;
		vel.y -= dir.y;
	}
	if (IsKeyDown(KEY_A)) {
		vel.x += dir.y;
		vel.y -= dir.x;
	}
	if (IsKeyDown(KEY_D)) {
		vel.x -= dir.y;
		vel.y += dir.x;
	}

	float speed = 2;
	Vector2 nvel = {vel.x * speed * GetFrameTime(), vel.y * speed * GetFrameTime()};

	DoCollision(pos, &nvel, 0.1);

	player.camera.position.x += nvel.x;
	player.camera.position.z += nvel.y;
	player.camera.target.x += nvel.x;
	player.camera.target.z += nvel.y;
	
	float mousespeed = 0.075;
	UpdateCameraPro(&player.camera,
		(Vector3){0, 0, 0},
		(Vector3){
			GetMouseDelta().x * mousespeed * capturedCursor,
			GetMouseDelta().y * mousespeed * capturedCursor,
			0,
		},
		0
	);

	if (IsKeyPressed(KEY_TAB)) {
		capturedCursor = !capturedCursor;
		if (capturedCursor)
			DisableCursor();
		else
			EnableCursor();
	}

	player.light->position = player.camera.position;
}

void PlayerDraw() {

}

float Dist2Player(Vector2 pos) {
	return Vector2Distance((Vector2){player.camera.position.x, player.camera.position.z}, pos);
}
