#include "map.h"
#include "draw.h"
#include "texture.h"
#include "object.h"
#include "player.h"

#include <raylib.h>

Shader shader;

int main(int argc, char* argv[]) {

  int width = 1920, height = 1080;
  // int width = 640, height = 360;
  InitWindow(1280, 720, "Gilded Cove");
  SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
  ToggleFullscreen();
  DisableCursor();
  SetExitKey(KEY_NULL);

  RenderTexture2D target = LoadRenderTexture(width, height);
  SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

  shader = LoadShader("res/shader/lighting.vs", "res/shader/lighting.fs");
  shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
  int lightsNumLoc = GetShaderLocation(shader, "lightsNum");
  
  int ambientLoc = GetShaderLocation(shader, "ambient");
  SetShaderValue(shader, ambientLoc, (float[4]){0.0f, 0.0f, 0.0f, 1.0f}, SHADER_UNIFORM_VEC4);

  LoadTextures();
  ParseMap(1);

  PlayerInit();
  
  while (!WindowShouldClose()) {

	  PlayerUpdate();

	  for (int i = 0; i < objects; i++) {
		  Object* obj = &object[i];
		  obj->update(obj);
	  }

	  SetShaderValue(shader, lightsNumLoc, &lights, SHADER_UNIFORM_INT);
	  SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], (float[3]){player.camera.position.x, player.camera.position.y, player.camera.position.z}, SHADER_UNIFORM_VEC3);
	  for (int i = 0; i < lights; i++) UpdateLightValues(shader, light[i]);

	  BeginTextureMode(target);
		  ClearBackground(BLACK);
		  BeginMode3D(player.camera);
			  BeginShaderMode(shader);
				  DrawBackground();
				  DrawForeground();
			  EndShaderMode();
		  EndMode3D();
		  PlayerDraw();
		  DrawFPS(0, 0);
	  EndTextureMode();
  
	  BeginDrawing();
		  DrawTexturePro(target.texture, (Rectangle){0, 0, width, -height}, (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, (Vector2){0, 0}, 0, WHITE);
	  EndDrawing();
  }

  UnloadShader(shader);
  FreeMap();
  UnloadTextures();
  return 0;
}


int WinMain(int argc, char* argv[]) {
  return main(argc, argv);
}
