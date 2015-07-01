#version 330

uniform sampler2D surface;
uniform samplerCube shadow;

in Data {
  vec3 normal;
  vec4 colour;
  vec2 tex_coord;
  vec3 eye_norm;
  vec3 light_pos;
  vec3 eye_pos;
  vec4 world_pos;
} data_in;

out vec4 output_colour;

void main()
{
  vec3 light_direction = data_in.world_pos.xyz - data_in.light_pos;
  float distance = length(light_direction);
  float sample_distance = texture(shadow, light_direction).r;

  float intensity = 1.0f;
  if (sample_distance < distance + 0.01) {
    intensity = 0.7f;
  }

  vec3 s = normalize(data_in.light_pos - data_in.world_pos.xyz);
  float diff = max(dot(normalize(data_in.normal), s), 0.0) + 0.2;
  vec4 diff_colour = diff * intensity * texture(surface, data_in.tex_coord);
  vec4 ambient_colour = vec4(0.01, 0.01, 0.01, 1.0);
  output_colour = vec4(diff_colour + ambient_colour);
}
