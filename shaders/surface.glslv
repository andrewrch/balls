#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 normal;

uniform mat4 WVP;
uniform mat4 WV;
uniform mat4 W;
uniform mat4 N;
uniform vec3 light_pos;

out Data {
  vec3 normal;
  vec4 colour;
  vec2 tex_coord;
  vec3 eye_norm;
  vec3 light_pos;
  vec3 eye_pos;
  vec4 world_pos;
} data_out;


void main()
{
  data_out.normal = (W * vec4(normal, 0)).xyz;
  data_out.colour = colour;
  data_out.tex_coord = tex_coord;

  data_out.eye_norm = normalize(mat3(N) * normal);
  data_out.eye_pos = (WV * vec4(position, 1.0)).xyz;
  data_out.light_pos = light_pos;
  data_out.world_pos = W * vec4(position, 1.0);
  // And assign vertex position
  gl_Position = WVP * vec4(position, 1.0);
}
