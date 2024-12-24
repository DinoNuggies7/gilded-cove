#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

#define MAX_LIGHTS        100
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT       1
#define LIGHT_SPOT        2

struct Light {
	int enabled;
	int type;
	vec3 position;
	vec3 direction;
	vec4 color;
	float attenuation;
	float falloff;
	float cone;
};

uniform Light lights[MAX_LIGHTS];
uniform vec4  ambient;
uniform vec3  viewPos;
uniform int   lightsNum;

void main() {
	vec4 texelColor = texture(texture0, fragTexCoord);
	vec3 lightDot = vec3(0.0);
	vec3 normal = normalize(fragNormal);
	vec3 viewD = normalize(viewPos - fragPosition);
	vec3 specular = vec3(0.0);

	for (int i = 0; i < lightsNum; i++) {
		if (lights[i].enabled == 1) {
			vec3 light = vec3(0.0);
			float factor = 1;
			if (lights[i].type == LIGHT_DIRECTIONAL) {
				light = -normalize(lights[i].direction);
			}
			else {
				float dist = distance(lights[i].position, fragPosition);

				if (lights[i].attenuation > 0 && dist > lights[i].attenuation) {
					if (dist > lights[i].attenuation + lights[i].falloff)
						factor = 0;
					else
						factor = 1.0f - ((dist - lights[i].attenuation)/lights[i].falloff);
				}

				light = normalize(lights[i].position - fragPosition);

				if (lights[i].type == LIGHT_SPOT) {
					float dotp = dot(-light, normalize(lights[i].direction));
					if (dotp < lights[i].cone) {   
						factor = 0;
					}
					else {
						if (dotp > lights[i].cone * 0.5f)
							factor *= ((dotp - lights[i].cone) * (1.0f/(1.0f- lights[i].cone)));
					}
				}
			}

			float NdotL = max(dot(normal, light), 0.0);

			lightDot += lights[i].color.rgb * factor * NdotL;

			float specCo = 0.0;
			if (NdotL > 0.0)
				specCo = pow(max(0.0, dot(viewD, reflect(-(light), normal))), 1.0); // 16 refers to shine

			specular += specCo;
		}
	}

	finalColor = (texelColor * ((colDiffuse + vec4(specular, 1.0)) * vec4(lightDot, 1.0)));
	finalColor += texelColor * (ambient)*colDiffuse;

	finalColor = pow(finalColor, vec4(1.0/2.2));
}
