#version 330 core

// MAX_LIGHTS_NUMBER is defined in Scene.h and the
// define statement is inserted by the ShaderProgram::LoadShaders function

in vec3 fragPosition;
in vec3 fragNormal;

uniform vec3 cameraLocation;

// Light
uniform int numberOfLights;
uniform vec3 lightsPositions[MAX_LIGHTS_NUMBER];
uniform vec4 lightColors[MAX_LIGHTS_NUMBER];

// Model
uniform vec4 ambiantColor;
uniform vec4 ambiantLighting;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;

// The final color of the fragment (pixel)
out vec4 frag_color;

vec4 calculateDiffusePart( vec3 lightSourceLocation, vec4 lightColor);
vec4 calculateSpecularPart(vec3 lightSourceLocation, vec4 lightColor);

void main()
{
	vec4 diffusePartSum =  vec4(0.0f);
	vec4 specularPartSum = vec4(0.0f);

	for(int i = 0; i < numberOfLights; i++)
	{
		diffusePartSum  = diffusePartSum  + calculateDiffusePart(lightsPositions[i],lightColors[i]);
		specularPartSum = specularPartSum + calculateSpecularPart(lightsPositions[i],lightColors[i]);
	}

	frag_color = ambiantColor * ambiantLighting + diffusePartSum + specularPartSum;
}

vec4 calculateDiffusePart(vec3 lightSourceLocation, vec4 lightColor)
{
	vec4 directionToLight = vec4(normalize(lightSourceLocation - fragPosition),0.0f);
	vec4 normalVector = vec4(fragNormal,0.0f);
	float scalar = dot(directionToLight, normalVector);
	vec4 result = diffuseColor * scalar * lightColor;
	
	return result;
}

vec4 calculateSpecularPart(vec3 lightSourceLocation, vec4 lightColor)
{
	vec4 directionToCamera = vec4(normalize(cameraLocation - fragPosition),0.0f);
	vec4 directionToLight = vec4(normalize(lightSourceLocation - fragPosition),0.0f);
	vec4 reflection = normalize(2.0f * (dot(directionToLight, vec4(fragNormal,0.0f))) * vec4(fragNormal,0.0f) - directionToLight);
	float scalar = dot(reflection, directionToCamera);
	if (scalar < 0) 
	{
		return vec4(0.0f);
	}

	scalar = pow(scalar, shininess);
	vec4 result = specularColor * scalar * lightColor;

	return result;
}
