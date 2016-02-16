#version 130
in vec4 vPosition;
in vec2 in_tex_coord;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
out vec2 vs_tex_coord;
void
main()
{
  gl_Position = projectionMatrix * modelViewMatrix *  vPosition;
  vs_tex_coord = in_tex_coord;
}
