#version 140

// Input from the vertex shader
varying vec4 vs_color;
void main()
{
   // Simply assign the color we were given by the vertex shader to our output
   gl_FragColor = vs_color;
}
