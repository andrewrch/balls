#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 normal;

uniform vec3 light_pos;
uniform mat4 WVP;
uniform mat4 W;

out float brightness;

void main()
{
  vec3 light_s_pos = (WVP * vec4(light_pos, 1)).xyz;
  vec3 world_s_pos = (WVP * vec4(position, 1)).xyz;

  /*vec3 world_pos = (W * vec4(position, 1)).xyz;*/
  brightness = length(light_s_pos.xy - world_s_pos.xy) * 10;
  gl_Position = WVP * vec4(position, 1.0);
}
