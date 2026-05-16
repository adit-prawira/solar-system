#version 330 core

in float vAlpha;

uniform vec3 line_color;
out vec4 FragColor;

void main(){
  FragColor = vec4(line_color, vAlpha);
}
