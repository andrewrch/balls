#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 normal;

uniform mat4 WVP;
uniform mat4 W;
uniform vec3 light_pos;

out float d;

void main(){
  vec4 world_pos = W * vec4(position, 1);
  d = length(world_pos.xyz - light_pos);
  gl_Position =  WVP * vec4(position, 1);
}
