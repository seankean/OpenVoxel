#version 120

uniform int iterations;
uniform float frame;
uniform float radius;

uniform float f_cx, f_cy;
uniform float f_sx, f_sy;
uniform float f_z;

int fmandel(void)
{
  vec2 c = vec2(f_cx, f_cy) + gl_TexCoord[0].xy*f_z + vec2(f_sx,f_sy);
  vec2 z=c;

  for(int n=0; n<iterations; n++)
    {
      z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
      if((z.x*z.x + z.y*z.y) > radius) return n;
    }
  return 0;
}

void main()
{
  int n = fmandel(); 

  gl_FragColor = vec4((-cos(0.025*float(n))+1.0)/2.0, 
		      (-cos(0.08*float(n))+1.0)/2.0, 
		      (-cos(0.12*float(n))+1.0)/2.0, 
		      1.0);
}
