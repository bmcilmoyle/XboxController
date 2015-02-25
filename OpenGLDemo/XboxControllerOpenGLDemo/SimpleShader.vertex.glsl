#version 400

// in and out indepen variables
layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Color;
layout(location=2) in vec2 in_UV;	// We'll only use vec3 of the normal

out vec4 ex_Color;
out vec2 ex_UV;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void)
{
  mat4 OneMatrixToRuleThemAll = ProjectionMatrix * ViewMatrix * ModelMatrix;

  // gl_Position is a built-in variable
  gl_Position = OneMatrixToRuleThemAll * in_Position;

  ex_Color =  in_Color;
  ex_UV = in_UV;
}