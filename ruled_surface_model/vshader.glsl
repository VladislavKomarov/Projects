#version 140

attribute vec3 vertex;
attribute float select;
uniform float pointSize;
uniform float pointSizeSelect;
uniform vec4 color;
uniform vec4 colorSelect;
uniform mat4 projMatrix;
uniform mat4 mvMatrix;
varying vec4 vs_color;
void main() {
   gl_Position = projMatrix * mvMatrix * vec4(vertex, 1.0);
   if (select != 0.f)
   {
       vs_color = colorSelect;
       gl_PointSize = pointSizeSelect;
   }
   else
   {
       gl_PointSize = pointSize;
       vs_color = color;
   }
}
