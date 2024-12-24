#include "player.h"
#include "map.h"
#include "draw.h"
#include "object.h"

#include <raylib.h>
#include <raymath.h>

Player player;
bool capturedCursor = true;
int mode = PLAY;

void PlayerInit() {
	player.camera.position = (Vector3){5, 1.6, 5};
	player.camera.target = (Vector3){0, 0.5, 0};
	player.camera.up = (Vector3){0, 1, 0};
	player.camera.projection = CAMERA_PERSPECTIVE;
	player.light = CreateLight(LIGHT_POINT, player.camera.position, Vector3Zero(), (Color){2, 2, 2}, shader);
	player.isClimbing = false;
}

void PlayerUpdate() {
	Vector3 target = Vector3Subtract(player.camera.target, player.camera.position);
	Vector2 dir = Vector2Normalize((Vector2){target.x, target.z});

	static float fov;
	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
		fov = 40;
	else
		fov = 80;
	player.camera.fovy += (fov - player.camera.fovy) * GetFrameTime() * 10;

	if (IsKeyPressed(KEY_E)) {
		Vector3 lookpos = player.camera.position;
		lookpos.x += dir.x * 0.25;
		lookpos.z += dir.y * 0.25;
		for (int i = 0; i < objects; i++) {
			Object* obj = &object[i];
			if (Vector3Distance(lookpos, obj->actor.pos) <= 0.5) {
				obj->interact(obj);
				break;
			}
		}
	}

	static Vector3 vel;
	static int jumps;
	float speed = 2;
	if (player.isClimbing == 0) {
		Vector2 vel2 = {0, 0};
		if (IsKeyDown(KEY_W)) {
			vel2.x += dir.x;
			vel2.y += dir.y;
		}
		if (IsKeyDown(KEY_S)) {
			vel2.x -= dir.x;
			vel2.y -= dir.y;
		}
		if (IsKeyDown(KEY_A)) {
			vel2.x += dir.y;
			vel2.y -= dir.x;
		}
		if (IsKeyDown(KEY_D)) {
			vel2.x -= dir.y;
			vel2.y += dir.x;
		}
		vel2 = Vector2Normalize(vel2);

		if (IsKeyDown(KEY_LEFT_SHIFT))
			speed = 3;

		vel.x = vel2.x;
		vel.z = vel2.y;

		if (IsKeyPressed(KEY_SPACE) && jumps > 0) {
			vel.y = 5;
			jumps--;
		}

		if (vel.y > -10)
			vel.y -= 10 * GetFrameTime();
		else
			vel.y = -10;
	}
	else {
		Object* obj;
		for (int i = 0; i < objects; i++) {
			if (object[i].id == player.isClimbing) {
				obj = &object[i];
				break;
			}
		}

		bool stopClimbing = false;
		if (player.camera.position.y > obj->actor.pos.y + 1) {
			stopClimbing = true;
			for (int i = 0; i < objects; i++) {
				if (object[i].type == OBJ_LADDER && object[i].id != obj->id) {
					if (Dist2Player((Vector2){object[i].actor.pos.x, object[i].actor.pos.z}) == Dist2Player((Vector2){obj->actor.pos.x, obj->actor.pos.z})) {
						if (object[i].actor.pos.y > obj->actor.pos.y) {
							obj = &object[i];
							player.isClimbing = obj->id;
							stopClimbing = false;
							break;
						}
					}
				}
			}
		}

		player.camera.target = (Vector3){obj->actor.pos.x, player.camera.position.y, obj->actor.pos.z};

		vel.x = vel.y = vel.z = 0;
		if (IsKeyDown(KEY_W)) {
			vel.y += speed;
		}
		if (IsKeyDown(KEY_S)) {
			vel.y -= speed;
		}

		if (stopClimbing) {
			player.isClimbing = 0;
		}
	}

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

	Vector2 lean = {vel.x * 0.1 - player.camera.up.x, vel.z * 0.1 - player.camera.up.z};
	player.camera.up.x += lean.x * GetFrameTime() * 10;
	player.camera.up.z += lean.y * GetFrameTime() * 10;
	
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
