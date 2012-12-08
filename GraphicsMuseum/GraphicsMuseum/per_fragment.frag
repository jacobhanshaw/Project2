#version 400
in vec3 Position;
in vec3 Normal;

flat in vec3 color;
flat in vec3 back_color;

struct LightInfo
{
	vec4 position;
	vec3 la;
	vec3 ld;
	vec3 ls;
};

struct MaterialInfo
{
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float shininess;
};

uniform LightInfo light;
uniform MaterialInfo material;
uniform bool do_flat_shading;

layout (location = 0) out vec4 FragColor;
in vec2 tcoord;

vec3 ads( )
{
  vec3 n = normalize( Normal );

  if (!gl_FrontFacing)
	n = -n;

  vec3 s = normalize( vec3(light.position) - Position );
  vec3 v = normalize(vec3(-Position));
  vec3 r = reflect(-s, n);
  float s_dot_n = max(dot(s, n), 0.0);

  return light.la * material.ka + light.ld * material.kd * s_dot_n + (s_dot_n > 0 ? light.ls * material.ks * pow(max(dot(r,v), 0.0), material.shininess) : vec3(0.0));
}

void main()
{
	if (do_flat_shading)
		FragColor = vec4(gl_FrontFacing ? color : back_color, 1.0);
	else
		FragColor = vec4(ads(), 1.0);
}
