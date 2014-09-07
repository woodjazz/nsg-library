uniform mat4 u_m;
uniform mat4 u_mvp;
uniform mat3 u_model_inv_transp;
uniform mat4 u_v_inv;
uniform vec4 u_scene_ambient;
uniform vec4 u_color;

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform Material u_material;

struct LightSource
{
	int type;
	vec3 position;
	vec4 diffuse;
	vec4 specular;
	float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
	float spotCutoff;
    float spotExponent;
	vec3 spotDirection;
};

const int POINT_LIGHT = 0;
const int DIRECTIONAL_LIGHT = 1;
const int SPOT_LIGHT = 2;
uniform int u_numOfLights;
uniform LightSource u_light0;

