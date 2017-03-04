#version 330 core

// Ouput data
out vec4 color;

uniform vec3 elementColor;
uniform float bulletOrLine;

void main()
{
  float r = 0.0, alpha = 1.0;
  if(bulletOrLine==1)
  {
    vec2 cxy = 2.0 * gl_PointCoord - 1.0;
    r = dot(cxy, cxy);
    if (r > 1.0)
    {
        discard;
    }
  }

	color = vec4(elementColor,1);

}
