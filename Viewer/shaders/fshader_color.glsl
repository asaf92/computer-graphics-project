#version 330 core

// Uniform ambiant color
uniform vec4 ambiantColor;
uniform vec4 ambiantLighting;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;

// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	frag_color = ambiantColor * ambiantLighting;
}
