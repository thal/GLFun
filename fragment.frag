#version 130

varying vec3 fPosition;
uniform sampler2D tex;
in vec2 vs_tex_coord;
void
main()
{
  gl_FragColor = texture(tex, vs_tex_coord).rrra;
}
