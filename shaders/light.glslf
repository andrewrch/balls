#version 330

in float brightness;
out vec4 output_colour;

void main()
{
  float b = brightness;
  //output_colour = vec4(b, b, b, b);
  /*output_colour = vec4(1, 1, 1, 1);*/
  output_colour = vec4(float(253) / 255, float(184) / 255, float(19) / 255, 1);
}
