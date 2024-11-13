#include "rlights.h"

Light light[MAX_LIGHTS];
int lights = 0;

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Create a light and get shader locations
Light* CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader) {
	if (lights < MAX_LIGHTS) {
		light[lights].enabled = true;
		light[lights].type = type;
		light[lights].position = position;
		light[lights].target = target;
		light[lights].color = color;
		light[lights].attenuation = 1;
		light[lights].falloff = 1;

		// NOTE: Lighting shader naming must be the provided ones
		light[lights].enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", lights));
		light[lights].typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", lights));
		light[lights].positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", lights));
		light[lights].targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", lights));
		light[lights].colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", lights));
		light[lights].attenuationLoc = GetShaderLocation(shader, TextFormat("lights[%i].attenuation", lights));
		light[lights].falloffLoc = GetShaderLocation(shader, TextFormat("lights[%i].falloff", lights));
		light[lights].coneLoc = GetShaderLocation(shader, TextFormat("lights[%i].cone", lights));

		UpdateLightValues(shader, light[lights]);

		lights++;
		return &light[lights-1];
	}
	else {
		return 0;
	}
}

void RemoveLight(int lightIndex) {
	for (int i = lightIndex; i < lights - 1; i++) {
		light[i] = light[i+1];
	}
	lights--;
}

// Send light properties to shader
// NOTE: Light shader locations should be available 
void UpdateLightValues(Shader shader, Light light)
{
    // Send to shader light enabled state and type
    SetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
    SetShaderValue(shader, light.typeLoc, &light.type, SHADER_UNIFORM_INT);

    // Send to shader light position values
    float position[3] = { light.position.x, light.position.y, light.position.z };
    SetShaderValue(shader, light.positionLoc, position, SHADER_UNIFORM_VEC3);

    // Send to shader light target position values
    float target[3] = { light.target.x, light.target.y, light.target.z };
    SetShaderValue(shader, light.targetLoc, target, SHADER_UNIFORM_VEC3);

    // Send to shader light color values
    float color[4] = { (float)light.color.r/(float)255, (float)light.color.g/(float)255, 
                       (float)light.color.b/(float)255, (float)light.color.a/(float)255 };
    SetShaderValue(shader, light.colorLoc, color, SHADER_UNIFORM_VEC4);

    SetShaderValue(shader, light.attenuationLoc, &light.attenuation, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, light.falloffLoc, &light.falloff, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, light.coneLoc, &light.cone, SHADER_UNIFORM_FLOAT);
}


