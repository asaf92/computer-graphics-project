#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;

// Light
uniform vec3 cameraLocation;
uniform vec3 lightSourceLocation;
uniform vec4 lightColor;
uniform vec4 ambiantColor;
uniform vec4 ambiantLighting;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;


// The final color of the fragment (pixel)
out vec4 frag_color;

vec4 calculateDiffusePart();
vec4 calculateSpecularPart();

void main()
{
	vec4 diffusePartSum = calculateDiffusePart();
	vec4 specularPartSum = calculateSpecularPart();

	frag_color = ambiantColor * ambiantLighting + diffusePartSum + specularPartSum;
}

vec4 calculateDiffusePart()
{
	vec4 directionToLight = vec4(normalize(lightSourceLocation - fragPosition),0.0f);
	vec4 normalVector = vec4(fragNormal,0.0f);
	float scalar = dot(directionToLight, normalVector);
	vec4 result = diffuseColor * scalar * lightColor;
	
	return result;
}

vec4 calculateSpecularPart()
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
