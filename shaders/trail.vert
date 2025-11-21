#version 330 core 

layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int total_vertices;

out float vAlpha;

void main(){
  gl_Position = projection * view * model * vec3(position, 1.0f);

  vAlpha = float(gl_VertexID) / float(total_vertices);
}