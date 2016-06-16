// version
#version 430

// output color
layout(location = 0) out vec4 OutColor;

uniform float Time;
uniform int PartNo;
layout(binding = 0) uniform sampler2D Texture;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ph;
uniform float Trans;
uniform int IsTexture;

// global variables
uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;

// input data (from vertex shader)
in vec4 DrawColor;
in vec3 DrawPos;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

vec3 Shade( vec3 N )
{
  vec3 L = normalize(vec3(1, 1, 1));
  vec3 col = Ka;
  vec3 V = normalize(mat3(MatrView) * vec3(-5, -5, -5)); // should be camera location */

  // Diffuse
  float nl = dot(L, N);
  if (IsTexture == 1)
  {
    vec4 tc = texture2D(Texture, DrawTexCoord);

    col += Kd * tc.rgb * max(nl, 0);
  }
  else
    col += Kd * max(nl, 0);



  // Specular
  vec3 R = reflect(-V, N);
  col += Ks * pow(dot(R, L), Ph);

  return col;
}

void main( void )
{
  /*
  if (DrawPos.z > 33.5 + 35 * sin(5 * Time))
    //discard;
    OutColor = vec4(1, 0, 0, 1);//DrawColor
  else
  */
  //if (PartNo < 150 + 150 * sin(Time * 2))
  //if (DrawPos.x > 10 || DrawPos.z + sin(Time * 2) > 1110)
  //if (gl_FragCoord.y < 100.5)
  //  discard;
  vec3 L = normalize(vec3(1, 1, 1));

  float nl = max(dot(L, normalize(DrawNormal)), 0);
  vec4 tc = texture2D(Texture, DrawTexCoord * 1);

  /*
  OutColor = 2 * nl * tc.rgba;
  OutColor = vec4(Ka + Kd * nl + Ks * 0, 1) + tc;
  */
  OutColor = vec4(Shade(normalize(DrawNormal)), Trans);
}
