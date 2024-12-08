#include "map.h"
#include "draw.h"
#include "texture.h"
#include "object.h"
#include "player.h"

#include <raylib.h>

Shader shader;

int main(int argc, char* argv[]) {

	int width = 426, height = 240;
  bool fullscreen = true;
  InitWindow(width, height, "Gilded Cove");
  SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
  ToggleFullscreen();
  DisableCursor();
  SetExitKey(KEY_NULL);

  RenderTexture2D target = LoadRenderTexture(width, height);
  SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
  Image image;
  Texture texture;

  shader = LoadShader("res/shader/lighting.vs", "res/shader/lighting.fs");
  shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
  int lightsNumLoc = GetShaderLocation(shader, "lightsNum");
  
  int ambientLoc = GetShaderLocation(shader, "ambient");
  SetShaderValue(shader, ambientLoc, (float[4]){0.0f, 0.0f, 0.0f, 1.0f}, SHADER_UNIFORM_VEC4);

  LoadTextures();
  ParseMap(1);

  PlayerInit();
  
  while (!WindowShouldClose()) {
  	if (IsKeyPressed(KEY_F11)) {
  		fullscreen = !fullscreen;
  		if (fullscreen)
			  SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
	  	else
			  SetWindowSize(GetMonitorWidth(GetCurrentMonitor()) / 2, GetMonitorHeight(GetCurrentMonitor()) / 2);
  		ToggleFullscreen();
  	}

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

	  image = LoadImageFromTexture(target.texture);
	  ImageDither(&image, 4, 4, 4, 4);
	  texture = LoadTextureFromImage(image);
  
	  BeginDrawing();
		  DrawTexturePro(texture, (Rectangle){0, 0, width, -height}, (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, (Vector2){0, 0}, 0, WHITE);
	  EndDrawing();

	  UnloadImage(image);
	  UnloadTexture(texture);
  }

  UnloadShader(shader);
  FreeMap();
  UnloadTextures();
  return 0;
}


int WinMain(int argc, char* argv[]) {
  return main(argc, argv);
}
