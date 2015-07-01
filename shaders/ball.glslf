#version 330

uniform sampler2D surface;
uniform samplerCube shadow;

in Data {
  vec3 normal;
  vec4 colour;
  vec2 tex_coord;
  vec3 eye_norm;
  vec4 light_pos;
  vec4 eye_pos;
  vec4 world_pos;
} data_in;

out vec4 output_colour;

void main()
{
  vec3 light_direction = data_in.world_pos.xyz - data_in.light_pos;
  float distance = length(light_direction);
  float sample_distance = texture(shadow, light_direction).r;

  float intensity = 1.0f;
  if (sample_distance < distance) {
    intensity = 0.5f;
  }

  float lintensity = max(dot(data_in.normal, normalize(-light_direction)), 0.0) * 10.0;
  lintensity /= distance * distance;
  lintensity /= lintensity + 0.5;
  vec3 s = normalize(data_in.light_pos - data_in.eye_pos);
  float diff = max(dot(data_in.eye_norm, s), 0.0);

  vec4 diff_colour = diff * intensity * texture(surface, data_in.tex_coord);
  vec4 ambient_colour = vec4(0.01, 0.01, 0.01, 1.0);

  output_colour = vec4(diff_colour + ambient_colour);
  //vec3 lvector = data_in.world_pos.xyz - data_in.light_pos.xyz;
  //float result = texture(shadow, lvector.xyz).r;

  //float ldistance = length(lvector);
  //float lintensity = max(dot(data_in.normal, normalize(lvector)), 0.0) * 10.0;
  //lintensity /= ldistance * ldistance;
  //lintensity /= lintensity + 0.5;
  ////vec3 diffuse = lintensity * result.xyz * color;
  /////*gl_FragColor = vec4(diffuse,1);*/
  ///*output_colour = vec4(result, 0, 0, 1);*/
  //output_colour = vec4(1, 1, 1, 1);
  ///*output_colour = vec4(ldistance, 0, 0, 1);*/
  ///*output_colour = vec4(data_in.world_pos.xyz, 1);*/

  //vec3 s = normalize((data_in.light_pos - data_in.eye_pos).xyz);
  //// //vec4 r = reflect(-s, data_in.eye_norm);
  //// //vec4 v = normalize(data_in.eye_pos);
  //// //float spec = max(dot(v, r), 0.0);
  //float diff = max(dot(data_in.eye_norm, s), 0.0);

  //// //vec4 diff_colour = diff * result * texture(surface, data_in.tex_coord);
  //vec4 diff_colour = diff * texture(surface, data_in.tex_coord);
  //// //vec3 spec_colour = pow(spec, 3) * vec3(1.0, 1.0, 1.0);
  //vec4 ambient_colour = vec4(0.01, 0.01, 0.01, 1.0);
}
