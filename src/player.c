#include "player.h"
#include "map.h"
#include "draw.h"
#include "object.h"

#include <raylib.h>
#include <raymath.h>

Player player;
bool capturedCursor = true;

void PlayerInit() {
	player.camera.position = (Vector3){5, 1.6, 5};
	player.camera.target = (Vector3){0, 0.5, 0};
	player.camera.up = (Vector3){0, 1, 0};
	player.camera.fovy = 80;
	player.camera.projection = CAMERA_PERSPECTIVE;
	player.light = CreateLight(LIGHT_POINT, player.camera.position, Vector3Zero(), (Color){2, 2, 2}, shader);
}

void PlayerUpdate() {
	Vector3 target = Vector3Subtract(player.camera.target, player.camera.position);
	Vector2 dir = Vector2Normalize((Vector2){target.x, target.z});

	if (IsKeyPressed(KEY_E)) {
		Vector3 lookpos = player.camera.position;
		lookpos.x += dir.x * 0.25;
		lookpos.z += dir.y * 0.25;
		for (int i = 0; i < objects; i++) {
			Object* obj = &object[i];
			if (Vector3Distance(lookpos, obj->actor.pos) <= 0.25) {
				obj->interact(obj);
				break;
			}
		}
	}

	static Vector3 vel;
	vel.x = vel.z = 0;
	if (IsKeyDown(KEY_W)) {
		vel.x += dir.x;
		vel.z += dir.y;
	}
	if (IsKeyDown(KEY_S)) {
		vel.x -= dir.x;
		vel.z -= dir.y;
	}
	if (IsKeyDown(KEY_A)) {
		vel.x += dir.y;
		vel.z -= dir.x;
	}
	if (IsKeyDown(KEY_D)) {
		vel.x -= dir.y;
		vel.z += dir.x;
	}

	static int jumps;
	if (IsKeyPressed(KEY_SPACE) && jumps > 0) {
		vel.y = 5;
		jumps--;
	}

	if (vel.y > -10)
		vel.y -= 10 * GetFrameTime();
	else
		vel.y = -10;

	float speed = 2;
	Vector3 nvel = {vel.x * speed * GetFrameTime(), vel.y * GetFrameTime(), vel.z * speed * GetFrameTime()};

	int col = DoCollision(player.camera.position, &nvel, 0.1);
	if (col > 0) {
		vel.y = 0;
		nvel.y = 0;
	}
	if (col > 1)
		jumps = 1;

	player.camera.position.x += nvel.x;
	player.camera.position.y += nvel.y;
	player.camera.position.z += nvel.z;
	player.camera.target.x += nvel.x;
	player.camera.target.y += nvel.y;
	player.camera.target.z += nvel.z;
	Vector2 lerp = {nvel.x * 10 - player.camera.up.x, nvel.z * 10 - player.camera.up.z};
	player.camera.up.x += lerp.x * GetFrameTime() * 10;
	player.camera.up.z += lerp.y * GetFrameTime() * 10;
	
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
