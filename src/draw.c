#include "draw.h"
#include "map.h"
#include "texture.h"
#include "object.h"
#include "player.h"

#include <raymath.h>
#include <rlgl.h>

void DrawForeground() {
	bool done = false;
	while (!done) {
		done = true;
		for (int i = 0; i < objects - 1; i++) {
			Object* obj1 = &object[i];
			Object* obj2 = &object[i+1];
			Vector2 obj1pos = {obj1->actor.pos.x, obj1->actor.pos.z};
			Vector2 obj2pos = {obj2->actor.pos.x, obj2->actor.pos.z};
			if (Dist2Player(obj2pos) > Dist2Player(obj1pos)) {
				Object tmp = *obj1;
				*obj1 = *obj2;
				*obj2 = tmp;
				done = false;
			}
		}
	}
	
	for (int i = 0; i < objects; i++) {
		Object* obj = &object[i];
		obj->draw(obj);
	}
}

void DrawBackground() {
	for (int t = 0; t < map.layers; t++) {
		for (int j = 0; j < map.height; j++) {
			for (int i = 0; i < map.width; i++) {
				int wall = GetWallTile(i, j, t);
				if (wall > 0) DrawCubeTextureRec(texture[TEX_DUNGEON], GetTextureSrcRect(TEX_DUNGEON, wall), (Vector3){i, t + 0.5, j}, 1, 1, 1, WHITE);
			}
		}
	}
}

void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color) {
	float x = position.x;
	float y = position.y;
	float z = position.z;
	float texWidth = (float)texture.width;
	float texHeight = (float)texture.height;

	rlSetTexture(texture.id);

	rlBegin(RL_QUADS);
	rlColor4ub(color.r, color.g, color.b, color.a);

	rlNormal3f(0.0f, 0.0f, 1.0f);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x - width / 2, y - height / 2, z + length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + width / 2, y - height / 2, z + length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x - width / 2, y + height / 2, z + length / 2);

	rlNormal3f(0.0f, 0.0f, -1.0f);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x - width / 2, y + height / 2, z - length / 2);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x + width / 2, y + height / 2, z - length / 2);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + width / 2, y - height / 2, z - length / 2);

	rlNormal3f(0.0f, 1.0f, 0.0f);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x - width / 2, y + height / 2, z - length / 2);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x - width / 2, y + height / 2, z + length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x + width / 2, y + height / 2, z - length / 2);

	rlNormal3f(0.0f, -1.0f, 0.0f);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x + width / 2, y - height / 2, z - length / 2);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + width / 2, y - height / 2, z + length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x - width / 2, y - height / 2, z + length / 2);

	rlNormal3f(1.0f, 0.0f, 0.0f);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + width / 2, y - height / 2, z - length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x + width / 2, y + height / 2, z - length / 2);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x + width / 2, y - height / 2, z + length / 2);

	rlNormal3f(-1.0f, 0.0f, 0.0f);
	rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
	rlVertex3f(x - width / 2, y - height / 2, z + length / 2);
	rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
	rlVertex3f(x - width / 2, y + height / 2, z + length / 2);
	rlTexCoord2f(source.x / texWidth, source.y / texHeight);
	rlVertex3f(x - width / 2, y + height / 2, z - length / 2);

	rlEnd();

	rlSetTexture(0);
}

// Draw a billboard with additional parameters
void DrawBillboardBetter(Camera camera, Texture2D texture, Rectangle source, Vector3 position, Vector3 up, Vector2 size, Vector2 origin, float rotation, Color tint) {
	// Compute the up vector and the right vector
	Matrix matView = MatrixLookAt(camera.position, camera.target, camera.up);
	Vector3 right = { matView.m0, matView.m4, matView.m8 };
	right = Vector3Scale(right, size.x);
	up = Vector3Scale(up, size.y);

	// Flip the content of the billboard while maintaining the counterclockwise edge rendering order
	if (size.x < 0.0f) {
		source.x += size.x;
		source.width *= -1.0;
		right = Vector3Negate(right);
		origin.x *= -1.0f;
	}
	if (size.y < 0.0f) {
		source.y += size.y;
		source.height *= -1.0;
		up = Vector3Negate(up);
		origin.y *= -1.0f;
	}

	// Draw the texture region described by source on the following rectangle in 3D space:
	//
	//                size.x          <--.
	//  3 ^---------------------------+ 2 \ rotation
	//    |                           |   /
	//    |                           |
	//    |   origin.x   position     |
	// up |..............             | size.y
	//    |             .             |
	//    |             . origin.y    |
	//    |             .             |
	//  0 +---------------------------> 1
	//                right
	Vector3 forward;
	forward = Vector3CrossProduct(right, up);

	Vector3 origin3D = Vector3Add(Vector3Scale(Vector3Normalize(right), origin.x), Vector3Scale(Vector3Normalize(up), origin.y));
	Vector3 points[4];
	points[0] = Vector3Zero();
	points[1] = right;
	points[2] = Vector3Add(up, right);
	points[3] = up;

	for (int i = 0; i < 4; i++) {
		points[i] = Vector3Subtract(points[i], origin3D);
		if (rotation != 0.0) points[i] = Vector3RotateByAxisAngle(points[i], forward, rotation * DEG2RAD);
		points[i] = Vector3Add(points[i], position);
	}

	Vector2 texcoords[4];
	texcoords[0] = (Vector2) {(float)source.x / texture.width, (float)(source.y + source.height) / texture.height};
	texcoords[1] = (Vector2) {(float)(source.x + source.width) / texture.width, (float)(source.y + source.height) / texture.height};
	texcoords[2] = (Vector2) {(float)(source.x + source.width) / texture.width, (float)source.y / texture.height};
	texcoords[3] = (Vector2) {(float)source.x / texture.width, (float)source.y / texture.height};

	Vector3 normal = Vector3Normalize(forward);

	rlSetTexture(texture.id);
	rlBegin(RL_QUADS);
		rlNormal3f(normal.x, normal.y, normal.z);
		rlColor4ub(tint.r, tint.g, tint.b, tint.a);
		for (int i = 0; i < 4; i++) {
			rlTexCoord2f(texcoords[i].x, texcoords[i].y);
			rlVertex3f(points[i].x, points[i].y, points[i].z);
		}
	rlEnd();
	rlSetTexture(0);
}
